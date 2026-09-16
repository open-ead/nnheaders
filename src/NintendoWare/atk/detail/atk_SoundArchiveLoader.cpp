#include <nn/atk/atk_SoundArchiveLoader.h>

#include <nn/atk/atk_BankFileReader.h>
#include <nn/atk/atk_WaveSoundFileReader.h>

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
        case SoundArchive::SoundType_Sequence:
            result = LoadSequenceSound(itemId, pAllocator, loadFlag, loadBlockSize);
            break;

        case SoundArchive::SoundType_Stream:
            result = LoadStreamSoundPrefetch(itemId, pAllocator, loadBlockSize);
            break;

        case SoundArchive::SoundType_Wave:
            if (soundArchivePlayerInfo.isAdvancedWaveSoundEnabled)
                result = LoadAdvancedWaveSound(itemId, pAllocator, loadFlag, loadBlockSize);
            else
                result = LoadWaveSound(itemId, pAllocator, loadFlag, loadBlockSize,
                                       SoundArchive::InvalidId);
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

    m_pSoundArchive->FileAccessEnd();

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

        for (int i{0}; i < static_cast<int>(SoundArchive::SequenceBankMax); ++i) {
            if (info.bankIds[i] != SoundArchive::InvalidId &&
                !LoadBank(info.bankIds[i], pAllocator, loadFlag, loadBlockSize))
                return false;
        }
    }

    return true;
}

bool SoundArchiveLoader::LoadAdvancedWaveSound(SoundArchive::ItemId soundId,
                                               SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                                               size_t loadBlockSize) {
    u32 awsdFileId{m_pSoundArchive->GetItemFileId(soundId)};

    if ((loadFlag & LoadFlag_Wsd) != 0) {
        const void* pFile{LoadImpl(awsdFileId, pAllocator, loadBlockSize, false)};

        if (pFile == nullptr)
            return false;
    }

    if ((loadFlag & LoadFlag_Warc) != 0) {
        const void* pAwsdFile{GetFileAddressImpl(awsdFileId)};
        if (pAwsdFile == nullptr)
            return false;

        SoundArchive::AdvancedWaveSoundInfo info;
        if (!m_pSoundArchive->detail_ReadAdvancedWaveSoundInfo(soundId, &info))
            return false;

        if (!LoadWaveArchive(info.waveArchiveId, pAllocator, loadFlag, loadBlockSize))
            return false;
    }

    return true;
}

bool SoundArchiveLoader::LoadWaveSound(SoundArchive::ItemId soundId,
                                       SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                                       size_t loadBlockSize, SoundArchive::ItemId waveSoundSetId) {
    u32 wsdFileId{m_pSoundArchive->GetItemFileId(soundId)};

    if ((loadFlag & LoadFlag_Wsd) != 0) {
        const void* pFile{LoadImpl(wsdFileId, pAllocator, loadBlockSize, false)};

        if (pFile == nullptr)
            return false;
    }

    if ((loadFlag & LoadFlag_Warc) != 0) {
        const void* pWsdFile{GetFileAddressImpl(wsdFileId)};
        if (pWsdFile != nullptr) {
            u32 index;
            {
                SoundArchive::WaveSoundInfo info;
                if (!m_pSoundArchive->detail_ReadWaveSoundInfo(soundId, &info))
                    return false;
                index = info.index;
            }

            u32 warcId{SoundArchive::InvalidId};
            u32 waveIndex;
            {
                WaveSoundFileReader reader{pWsdFile};
                WaveSoundNoteInfo info;

                if (!reader.ReadNoteInfo(&info, index, 0))
                    return false;

                warcId = info.waveArchiveId;
                waveIndex = info.waveIndex;
            }

            if (!LoadWaveArchiveImpl(warcId, waveIndex, pAllocator, loadFlag, loadBlockSize))
                return false;

        } else {
            SoundArchive::ItemId itemId{waveSoundSetId != SoundArchive::InvalidId ? waveSoundSetId :
                                                                                    soundId};

            const Util::Table<u32>* pWarcIdTable{
                m_pSoundArchive->detail_GetWaveArchiveIdTable(itemId)};

            for (u32 i{0}; i < pWarcIdTable->count; ++i) {
                if (!LoadWaveArchive(pWarcIdTable->item[i], pAllocator, loadFlag, loadBlockSize))
                    return false;
            }
        }
    }

    return true;
}

bool SoundArchiveLoader::LoadStreamSoundPrefetch(SoundArchive::ItemId soundId,
                                                 SoundMemoryAllocatable* pAllocator,
                                                 size_t loadBlockSize) {
    u32 prefetchFileId{m_pSoundArchive->GetItemPrefetchFileId(soundId)};
    const void* pFile{LoadImpl(prefetchFileId, pAllocator, loadBlockSize, true)};
    return pFile != nullptr;
}

bool SoundArchiveLoader::LoadBank(SoundArchive::ItemId bankId, SoundMemoryAllocatable* pAllocator,
                                  u32 loadFlag, size_t loadBlockSize) {
    u32 bankFileId{m_pSoundArchive->GetItemFileId(bankId)};

    if ((loadFlag & LoadFlag_Bank) != 0) {
        const void* pFile{LoadImpl(bankFileId, pAllocator, loadBlockSize, false)};

        if (pFile == nullptr)
            return false;
    }

    if ((loadFlag & LoadFlag_Warc) != 0) {
        const void* pFile{GetFileAddressImpl(bankFileId)};
        if (pFile != nullptr) {
            BankFileReader reader{pFile};
            const Util::WaveIdTable* table{reader.GetWaveIdTable()};

            if (table == nullptr)
                return false;

            for (u32 i{0}; i < table->GetCount(); ++i) {
                const Util::WaveId* pWaveId{table->GetWaveId(i)};
                if (pWaveId == nullptr)
                    return false;

                if (!LoadWaveArchiveImpl(pWaveId->waveArchiveId, pWaveId->waveIndex, pAllocator,
                                         loadFlag, loadBlockSize))
                    return false;
            }

        } else {
            const Util::Table<u32>* pWarcIdTable{
                m_pSoundArchive->detail_GetWaveArchiveIdTable(bankId)};

            for (u32 i{0}; i < pWarcIdTable->count; ++i) {
                if (!LoadWaveArchive(pWarcIdTable->item[i], pAllocator, loadFlag, loadBlockSize))
                    return false;
            }
        }
    }

    return true;
}

bool SoundArchiveLoader::LoadWaveArchive(SoundArchive::ItemId warcId,
                                         SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                                         size_t loadBlockSize) {
    if ((loadFlag & LoadFlag_Warc) != 0) {
        u32 fileId{m_pSoundArchive->GetItemFileId(warcId)};

        const void* pFile{LoadImpl(fileId, pAllocator, loadBlockSize, true)};
        if (pFile == nullptr)
            return false;
    }

    return true;
}

bool SoundArchiveLoader::LoadGroup(SoundArchive::ItemId groupId, SoundMemoryAllocatable* pAllocator,
                                   size_t loadBlockSize) {
    const void* pGroupFile{
        LoadImpl(m_pSoundArchive->GetItemFileId(groupId), pAllocator, loadBlockSize, true)};
    if (pGroupFile == nullptr)
        return false;

    u32 fileId{PostProcessForLoadedGroupFile(pGroupFile, pAllocator, loadBlockSize)};
    return fileId;
}

bool SoundArchiveLoader::LoadSoundGroup(SoundArchive::ItemId soundGroupId,
                                        SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                                        size_t loadBlockSize) {
    SoundArchive::SoundGroupInfo info;
    if (!m_pSoundArchive->detail_ReadSoundGroupInfo(soundGroupId, &info))
        return false;

    if (info.startId != SoundArchive::InvalidId) {
        switch (m_pSoundArchive->GetSoundType(info.startId)) {
        case SoundArchive::SoundType_Sequence:
            for (u32 id{info.startId}; id <= info.endId; ++id) {
                if (!LoadSequenceSound(id, pAllocator, loadFlag, loadBlockSize))
                    return false;
            }
            break;
        case SoundArchive::SoundType_Wave: {
            for (u32 id{info.startId}; id <= info.endId; ++id) {
                if (!LoadWaveSound(id, pAllocator, loadFlag, loadBlockSize, soundGroupId))
                    return false;
            }
        }
        default:
            break;
        }
    }

    return true;
}

bool SoundArchiveLoader::LoadData(const char* pItemName, SoundMemoryAllocatable* pAllocator,
                                  u32 loadFlag, size_t loadBlockSize) {
    SoundArchive::ItemId itemId{m_pSoundArchive->GetItemId(pItemName)};
    return LoadData(itemId, pAllocator, loadFlag, loadBlockSize);
}

bool SoundArchiveLoader::LoadWaveArchiveImpl(SoundArchive::ItemId warcId, u32 waveIndex,
                                             SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                                             size_t loadBlockSize) {
    SoundArchive::WaveArchiveInfo info;
    if (!m_pSoundArchive->ReadWaveArchiveInfo(warcId, &info))
        return false;

    if (info.isLoadIndividual) {
        if (!LoadIndividualWave(warcId, waveIndex, pAllocator, loadBlockSize))
            return false;
    } else {
        if (!LoadWaveArchive(warcId, pAllocator, loadFlag, loadBlockSize))
            return false;
    }

    return true;
}

}  // namespace nn::atk::detail
