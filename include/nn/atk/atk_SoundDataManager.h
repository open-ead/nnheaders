#pragma once

#include <nn/atk/atk_DisposeCallback.h>
#include <nn/atk/atk_SoundArchiveLoader.h>
#include <nn/atk/atk_Util.h>

namespace nn::atk {

namespace detail {

class SoundFileManager {
public:
    virtual ~SoundFileManager() = default;
    virtual const void* GetFileAddressImpl(SoundArchive::FileId fileId);
};

}  // namespace detail

class SoundDataManager : public detail::driver::DisposeCallback, public detail::SoundArchiveLoader {
public:
    static const u32 BufferAlignSize{8};

    SoundDataManager();
    ~SoundDataManager() override;

    size_t GetRequiredMemSize(const SoundArchive* arc) const;

    bool Initialize(const SoundArchive* pArchive, void* buffer, size_t size);
    void Finalize();

    void detail_SetFileManager(detail::SoundFileManager* pFileManager) {
        m_pFileManager = pFileManager;
    }

    const void* SetFileAddress(SoundArchive::FileId fileId, const void* address);

    bool SetFileAddressInGroupFile(const void* address, size_t size);
    void ClearFileAddressInGroupFile(const void* address, size_t size);

    const void* detail_GetFileAddress(SoundArchive::FileId fileId) const;
    SoundArchive::FileId detail_GetFileIdFromTable(const void* address) const;

    void InvalidateSoundData(const void* address, size_t size);

protected:
    void InvalidateData(const void* start, const void* end) override;

    const void* SetFileAddressToTable(SoundArchive::FileId fileId, const void* address) override;
    const void* GetFileAddressFromTable(SoundArchive::FileId fileId) const override;
    const void* GetFileAddressImpl(SoundArchive::FileId fileId) const override;

private:
    bool CreateTables(void** pOutBuffer, const SoundArchive* pArchive, void* endAddress);

    struct FileAddress {
        const void* address;
    };
    static_assert(sizeof(FileAddress) == 0x8);

    using FileTable = detail::Util::Table<FileAddress>;

    FileTable* m_pFileTable{};
    detail::SoundFileManager* m_pFileManager{};
};
static_assert(sizeof(SoundDataManager) == 0x240);

}  // namespace nn::atk
