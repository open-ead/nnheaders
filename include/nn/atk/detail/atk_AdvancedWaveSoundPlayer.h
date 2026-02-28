#pragma once

#include <nn/atk/detail/atk_AdvancedWaveSoundFileReader.h>
#include <nn/atk/detail/atk_BasicSoundPlayer.h>
#include <nn/atk/detail/thread/atk_SoundThread.h>
#include <nn/atk/detail/voice/atk_Channel.h>
#include <nn/atk/submix/atk_OutputReceiver.h>

namespace nn::atk::detail::driver {
class AdvancedWaveSoundPlayer : BasicSoundPlayer, SoundThread::PlayerCallback {
public:
    struct ClipParam {
        bool isPlayed;
        Channel* pChannel;
    };
    static_assert(sizeof(ClipParam) == 0x10);

    struct TrackParam {
        bool isPlayed;
        ClipParam clipParam[10];
    };
    static_assert(sizeof(TrackParam) == 0xa8);

    struct TrackParamSet {
        bool isPlayed;
        TrackParam trackParam[4];
    };
    static_assert(sizeof(TrackParamSet) == 0x2a8);

    struct PrepareParameter {
        SoundArchive::AdvancedWaveSoundInfo advancedWaveSoundInfo;
        UpdateType updateType;
        void* pAwsdFile;
        void* pWarcFile;
    };
    static_assert(sizeof(PrepareParameter) == 0x18);

    AdvancedWaveSoundPlayer();
    ~AdvancedWaveSoundPlayer() override;

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    bool Initialize() override;
#else
    bool Initialize(OutputReceiver* pOutputReceiver) override;
#endif
    void Finalize() override;

    void TearDownPlayer();
    void ReleaseTracks();

    void Start() override;
    void Stop() override;
    void Pause(bool isPauseEnabled) override;

    void Prepare(const PrepareParameter& parameter);
    
    void SetupPlayer();
    bool SetupTracks();

    void Update();
    bool UpdateTracks();

    void InitializeTrackParams();

    bool StartClip(ClipParam* pClipParam, 
                   SoundArchive::AdvancedWaveSoundInfo* pWaveSoundClipInfo);
    bool UpdateClip(ClipParam* pClipParam, 
                    SoundArchive::AdvancedWaveSoundInfo* pWaveSoundClipInfo);
    void ReleaseClip(ClipParam* pClipParam);
    void StopClip(ClipParam* pClipParam);

    void OnUpdateFrameSoundThread() override;
    void OnUpdateFrameSoundThreadWithAudioFrameFrequency() override;
    void OnShutdownSoundThread() override;

private:
    SoundArchive::AdvancedWaveSoundInfo m_AdvancedWaveSoundInfo;
    AdvancedWaveSoundTrackInfoSet m_AdvancedWaveSoundTrackInfoSet;
    TrackParamSet m_TrackParamSet;
    void* m_pAwsdFile;
    void* m_pWarcFile;
    UpdateType m_UpdateType;
    u32 m_CurrentTime;
    bool m_IsPrepared;
    bool m_IsInitialized;
    bool m_IsRegisterPlayerCallback;
};
static_assert(sizeof(AdvancedWaveSoundPlayer) == 0x778);
} // namespace nn::atk::detail::driver