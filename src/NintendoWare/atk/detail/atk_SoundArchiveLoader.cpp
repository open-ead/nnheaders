#include <nn/atk/atk_SoundArchiveLoader.h>

namespace nn::atk::detail {

SoundArchiveLoader::SoundArchiveLoader() = default;

SoundArchiveLoader::~SoundArchiveLoader() {
    m_pSoundArchive = nullptr;
};

void SoundArchiveLoader::SetSoundArchive(const SoundArchive* arc) {
    m_pSoundArchive = arc;
}

bool SoundArchiveLoader::IsAvailable() const {
    if (m_pSoundArchive == nullptr)
        return false;

    return m_pSoundArchive->IsAvailable();
}

bool SoundArchiveLoader::LoadData(SoundArchive::ItemId itemId, SoundMemoryAllocatable* pAllocator,
                                  u32 loadFlag, size_t loadBlockSize) {
    if (!IsAvailable())
        return false;

    if (itemId == SoundArchive::InvalidId)
        return false;

    if (pAllocator == nullptr)
        return false;

    m_pSoundArchive->FileAccessBegin();

    SoundArchive::SoundArchivePlayerInfo soundArchivePlayerInfo;
    if (!m_pSoundArchive->ReadSoundArchivePlayerInfo(&soundArchivePlayerInfo))
        return false;

    bool result;
    switch (Util::GetItemType(itemId)) {
    case ItemType_Sound:
        switch (m_pSoundArchive->GetSoundType(itemId)) {
        case SoundArchive::SoundType_Wave:
            if (soundArchivePlayerInfo.isAdvancedWaveSoundEnabled)
                result = LoadAdvancedWaveSound(itemId, pAllocator, loadFlag, loadBlockSize);
            else
                result = LoadWaveSound(itemId, pAllocator, loadFlag, loadBlockSize,
                                       SoundArchive::InvalidId);
            break;
        case SoundArchive::SoundType_Stream:
            result = LoadStreamSoundPrefetch(itemId, pAllocator, loadBlockSize);
            break;
        case SoundArchive::SoundType_Sequence:
            result = LoadSequenceSound(itemId, pAllocator, loadFlag, loadBlockSize);
            break;
        default:
            result = false;
            break;
        }
        break;
    case ItemType_SoundGroup:
        result = LoadSoundGroup(itemId, pAllocator, loadFlag, loadBlockSize);
        break;
    case ItemType_Bank:
        result = LoadBank(itemId, pAllocator, loadFlag, loadBlockSize);
        break;
    case ItemType_Player:
        result = false;
        break;
    case ItemType_WaveArchive:
        result = LoadWaveArchive(itemId, pAllocator, loadFlag, loadBlockSize);
        break;
    case ItemType_Group:
        result = LoadGroup(itemId, pAllocator, loadBlockSize);
        break;
    default:
        result = false;
        break;
    }

    return result;
}

bool SoundArchiveLoader::LoadSequenceSound(SoundArchive::ItemId soundId,
                                           SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                                           size_t loadBlockSize) {
    if ((loadFlag & LoadFlag_Seq) != 0) {
        u32 fileId{m_pSoundArchive->GetItemFileId(soundId)};

        const void* pFile{LoadImpl(fileId, pAllocator, loadBlockSize, false)};

        if (pFile == nullptr)
            return false;
    }

    if ((loadFlag & (LoadFlag_Bank | LoadFlag_Warc)) != 0) {
        SoundArchive::SequenceSoundInfo info;

        if (!m_pSoundArchive->ReadSequenceSoundInfo(&info, soundId))
            return false;

        for (int i{0}; i < SoundArchive::SequenceBankMax; ++i) {
            if (info.bankIds[i] != SoundArchive::InvalidId &&
                !LoadBank(info.bankIds[i], pAllocator, loadFlag, loadBlockSize))
                return false;
        }
    }

    return true;
}

}  // namespace nn::atk::detail
