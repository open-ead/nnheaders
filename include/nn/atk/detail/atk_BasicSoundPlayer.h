#pragma once

#include <nn/os.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/submix/atk_OutputReceiver.h>
#include <nn/atk/detail/atk_OutputAdditionalParam.h>
#include <nn/atk/detail/atk_PlayerHeapDataManager.h>

namespace nn::atk::detail::driver {
struct PlayerParamSet {
    f32 volume;
    f32 pitch;
    f32 lpfFreq;
    f32 biquadValue;
    s8 biquadType;
    PanMode panMode;
    PanCurve panCurve;
    u32 outputLineFlag;
    OutputParam tvParam;
};

class BasicSoundPlayer {
public:
    virtual ~BasicSoundPlayer();
    virtual void Initialize(OutputReceiver* pOutputReceiver);
    virtual void Finalize();
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Pause(bool isPauseEnabled) = 0;

    BasicSoundPlayer();
    void SetBiquadFilter(s32 type, f32 value);

private:
    os::Event m_Event;
    OutputReceiver* m_pOutputReceiver;
    bool m_ActiveFlag;
    bool m_StartedFlag;
    bool m_PauseFlag;
    bool m_FinishFlag;
    bool m_IsFinalizedForCannotAllocateResource;
    PlayerParamSet m_PlayerParamSet;
    OutputAdditionalParam* m_pTvAdditionalParam;
    PlayerHeapDataManager* m_pPlayerHeapDataManager;
};
static_assert(sizeof(BasicSoundPlayer) == 0xc0);
} //namespace nn::atk::detail::driver