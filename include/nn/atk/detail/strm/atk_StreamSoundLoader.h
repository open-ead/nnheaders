#pragma once

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/atk_Config.h>
#include <nn/atk/detail/atk_IStreamDataDecoder.h>
#include <nn/atk/detail/atk_LoaderManager.h>
#include <nn/atk/detail/atk_RegionManager.h>
#include <nn/atk/detail/strm/atk_StreamSoundFileLoader.h>
#include <nn/atk/detail/strm/atk_StreamSoundFileReader.h>
#include <nn/atk/detail/thread/atk_Task.h>
#include <nn/atk/detail/util/atk_InstancePool.h>

namespace nn::atk::detail {
struct IStreamDataDecoderManager;
struct DriverCommandStreamSoundLoadHeader;
struct DriverCommandStreamSoundLoadData;

struct TrackDataInfo {
    u8 volume;
    u8 pan;
    u8 span;
    u8 flags;
    u8 mainSend;
    u8 fxSend[3];
    u8 lpfFreq;
    u8 biquadType;
    u8 biquadValue;
    u8 channelCount;
    u8 channelIndex[2];
};
static_assert(sizeof(TrackDataInfo) == 0xe);

struct TrackDataInfos {
    TrackDataInfo track[8];
};
static_assert(sizeof(TrackDataInfos) == 0x70);

struct StreamDataInfoDetail {
    void SetStreamSoundInfo(const StreamSoundFile::StreamSoundInfo&, bool);

    SampleFormat sampleFormat;
    s32 sampleRate;
    bool loopFlag;
    position_t loopStart;
    std::size_t sampleCount;
    position_t originalLoopStart;
    position_t originalLoopEnd;
    bool isRevisionCheckEnabled;
    bool isRegionIndexCheckEnabled;
    u32 revisionValue;
    std::size_t blockSampleCount;
    std::size_t blockSize;
    std::size_t lastBlockSize;
    std::size_t lastBlockSampleCount;
    s32 channelCount;
    s32 trackCount;
    TrackDataInfo trackInfo[8];
    s32 regionCount;
};
static_assert(sizeof(StreamDataInfoDetail) == 0xd8);

struct LoadDataParam {
    u32 blockIndex;
    std::size_t samples;
    position_t sampleBegin;
    position_t sampleOffset;
    std::size_t sampleBytes;
    bool adpcmContextEnable;
    AdpcmContextNotAligned adpcmContext[16];
    s32 loopCount;
    bool lastBlockFlag;
    bool isStartOffsetOfLastBlockApplied;
};
static_assert(sizeof(LoadDataParam) == 0x98);

struct FileStreamHookParam {
    SoundArchiveFilesHook* pSoundArchiveFilesHook;
    char* itemLabel;
};
static_assert(sizeof(FileStreamHookParam) == 0x10);

namespace driver {
class StreamSoundPlayer;

class StreamSoundLoader;
using StreamSoundLoaderManager = LoaderManager<StreamSoundLoader>;

class StreamSoundLoader {
public:
    constexpr static size_t DataBlockSizeBase = 0x2000;
    constexpr static size_t DataBlockSizeMargin = 0x900;
    constexpr static size_t DataBlockSizeMax = DataBlockSizeBase + DataBlockSizeMargin;

    constexpr static u32 FileStreamBufferSize = 0x200;
    constexpr static u32 LoadBufferChannelCount = 2;
    constexpr static size_t LoadBufferSize = 0x5200;

    class StreamHeaderLoadTask : Task {
    public:
        StreamHeaderLoadTask();
        ~StreamHeaderLoadTask() override;

        void Execute(TaskProfileLogger& logger) override;

    private:
        StreamSoundLoader* m_pLoader;
    };
    static_assert(sizeof(StreamHeaderLoadTask) == 0x50);

    class StreamCloseTask : Task {
    public:
        StreamCloseTask();
        ~StreamCloseTask() override;

        void Execute(TaskProfileLogger& logger) override;

    private:
        StreamSoundLoader* m_pLoader;
    };
    static_assert(sizeof(StreamCloseTask) == 0x50);

    class StreamDataLoadTask : Task {
    public:
        StreamDataLoadTask();
        ~StreamDataLoadTask() override;

        void Execute(TaskProfileLogger& logger) override;

    private:
        friend StreamSoundLoader;

        void* m_BufferAddress[16];
        u32 m_BufferBlockIndex;
        position_t m_StartOffsetSamples;
        position_t m_PrefetchOffsetSamples;
        StreamSoundLoader* m_pLoader;
        util::IntrusiveListNode m_Link;
    };
    static_assert(sizeof(StreamDataLoadTask) == 0xf8);

    using StreamDataLoadTaskList = util::IntrusiveList<StreamDataLoadTask, 
                                    util::IntrusiveListMemberNodeTraits<StreamDataLoadTask, 
                                        &StreamDataLoadTask::m_Link>>;

    struct AdpcmInfo {
        AdpcmParam param;
        AdpcmContext beginContext;
        AdpcmContext loopContext;
    };
    static_assert(sizeof(AdpcmInfo) == 0xc0);

    struct BlockInfo {
        std::size_t size;
        std::size_t samples;
        std::size_t startOffsetSamples;
        std::size_t startOffsetSamplesAlign;
        std::size_t startOffsetByte;
        std::size_t copyByte;

    };
    static_assert(sizeof(BlockInfo) == 0x30);

    StreamSoundLoader();
    ~StreamSoundLoader();

    void WaitFinalize();

    void Initialize();
    void Finalize();

    void CancelRequest();
    void RequestClose();

    void RegisterStreamDataDecoderManager(IStreamDataDecoderManager* manager);
    void UnregisterStreamDataDecoderManager(IStreamDataDecoderManager* manager);

    void* detail_SetFsAccessLog(fnd::FsAccessLog* pFsAccessLog);

    position_t detail_GetCurrentPosition();
    position_t detail_GetCachePosition();

    size_t detail_GetCachedLength();

    void RequestLoadHeader();
    void RequestLoadData(void** bufferAddress, u32 bufferBlockIndex, position_t startOffsetSamples,
                         position_t prefetchOffsetSamples, s32 priority);
    
    void Update();
    void ForceFinish();

    bool IsBusy() const;
    bool IsInUse();

    fnd::FndResult Open();
    void Close();

    void LoadHeader();
    bool LoadHeader1(DriverCommandStreamSoundLoadHeader* command);
    bool LoadHeaderForOpus(DriverCommandStreamSoundLoadHeader* command, 
                           StreamFileType type, DecodeMode decodeMode);
    
    bool ReadTrackInfoFromStreamSoundFile(StreamSoundFileReader& reader);

    bool SetAdpcmInfo(StreamSoundFileReader& reader, s32 channelCount, AdpcmParam** adpcmParam);

    void UpdateLoadingDataBlockIndex();

    IStreamDataDecoderManager* SelectStreamDataDecoderManager(StreamFileType, DecodeMode);

    void SetStreamSoundInfoForOpus(const IStreamDataDecoder::DataInfo& dataInfo);

    void LoadData(void** bufferAddress, u32 bufferBlockIndex, std::size_t startOffsetSamples, 
                  std::size_t prefetchOffsetSamples, TaskProfileLogger& logger);
    bool LoadData1(DriverCommandStreamSoundLoadData* command, void** bufferAddress, 
                   u32 bufferBlockIndex, std::size_t startOffsetSamples, 
                   std::size_t prefetchOffsetSamples, TaskProfileLogger& logger);
    bool LoadDataForOpus(DriverCommandStreamSoundLoadData* command, void** bufferAddress, 
                         u32 bufferBlockIndex, std::size_t startOffsetSamples, 
                         std::size_t prefetchOffsetSamples, TaskProfileLogger& logger);

    bool ApplyStartOffset(s64, s32*);
    
    void CalculateBlockInfo(BlockInfo&);

    bool LoadAdpcmContextForStartOffset();

    bool LoadOneBlockDataViaCache(void** bufferAddress, const BlockInfo& blockInfo, 
                                  position_t destAddressOffset, bool firstBlock, 
                                  bool updateAdpcmContext);
    bool LoadOneBlockData(void** bufferAddress, const BlockInfo& blockInfo, 
                          position_t destAddressOffset, bool firstBlock, 
                          bool updateAdpcmContext);

    bool MoveNextRegion(s32*);

    bool DecodeStreamData(void**, IStreamDataDecoder::DecodeType);

    void UpdateLoadingDataBlockIndexForOpus(void**);

    bool IsLoopStartFilePos(u32);

    s32 GetLoadChannelCount(s32);

    bool LoadStreamBuffer(u8*, const BlockInfo&, u32);
    bool LoadStreamBuffer(u8*, u64);

    bool SkipStreamBuffer(u64);

    void UpdateAdpcmInfoForStartOffset(const void*, s32, const BlockInfo&);

private:
    friend StreamSoundLoaderManager;
    
    StreamSoundFileLoader m_FileLoader;
    StreamSoundPlayer* m_PlayerHandle;
    fnd::FileStream* m_pFileStream;
    StreamDataInfoDetail* m_DataInfo;
    StreamFileType m_FileType;
    DecodeMode m_DecodeMode;
    FileStreamHookParam m_FileStreamHookParam;
    s32 m_ChannelCount;
    u16 m_AssignNumber;
    bool m_LoopFlag;
    bool m_IsStreamOpenFailureHalt;
    position_t m_LoopStart;
    position_t m_LoopEnd;
    char m_FilePath[SoundArchive::FilePathMax];
    void* m_pExternalData;
    std::size_t m_ExternalDataSize;
    void* m_pCacheBuffer;
    std::size_t m_CacheSize;
    u32 m_LoadingDataBlockIndex;
    u32 m_LastBlockIndex;
    u32 m_LoopStartBlockIndex;
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
    u8 m_StreamDataLoadTaskArea[0x1f00];
    SampleFormat m_SampleFormat;
    AdpcmInfo m_AdpcmInfo[16];
    u32 m_FileStreamBuffer[128];
    IStreamDataDecoder* m_pStreamDataDecoder; 
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    static IStreamDataDecoderManager* g_pStreamDataDecoderManager;
#else
    IStreamDataDecoderManager* m_pStreamDataDecoderManager;
#endif
    util::IntrusiveListNode m_LinkForLoaderManager;

    static u8 g_LoadBuffer[LoadBufferSize];
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(StreamSoundLoader) == 0x35c0);
#else
static_assert(sizeof(StreamSoundLoader) == 0x3640);
#endif
} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail