#pragma once

#include <nn/os.h>

#include <nn/atk/atk_OutputAdditionalParam.h>
#include <nn/atk/atk_PlayerHeapDataManager.h>

namespace nn::atk::detail::driver {

struct PlayerParamSet {
    float volume;
    float pitch;
    float lpfFreq;
    float biquadValue;
    s8 biquadType;
    PanMode panMode;
    PanCurve panCurve;
    u32 outputLineFlag;
    OutputParam tvParam;

    PlayerParamSet() { Initialize(); }

    void Initialize() {
        volume = 1.0f;
        pitch = 1.0f;
        lpfFreq = 0.0f;
        biquadValue = 0.0f;
        biquadType = BiquadFilterType_Inherit;
        panMode = PanMode_Dual;
        panCurve = PanCurve_Sqrt;
        outputLineFlag = 1;
        tvParam.Initialize();
    }
};
static_assert(sizeof(PlayerParamSet) == 0x70);

class BasicSoundPlayer {
public:
    BasicSoundPlayer();
    virtual ~BasicSoundPlayer() = default;

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    virtual void Initialize();
#else
    virtual void Initialize(OutputReceiver* pOutputReceiver);
#endif
    virtual void Finalize();

    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Pause(bool isPauseEnabled) = 0;

    bool IsActive() const { return m_ActiveFlag; }
    bool IsStarted() const { return m_StartedFlag; }
    bool IsPause() const { return m_PauseFlag; }
    bool IsPlayFinished() const { return m_FinishFlag; }

    bool IsFinalizedForCannotAllocateResource() const {
        return m_IsFinalizedForCannotAllocateResource;
    }

    void SetVolume(float volume) { m_PlayerParamSet.volume = volume; }
    void SetPitch(float pitch) { m_PlayerParamSet.pitch = pitch; }
    void SetLpfFreq(float lpfFreq) { m_PlayerParamSet.lpfFreq = lpfFreq; }
    void SetBiquadFilter(int type, float value);
    void SetPanMode(PanMode mode) { m_PlayerParamSet.panMode = mode; }
    void SetPanCurve(PanCurve curve) { m_PlayerParamSet.panCurve = curve; }

    float GetVolume() const { return m_PlayerParamSet.volume; }
    float GetPitch() const { return m_PlayerParamSet.pitch; }
    float GetLpfFreq() const { return m_PlayerParamSet.lpfFreq; }
    int GetBiquadFilterType() const { return m_PlayerParamSet.biquadType; }
    float GetBiquadFilterValue() const { return m_PlayerParamSet.biquadValue; }
    PanMode GetPanMode() const { return m_PlayerParamSet.panMode; }
    PanCurve GetPanCurve() const { return m_PlayerParamSet.panCurve; }

    void SetOutputLine(u32 outputLineFlag) { m_PlayerParamSet.outputLineFlag = outputLineFlag; }
    u32 GetOutputLine() const { return m_PlayerParamSet.outputLineFlag; }

    void SetTvParam(const OutputParam& param) { m_PlayerParamSet.tvParam = param; }
    const OutputParam& GetTvParam() const { return m_PlayerParamSet.tvParam; }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetTvAdditionalParamAddr(OutputAdditionalParam* pParam) { m_pTvAdditionalParam = pParam; }

    const OutputAdditionalParam* GetTvAdditionalParamAddr() const { return m_pTvAdditionalParam; };

    void SetTvAdditionalSend(int bus, float send);
    void SetTvBusMixVolumeUsed(bool isUsed);
    void SetTvBusMixVolume(const OutputBusMixVolume& busMixVolume);
    void SetTvBusMixVolumeEnabled(int bus, bool isEnabled);
    void SetTvBinaryVolume(float volume);
    void SetTvVolumeThroughModeUsed(bool isVolumeThroughModeEnabled);
    void SetTvVolumeThroughMode(int bus, u8 volumeThroughMode);

    void SetTvAdditionalParam(const OutputAdditionalParam& param) {
        *m_pTvAdditionalParam = param;
    };
#endif

    bool TryWaitInstanceFree() { return m_Event.TryWait(); }
    void WaitInstanceFree() { m_Event.Wait(); }

    void InitializeEvent() { m_Event.Clear(); }

    const PlayerHeapDataManager* GetPlayerHeapDataManager() const {
        return m_pPlayerHeapDataManager;
    }

    void ClearIsFinalizedForCannotAllocatedResourceFlag() {
        m_IsFinalizedForCannotAllocateResource = false;
    }

protected:
    void SetPlayerHeapDataManagerImpl(const PlayerHeapDataManager* mgr) {
        m_pPlayerHeapDataManager = mgr;
    }

    s32 ToDelayCount(s32 delayTimeMilliSec) { return delayTimeMilliSec / 5; };

    void SetActiveFlag(bool isActive) { m_ActiveFlag = isActive; }
    void SetStartedFlag(bool isStarted) { m_StartedFlag = isStarted; }
    void SetPauseFlag(bool isPauseEnabled) { m_PauseFlag = isPauseEnabled; }
    void SetFinishFlag(bool isPlayFinished) { m_FinishFlag = isPlayFinished; }

    void SetFinalizedForCannotAllocateResourceFlag(bool flag) {
        m_IsFinalizedForCannotAllocateResource = flag;
    }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* GetOutputReceiver() { return m_pOutputReceiver; }
#endif

private:
    os::Event m_Event;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* m_pOutputReceiver{};
#endif
    bool m_ActiveFlag{false};
    bool m_StartedFlag{false};
    bool m_PauseFlag{false};
    bool m_FinishFlag{false};
    bool m_IsFinalizedForCannotAllocateResource{false};
    PlayerParamSet m_PlayerParamSet;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputAdditionalParam* m_pTvAdditionalParam{};
#endif
    const PlayerHeapDataManager* m_pPlayerHeapDataManager;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(BasicSoundPlayer) == 0xb0);
#else
static_assert(sizeof(BasicSoundPlayer) == 0xc0);
#endif

}  // namespace nn::atk::detail::driver
