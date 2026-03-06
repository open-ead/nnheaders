#pragma once

#include <nn/os.h>

#include <nn/atk/submix/atk_OutputReceiver.h>
#include <nn/atk/detail/atk_OutputAdditionalParam.h>
#include <nn/atk/detail/atk_PlayerHeapDataManager.h>

namespace nn::atk::detail::driver {
struct PlayerParamSet {
    
    void Initialize();

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
static_assert(sizeof(PlayerParamSet) == 0x70);

class BasicSoundPlayer {
public:
    virtual ~BasicSoundPlayer();
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    virtual bool Initialize();
#else
    virtual bool Initialize(OutputReceiver* pOutputReceiver);
#endif
    virtual void Finalize();
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Pause(bool isPauseEnabled) = 0;

    BasicSoundPlayer();
    void SetBiquadFilter(s32 type, f32 value);

private:
    os::Event m_Event;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* m_pOutputReceiver;
#endif
    bool m_ActiveFlag;
    bool m_StartedFlag;
    bool m_PauseFlag;
    bool m_FinishFlag;
    bool m_IsFinalizedForCannotAllocateResource;
    PlayerParamSet m_PlayerParamSet;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputAdditionalParam* m_pTvAdditionalParam;
#endif
    PlayerHeapDataManager* m_pPlayerHeapDataManager;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(BasicSoundPlayer) == 0xb0);
#else
static_assert(sizeof(BasicSoundPlayer) == 0xc0);
#endif
} //namespace nn::atk::detail::driver