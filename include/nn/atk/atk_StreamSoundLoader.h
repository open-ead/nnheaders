#pragma once

#include <cstring>

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_InstancePool.h>
#include <nn/atk/atk_LoaderManager.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_StreamSoundFileLoader.h>
#include <nn/atk/atk_StreamSoundFileReader.h>
#include <nn/atk/atk_Task.h>
#include <nn/atk/detail/atk_IStreamDataDecoder.h>
#include <nn/atk/detail/atk_MemoryFileStream.h>
#include <nn/atk/detail/atk_RegionManager.h>

namespace nn::atk::detail {

struct DriverCommandStreamSoundLoadHeader;
struct DriverCommandStreamSoundLoadData;

struct TrackDataInfo {
    uint8_t volume;
    uint8_t pan;
    uint8_t span;
    uint8_t flags;
    uint8_t mainSend;
    uint8_t fxSend[AuxBus_Count];
    uint8_t lpfFreq;
    uint8_t biquadType;
    uint8_t biquadValue;
    uint8_t channelCount;
    uint8_t channelIndex[2];

    void Dump() const;
};
static_assert(sizeof(TrackDataInfo) == 0xe);

struct TrackDataInfos {
    TrackDataInfo track[StreamTrackCount];
};
static_assert(sizeof(TrackDataInfos) == 0x70);

struct StreamDataInfoDetail {
    SampleFormat sampleFormat;
    int sampleRate;
    bool loopFlag;
    position_t loopStart;
    size_t sampleCount;
    position_t originalLoopStart;
    position_t originalLoopEnd;
    bool isRevisionCheckEnabled;
    bool isRegionIndexCheckEnabled;
    uint32_t revisionValue;
    size_t blockSampleCount;
    size_t blockSize;
    size_t lastBlockSize;
    size_t lastBlockSampleCount;
    int channelCount;
    int trackCount;
    TrackDataInfo trackInfo[StreamTrackCount];
    int regionCount;

    void SetStreamSoundInfo(const StreamSoundFile::StreamSoundInfo& info, bool isCrc32CheckEnabled);

    uint32_t GetLastBlockIndex() const { return (sampleCount - 1) / blockSampleCount; }

    position_t GetLoopStartInBlock() const { return loopStart; }

    uint32_t GetLoopStartBlockIndex(position_t loopStartInBlock) const {
        return loopStartInBlock / blockSampleCount;
    }

    void Dump(bool);
};
static_assert(sizeof(StreamDataInfoDetail) == 0xd8);

struct LoadDataParam {
    uint32_t blockIndex;
    size_t samples;
    position_t sampleBegin;
    position_t sampleOffset;
    size_t sampleBytes;
    bool adpcmContextEnable;
    AdpcmContextNotAligned adpcmContext[StreamChannelCount];
    int loopCount;
    bool lastBlockFlag;
    bool isStartOffsetOfLastBlockApplied;

    LoadDataParam() = default;

    void Initialize();

    void Dump();
};
static_assert(sizeof(LoadDataParam) == 0x98);

struct FileStreamHookParam {
    SoundArchiveFilesHook* pSoundArchiveFilesHook{};
    const char* itemLabel{};

    FileStreamHookParam() = default;

    bool IsHookEnabled() const { return pSoundArchiveFilesHook != nullptr; }
};
static_assert(sizeof(FileStreamHookParam) == 0x10);

namespace driver {

class StreamSoundPlayer;

class StreamSoundLoader;
using StreamSoundLoaderManager = LoaderManager<StreamSoundLoader>;

class StreamSoundLoader {
public:
    static const size_t DataBlockSizeBase{8192};
    static const size_t DataBlockSizeMargin{2304};
    static const size_t DataBlockSizeMax{DataBlockSizeBase + DataBlockSizeMargin};

    static const int FileStreamBufferSize{512};

    static const int LoadBufferChannelCount{2};
    static const size_t LoadBufferSize{DataBlockSizeMax * LoadBufferChannelCount};

    StreamSoundLoader();
    ~StreamSoundLoader();

    void Initialize();
    void Finalize();

    static void RegisterStreamDataDecoderManager(IStreamDataDecoderManager* pManager);
    static void UnregisterStreamDataDecoderManager(IStreamDataDecoderManager* pManager);

    void Update();

    void ForceFinish();

    bool IsBusy() const;
    bool IsInUse();

    void RequestLoadHeader();
    void RequestLoadData(void** bufferAddress, uint32_t bufferBlockIndex,
                         position_t startOffsetSamples, position_t prefetchOffsetSamples,
                         int priority);
    void RequestClose();

    void CancelRequest();

    RegionManager& GetRegionManager() { return m_RegionManager; }

    void SetStreamSoundPlayer(StreamSoundPlayer* pPlayer) { m_PlayerHandle = pPlayer; }

    void SetFileType(StreamFileType fileType) { m_FileType = fileType; }

    void SetDecodeMode(DecodeMode mode) { m_DecodeMode = mode; }

    void SetAssignNumber(uint16_t assignNumber) { m_AssignNumber = assignNumber; }

    void SetStreamDataInfo(StreamDataInfoDetail* pStreamDataInfo) { m_DataInfo = pStreamDataInfo; }

    void SetRegionCallback(const StreamRegionCallback& function, void* argument) {
        m_RegionManager.SetRegionCallback(function, argument);
    }

    void SetLoopParameter(bool loopFlag, position_t loopStart, position_t loopEnd) {
        m_LoopFlag = loopFlag;
        m_LoopStart = loopStart;
        m_LoopEnd = loopEnd;
    }

    void SetExternalData(const void* pData, size_t size) {
        m_pExternalData = pData;
        m_ExternalDataSize = size;
    }

    void SetCacheBuffer(void* buffer, size_t size) {
        m_pCacheBuffer = buffer;
        m_CacheSize = size;
    }

    void InitializeFileStream(bool isStreamOpenFailureHalt) {
        m_pFileStream = nullptr;
        m_IsStreamOpenFailureHalt = isStreamOpenFailureHalt;
    }

    void SetFilePath(const char* filePath, int filePathLength) {
        std::strncpy(m_FilePath, filePath, filePathLength);
    }

    const char* GetFilePath() const { return m_FilePath; }

    void SetFileStreamHookParam(const FileStreamHookParam& fileStreamHookParam) {
        m_FileStreamHookParam = fileStreamHookParam;
    }

    void* detail_SetFsAccessLog(fnd::FsAccessLog* pFsAccessLog);

    position_t detail_GetCurrentPosition();
    position_t detail_GetCachePosition();
    size_t detail_GetCachedLength();

private:
    class StreamHeaderLoadTask : public Task {
    public:
        StreamHeaderLoadTask() = default;
        ~StreamHeaderLoadTask() override = default;

        void Execute(TaskProfileLogger& logger) override;

        StreamSoundLoader* m_pLoader;
    };
    static_assert(sizeof(StreamHeaderLoadTask) == 0x50);

    class StreamDataLoadTask : public Task {
    public:
        StreamDataLoadTask() = default;
        ~StreamDataLoadTask() override = default;

        void Execute(TaskProfileLogger& logger) override;

        void* m_BufferAddress[StreamChannelCount];
        uint32_t m_BufferBlockIndex;
        position_t m_StartOffsetSamples;
        position_t m_PrefetchOffsetSamples;
        StreamSoundLoader* m_pLoader;
        util::IntrusiveListNode m_Link;
    };
    static_assert(sizeof(StreamDataLoadTask) == 0xf8);

    class StreamCloseTask : public Task {
    public:
        StreamCloseTask() = default;
        ~StreamCloseTask() override = default;

        void Execute(TaskProfileLogger& logger) override;

        StreamSoundLoader* m_pLoader;
    };
    static_assert(sizeof(StreamCloseTask) == 0x50);

    using StreamDataLoadTaskList = util::IntrusiveList<
        StreamDataLoadTask,
        util::IntrusiveListMemberNodeTraits<StreamDataLoadTask, &StreamDataLoadTask::m_Link>>;

    struct BlockInfo {
        BlockInfo() = default;

        size_t size;
        size_t samples;
        size_t startOffsetSamples;
        size_t startOffsetSamplesAlign;
        size_t startOffsetByte;
        size_t copyByte;

        size_t GetStartOffsetInFrame() const;
    };
    static_assert(sizeof(BlockInfo) == 0x30);

    void WaitFinalize();

    fnd::FndResult Open();
    void Close();

    void LoadHeader();
    bool LoadHeader1(DriverCommandStreamSoundLoadHeader* command);
    bool LoadHeaderForOpus(DriverCommandStreamSoundLoadHeader* command, StreamFileType type,
                           DecodeMode decodeMode);

    IStreamDataDecoderManager* SelectStreamDataDecoderManager(StreamFileType type,
                                                              DecodeMode decodeMode);

    void LoadData(void** bufferAddress, uint32_t bufferBlockIndex, size_t startOffsetSamples,
                  size_t prefetchOffsetSamples, TaskProfileLogger& logger);
    bool LoadData1(DriverCommandStreamSoundLoadData* command, void** bufferAddress,
                   uint32_t bufferBlockIndex, size_t startOffsetSamples,
                   size_t prefetchOffsetSamples, TaskProfileLogger& logger);
    bool LoadDataForOpus(DriverCommandStreamSoundLoadData* command, void** bufferAddress,
                         uint32_t bufferBlockIndex, size_t startOffsetSamples,
                         size_t prefetchOffsetSamples, TaskProfileLogger& logger);

    void SetStreamSoundInfoForOpus(const IStreamDataDecoder::DataInfo& info);

    bool ApplyStartOffset(position_t startOffsetSamples, int* loopCount);

    bool MoveNextRegion(int* loopCount);

    bool ReadTrackInfoFromStreamSoundFile(StreamSoundFileReader& reader);

    bool IsLoopStartFilePos(uint32_t loadingDataBlockIndex);

    void UpdateLoadingDataBlockIndex();
    void UpdateLoadingDataBlockIndexForOpus(void** bufferAddress);

    int GetLoadChannelCount(int loadStartChannel);

    bool LoadStreamBuffer(uint8_t* buffer, const BlockInfo& blockInfo, uint32_t loadChannelCount);
    bool LoadStreamBuffer(uint8_t* buffer, size_t size);

    bool SkipStreamBuffer(size_t skipSize);

    void CalculateBlockInfo(BlockInfo& blockInfo);

    bool LoadOneBlockDataViaCache(void** bufferAddress, const BlockInfo& blockInfo,
                                  position_t destAddressOffset, bool firstBlock,
                                  bool updateAdpcmContext);
    bool LoadOneBlockData(void** bufferAddress, const BlockInfo& blockInfo,
                          position_t destAddressOffset, bool firstBlock, bool updateAdpcmContext);

    bool LoadAdpcmContextForStartOffset();

    void UpdateAdpcmInfoForStartOffset(const void* blockBegin, int channelIndex,
                                       const BlockInfo& blockInfo);
    bool SetAdpcmInfo(StreamSoundFileReader& reader, int channelCount, AdpcmParam** adpcmParam);

    bool DecodeStreamData(void** pOutBufferAddresses, IStreamDataDecoder::DecodeType decodeType);

    bool IsStreamCacheEnabled() const { return m_pCacheBuffer != nullptr && m_CacheSize != 0; }

    struct AdpcmInfo {
        AdpcmParam param;
        AdpcmContext beginContext;
        AdpcmContext loopContext;
    };
    static_assert(sizeof(AdpcmInfo) == 0xc0);

    StreamSoundFileLoader m_FileLoader;
    StreamSoundPlayer* m_PlayerHandle;
    fnd::FileStream* m_pFileStream{};
    StreamDataInfoDetail* m_DataInfo;
    StreamFileType m_FileType;
    DecodeMode m_DecodeMode;
    FileStreamHookParam m_FileStreamHookParam;
    int m_ChannelCount;
    uint16_t m_AssignNumber;
    bool m_LoopFlag;
    bool m_IsStreamOpenFailureHalt;
    position_t m_LoopStart;
    position_t m_LoopEnd;
    char m_FilePath[FilePathMax];
    const void* m_pExternalData;
    size_t m_ExternalDataSize;
    void* m_pCacheBuffer;
    size_t m_CacheSize;
    uint32_t m_LoadingDataBlockIndex;
    uint32_t m_LastBlockIndex;
    uint32_t m_LoopStartBlockIndex;
    position_t m_DataStartFilePos;
    position_t m_LoopStartFilePos;
    position_t m_LoopStartBlockSampleOffset;
    bool m_LoopJumpFlag;
    bool m_LoadFinishFlag;
    RegionManager m_RegionManager;
    StreamHeaderLoadTask m_StreamHeaderLoadTask;
    StreamCloseTask m_StreamCloseTask;
    StreamDataLoadTaskList m_StreamDataLoadTaskList;
    InstancePool<StreamDataLoadTask> m_StreamDataLoadTaskPool;
    u8 m_StreamDataLoadTaskArea[DataBlockSizeBase - sizeof(StreamDataLoadTask) - 8];
    SampleFormat m_SampleFormat;
    AdpcmInfo m_AdpcmInfo[StreamChannelCount];
    u32 m_FileStreamBuffer[sizeof(MemoryFileStream) * 4];
    IStreamDataDecoder* m_pStreamDataDecoder{};
#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
    static IStreamDataDecoderManager* g_pStreamDataDecoderManager;
#else
    IStreamDataDecoderManager* m_pStreamDataDecoderManager{};
#endif

    static u8 g_LoadBuffer[LoadBufferSize];

public:
    util::IntrusiveListNode m_LinkForLoaderManager;
};
#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
static_assert(sizeof(StreamSoundLoader) == 0x35c0);
#else
static_assert(sizeof(StreamSoundLoader) == 0x3640);
#endif

}  // namespace driver
}  // namespace nn::atk::detail
