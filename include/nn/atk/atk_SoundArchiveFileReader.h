#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundArchiveFile.h>

namespace nn::atk::detail {
class SoundArchiveFileReader {
public:
    constexpr static s32 SignatureFile = 0x52415346; // FSAR
    constexpr static s32 InvalidOffset = -1;
    constexpr static s32 InvalidSize = -1;

    SoundArchiveFileReader();

    void Initialize(const void* soundArchiveData);
    void Finalize();

    bool IsStreamSendAvailable() const;
    bool IsFilterSupportedVersion() const;
    bool IsStreamPrefetchAvailable() const;

    void SetStringBlock(const void* stringBlock);
    void SetInfoBlock(const void* infoBlock);

    s32 GetStringCount() const;
    char* GetString(u32) const;

    void DumpTree() const;

    SoundArchive::ItemId GetItemId(const char* pStr);
    const char* GetItemLabel(SoundArchive::ItemId id) const;

    SoundArchive::FileId GetItemFileId(SoundArchive::ItemId id) const;
    SoundArchive::FileId GetItemPrefetchFileId(SoundArchive::ItemId id) const;

    u32 GetSoundCount() const;
    u32 GetBankCount() const;
    u32 GetPlayerCount() const;
    u32 GetSoundGroupCount() const;
    u32 GetGroupCount() const;
    u32 GetWaveArchiveCount() const;
    u32 GetFileCount() const;

    bool ReadSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SoundInfo* info) const;
    bool ReadBankInfo(SoundArchive::ItemId bankId, SoundArchive::BankInfo* info) const;
    bool ReadPlayerInfo(SoundArchive::ItemId playerId, SoundArchive::PlayerInfo* info) const;
    bool ReadSoundGroupInfo(SoundArchive::ItemId soundGroupId, SoundArchive::SoundGroupInfo* info) const;
    bool ReadGroupInfo(SoundArchive::ItemId groupId, SoundArchive::GroupInfo* info) const;
    bool ReadFileInfo(SoundArchive::FileId id, SoundArchive::FileInfo* info, s32 index) const;
    bool ReadWaveArchiveInfo(SoundArchive::ItemId warcId, SoundArchive::WaveArchiveInfo* info) const;
    bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* info) const;
    bool ReadSound3DInfo(SoundArchive::ItemId soundId, SoundArchive::Sound3DInfo* info) const;
    bool ReadSequenceSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SequenceSoundInfo* info) const;
    bool ReadStreamSoundInfo(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo* info) const;
    bool ReadStreamSoundInfo2(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo2* info) const;
    bool ReadWaveSoundInfo(SoundArchive::ItemId soundId, SoundArchive::WaveSoundInfo* info) const;
    bool ReadAdvancedWaveSoundInfo(SoundArchive::ItemId soundId, SoundArchive::AdvancedWaveSoundInfo* info) const;

    Util::Table<u32>* GetWaveArchiveIdTable(SoundArchive::ItemId id) const;
    SoundArchive::SoundType GetSoundType(SoundArchive::ItemId soundId) const;
    u32 GetSoundUserParam(u32) const;

    bool ReadSoundUserParam(u32*, u32, s32) const;
    const detail::Util::Table<u32>* GetAttachedGroupTable(u32) const;

    int GetInfoBlockOffset() const { return m_Header.GetInfoBlockOffset(); }
    int GetFileBlockOffset() const { return m_Header.GetFileBlockOffset(); }

    u32 GetInfoBlockSize() const { return m_Header.GetInfoBlockSize(); }
    u32 GetFileBlockSize() const { return m_Header.GetFileBlockSize(); }

private:
    friend SoundArchive;

    SoundArchiveFile::FileHeader m_Header;
    SoundArchiveFile::StringBlockBody* m_pStringBlockBody;
    SoundArchiveFile::InfoBlockBody* m_pInfoBlockBody;
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    SoundArchiveFile::FileBlock* m_pFileBlock;
#endif
};
} // namespace nn::atk::detail