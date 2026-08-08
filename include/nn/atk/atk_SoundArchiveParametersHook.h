#pragma once

#include <nn/atk/atk_SoundArchive.h>

namespace nn::atk::detail {
class SoundArchiveParametersHook {
public:
    bool SetIsEnable(bool value);

    const char* GetItemLabel(SoundArchive::ItemId id) const;
    SoundArchive::ItemId GetItemId(const char* itemLabel) const;

    SoundArchive::SoundType GetSoundType(const char* itemLabel);

    bool ReadSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SoundInfo* info) const;
    bool ReadBankInfo(SoundArchive::ItemId bankId, SoundArchive::BankInfo* info) const;
    bool ReadPlayerInfo(SoundArchive::ItemId playerId, SoundArchive::PlayerInfo* info) const;
    bool ReadSoundGroupInfo(SoundArchive::ItemId soundGroupId, SoundArchive::SoundGroupInfo* info) const;
    bool ReadGroupInfo(SoundArchive::ItemId groupId, SoundArchive::GroupInfo* info) const;
    bool ReadFileInfo(SoundArchive::FileId id, SoundArchive::FileInfo* info, int index) const;
    bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* info) const;
    bool ReadSound3DInfo(SoundArchive::ItemId soundId, SoundArchive::Sound3DInfo* info) const;
    bool ReadWaveArchiveInfo(SoundArchive::ItemId warcId, SoundArchive::WaveArchiveInfo* info) const;
    bool ReadSequenceSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SequenceSoundInfo* info) const;
    bool ReadStreamSoundInfo(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo* info) const;
    bool ReadStreamSoundInfo2(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo2* info) const;
    bool ReadWaveSoundInfo(SoundArchive::ItemId soundId, SoundArchive::WaveSoundInfo* info) const;

    bool ReadSoundUserParam(u32* pOutValue, SoundArchive::ItemId soundId, int index) const;
    u32 GetSoundUserParam(SoundArchive::ItemId soundId, u32 userParam) const;
};
} // namespace nn::atk::detail

