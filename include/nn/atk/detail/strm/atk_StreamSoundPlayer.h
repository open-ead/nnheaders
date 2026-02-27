#pragma once

#include <nn/atk/detail/atk_BasicSoundPlayer.h>
#include <nn/atk/detail/strm/atk_StreamBufferPool.h>
#include <nn/atk/detail/strm/atk_StreamSoundLoader.h>
#include <nn/atk/detail/strm/atk_StreamSoundPrefetchFileReader.h>
#include <nn/atk/detail/strm/atk_StreamTrack.h>

namespace nn::atk::detail {
struct StreamSoundDataInfo {
    bool loopFlag;
    s32 sampleRate;
    s64 loopStart;
    s64 loopEnd;
    s64 compatibleLoopStart;
    s64 compatibleLoopEnd;
    s32 channelCount;
};
static_assert(sizeof(StreamSoundDataInfo) == 0x30);

struct StreamSoundRegionDataInfo {
    u32 startSamplePosition;
    u32 endSamplePosition;
    s32 regionNo;
    char regionName[64];
};
static_assert(sizeof(StreamSoundRegionDataInfo) == 0x4c);

namespace driver {
class StreamSoundPlayer : BasicSoundPlayer, SoundThread::PlayerCallback {
public:
    enum StartOffsetType {
        StartOffsetType_Sample,
        StartOffsetType_Millisec,
    };

    struct SetupArg;
    struct ItemData {
        f32 pitch;
        f32 mainSend;
        f32 fxSend[3];

        void Set(const SetupArg& arg);
    };
    static_assert(sizeof(ItemData) == 0x14);

    struct TrackData {
        f32 volume;
        f32 lpfFreq;
        s32 biquadType;
        f32 biquadValue;
        f32 pan;
        f32 span;
        f32 mainSend;
        f32 fxSend[3];
      
        void Set(const StreamTrack* pStreamTrack);
    };
    static_assert(sizeof(TrackData) == 0x28);

    struct WaveBufferInfo {
        position_t sampleBegin;
        std::size_t sampleLength;
        s32 loopCount;
    };
    static_assert(sizeof(WaveBufferInfo) == 0x18);

    struct PrepareBaseArg {
        StartOffsetType startOffsetType;
        position_t offset;
        s32 delayTime;
        s32 delayCount;
        UpdateType updateType;
        StreamRegionCallback regionCallback;
        void* regionCallbackArg;
        char filePath[639];
        void* pExternalData;
        std::size_t externalDataSize;
        FileStreamHookParam fileStreamHookParam;
    };
    static_assert(sizeof(PrepareBaseArg) == 0x2d0);

    struct PrepareArg {
        PrepareBaseArg baseArg;
        void* cacheBuffer;
        std::size_t cacheSize;
    };
    static_assert(sizeof(PrepareArg) == 0x2e0);

    struct PreparePrefetchArg {
        PrepareBaseArg baseArg;
        void* strmPrefetchFile;
    };
    static_assert(sizeof(PreparePrefetchArg) == 0x2d8);

    struct SetupArg {
        StreamBufferPool* pBufferPool;
        u32 allocChannelCount;
        u16 allocTrackFlag;
        u8 fileType;
        bool loopFlag;
        TrackDataInfos trackInfos;
        position_t loopStart;
        position_t loopEnd;
        f32 pitch;
        u8 mainSend;
        u8 fxSend[3];
        DecodeMode decodeMode;
    };
    static_assert(sizeof(SetupArg) == 0xa0);

    struct PrefetchIndexInfo {
        u32 lastBlockIndex;
        position_t loopStartInBlock;
        u32 loopStartBlockIndex;
        s32 loopBlockCount;

        void Initialize(const StreamDataInfoDetail& streamDataInfo);
    };
    static_assert(sizeof(PrefetchIndexInfo) == 0x18);

    struct PrefetchLoadDataParam : LoadDataParam {
        u32 prefetchBlockIndex;
        std::size_t prefetchBlockBytes;
    };
    static_assert(sizeof(PrefetchLoadDataParam) == 0xa8);

    StreamSoundPlayer();
    ~StreamSoundPlayer() override;


#if NN_SDK_VER < NN_MAKE_VER(5, 3, 0)
    bool Initialize() override;
#else
    bool Initialize(OutputReceiver* pOutputReceiver) override;
#endif

    bool TryAllocLoader();

    void Finalize() override;

    void FinishPlayer();

    void FreeStreamBuffers();
    void FreeVoices();
    void FreeLoader();

    void Setup(const SetupArg& arg);
    void SetupTrack(const SetupArg& arg);

    void Prepare(const PrepareArg& arg);
    
    void SetPrepareBaseArg(const PrepareBaseArg& arg);

    void RequestLoadHeader(const PrepareArg& arg);

    void PreparePrefetch(const PreparePrefetchArg& arg);
    
    bool ReadPrefetchFile(StreamSoundPrefetchFileReader& reader);

    bool ApplyStreamDataInfo(const StreamDataInfoDetail& streamDataInfo);

    bool SetupPlayer();

    bool AllocVoices();

    bool LoadPrefetchBlocks(StreamSoundPrefetchFileReader& reader);

    void Start() override;
    void StartPlayer();

    void Stop() override;

    void Pause(bool flag) override;

    void UpdatePauseStatus();

    bool IsLoadingDelayState() const;
    bool IsBufferEmpty() const;

    bool ReadStreamDataInfo(StreamDataInfo* strmDataInfo) const;

    position_t GetPlaySamplePosition(bool) const;
    f32 GetFilledBufferPercentage() const;
    s32 GetBufferBlockCount(WaveBuffer::Status waveBufferStatus) const;
    s32 GetTotalBufferBlockCount() const;

    bool LoadHeader(bool result, AdpcmParam** adpcmParam, u16 assignNumber);

    bool CheckPrefetchRevision(const StreamDataInfoDetail& streamDataInfo) const;

    bool AllocStreamBuffers();

    void UpdateLoadingBlockIndex();

    bool LoadStreamData(bool result, const LoadDataParam& loadDataParam, u16 assignNumber);
    bool LoadStreamData(bool result, const LoadDataParam& loadDataParam, u16 assignNumber, 
                        bool usePrefetchFlag, u32 currentPrefetchBlockIndex, 
                        std::size_t currentPrefetchBlockBytes);

    bool IsStoppedByLoadingDelay() const;

    static void VoiceCallbackFunc(MultiVoice* voice, 
                                  MultiVoice::VoiceCallbackStatus status, void* arg);

    void Update();
    void UpdateBuffer();
    void UpdateVoiceParams(StreamTrack* track);

    bool CheckDiskDriveError();

    void SetOutputParam(const OutputParam*, const OutputParam&, const TrackData&);
    
    void ApplyTvOutputParamForMultiChannel(OutputParam* outputParam, 
                                           MultiVoice* MultiVoice, s32 channelIndex, 
                                           MixMode mixMode);
    void ApplyTvOutputParamForMultiChannel(OutputParam* outputParam, 
                                           OutputAdditionalParam* pOutputAdditionalParam, 
                                           MultiVoice* MultiVoice, s32 channelIndex, 
                                           MixMode mixMode);
    
    void MixSettingForOutputParam(OutputParam* outputParam, s32 channelIndex, MixMode mixMode);
    
    position_t GetOriginalPlaySamplePosition(position_t, const StreamDataInfoDetail& streamDataInfo) const;

    bool IsValidStartOffset(const StreamDataInfoDetail& streamDataInfo);

    void ApplyTrackDataInfo(const StreamDataInfoDetail& streamDataInfo);

    u64 GetStartOffsetSamples(const StreamDataInfoDetail& streamDataInfo);

    void PreparePrefetchOnLastBlock(PrefetchLoadDataParam*, const PrefetchIndexInfo&);
    void PreparePrefetchOnLoopStartBlock(PrefetchLoadDataParam*, const PrefetchIndexInfo&,
                                         StreamSoundPrefetchFileReader& reader);
    void PreparePrefetchOnLoopBlock(PrefetchLoadDataParam*, const PrefetchIndexInfo&, u32);
    bool PreparePrefetchOnNormalBlock(PrefetchLoadDataParam*, u32, 
                                      StreamSoundPrefetchFileReader* reader);

    bool SetAdpcmLoopInfo(StreamSoundPrefetchFileReader& reader, 
                          const StreamDataInfoDetail& streamDataInfo, AdpcmParam* adpcmParam,
                          AdpcmContextNotAligned* adpcmContext);
    bool SetAdpcmInfo(StreamSoundPrefetchFileReader& reader, 
                      const StreamDataInfoDetail& streamDataInfo, AdpcmParam* adpcmParam,
                      AdpcmContextNotAligned* adpcmContext);

    void SetTrackVolume(u32 trackBitFlag, f32 volume);
    void SetTrackInitialVolume(u32 trackBitFlag, u32 volume);

    void SetTrackOutputLine(u32 trackBitFlag, u32 outputLine);
    void ResetTrackOutputLine(u32 trackBitFlag);

    void SetTrackTvVolume(u32 trackBitFlag, f32 volume);
    void SetTrackChannelTvMixParameter(u32 trackBitFlag, u32 srcChNo, const MixParameter& param);
    void SetTrackTvPan(u32 trackBitFlag, f32 pan);
    void SetTrackTvSurroundPan(u32 trackBitFlag, f32 span);
    void SetTrackTvMainSend(u32 trackBitFlag, f32 send);
    void SetTrackTvFxSend(u32 trackBitFlag, AuxBus bus, f32 send);

    StreamTrack* GetPlayerTrack(s32 index);
    StreamTrack* GetPlayerTrack(s32 index) const;

    void OnUpdateFrameSoundThread() override;
    void OnUpdateFrameSoundThreadWithAudioFrameFrequency() override;
    void OnShutdownSoundThread() override;

private:
    bool m_IsInitialized;
    bool m_IsPrepared;
    bool m_IsFinalizing;
    bool m_IsPreparedPrefetch;
    bool m_PauseStatus;
    bool m_LoadWaitFlag;
    bool m_LoadFinishFlag;
    bool m_ReportLoadingDelayFlag;
    bool m_IsStoppedByLoadingDelay;
    bool m_IsRegisterPlayerCallback;
    bool m_UseDelayCount;
    u8 m_Padding1[2];
    s32 m_LoopCounter;
    s32 m_PlayingBlockLoopCounter;
    s32 m_PrepareCounter;
    StreamSoundLoaderManager* m_pLoaderManager;
    StreamSoundLoader* m_pLoader;
    detail::driver::StreamBufferPool* m_pBufferPool;
    s32 m_BufferBlockCount;
    u32 m_LoadingBufferBlockIndex;
    u32 m_PlayingBufferBlockIndex;
    u32 m_LastPlayFinishBufferBlockIndex;
    StartOffsetType m_StartOffsetType;
    position_t m_StartOffset;
    s32 m_DelayCount;
    u16 m_AssignNumber;
    u8 m_FileType;
    DecodeMode m_DecodeMode;
    bool m_LoopFlag;
    u8 m_Padding2[2];
    StreamDataInfoDetail m_StreamDataInfo;
    position_t m_LoopStart;
    position_t m_LoopEnd;
    ItemData m_ItemData;
    void* m_pStreamPrefetchFile;
    AdpcmParam m_PrefetchAdpcmParam[16];
    StreamSoundPrefetchFileReader::PrefetchDataInfo m_PrefetchDataInfo;
    position_t m_PrefetchOffset;
    bool m_IsPrefetchRevisionCheckEnabled;
    u32 m_PrefetchRevisionValue;
    s32 m_ChannelCount;
    s32 m_TrackCount;
    StreamChannel m_Channels[16];
    StreamTrack m_Tracks[8];
    UpdateType m_UpdateType;
    WaveBufferInfo m_WaveBufferInfo[32];
    PrepareArg m_PrepareArg;
    bool m_IsSucceedPrepare;
    SetupArg m_SetupArg;
    position_t m_PlaySamplePosition;
    position_t m_OriginalPlaySamplePosition;

    static u16 g_TaskRequestIndexCount;
};
static_assert(sizeof(StreamSoundPlayer) == 0x11740);
} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail