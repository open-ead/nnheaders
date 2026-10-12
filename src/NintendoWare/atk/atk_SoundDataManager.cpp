#include <nn/atk/atk_SoundDataManager.h>

#include <nn/atk/atk_DriverCommand.h>
#include <nn/atk/atk_WaveArchiveFileReader.h>
#include "nn/atk/atk_GroupFileReader.h"

namespace nn::atk {

SoundDataManager::SoundDataManager() = default;

SoundDataManager::~SoundDataManager() = default;

size_t SoundDataManager::GetRequiredMemSize(const SoundArchive* arc) const {
    size_t size{0};
    size += arc->detail_GetFileCount() * sizeof(FileAddress) + 4;
    size = util::align_up(size, BufferAlignSize);

    return size;
}

bool SoundDataManager::Initialize(const SoundArchive* pArchive, void* buffer, size_t size) {
    void* endp{util::BytePtr(buffer, static_cast<ptrdiff_t>(size)).Get()};
    void* buf{buffer};

    if (!CreateTables(&buf, pArchive, endp))
        return false;

    SetSoundArchive(pArchive);

    detail::DriverCommand& cmdmgr{detail::DriverCommand::GetInstance()};

    auto* command{cmdmgr.AllocCommand<detail::DriverCommandDisposeCallback>()};
#if NN_WARE_VER < NN_MAKE_VER(4, 0, 0)
    if (command == nullptr)
        return false;
#endif

    command->id = detail::DriverCommandId_RegistDisposeCallback;
    command->callback = this;
    cmdmgr.PushCommand(command);
    return true;
}

bool SoundDataManager::CreateTables(void** pOutBuffer, const SoundArchive* pArchive,
                                    void* endAddress) {
    size_t fileTableSize{pArchive->detail_GetFileCount()};
    fileTableSize *= sizeof(FileAddress);
    fileTableSize += 4;

    void* ep{util::BytePtr(*pOutBuffer, static_cast<ptrdiff_t>(fileTableSize))
                 .AlignUp(BufferAlignSize)
                 .Get()};
    if (util::BytePtr(endAddress).Distance(ep) > 0)
        return false;

    m_pFileTable = reinterpret_cast<FileTable*>(*pOutBuffer);
    *pOutBuffer = ep;

    m_pFileTable->count = pArchive->detail_GetFileCount();

    for (u32 i{0}; i < m_pFileTable->count; ++i)
        m_pFileTable->item[i].address = nullptr;

    return true;
}

void SoundDataManager::Finalize() {
    detail::DriverCommand& cmdmgr{detail::DriverCommand::GetInstance()};

    auto* command{cmdmgr.AllocCommand<detail::DriverCommandDisposeCallback>()};
#if NN_WARE_VER < NN_MAKE_VER(4, 0, 0)
    if (command == nullptr)
        return;
#endif
    command->id = detail::DriverCommandId_UnregistDisposeCallback;
    command->callback = this;

    cmdmgr.PushCommand(command);
    u32 tag{cmdmgr.FlushCommand(true)};
    cmdmgr.WaitCommandReply(tag);

    m_pFileManager = nullptr;
    m_pFileTable = nullptr;
}

void SoundDataManager::InvalidateData(const void* start, const void* end) {
    if (m_pFileTable != nullptr) {
        for (u32 i{0}; i < m_pFileTable->count; ++i) {
            const void* addr{m_pFileTable->item[i].address};

            if (start <= addr && addr <= end)
                m_pFileTable->item[i].address = nullptr;
        }
    }

    const SoundArchive* arc{GetSoundArchive()};
    if (arc == nullptr)
        return;

    u32 waveArchiveCount{arc->GetWaveArchiveCount()};

    for (u32 i{0}; i < waveArchiveCount; ++i) {
        SoundArchive::WaveArchiveInfo info;
        if (!arc->ReadWaveArchiveInfo(SoundArchive::GetWaveArchiveIdFromIndex(i), &info))
            continue;

        if (!info.isLoadIndividual)
            continue;

        const void* pWarcTable{GetFileAddressFromTable(info.fileId)};
        if (pWarcTable == nullptr)
            continue;

        detail::WaveArchiveFileReader reader{pWarcTable, true};

        for (u32 j{0}; j < info.waveCount; ++j) {
            const void* waveAddr{reader.GetWaveFile(j)};

            if (waveAddr != nullptr && start <= waveAddr && waveAddr <= end)
                reader.SetWaveFile(j, nullptr);
        }
    }
}

const void* SoundDataManager::detail_GetFileAddress(SoundArchive::FileId fileId) const {
    return GetFileAddressImpl(fileId);
}

const void* SoundDataManager::GetFileAddressImpl(SoundArchive::FileId fileId) const {
    if (m_pFileManager != nullptr) {
        const void* addr{m_pFileManager->GetFileAddressImpl(fileId)};
        if (addr != nullptr)
            return addr;
    }

    {
        const void* addr{GetFileAddressFromSoundArchive(fileId)};
        if (addr != nullptr)
            return addr;
    }

    {
        const void* fileData{GetFileAddressFromTable(fileId)};
        return fileData;
    }
}

const void* SoundDataManager::SetFileAddressToTable(SoundArchive::FileId fileId,
                                                    const void* address) {
    if (m_pFileTable == nullptr)
        return nullptr;

    const void* preAddress{m_pFileTable->item[fileId].address};
    m_pFileTable->item[fileId].address = address;
    return preAddress;
}

const void* SoundDataManager::GetFileAddressFromTable(SoundArchive::FileId fileId) const {
    if (m_pFileTable == nullptr)
        return nullptr;

    if (fileId >= m_pFileTable->count)
        return nullptr;

    return m_pFileTable->item[fileId].address;
}

const void* SoundDataManager::SetFileAddress(SoundArchive::FileId fileId, const void* address) {
    return SetFileAddressToTable(fileId, address);
}

SoundArchive::FileId SoundDataManager::detail_GetFileIdFromTable(const void* address) const {
    for (u32 i{0}; i < m_pFileTable->count; ++i) {
        if (m_pFileTable->item[i].address == address)
            return i;
    }

    return SoundArchive::InvalidId;
}

bool SoundDataManager::SetFileAddressInGroupFile(const void* address,
                                                 [[maybe_unused]] size_t size) {
    if (address == nullptr)
        return false;

    detail::GroupFileReader reader{address};
    u32 groupItemCount{reader.GetGroupItemCount()};

    for (u32 i{0}; i < groupItemCount; ++i) {
        detail::GroupItemLocationInfo info;
        if (!reader.ReadGroupItemLocationInfo(&info, i))
            return false;

        if (info.address == nullptr)
            return false;

        SetFileAddressToTable(info.fileId, info.address);
    }

    return true;
}

void SoundDataManager::ClearFileAddressInGroupFile(const void* address, size_t size) {
    InvalidateSoundData(address, size);
}

void SoundDataManager::InvalidateSoundData(const void* address, size_t size) {
    detail::DriverCommand& cmdmgr{detail::DriverCommand::GetInstance()};
    if (cmdmgr.IsAvailable()) {
        auto* command{cmdmgr.AllocCommand<detail::DriverCommandInvalidateData>()};
        if (command == nullptr)
            return;

        command->id = detail::DriverCommandId_InvalidateData;
        command->mem = address;
        command->size = size;

        cmdmgr.PushCommand(command);
        u32 tag{cmdmgr.FlushCommand(true)};
        cmdmgr.WaitCommandReply(tag);
    }
}

}  // namespace nn::atk
