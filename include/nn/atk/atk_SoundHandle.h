#pragma once

#include <nn/atk/atk_BasicSound.h>

namespace nn::atk {

class SoundHandle {
public:
    enum PauseState {
        PauseState_Normal,
        PauseState_Pausing,
        PauseState_Paused,
        PauseState_Unpausing,
        PauseState_Invalid,
    };

    enum MuteState {
        MuteState_Normal,
        MuteState_Muting,
        MuteState_Muted,
        MuteState_Unmuting,
        MuteState_Invalid,
    };

    SoundHandle() = default;
    ~SoundHandle() = default;

    void StartPrepared() {
        if (IsAttachedSound())
            m_pSound->StartPrepared();
    }

    void Stop(int fadeFrames) {
        if (IsAttachedSound())
            m_pSound->Stop(fadeFrames);
    }

    void Pause(bool flag, int fadeFrames) {
        if (IsAttachedSound())
            m_pSound->Pause(flag, fadeFrames);
    }

    void Pause(bool flag, int fadeFrames, PauseMode pauseMode) {
        if (IsAttachedSound())
            m_pSound->Pause(flag, fadeFrames, pauseMode);
    }

    void Mute(bool flag, int fadeFrames) {
        if (IsAttachedSound())
            m_pSound->Mute(flag, fadeFrames);
    }

    bool IsPrepared() const {
        if (!IsAttachedSound())
            return false;

        return m_pSound->IsPrepared();
    }

    bool IsPause() const {
        if (!IsAttachedSound())
            return false;

        return m_pSound->IsPause();
    }

    bool IsMute() const {
        if (!IsAttachedSound())
            return false;

        return m_pSound->IsMute();
    }

    PauseState GetPauseState() const {
        if (!IsAttachedSound())
            return PauseState_Invalid;

        return static_cast<PauseState>(m_pSound->GetPauseState());
    }

    MuteState GetMuteState() const {
        if (!IsAttachedSound())
            return MuteState_Invalid;

        return static_cast<MuteState>(m_pSound->GetMuteState());
    }

    void FadeIn(int frames) {
        if (IsAttachedSound())
            m_pSound->FadeIn(frames);
    }

    int GetRemainingFadeFrames() const {
        if (!IsAttachedSound())
            return 0;

        return m_pSound->GetRemainingFadeFrames();
    }

    int GetRemainingPauseFadeFrames() const {
        if (!IsAttachedSound())
            return 0;

        return m_pSound->GetRemainingPauseFadeFrames();
    }

    u32 GetPlayFrameCount() const {
        if (!IsAttachedSound())
            return 0;

        return m_pSound->GetPlayFrameCount();
    }

    int GetRemainingMuteFadeFrames() const {
        if (!IsAttachedSound())
            return 0;

        return m_pSound->GetRemainingMuteFadeFrames();
    }

    void SetVolume(float volume, int frames) {
        if (IsAttachedSound())
            m_pSound->SetVolume(volume, frames);
    }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetVolumeThroughMode(int bus, u8 modeBitFlag) {
        if (IsAttachedSound())
            m_pSound->SetVolumeThroughMode(bus, modeBitFlag);
    }
#endif

    void SetPan(float pan) {
        if (IsAttachedSound())
            m_pSound->SetPan(pan);
    }

    void SetSurroundPan(float span) {
        if (IsAttachedSound())
            m_pSound->SetSurroundPan(span);
    }

    void SetMixMode(MixMode mixMode) {
        if (IsAttachedSound())
            m_pSound->SetMixMode(mixMode);
    }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    // UNCHECKED
    void SetMixVolume(int subMixBus, const MixVolume& mixVolume) {
        if (IsAttachedSound())
            m_pSound->SetOutputBusMixVolume(OutputDevice_Main, 0, subMixBus,
                                            {mixVolume.channel, 6});
    }

    // UNCHECKED
    void SetBusMixVolumeEnabled(int subMixBus, bool isEnabled) {
        if (IsAttachedSound())
            m_pSound->SetOutputBusMixVolumeEnabled(OutputDevice_Main, subMixBus, isEnabled);
    }

    // UNCHECKED
    void SetBusMixVolume(int srcChNo, int subMixBus, const ChannelMixVolume& param) {
        if (IsAttachedSound())
            m_pSound->SetOutputBusMixVolume(OutputDevice_Main, srcChNo, subMixBus, param);
    }

    // UNCHECKED
    void SetMixVolume(const MixVolume& mixVolume) {
        if (IsAttachedSound())
            m_pSound->SetOutputBusMixVolume(OutputDevice_Main, 0, 0, {mixVolume.channel, 6});
    }

    // UNCHECKED
    void SetBusMixVolume(int subMixBus, const ChannelMixVolume& param) {
        if (IsAttachedSound())
            m_pSound->SetOutputBusMixVolume(OutputDevice_Main, 0, subMixBus, param);
    }
#endif

    void SetMainSend(float send) {
        if (IsAttachedSound())
            m_pSound->SetMainSend(send);
    }

    void SetEffectSend(AuxBus bus, float send) {
        if (IsAttachedSound())
            m_pSound->SetFxSend(bus, send);
    }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetSend(int bus, float send) {
        if (IsAttachedSound())
            m_pSound->SetSend(bus, send);
    }
#endif

    void SetPitch(float pitch) {
        if (IsAttachedSound())
            m_pSound->SetPitch(pitch);
    }

    void SetLowPassFilterFrequency(float lpfFreq) {
        if (IsAttachedSound())
            m_pSound->SetLpfFreq(lpfFreq);
    }

    void SetBiquadFilter(int type, float value) {
        if (IsAttachedSound())
            m_pSound->SetBiquadFilter(type, value);
    }

    void SetPlayerPriority(int priority) {
        if (IsAttachedSound())
            m_pSound->SetPlayerPriority(priority);
    }

    void SetOutputLine(u32 lineFlag) {
        if (IsAttachedSound())
            m_pSound->SetOutputLine(lineFlag);
    }

    void ResetOutputLine() {
        if (IsAttachedSound())
            m_pSound->ResetOutputLine();
    }

    void SetOutputVolume(OutputDevice device, float volume) {
        if (IsAttachedSound())
            m_pSound->SetOutputVolume(device, volume);
    }

    void SetOutputPan(OutputDevice device, float pan) {
        if (IsAttachedSound())
            m_pSound->SetOutputPan(device, pan);
    }

    void SetOutputSurroundPan(OutputDevice device, float span) {
        if (IsAttachedSound())
            m_pSound->SetOutputSurroundPan(device, span);
    }

    void SetOutputMainSend(OutputDevice device, float send) {
        if (IsAttachedSound())
            m_pSound->SetOutputMainSend(device, send);
    }

    void SetOutputEffectSend(OutputDevice device, AuxBus bus, float send) {
        if (IsAttachedSound())
            m_pSound->SetOutputFxSend(device, bus, send);
    }

    bool IsAttachedSound() const { return m_pSound != nullptr; }

    void DetachSound();

    u32 GetId() const {
        if (IsAttachedSound())
            return m_pSound->GetId();

        return InvalidSoundId;
    }

    const SoundParam* GetAmbientParam() const {
        if (IsAttachedSound())
            return &m_pSound->GetAmbientParam();

        return nullptr;
    }

    void* GetUserParam() const {
        if (IsAttachedSound())
            return m_pSound->GetUserParam();

        return nullptr;
    }

    bool CalculateSoundParamCalculationValues(SoundParamCalculationValues* pOutValue) const;

    // UNCHECKED
    void SetChannelMixParameter(u32 srcChNo, const MixParameter& param) {
        if (IsAttachedSound())
            m_pSound->SetOutputChannelMixParameter(OutputDevice_Main, srcChNo, param);
    }

    // UNCHECKED
    void SetChannelMixParameter(const MixParameter& param) {
        if (IsAttachedSound())
            m_pSound->SetOutputChannelMixParameter(OutputDevice_Main, 0, param);
    }

    void SetOutputChannelMixParameter(OutputDevice device, u32 srcChNo, const MixParameter& param) {
        if (IsAttachedSound())
            m_pSound->SetOutputChannelMixParameter(device, srcChNo, param);
    }

    void SetOutputChannelMixParameter(OutputDevice device, const MixParameter& param) {
        if (IsAttachedSound())
            m_pSound->SetOutputChannelMixParameter(device, 0, param);
    }

    void detail_AttachSound(detail::BasicSound* sound);

    void detail_AttachSoundAsTempHandle(detail::BasicSound* sound);

    detail::BasicSound* detail_GetAttachedSound() { return m_pSound; }

    const detail::BasicSound* detail_GetAttachedSound() const { return m_pSound; }

    void detail_DuplicateHandle(SoundHandle* handle);

private:
    NN_NO_COPY(SoundHandle);

    detail::BasicSound* m_pSound{};
};
static_assert(sizeof(SoundHandle) == 0x8);

}  // namespace nn::atk
