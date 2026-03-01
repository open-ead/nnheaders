#pragma once

#include <nn/atk/atk_Global.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/detail/atk_BasicSound.h>
#include <nn/atk/detail/atk_PlayerHeap.h>
#include <nn/atk/detail/atk_OutputAdditionalParam.h>

namespace nn::atk  {
class SoundPlayer {
public:
    using SoundList = util::IntrusiveList<
                            detail::BasicSound, util::IntrusiveListMemberNodeTraits<
                                detail::BasicSound, &detail::BasicSound::m_SoundPlayerPlayLink>>;
    using PriorityList = util::IntrusiveList<
                            detail::BasicSound, util::IntrusiveListMemberNodeTraits<
                                detail::BasicSound, &detail::BasicSound::m_SoundPlayerPriorityLink>>;
    using PlayerHeapList = util::IntrusiveList<
                            detail::PlayerHeap, util::IntrusiveListMemberNodeTraits<
                                detail::PlayerHeap, &detail::PlayerHeap::m_Link>>;
    struct OutputParam {
        f32 volume;
        f32 mainSend;
        f32 fxSend[3];
    };
    static_assert(sizeof(OutputParam) == 0x14);

    SoundPlayer();
    explicit SoundPlayer(detail::OutputAdditionalParam* pParam);
    ~SoundPlayer();

    void StopAllSound(s32 fadeFrames);

    void Update();

    void DoFreePlayerHeap();

    void detail_SortPriorityList(bool reverse);

    void PauseAllSound(s32, bool);
    void PauseAllSound(bool, s32);
    void PauseAllSound(bool, s32, PauseMode);
    
    void SetVolume(f32 volume);
    void SetLowPassFilterFrequency(f32 lpfFreq);
    void SetBiquadFilter(s32 biquadFilterType, f32 biquadFilterValue);
    void SetDefaultOutputLine(u32 line);

    void SetMainSend(f32 send);
    f32 GetMainSend() const;

    void SetEffectSend(AuxBus bus, f32 send);
    void GetEffectSend(AuxBus bus) const;

    void SetSend(s32 subMixBus, f32 send);
    f32 GetSend(s32 subMixBus);

    void SetOutputVolume(OutputDevice device, f32 volume);

    void RemoveSoundList(detail::BasicSound* pSound);

    void InsertPriorityList(detail::BasicSound* pSound);
    void RemovePriorityList(detail::BasicSound* pSound);

    void detail_SortPriorityList(detail::BasicSound* pSound);

    bool detail_AppendSound(detail::BasicSound* pSound);
    bool detail_RemoveSound(detail::BasicSound* pSound);

    void SetPlayableSoundCount(s32 count);

    void detail_SetPlayableSoundLimit(s32 limit);
    
    bool detail_CanPlaySound(s32 startPriority);
    bool CanPlaySound(s32 startPriority);

    void detail_AppendPlayerHeap(detail::PlayerHeap* pHeap);
    detail::PlayerHeap* detail_AllocPlayerHeap();
    void detail_FreePlayerHeap(detail::PlayerHeap* pHeap);

private:
    SoundList m_SoundList;
    PriorityList m_PriorityList;
    PlayerHeapList m_PlayerHeapFreeList;
    PlayerHeapList m_PlayerHeapFreeReqList;
    s32 m_PlayableCount;
    s32 m_PlayableLimit;
    u32 m_PlayerHeapCount;
    f32 m_Volume;
    f32 m_LpfFreq;
    s32 m_BiquadType;
    f32 m_BiquadValue;
    u32 m_OutputLineFlag;
    OutputParam m_TvParam;
    detail::OutputAdditionalParam* m_pOutputAdditionalParam;
    bool m_IsFirstComeBased;
};
static_assert(sizeof(SoundPlayer) == 0x88);
}  // namespace nn::atk
