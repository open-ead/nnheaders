#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/atk_SoundArchiveFile.h>

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
    char* GetItemLabel(SoundArchive::ItemId id) const;

    SoundArchive::FileId GetItemFileId(SoundArchive::ItemId id) const;
    SoundArchive::FileId GetItemPrefetchFileId(SoundArchive::ItemId id) const;

    s32 GetSoundCount() const;
    s32 GetBankCount() const;
    s32 GetPlayerCount() const;
    s32 GetSoundGroupCount() const;
    s32 GetGroupCount() const;
    s32 GetWaveArchiveCount() const;
    s32 GetFileCount() const;

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
    void* GetAttachedGroupTable(u32) const; // unknown return type

private:
    SoundArchiveFile::FileHeader m_Header;
    SoundArchiveFile::StringBlockBody* m_pStringBlockBody;
    SoundArchiveFile::InfoBlockBody* m_pInfoBlockBody;
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    SoundArchiveFile::FileBlock* m_pFileBlock;
#endif
};

struct SoundArchiveFilesHook {
    constexpr static const char* ItemTypeStreamSound = "stm";
    constexpr static const char* ItemTypeWaveSound = "wsd";
    constexpr static const char* ItemTypeSequenceSound = "seq";

    constexpr static const char* FileTypeStreamBinary = "bxstm";
    constexpr static const char* FileTypeWaveSoundBinary = "bxwsd";
    constexpr static const char* FileTypeSequenceBinary = "bxseq";
    constexpr static const char* FileTypeBankBinary = "bxbnk";
    constexpr static const char* FileTypeWaveArchiveBinary = "bxwar";
    constexpr static const char* FileTypeStreamPrefetchBinary = "bxstp";
};
} // namespace nn::atk::detail