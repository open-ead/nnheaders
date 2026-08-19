#include <nn/atk/atk_SoundArchive.h>

#include <nn/atk/atk_SoundArchiveFileReader.h>
#include <nn/atk/atk_SoundArchiveParametersHook.h>

namespace nn::atk {
SoundArchive::SoundArchive() {
    m_ExtFileRoot[0] = '/';
    m_ExtFileRoot[1] = '\0';
};

SoundArchive::~SoundArchive() = default;

bool SoundArchive::IsAvailable() const {
    return m_pFileReader != nullptr;
}

void SoundArchive::Initialize(detail::SoundArchiveFileReader* fileReader) {
    m_pFileReader = fileReader;
    m_FileBlockOffset = fileReader->GetFileBlockOffset();
}

void SoundArchive::Finalize() {
    if (m_pFileReader != nullptr) {
        m_pFileReader->Finalize();
        m_pFileReader = nullptr;
    }

    m_ExtFileRoot[0] = '/';
    m_ExtFileRoot[1] = '\0';
}

u32 SoundArchive::GetSoundCount() const {
    return m_pFileReader->GetSoundCount();
}

u32 SoundArchive::GetGroupCount() const {
    return m_pFileReader->GetGroupCount();
}

u32 SoundArchive::GetPlayerCount() const {
    return m_pFileReader->GetPlayerCount();
}

u32 SoundArchive::GetSoundGroupCount() const {
    return m_pFileReader->GetSoundGroupCount();
}

u32 SoundArchive::GetBankCount() const {
    return m_pFileReader->GetBankCount();
}

u32 SoundArchive::GetWaveArchiveCount() const {
    return m_pFileReader->GetWaveArchiveCount();
}

u32 SoundArchive::detail_GetFileCount() const {
    return m_pFileReader->GetFileCount();
}

const char* SoundArchive::GetItemLabel(ItemId id) const {
    if (m_pParametersHook != nullptr) {
        const char* result {m_pParametersHook->GetItemLabel(id)};
        if (result != nullptr)
            return result;
    }

    return m_pFileReader->GetItemLabel(id);
}

SoundArchive::ItemId SoundArchive::GetItemId(const char* pStr) const {
    if (m_pParametersHook != nullptr) {
        ItemId result {m_pParametersHook->GetItemId(pStr)};
        if (result != InvalidId)
            return result;
    }

    return m_pFileReader->GetItemId(pStr);
}

SoundArchive::FileId SoundArchive::GetItemFileId(ItemId id) const {
    return m_pFileReader->GetItemFileId(id);
}

SoundArchive::FileId SoundArchive::GetItemPrefetchFileId(ItemId id) const {
    return m_pFileReader->GetItemPrefetchFileId(id);
}

u32 SoundArchive::GetSoundUserParam(ItemId soundId) const {
    u32 userParam {m_pFileReader->GetSoundUserParam(soundId)};

    if (m_pParametersHook != nullptr && m_pParametersHook->GetIsEnable())
        userParam = m_pParametersHook->GetSoundUserParam(soundId, userParam);

    return userParam;
}

bool SoundArchive::ReadSoundUserParam(u32* pOutValue, ItemId soundId, int index) const {
    bool result {m_pFileReader->ReadSoundUserParam(pOutValue, soundId, index)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadSoundUserParam(pOutValue, soundId, index);

    return result;
}

SoundArchive::SoundType SoundArchive::GetSoundType(ItemId soundId) const {
#if NN_SDK_VER > NN_MAKE_VER(1, 6, 0)
    if (m_pParametersHook != nullptr) {
        const char* label {m_pParametersHook->GetItemLabel(soundId)};
        if (label != nullptr) {
            SoundType type {m_pParametersHook->GetSoundType(label)};
            if (type != SoundType_Invalid)
                return type;
        }
    }
#endif
    return m_pFileReader->GetSoundType(soundId);
}

bool SoundArchive::ReadSoundInfo(SoundInfo* pOutValue, ItemId soundId) const {
    bool result {m_pFileReader->ReadSoundInfo(soundId, pOutValue)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadSoundInfo(soundId, pOutValue);

    return result;
}

bool SoundArchive::ReadSequenceSoundInfo(SequenceSoundInfo* pOutValue, ItemId soundId) const {
    bool result {m_pFileReader->ReadSequenceSoundInfo(soundId, pOutValue)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadSequenceSoundInfo(soundId, pOutValue);

    return result;
}

bool SoundArchive::ReadBankInfo(BankInfo* pOutValue, ItemId bankId) const {
    bool result {m_pFileReader->ReadBankInfo(bankId, pOutValue)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadBankInfo(bankId, pOutValue);

    return result;
}

bool SoundArchive::ReadPlayerInfo(PlayerInfo* pOutValue, ItemId playerId) const {
    bool result {m_pFileReader->ReadPlayerInfo(playerId, pOutValue)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadPlayerInfo(playerId, pOutValue);

    return result;
}

bool SoundArchive::ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* pOutValue) const {
    bool result {m_pFileReader->ReadSoundArchivePlayerInfo(pOutValue)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadSoundArchivePlayerInfo(pOutValue);

    return result;
}

bool SoundArchive::ReadStreamSoundInfo(StreamSoundInfo* pOutValue, ItemId soundId) const {
    bool result {m_pFileReader->ReadStreamSoundInfo(soundId, pOutValue)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadStreamSoundInfo(soundId, pOutValue);

    return result;
}

bool SoundArchive::detail_ReadStreamSoundInfo2(ItemId soundId, StreamSoundInfo2* info) const {
    bool result {m_pFileReader->ReadStreamSoundInfo2(soundId, info)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadStreamSoundInfo2(soundId, info);

    return result;
}

bool SoundArchive::detail_ReadWaveSoundInfo(ItemId soundId, WaveSoundInfo* info) const {
    bool result {m_pFileReader->ReadWaveSoundInfo(soundId, info)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadWaveSoundInfo(soundId, info);

    return result;
}

bool SoundArchive::detail_ReadAdvancedWaveSoundInfo(ItemId soundId, AdvancedWaveSoundInfo* info) const {
    bool result {m_pFileReader->ReadAdvancedWaveSoundInfo(soundId, info)};
    return result;
}

bool SoundArchive::ReadSound3DInfo(Sound3DInfo* pOutValue, ItemId soundId) const {
    bool result {m_pFileReader->ReadSound3DInfo(soundId, pOutValue)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadSound3DInfo(soundId, pOutValue);

    return result;
}

bool SoundArchive::ReadWaveArchiveInfo(ItemId warcId, WaveArchiveInfo* info) const {
    bool result {m_pFileReader->ReadWaveArchiveInfo(warcId, info)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadWaveArchiveInfo(warcId, info);

    return result;
}

bool SoundArchive::detail_ReadSoundGroupInfo(ItemId soundGroupId, SoundGroupInfo* info) const {
    bool result {m_pFileReader->ReadSoundGroupInfo(soundGroupId, info)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadSoundGroupInfo(soundGroupId, info);

    return result;
}

bool SoundArchive::ReadGroupInfo(GroupInfo* pOutValue, ItemId groupId) const {
    bool result {m_pFileReader->ReadGroupInfo(groupId, pOutValue)};

    if (m_pParametersHook != nullptr)
        result |= m_pParametersHook->ReadGroupInfo(groupId, pOutValue);

    return result;
}

bool SoundArchive::detail_ReadFileInfo(FileId fileId, FileInfo* info) const {
    bool result {m_pFileReader->ReadFileInfo(fileId, info, 0)};

    if (result && m_pParametersHook != nullptr) {
        m_pParametersHook->ReadFileInfo(fileId, info, 0);
        result = true;
    }

    return result;
}

const detail::Util::Table<u32>* SoundArchive::detail_GetWaveArchiveIdTable(ItemId id) const {
    return m_pFileReader->GetWaveArchiveIdTable(id);
}

detail::fnd::FileStream* SoundArchive::detail_OpenFileStream(FileId fileId, void* buffer, size_t size, 
                                                             void* cacheBuffer, size_t cacheSize) const {
    FileInfo fileInfo {};
    detail::fnd::FileStream* stream {};
    if (detail_ReadFileInfo(fileId, &fileInfo)) {
        if (fileInfo.externalFilePath == nullptr) {
            if (fileInfo.fileSize != FileInfo::InvalidSize && fileInfo.offsetFromFileBlockHead != FileInfo::InvalidOffset)
                stream = OpenStream(buffer, size, m_FileBlockOffset + fileInfo.offsetFromFileBlockHead, fileInfo.fileSize);
        }
        else {
            stream = OpenExtStreamImpl(buffer, size, fileInfo.externalFilePath, cacheBuffer, cacheSize);
        }
    }

    return stream;
}
} // namespace nn::atk