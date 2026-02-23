#pragma once

#include <nn/util/util_BitFlagSet.h>

#include <nn/atk/detail/voice/atk_LowLevelVoice.h>
#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk::detail {
class Voice {
public:
    constexpr static s32 PriorityMin = 0;
    constexpr static s32 PriorityMax = 255;
    constexpr static s32 PriorityNoDrop = 255;

    Voice();
    ~Voice();

    void Initialize(u32);

    bool IsAvailable() const;

    bool AllocVoice(u32 priority);
    void Free();

    void SetPriority(u32 priority);
    void SetState(VoiceState state);

    void AppendWaveBuffer(WaveBuffer* waveBuffer);

    void UpdateParam();

    position_t GetPlayPosition() const;

    bool SetMonoFilter(bool enable, u16 cutoff);
    void SetBiquadFilter(bool enable, const BiquadFilterCoefficients* coef);    

    void UpdateVoiceStatus();

private:
    u32 m_Priority;
    VoiceState m_State;
    VoiceParam m_VoiceParam;
    SampleFormat m_SampleFormat;
    u32 m_SampleRate;
    AdpcmParam m_AdpcmParam;
    OutputReceiver* m_pOutputReceiver;
    u32 m_VoiceId;
    position_t m_PlayPosition;
    u32 m_VoiceInfoEnableFlag;
    u32 m_CommandTag;
    WaveBuffer* m_WaveBufferListBegin;
    WaveBuffer* m_WaveBufferListEnd;
    LowLevelVoice* m_pLowLevelVoice;
};
static_assert(sizeof(Voice) == 0xe0);

class VirtualVoiceManager : Util::Singleton<VirtualVoiceManager> {
public:
    constexpr static s32 InvalidVoiceId = -1;
    
    constexpr static u32 VirtualVoiceCount = 256;
    constexpr static u32 VirtualVoiceElementCount = 8;

    bool AllocVirtualVoice();
    void FreeVirtualVoice(u32);

    void Initialize();
    
    void UpdateVoiceInfo();
    
    s32 GetAllocatedVirtualVoiceCount() const;
    s32 GetUnreleasedLowLevelVoiceCount() const;

private:
    u32 m_VirtualVoiceAllocationTable[8];
    u32 m_VoiceInfoTableRead;
    LowLevelVoice* m_LowLevelVoiceTable[256];
    VoiceInfo m_VoiceInfoTable[2][256];
    util::BitFlagSet<256, void> m_VoiceInfoDirtyTable[2];
};
static_assert(sizeof(VirtualVoiceManager) == 0x4868);
} // namespace nn::atk::voice