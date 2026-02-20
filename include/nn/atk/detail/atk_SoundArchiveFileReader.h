#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/atk_SoundArchiveFile.h>

namespace nn::atk::detail {
class SoundArchiveFileReader {
public:
    SoundArchiveFileReader();

    void Initialize(void* soundArchiveData);
    void Finalize();

    void SetStringBlock(void* stringBlock);
    void SetInfoBlock(void* infoBlock);

    SoundArchive::ItemId GetItemId(char* pStr);
    char* GetItemLabel(SoundArchive::ItemId id);

    SoundArchive::FileId GetItemFileId(SoundArchive::ItemId id);
    SoundArchive::FileId GetItemPrefetchFileId(SoundArchive::ItemId id);

    u32 GetPlayerCount();
    u32 GetWaveArchiveCount();
    u32 GetFileCount();

    bool ReadSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SoundInfo* info);
    bool ReadBankInfo(SoundArchive::ItemId bankId, SoundArchive::BankInfo* info);
    bool ReadPlayerInfo(SoundArchive::ItemId playerId, SoundArchive::PlayerInfo* info);
    bool ReadSoundGroupInfo(SoundArchive::ItemId soundGroupId, SoundArchive::SoundGroupInfo* info);
    bool ReadGroupInfo(SoundArchive::ItemId groupId, SoundArchive::GroupInfo* info);
    bool ReadFileInfo(SoundArchive::FileId id, SoundArchive::FileInfo* info, s32 index);
    bool ReadWaveArchiveInfo(SoundArchive::ItemId warcId, SoundArchive::WaveArchiveInfo* info);
    bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* info);
    bool ReadSequenceSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SequenceSoundInfo* info);
    bool ReadStreamSoundInfo(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo* info);
    bool ReadStreamSoundInfo2(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo2* info);
    bool ReadWaveSoundInfo(SoundArchive::ItemId soundId, SoundArchive::WaveSoundInfo* info);
    bool ReadAdvancedWaveSoundInfo(SoundArchive::ItemId soundId, SoundArchive::AdvancedWaveSoundInfo* info);

    Util::Table<u32>* GetWaveArchiveIdTable(SoundArchive::ItemId id);
    SoundArchive::SoundType GetSoundType(SoundArchive::ItemId soundId);

private:
    detail::SoundArchiveFile::FileHeader m_pHeader;
    detail::SoundArchiveFile::StringBlockBody* m_pStringBlockBody;
    detail::SoundArchiveFile::InfoBlockBody* m_pInfoBlockBody;
};
} // namespace nn::atk::detail