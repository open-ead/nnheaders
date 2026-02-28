#pragma once

#include <nn/atk/detail/voice/atk_MultiVoice.h>

namespace nn::atk::detail::driver {
class MultiVoiceManager {
public:
    using VoiceList = util::IntrusiveList<MultiVoice, 
                        util::IntrusiveListMemberNodeTraits<MultiVoice, &MultiVoice::m_LinkNode>>;

    MultiVoiceManager();

    size_t GetObjectSize(const SoundInstanceConfig& config);
    size_t GetRequiredMemSize(s32 voiceCount, const SoundInstanceConfig& config);
    
    void Initialize(void* mem, size_t memSize, const SoundInstanceConfig& config);
    void Finalize();

    void StopAllVoices();

    MultiVoice* AllocVoice(s32 voiceChannelCount, s32 priority, 
                           MultiVoice::VoiceCallback callback, void* callbackData);

    bool DropLowestPriorityVoice(s32);

    void AppendVoiceList(MultiVoice* voice);

    void FreeVoice(MultiVoice* voice);

    void RemoveVoiceList(MultiVoice* voice);

    void UpdateAllVoiceStatus();
    void UpdateAudioFrameVoiceStatus();
    void UpdateAllVoices();
    void UpdateAudioFrameVoices();

    void ChangeVoicePriority(MultiVoice* voice);

    void UpdateAllVoicesSync(u32 syncFlag);

    s32 GetVoiceCount() const;
    s32 GetActiveCount() const;
    s32 GetFreeCount() const;

    VoiceList* GetVoiceList() const;

private:
    bool m_Initialized;
    VoiceList m_PrioVoiceList;
    VoiceList m_FreeVoiceList;
};
static_assert(sizeof(MultiVoiceManager) == 0x28);
} // namespace nn::atk::detail::driver