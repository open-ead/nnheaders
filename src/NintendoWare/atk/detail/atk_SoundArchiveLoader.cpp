#include <nn/atk/atk_SoundArchiveLoader.h>

#include <algorithm>
#include <cstring>

#include <nn/atk/atk_BankFileReader.h>
#include <nn/atk/atk_GroupFileReader.h>
#include <nn/atk/atk_HardwareManager.h>
#include <nn/atk/atk_WaveArchiveFileReader.h>
#include <nn/atk/atk_WaveSoundFileReader.h>

namespace nn::atk::detail {

namespace {

class FileStreamHandle {
public:
    explicit FileStreamHandle(fnd::FileStream* pStream) : m_pStream(pStream) {};
    ~FileStreamHandle() = default;

    fnd::FileStream* operator->() { return m_pStream; }
    explicit operator bool() const { return m_pStream != nullptr; }

private:
    fnd::FileStream* m_pStream;
};

}  // anonymous namespace

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

const void* SoundArchiveLoader::LoadImpl(SoundArchive::FileId fileId,
                                         SoundMemoryAllocatable* pAllocator, size_t loadBlockSize,
                                         bool needMemoryPool) {
    const void* fileAddress{GetFileAddressImpl(fileId)};

    if (fileAddress == nullptr) {
        fileAddress = LoadFile(fileId, pAllocator, loadBlockSize, needMemoryPool);
        if (fileAddress != nullptr)
            SetFileAddressToTable(fileId, fileAddress);
    }

    return fileAddress;
}

void* SoundArchiveLoader::LoadFile(SoundArchive::FileId fileId, SoundMemoryAllocatable* allocator,
                                   size_t loadBlockSize, bool needMemoryPool) {
    SoundArchive::FileInfo fileInfo;
    if (!m_pSoundArchive->detail_ReadFileInfo(fileId, &fileInfo))
        return nullptr;

    u32 fileSize{fileInfo.fileSize};

    if (fileSize + 1 <= 1)
        return nullptr;

    void* buffer{allocator->Allocate(allocator->GetAllocateSize(fileSize, needMemoryPool))};

    if (buffer == nullptr)
        return nullptr;

    if (ReadFile(fileId, buffer, static_cast<int>(fileSize), 0, loadBlockSize) !=
        static_cast<size_t>(static_cast<int>(fileSize)))
        return nullptr;

    driver::HardwareManager::FlushDataCache(buffer, fileSize);
    return buffer;
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

bool SoundArchiveLoader::LoadIndividualWave(SoundArchive::ItemId warcId, u32 waveIndex,
                                            SoundMemoryAllocatable* pAllocator,
                                            size_t loadBlockSize) {
    u32 fileId{m_pSoundArchive->GetItemFileId(warcId)};
    const void* pWaveArchiveFile{GetFileAddressImpl(fileId)};

    if (pWaveArchiveFile == nullptr) {
        pWaveArchiveFile = LoadWaveArchiveTable(warcId, pAllocator, loadBlockSize);

        if (pWaveArchiveFile == nullptr)
            return false;
    }

    WaveArchiveFileReader reader{pWaveArchiveFile, true};

    if (reader.IsLoaded(waveIndex))
        return true;

    const size_t WaveFileSize{reader.GetWaveFileSize(waveIndex)};
    const size_t RequiredSize{
        util::align_up(WaveFileSize + sizeof(IndividualWaveInfo) + WaveBufferAlignSize,
                       fnd::Thread::StackAlignment)};

    void* buffer{pAllocator->Allocate(RequiredSize)};
    if (buffer == nullptr)
        return false;

    u8* pAlignedBuffer{util::BytePtr(buffer).AlignUp(WaveBufferAlignSize).Get<u8>()};
    {
        IndividualWaveInfo iWavInfo{fileId, waveIndex};
        std::memcpy(pAlignedBuffer, &iWavInfo, sizeof(IndividualWaveInfo));
    }

    {
        void* loadingAddress{pAlignedBuffer + sizeof(IndividualWaveInfo)};

        size_t readSize{ReadFile(fileId, loadingAddress, WaveFileSize,
                                 static_cast<int>(reader.GetWaveFileOffsetFromFileHead(waveIndex)),
                                 loadBlockSize)};

        if (readSize != WaveFileSize)
            return false;

        reader.SetWaveFile(waveIndex, loadingAddress);
        driver::HardwareManager::FlushDataCache(loadingAddress, WaveFileSize);
    }

    return true;
}

const void* SoundArchiveLoader::LoadWaveArchiveTable(SoundArchive::ItemId warcId,
                                                     SoundMemoryAllocatable* pAllocator,
                                                     size_t loadBlockSize) {
    u32 fileId{m_pSoundArchive->GetItemFileId(warcId)};

    const void* pWaveArchiveFile{GetFileAddressImpl(fileId)};
    if (pWaveArchiveFile != nullptr)
        return pWaveArchiveFile;

    u32 waveCount;
    {
        SoundArchive::WaveArchiveInfo info;
        if (!m_pSoundArchive->ReadWaveArchiveInfo(warcId, &info))
            return nullptr;

        if (info.waveCount == 0)
            return nullptr;

        waveCount = info.waveCount;
    }

    position_t fileBlockOffset;
    {
        char pBuffer[sizeof(WaveArchiveFile::FileHeader)];
        size_t readSize{
            ReadFile(fileId, pBuffer, sizeof(WaveArchiveFile::FileHeader), 0, loadBlockSize)};

        if (readSize != sizeof(WaveArchiveFile::FileHeader))
            return nullptr;

        const WaveArchiveFile::FileHeader* pHeader{
            reinterpret_cast<const WaveArchiveFile::FileHeader*>(pBuffer)};

        fileBlockOffset = pHeader->GetFileBlockOffset();
        position_t infoBlockOffset{pHeader->GetInfoBlockOffset()};

        if (infoBlockOffset > fileBlockOffset)
            return nullptr;
    }

    const size_t RequiredSize{fileBlockOffset + static_cast<size_t>(waveCount) * 8 + 4};

    void* buffer{pAllocator->Allocate(RequiredSize)};
    if (buffer == nullptr)
        return nullptr;

    {
        size_t readSize{ReadFile(fileId, buffer, fileBlockOffset, 0, loadBlockSize)};
        if (readSize != static_cast<size_t>(fileBlockOffset))
            return nullptr;
    }

    util::BytePtr(buffer, fileBlockOffset).Get<WaveArchiveFile::FileBlock>()->header.kind =
        static_cast<int>(WaveArchiveFileReader::SignatureWarcTable);

    WaveArchiveFileReader reader{buffer, true};
    reader.InitializeFileTable();

    SetFileAddressToTable(fileId, buffer);
    return buffer;
}

// NON_MATCHING: something is wrong with they way ptr and restSize are checked and modified
size_t SoundArchiveLoader::ReadFile(SoundArchive::FileId fileId, void* buffer, size_t size,
                                    int offset, size_t loadBlockSize) {
    FileStreamHandle stream{m_pSoundArchive->detail_OpenFileStream(
        fileId, m_StreamArea, sizeof(m_StreamArea), nullptr, 0)};

    if (stream && stream->CanSeek() && stream->CanRead()) {
        stream->Seek(offset, fnd::FileStream::SeekOrigin_Begin);

        if (loadBlockSize != 0) {
            u8* ptr{static_cast<u8*>(buffer)};
            size_t restSize{size};

            while (restSize != 0) {
                fnd::FndResult result;
                size_t curReadingSize{std::min(loadBlockSize, restSize)};
                size_t readByte{stream->Read(ptr, curReadingSize, &result)};
                if (result.IsFailed())
                    return 0;

                ptr += readByte;
                restSize -= readByte;
                if (restSize + readByte <= readByte || restSize - readByte == 0)
                    return restSize;
            }
        } else {
            fnd::FndResult result;
            stream->Read(buffer, size, &result);
            if (!result.IsFailed())
                return size;
        }
    }

    return 0;
}

// NON_MATCHING: unknown reason
bool SoundArchiveLoader::PostProcessForLoadedGroupFile(const void* pGroupFile,
                                                       SoundMemoryAllocatable* pAllocator,
                                                       size_t loadBlockSize) {
    GroupFileReader reader{pGroupFile};
    bool isLinkGroup{false};
    u32 groupItemCount{reader.GetGroupItemCount()};

    for (u32 i{0}; i < groupItemCount; ++i) {
        GroupItemLocationInfo info;
        if (!reader.ReadGroupItemLocationInfo(&info, i))
            return false;

        if (info.address == nullptr)
            isLinkGroup = i == 0;
        else
            SetFileAddressToTable(info.fileId, info.address);

        if (isLinkGroup) {
            u32 groupItemInfoExCount{reader.GetGroupItemExCount()};
            if (groupItemInfoExCount == 0)
                return true;

            for (u32 i{0}; i < groupItemInfoExCount; ++i) {
                GroupFile::GroupItemInfoEx infoEx;
                if (!reader.ReadGroupItemInfoEx(&infoEx, i))
                    return false;

                if (!LoadData(infoEx.itemId, pAllocator, infoEx.loadFlag, loadBlockSize))
                    return false;
            }

            return true;
        }
    }

    u32 groupItemInfoExCount{reader.GetGroupItemExCount()};
    if (groupItemInfoExCount == 0)
        return true;

    for (u32 i{0}; i != groupItemInfoExCount; ++i) {
        GroupFile::GroupItemInfoEx infoEx;
        if (!reader.ReadGroupItemInfoEx(&infoEx, i))
            break;

        if ((infoEx.loadFlag & LoadFlag_Warc) == 0)
            break;

        switch (Util::GetItemType(infoEx.itemId)) {
        case ItemType_Sound: {
            if (m_pSoundArchive->GetSoundType(infoEx.itemId) == SoundArchive::SoundType_Sequence)
                SetWaveArchiveTableWithSeqInEmbeddedGroup(infoEx.itemId, pAllocator);
            break;
        }
        case ItemType_SoundGroup: {
            SoundArchive::SoundGroupInfo info;
            if (!m_pSoundArchive->detail_ReadSoundGroupInfo(infoEx.itemId, &info))
                break;

            if (info.startId == SoundArchive::InvalidId)
                break;

            switch (m_pSoundArchive->GetSoundType(info.startId)) {
            case SoundArchive::SoundType_Sequence:
                for (u32 id{info.startId}; id <= info.endId; ++id) {
                    SetWaveArchiveTableWithSeqInEmbeddedGroup(id, pAllocator);
                }
                break;

            case SoundArchive::SoundType_Wave:
                SetWaveArchiveTableWithWsdInEmbeddedGroup(info.startId, pAllocator);
                break;

            default:
                break;
            }

            break;
        }
        case ItemType_Bank: {
            SetWaveArchiveTableWithBankInEmbeddedGroup(infoEx.itemId, pAllocator);
            break;
        }
        case ItemType_Player:
        case ItemType_WaveArchive:
        case ItemType_Group:
            break;
        }
    }

    return true;
}

void SoundArchiveLoader::SetWaveArchiveTableWithSeqInEmbeddedGroup(
    SoundArchive::ItemId seqId, SoundMemoryAllocatable* pAllocator) {
    SoundArchive::SequenceSoundInfo info;
    if (!m_pSoundArchive->ReadSequenceSoundInfo(&info, seqId))
        return;

    for (int i{0}; i < static_cast<int>(SoundArchive::SequenceBankMax); ++i)
        SetWaveArchiveTableWithBankInEmbeddedGroup(info.bankIds[i], pAllocator);
}

void SoundArchiveLoader::SetWaveArchiveTableWithBankInEmbeddedGroup(
    SoundArchive::ItemId bankId, SoundMemoryAllocatable* pAllocator) {
    if (bankId == SoundArchive::InvalidId)
        return;

    SoundArchive::BankInfo bankInfo;
    if (!m_pSoundArchive->ReadBankInfo(&bankInfo, bankId))
        return;

    const void* bankFile{GetFileAddressImpl(bankInfo.fileId)};
    if (bankFile == nullptr)
        return;

    BankFileReader reader{bankFile};
    const Util::WaveIdTable* table{reader.GetWaveIdTable()};
    if (table == nullptr)
        return;

    const Util::WaveId* pWaveId{table->GetWaveId(0)};
    if (pWaveId == nullptr)
        return;

    SoundArchive::ItemId warcId{pWaveId->waveArchiveId};
    SetWaveArchiveTableInEmbeddedGroupImpl(warcId, pAllocator);
}

void SoundArchiveLoader::SetWaveArchiveTableWithWsdInEmbeddedGroup(
    SoundArchive::ItemId wsdId, SoundMemoryAllocatable* pAllocator) {
    if (wsdId == SoundArchive::InvalidId)
        return;

    SoundArchive::SoundInfo soundInfo;
    if (!m_pSoundArchive->ReadSoundInfo(&soundInfo, wsdId))
        return;

    SoundArchive::WaveSoundInfo wsdInfo;
    if (!m_pSoundArchive->detail_ReadWaveSoundInfo(wsdId, &wsdInfo))
        return;

    const void* wsdFile{GetFileAddressImpl(soundInfo.fileId)};
    if (wsdFile == nullptr)
        return;

    WaveSoundFileReader reader{wsdFile};
    WaveSoundNoteInfo noteInfo;
    if (!reader.ReadNoteInfo(&noteInfo, wsdInfo.index, 0))
        return;

    SetWaveArchiveTableInEmbeddedGroupImpl(noteInfo.waveArchiveId, pAllocator);
}

void SoundArchiveLoader::SetWaveArchiveTableInEmbeddedGroupImpl(
    SoundArchive::ItemId warcId, SoundMemoryAllocatable* pAllocator) {
    SoundArchive::WaveArchiveInfo info;
    if (!m_pSoundArchive->ReadWaveArchiveInfo(warcId, &info))
        return;

    if (!info.isLoadIndividual)
        return;

    const void* warcFile{GetFileAddressImpl(info.fileId)};
    WaveArchiveFileReader loadedFileReader{warcFile, false};
    if (loadedFileReader.HasIndividualLoadTable())
        return;

    u32 fileBlockOffset{
        static_cast<const WaveArchiveFile::FileHeader*>(warcFile)->GetFileBlockOffset()};

    const u32 RequiredTableSize{
        static_cast<u32>(fileBlockOffset + info.waveCount * sizeof(u32) + sizeof(int))};

    void* buffer{pAllocator->Allocate(RequiredTableSize)};
    if (buffer == nullptr)
        return;

    std::memcpy(buffer, warcFile, fileBlockOffset);
    util::BytePtr(buffer, fileBlockOffset).Get<WaveArchiveFile::FileBlock>()->header.kind =
        static_cast<int>(WaveArchiveFileReader::SignatureWarcTable);

    WaveArchiveFileReader reader{buffer, true};
    reader.InitializeFileTable();

    for (u32 i{0}; i < info.waveCount; ++i)
        reader.SetWaveFile(i, loadedFileReader.GetWaveFile(i));

    SetFileAddressToTable(info.fileId, buffer);
}

bool SoundArchiveLoader::IsDataLoaded(const char* pItemName, u32 loadFlag) const {
    SoundArchive::ItemId id{m_pSoundArchive->GetItemId(pItemName)};
    return IsDataLoaded(id, loadFlag);
}

bool SoundArchiveLoader::IsDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const {
    if (!IsAvailable())
        return false;

    if (itemId == SoundArchive::InvalidId)
        return false;

    switch (Util::GetItemType(itemId)) {
    case ItemType_Sound:
        switch (m_pSoundArchive->GetSoundType(itemId)) {
        case SoundArchive::SoundType_Sequence:
            return IsSequenceSoundDataLoaded(itemId, loadFlag);

        case SoundArchive::SoundType_Wave:
            return IsWaveSoundDataLoaded(itemId, loadFlag);

        default:
            return false;
        }
        break;
    case ItemType_SoundGroup:
        return IsSoundGroupDataLoaded(itemId, loadFlag);

    case ItemType_Bank:
        return IsBankDataLoaded(itemId, loadFlag);

    case ItemType_Player:
        return false;

    case ItemType_WaveArchive:
        return IsWaveArchiveDataLoaded(itemId, SoundArchive::InvalidId);

    case ItemType_Group:
        return IsGroupDataLoaded(itemId);
    }

    return false;
}

bool SoundArchiveLoader::IsSequenceSoundDataLoaded(SoundArchive::ItemId itemId,
                                                   u32 loadFlag) const {
    if ((loadFlag & LoadFlag_Seq) != 0) {
        u32 fileId{m_pSoundArchive->GetItemFileId(itemId)};

        if (GetFileAddressImpl(fileId) == nullptr)
            return false;
    }

    if ((loadFlag & (LoadFlag_Warc | LoadFlag_Bank)) != 0) {
        SoundArchive::SequenceSoundInfo info;
        if (!m_pSoundArchive->ReadSequenceSoundInfo(&info, itemId))
            return false;

        for (int i{0}; i < static_cast<int>(SoundArchive::SequenceBankMax); ++i) {
            u32 bankId{info.bankIds[i]};
            if (bankId != SoundArchive::InvalidId && !IsBankDataLoaded(bankId, loadFlag))
                return false;
        }
    }

    return true;
}

bool SoundArchiveLoader::IsWaveSoundDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const {
    const void* pWsdFile;
    {
        u32 wsdFileId{m_pSoundArchive->GetItemFileId(itemId)};
        pWsdFile = GetFileAddressImpl(wsdFileId);
    }

    if ((loadFlag & LoadFlag_Wsd) != 0 || (loadFlag & LoadFlag_Warc) != 0) {
        if (pWsdFile == nullptr)
            return false;
    }

    if ((loadFlag & LoadFlag_Warc) != 0) {
        u32 index;
        {
            SoundArchive::WaveSoundInfo info;
            if (!m_pSoundArchive->detail_ReadWaveSoundInfo(itemId, &info))
                return false;

            index = info.index;
        }

        u32 warcId;
        u32 waveIndex;
        {
            WaveSoundFileReader reader{pWsdFile};
            WaveSoundNoteInfo info;
            if (!reader.ReadNoteInfo(&info, index, 0))
                return false;

            warcId = info.waveArchiveId;
            waveIndex = info.waveIndex;
        }

        if (!IsWaveArchiveDataLoaded(warcId, waveIndex))
            return false;
    }

    return true;
}

bool SoundArchiveLoader::IsBankDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const {
    const void* pBankFile;
    {
        u32 fileId{m_pSoundArchive->GetItemFileId(itemId)};
        pBankFile = GetFileAddressImpl(fileId);
    }

    if ((loadFlag & LoadFlag_Bank) != 0 || (loadFlag & LoadFlag_Warc) != 0) {
        if (pBankFile == nullptr)
            return false;
    }

    if ((loadFlag & LoadFlag_Warc) != 0) {
        BankFileReader reader{pBankFile};
        const Util::WaveIdTable* table{reader.GetWaveIdTable()};

        if (table == nullptr)
            return false;

        for (u32 i{0}; i < table->GetCount(); ++i) {
            const Util::WaveId* pWaveId{table->GetWaveId(i)};

            u32 warcId{pWaveId->waveArchiveId};
            u32 waveIndex{pWaveId->waveIndex};

            if (!IsWaveArchiveDataLoaded(warcId, waveIndex))
                return false;
        }
    }

    return true;
}

bool SoundArchiveLoader::IsWaveArchiveDataLoaded(SoundArchive::ItemId itemId, u32 waveIndex) const {
    const void* pWarcFile;
    {
        u32 fileId{m_pSoundArchive->GetItemFileId(itemId)};
        pWarcFile = GetFileAddressImpl(fileId);
    }

    if (pWarcFile == nullptr)
        return false;

    SoundArchive::WaveArchiveInfo info;
    if (!m_pSoundArchive->ReadWaveArchiveInfo(itemId, &info))
        return false;

    if (info.isLoadIndividual) {
        WaveArchiveFileReader reader{pWarcFile, info.isLoadIndividual};
        if (waveIndex != SoundArchive::InvalidId && !reader.IsLoaded(waveIndex))
            return false;

        if (waveIndex == SoundArchive::InvalidId) {
            u32 waveCount{reader.GetWaveFileCount()};
            for (u32 i{0}; i < waveCount; ++i) {
                if (!reader.IsLoaded(i))
                    return false;
            }
        }
    }

    return true;
}

bool SoundArchiveLoader::IsGroupDataLoaded(SoundArchive::ItemId itemId) const {
    u32 fileId{m_pSoundArchive->GetItemFileId(itemId)};
    return GetFileAddressImpl(fileId) != nullptr;
}

bool SoundArchiveLoader::IsSoundGroupDataLoaded(SoundArchive::ItemId itemId, u32 loadFlag) const {
    SoundArchive::SoundGroupInfo info;
    if (!m_pSoundArchive->detail_ReadSoundGroupInfo(itemId, &info))
        return false;

    switch (m_pSoundArchive->GetSoundType(info.startId)) {
    case SoundArchive::SoundType_Sequence:
        for (u32 id{info.startId}; id <= info.endId; ++id) {
            if (!IsSequenceSoundDataLoaded(id, loadFlag))
                return false;
        }
        break;

    case SoundArchive::SoundType_Wave:
        for (u32 id{info.startId}; id <= info.endId; ++id) {
            if (!IsWaveSoundDataLoaded(id, loadFlag))
                return false;
        }
        break;

    default:
        break;
    }

    return true;
}

const void* SoundArchiveLoader::GetFileAddressFromSoundArchive(SoundArchive::FileId fileId) const {
    if (m_pSoundArchive == nullptr)
        return nullptr;

    return m_pSoundArchive->detail_GetFileAddress(fileId);
}

const void* SoundArchiveLoader::detail_GetFileAddressByItemId(SoundArchive::ItemId itemId) const {
    if (m_pSoundArchive == nullptr)
        return nullptr;

    SoundArchive::FileId fileId{SoundArchive::InvalidId};
    const void* result{};

    switch (Util::GetItemType(itemId)) {
    case ItemType_Sound: {
        SoundArchive::SoundInfo info;
        m_pSoundArchive->ReadSoundInfo(&info, itemId);
        fileId = info.fileId;
        break;
    }
    case ItemType_SoundGroup:
        break;

    case ItemType_Bank: {
        SoundArchive::BankInfo info;
        m_pSoundArchive->ReadBankInfo(&info, itemId);
        fileId = info.fileId;
        break;
    }
    case ItemType_Player:
        break;

    case ItemType_WaveArchive: {
        SoundArchive::WaveArchiveInfo info;
        m_pSoundArchive->ReadWaveArchiveInfo(itemId, &info);
        fileId = info.fileId;
        break;
    }
    case ItemType_Group: {
        SoundArchive::GroupInfo info;
        m_pSoundArchive->ReadGroupInfo(&info, itemId);
        fileId = info.fileId;
        break;
    }
    }

    if (fileId != SoundArchive::InvalidId)
        result = GetFileAddressImpl(fileId);

    return result;
}

bool SoundArchiveLoader::detail_LoadWaveArchiveByBankFile(const void* bankFile,
                                                          SoundMemoryAllocatable* pAllocator) {
    if (bankFile == nullptr)
        return false;

    BankFileReader reader{bankFile};
    const Util::WaveIdTable* table{reader.GetWaveIdTable()};

    if (table == nullptr)
        return false;

    for (u32 i{0}; i < table->GetCount(); ++i) {
        const Util::WaveId* pWaveId{table->GetWaveId(i)};

        if (!LoadWaveArchiveImpl(pWaveId->waveArchiveId, pWaveId->waveIndex, pAllocator,
                                 LoadFlag_Warc, 0))
            return false;
    }

    return true;
}

bool SoundArchiveLoader::detail_LoadWaveArchiveByWaveSoundFile(const void* wsdFile, int wsdIndex,
                                                               SoundMemoryAllocatable* pAllocator) {
    if (wsdFile == nullptr)
        return false;

    u32 warcId{SoundArchive::InvalidId};
    u32 waveIndex;
    {
        WaveSoundFileReader reader{wsdFile};
        WaveSoundNoteInfo info;

        if (!reader.ReadNoteInfo(&info, wsdIndex, 0))
            return false;

        warcId = info.waveArchiveId;
        waveIndex = info.waveIndex;
    }

    if (!LoadWaveArchiveImpl(warcId, waveIndex, pAllocator, LoadFlag_Warc, 0))
        return false;

    return true;
}

}  // namespace nn::atk::detail
