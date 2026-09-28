#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_OutputAdditionalParam.h>
#include <nn/atk/atk_PlayerHeap.h>

namespace nn::atk {

class SoundPlayer {
public:
    using PlayerHeapList = util::IntrusiveList<
        detail::PlayerHeap,
        util::IntrusiveListMemberNodeTraits<detail::PlayerHeap, &detail::PlayerHeap::m_Link>>;

    using SoundList =
        util::IntrusiveList<detail::BasicSound,
                            util::IntrusiveListMemberNodeTraits<
                                detail::BasicSound, &detail::BasicSound::m_SoundPlayerPlayLink>>;

    using PriorityList = util::IntrusiveList<
        detail::BasicSound,
        util::IntrusiveListMemberNodeTraits<detail::BasicSound,
                                            &detail::BasicSound::m_SoundPlayerPriorityLink>>;

    struct OutputParam {
        f32 volume;
        f32 mainSend;
        f32 fxSend[AuxBus_Count];

        void Initialize() {
            volume = 1.0f;
            mainSend = 0.0f;

            for (int i{0}; i < AuxBus_Count; ++i)
                fxSend[i] = 0.0f;
        }
    };
    static_assert(sizeof(OutputParam) == 0x14);

    SoundPlayer();

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    explicit SoundPlayer(detail::OutputAdditionalParam* pParam);
#endif

    ~SoundPlayer();

    void Update();

    void StopAllSound(int fadeFrames);

    void PauseAllSound(bool flag, int fadeFrames);
    void PauseAllSound(bool flag, int fadeFrames, PauseMode pauseMode);

    void SetVolume(float volume);
    float GetVolume() const { return m_Volume; }

    void SetLowPassFilterFrequency(float lpfFreq);
    float GetLowPassFilterFrequency() const { return m_LpfFreq; }

    void SetBiquadFilter(int type, float value);
    int GetBiquadFilterType() const { return m_BiquadType; }
    float GetBiquadFilterValue() const { return m_BiquadValue; }

    void SetDefaultOutputLine(u32 outputLineFlag);
    u32 GetDefaultOutputLine() const { return m_OutputLineFlag; }

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetMainSend(float send);
    float GetMainSend() const;

    void SetEffectSend(AuxBus bus, float send);
    float GetEffectSend(AuxBus bus) const;

    void SetSend(int subMixBus, float send);
    float GetSend(int subMixBus);
#endif

    void SetOutputVolume(OutputDevice device, float volume);
    float GetOutputVolume([[maybe_unused]] OutputDevice device) const { return m_TvParam.volume; }

    void SetOutputMainSend([[maybe_unused]] OutputDevice device, float send) {
        m_TvParam.mainSend = send;
    }
    float GetOutputMainSend([[maybe_unused]] OutputDevice device) const {
        return m_TvParam.mainSend;
    }

    void SetOutputFxSend([[maybe_unused]] OutputDevice device, AuxBus bus, float send) {
        m_TvParam.fxSend[bus] = send;
    }
    float GetOutputFxSend([[maybe_unused]] OutputDevice device, AuxBus bus) const {
        return m_TvParam.fxSend[bus];
    }

    int GetPlayingSoundCount() const { return m_SoundList.size(); }

    void SetPlayableSoundCount(int count);
    int GetPlayableSoundCount() const { return m_PlayableCount; }

    void detail_SetPlayableSoundLimit(int limit);

    bool detail_CanPlaySound(int startPriority);

    bool detail_AppendSound(detail::BasicSound* pSound);
    void detail_RemoveSound(detail::BasicSound* pSound);

    void detail_SortPriorityList(bool reverse);
    void detail_SortPriorityList(detail::BasicSound* pSound);

    void detail_AppendPlayerHeap(detail::PlayerHeap* pHeap);
    bool detail_CanUsePlayerHeap() const { return m_PlayerHeapCount != 0; }
    detail::PlayerHeap* detail_AllocPlayerHeap();
    void detail_FreePlayerHeap(detail::PlayerHeap* pHeap);

    bool IsFirstComeBased() { return m_IsFirstComeBased; }
    void SetFirstComeBased(bool isFirstComeBased) { m_IsFirstComeBased = isFirstComeBased; }

private:
    detail::BasicSound* GetLowestPrioritySound() { return &m_PriorityList.front(); }

    void InsertPriorityList(detail::BasicSound* pSound);
    void RemovePriorityList(detail::BasicSound* pSound);
    void RemoveSoundList(detail::BasicSound* pSound);

    void DoFreePlayerHeap();

    SoundList m_SoundList;
    PriorityList m_PriorityList;
    PlayerHeapList m_PlayerHeapFreeList;
    PlayerHeapList m_PlayerHeapFreeReqList;
    int m_PlayableCount{1};
    int m_PlayableLimit{INT_MAX};
    u32 m_PlayerHeapCount{0};
    float m_Volume{1.0f};
    float m_LpfFreq{0.0f};
    int m_BiquadType{BiquadFilterType_Inherit};
    float m_BiquadValue{0.0f};
    u32 m_OutputLineFlag{1};
    OutputParam m_TvParam;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    detail::OutputAdditionalParam* m_pOutputAdditionalParam{};
#endif
    bool m_IsFirstComeBased{false};
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(SoundPlayer) == 0x78);
#else
static_assert(sizeof(SoundPlayer) == 0x88);
#endif

}  // namespace nn::atk
