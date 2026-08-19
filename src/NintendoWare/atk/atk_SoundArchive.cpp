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
} // namespace nn::atk