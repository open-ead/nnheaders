#include <nn/atk/atk_StreamSoundLoader.h>

#include <nn/atk/atk_DriverCommand.h>
#include <nn/atk/atk_SoundArchiveFilesHook.h>
#include <nn/atk/atk_TaskManager.h>
#include <nn/atk/atk_WaveFileReader.h>
#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>

namespace {

using StreamDataDecoderManagerList = nn::util::IntrusiveList<
    nn::atk::detail::IStreamDataDecoderManager,
    nn::util::IntrusiveListMemberNodeTraits<nn::atk::detail::IStreamDataDecoderManager,
                                            &nn::atk::detail::IStreamDataDecoderManager::m_Link,
                                            nn::atk::detail::IStreamDataDecoderManager>>;

StreamDataDecoderManagerList g_StreamDataDecoderManagerList;

const uint8_t DefaultLpfFreq{64};
const uint8_t DefaultBiquadType{0};
const uint8_t DefaultBiquadValue{0};

const int LoopDecodeStartOffset{1};

}  // anonymous namespace

namespace nn::atk::detail {

void StreamDataInfoDetail::SetStreamSoundInfo(const StreamSoundFile::StreamSoundInfo& info,
                                              bool isCrc32CheckEnabled) {
    sampleFormat = WaveFileReader::GetSampleFormat(info.encodeMethod);
    sampleRate = static_cast<int>(info.sampleRate);
    loopFlag = info.isLoop;
    loopStart = info.loopStart;
    sampleCount = info.frameCount;
    originalLoopStart = info.originalLoopStart;
    originalLoopEnd = info.originalLoopEnd;
    blockSampleCount = info.oneBlockSamples;
    blockSize = info.oneBlockBytes;
    lastBlockSampleCount = info.lastBlockSamples;
    lastBlockSize = info.lastBlockPaddedBytes;

    revisionValue = info.crc32Value;
    isRevisionCheckEnabled = isCrc32CheckEnabled;

    regionCount = info.regionCount;
}

namespace driver {

StreamSoundLoader::StreamSoundLoader() {
    [[maybe_unused]] u32 taskCount = m_StreamDataLoadTaskPool.Create(
        m_StreamDataLoadTaskArea, DataBlockSizeBase - sizeof(StreamDataLoadTask) - 8);
    std::memset(m_FilePath, 0, sizeof(m_FilePath));
};

StreamSoundLoader::~StreamSoundLoader() {
    WaitFinalize();

#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
    if (g_pStreamDataDecoderManager != nullptr) {
        if (m_pStreamDataDecoder != nullptr) {
            g_pStreamDataDecoderManager->FreeImpl(m_pStreamDataDecoder);
            m_pStreamDataDecoder = nullptr;
        }
    }
#else
    if (m_pStreamDataDecoderManager != nullptr) {
        if (m_pStreamDataDecoder != nullptr) {
            m_pStreamDataDecoderManager->FreeImpl(m_pStreamDataDecoder);
            m_pStreamDataDecoder = nullptr;
        }
        m_pStreamDataDecoderManager = nullptr;
    }
#endif

    m_StreamDataLoadTaskPool.Destroy();
}

void StreamSoundLoader::WaitFinalize() {
    m_StreamHeaderLoadTask.Wait();
    m_StreamCloseTask.Wait();

    for (auto itr{m_StreamDataLoadTaskList.begin()}; itr != m_StreamDataLoadTaskList.end();) {
        auto curItr{itr++};
        StreamDataLoadTask* task{&*curItr};
        task->Wait();
        m_StreamDataLoadTaskList.erase(m_StreamDataLoadTaskList.iterator_to(*task));
        m_StreamDataLoadTaskPool.Free(task);
    }
}

void StreamSoundLoader::Initialize() {
    WaitFinalize();
    m_LoadingDataBlockIndex = 0;
    m_LastBlockIndex = 0xffffffff;
    m_LoopStartBlockIndex = 0;
    m_LoopStartFilePos = 0;
    m_LoopStartBlockSampleOffset = 0;
    m_LoopJumpFlag = false;
    m_LoadFinishFlag = false;
    m_RegionManager.Initialize();
    m_SampleFormat = SampleFormat_DspAdpcm;
#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
    m_DecodeMode = DecodeMode_Invalid;
    m_pStreamDataDecoderManager = nullptr;
#endif
    m_pStreamDataDecoder = nullptr;
}

void StreamSoundLoader::Finalize() {
    CancelRequest();
    RequestClose();
}

void StreamSoundLoader::CancelRequest() {
    TaskManager::GetInstance().CancelTaskById(reinterpret_cast<uintptr_t>(this));
}

void StreamSoundLoader::RequestClose() {
    m_StreamCloseTask.Wait();
    m_StreamCloseTask.m_pLoader = this;
    TaskManager::GetInstance().AppendTask(&m_StreamCloseTask, TaskManager::TaskPriority_Middle);
}

void StreamSoundLoader::RegisterStreamDataDecoderManager(IStreamDataDecoderManager* pManager) {
    g_StreamDataDecoderManagerList.push_back(*pManager);
}

void StreamSoundLoader::UnregisterStreamDataDecoderManager(IStreamDataDecoderManager* pManager) {
    g_StreamDataDecoderManagerList.erase(g_StreamDataDecoderManagerList.iterator_to(*pManager));
}

void* StreamSoundLoader::detail_SetFsAccessLog(fnd::FsAccessLog* pFsAccessLog) {
    if (m_pFileStream == nullptr)
        return nullptr;

    if (!m_pFileStream->CanSetFsAccessLog())
        return nullptr;

    return m_pFileStream->SetFsAccessLog(pFsAccessLog);
}

position_t StreamSoundLoader::detail_GetCurrentPosition() {
    if (m_pFileStream == nullptr)
        return 0;

    if (!m_pFileStream->IsCacheEnabled())
        return 0;

    return m_pFileStream->GetCurrentPosition();
}

position_t StreamSoundLoader::detail_GetCachePosition() {
    if (m_pFileStream == nullptr)
        return 0;

    if (!m_pFileStream->IsCacheEnabled())
        return 0;

    return m_pFileStream->GetCachePosition();
}

size_t StreamSoundLoader::detail_GetCachedLength() {
    if (m_pFileStream == nullptr)
        return 0;

    if (!m_pFileStream->IsCacheEnabled())
        return 0;

    return m_pFileStream->GetCachedLength();
}

void StreamSoundLoader::RequestLoadHeader() {
    m_StreamHeaderLoadTask.m_pLoader = this;
    m_StreamHeaderLoadTask.SetId(reinterpret_cast<uintptr_t>(this));
    TaskManager::GetInstance().AppendTask(&m_StreamHeaderLoadTask,
                                          TaskManager::TaskPriority_Middle);
}

void StreamSoundLoader::RequestLoadData(void** bufferAddress, uint32_t bufferBlockIndex,
                                        position_t startOffsetSamples,
                                        position_t prefetchOffsetSamples, int priority) {
    StreamDataLoadTask* task{m_StreamDataLoadTaskPool.Alloc()};

    if (task != nullptr)
        new (task) StreamDataLoadTask();

    task->m_pLoader = this;
    task->m_BufferBlockIndex = bufferBlockIndex;
    task->m_PrefetchOffsetSamples = prefetchOffsetSamples;
    task->m_StartOffsetSamples = startOffsetSamples;
    task->SetId(reinterpret_cast<uintptr_t>(this));

    for (int ch{0}; ch < m_ChannelCount; ++ch)
        task->m_BufferAddress[ch] = bufferAddress[ch];

    m_StreamDataLoadTaskList.push_back(*task);
    TaskManager::GetInstance().AppendTask(task, static_cast<TaskManager::TaskPriority>(priority));
}

void StreamSoundLoader::Update() {
    for (auto itr{m_StreamDataLoadTaskList.begin()}; itr != m_StreamDataLoadTaskList.end();) {
        auto curItr{itr++};
        StreamDataLoadTask* task{&*curItr};
        switch (task->GetStatus()) {
        case Task::Status_Done:
        case Task::Status_Cancel:
            task->Wait();
            m_StreamDataLoadTaskList.erase(m_StreamDataLoadTaskList.iterator_to(*task));
            m_StreamDataLoadTaskPool.Free(task);
            break;

        default:
            return;
        }
    }
}

void StreamSoundLoader::ForceFinish() {
    DriverCommand& cmdmgr{DriverCommand::GetInstanceForTaskThread()};
    DriverCommandStreamSoundForceFinish* command{
        cmdmgr.AllocCommand<DriverCommandStreamSoundForceFinish>(false)};
    command->id = DriverCommandId_StrmForceFinish;
    command->player = m_PlayerHandle;
    cmdmgr.PushCommand(command);
    cmdmgr.FlushCommand(true, false);
}

bool StreamSoundLoader::IsBusy() const {
    if (m_LoadFinishFlag)
        return false;

    return !m_StreamDataLoadTaskList.empty();
}

bool StreamSoundLoader::IsInUse() {
    Update();
    return !m_StreamDataLoadTaskList.empty();
}

fnd::FndResult StreamSoundLoader::Open() {
    if (m_pExternalData != nullptr) {
        m_pFileStream =
            new (m_FileStreamBuffer) MemoryFileStream(m_pExternalData, m_ExternalDataSize);
    } else {
        if (m_FileStreamHookParam.IsHookEnabled())
            m_pFileStream = m_FileStreamHookParam.pSoundArchiveFilesHook->OpenFile(
                m_FileStreamBuffer, sizeof(m_FileStreamBuffer), m_pCacheBuffer, m_CacheSize,
                m_FileStreamHookParam.itemLabel, SoundArchiveFilesHook::FileTypeStreamBinary);

        if (m_pFileStream == nullptr) {
            fnd::FileStream* stream{new (m_FileStreamBuffer) fnd::FileStreamImpl()};
            fnd::FndResult result{stream->Open(m_FilePath, fnd::FileStream::AccessMode_Read)};
            if (result.IsFailed())
                return result;

            if (stream->IsOpened() && IsStreamCacheEnabled())
                stream->EnableCache(m_pCacheBuffer, m_CacheSize);

            m_pFileStream = stream;
        }
    }

    m_FileLoader.Initialize(m_pFileStream);

    return fnd::FndResult{fnd::FndResultType_True};
}

void StreamSoundLoader::Close() {
#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
    if (g_pStreamDataDecoderManager != nullptr && m_pStreamDataDecoder != nullptr) {
        g_pStreamDataDecoderManager->FreeImpl(m_pStreamDataDecoder);
        m_pStreamDataDecoder = nullptr;
    }
#else
    if (m_pStreamDataDecoderManager != nullptr && m_pStreamDataDecoder != nullptr) {
        m_pStreamDataDecoderManager->FreeImpl(m_pStreamDataDecoder);
        m_pStreamDataDecoder = nullptr;
    }
#endif

    if (m_pFileStream == nullptr)
        return;

    m_pFileStream->Close();
    m_pFileStream = nullptr;
    m_FileLoader.Finalize();
}

void StreamSoundLoader::LoadHeader() {
    DriverCommand& cmdmgr{DriverCommand::GetInstanceForTaskThread()};
    auto* command{cmdmgr.AllocCommand<DriverCommandStreamSoundLoadHeader>(false)};

    command->id = DriverCommandId_StrmLoadHeader;
    command->player = m_PlayerHandle;
    command->assignNumber = m_AssignNumber;

    bool result{false};
    switch (m_FileType) {
    case StreamFileType_Bfstm:
        result = LoadHeader1(command);
        break;
    case StreamFileType_Opus:
        result = LoadHeaderForOpus(command, m_FileType, m_DecodeMode);
        break;
    }

    command->result = result;

    cmdmgr.PushCommand(command);
    cmdmgr.FlushCommand(true, false);
}

}  // namespace driver
}  // namespace nn::atk::detail
