#pragma once

#include <nn/atk/atk_StreamSoundHandle.h>
#include <nn/atk/detail/atk_SoundInstanceManager.h>
#include <nn/atk/detail/strm/atk_StreamSoundPlayer.h>

namespace nn::atk::detail {
class StreamSound;
using StreamSoundInstanceManager = SoundInstanceManager<StreamSound>;

class StreamSound : BasicSound {
public:
    explicit StreamSound(const StreamSoundInstanceManager& manager);

#if NN_SDK_VER < NN_MAKE_VER(5, 3, 0)
    bool Initialize() override;
#else
    bool Initialize(OutputReceiver* pOutputReceiver) override;
#endif
    void Finalize() override;

    void Setup(const driver::StreamSoundPlayer::SetupArg& arg);

    void Prepare(const driver::StreamSoundPlayer::PrepareArg& arg);
    void PreparePrefetch(const void* strmPrefetchFile, 
                         const driver::StreamSoundPlayer::PrepareArg& arg);

    void UpdateMoveValue() override;
    
    void OnUpdateParam() override;

    void SetTrackVolume(u32 trackBitFlag, f32 volume, s32);
    void SetTrackInitialVolume(u32 trackBitFlag, u32 volume);

    void SetTrackOutputLine(u32 trackBitFlag, u32 outputLine);
    void ResetTrackOutputLine(u32 trackBitFlag);

    void SetTrackMainOutVolume(u32 trackBitFlag, f32 volume);
    void SetTrackChannelMixParameter(u32 trackBitFlag, u32 srcChNo, const MixParameter& param);
    void SetTrackPan(u32 trackBitFlag, f32 pan);
    void SetTrackSurroundPan(u32 trackBitFlag, f32 span);
    void SetTrackMainSend(u32 trackBitFlag, f32 send);
    void SetTrackFxSend(u32 trackBitFlag, AuxBus bus, f32 send);

    void OnUpdatePlayerPriority() override;

    bool IsAttachedTempSpecialHandle() override;
    void DetachTempSpecialHandle() override;
    
    bool ReadStreamDataInfo(StreamDataInfo*) const;

    s32 GetPlayLoopCount() const;
    position_t GetPlaySamplePosition() const;
    f32 GetFilledBufferPercentage() const;
    s32 GetBufferBlockCount(WaveBuffer::Status waveBufferStatus) const;
    s32 GetTotalBufferBlockCount() const;

    bool IsPrepared() const override;
    bool IsSuspendByLoadingDelay() const;
    bool IsLoadingDelayState() const;

    driver::BasicSoundPlayer * GetBasicSoundPlayerHandle() override;

    util::IntrusiveListNode m_PriorityLink;

private:
    StreamSoundHandle* m_pTempSpecialHandle;
    StreamSoundInstanceManager* m_Manager;
    MoveValue<f32, s32> m_TrackVolume[8];
    u16 m_AllocTrackFlag;
    bool m_InitializeFlag;
    u8 m_Padding[1];
    u32 m_AvailableTrackBitFlag[2];
    void* m_pCacheBuffer;
    std::size_t m_CacheSize;
    driver::StreamSoundPlayer m_PlayerInstance;
};
static_assert(sizeof(StreamSound) == 0x11a40);
} // namespace nn::atk::detail