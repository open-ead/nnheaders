#pragma once

#include <nn/atk/atk_SoundStartable.h>

namespace nn::atk::detail {
class StartInfoReader {
public:
    explicit StartInfoReader(const SoundArchive::SoundInfo& soundInfo);

    void Read(const SoundStartable::StartInfo* startInfo);

private:
    SoundStartable::StartInfo::StartOffsetType m_StartOffsetType;
    s32 m_StartOffset;
    s32 m_DelayTime;
    s32 m_DelayCount;
    UpdateType m_UpdateType;
    s32 m_PlayerPriority;
    SoundArchive::ItemId m_PlayerId;
    s32 m_ActorPlayerId;
    SoundStartable::StartInfo::SequenceSoundInfo* m_pSeqInfo;
    SoundStartable::StartInfo::StreamSoundInfo* m_pStrmInfo;
    SoundArchive::StreamSoundInfo* m_pStrmMetaInfo;
    SoundArchive::StreamSoundInfo2* m_pStrmMetaInfo2;
    SoundStartable::StartInfo::WaveSoundInfo* m_pWsdInfo;
    s32 m_SubMixIndex;
    OutputReceiver* m_pOutputReceiver;
};
static_assert(sizeof(StartInfoReader) == 0x58);
} // namespace nn::atk::detail