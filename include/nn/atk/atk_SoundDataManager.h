#pragma once

#include <nn/atk/detail/atk_DisposeCallback.h>
#include <nn/atk/detail/atk_SoundArchiveLoader.h>
#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk {

namespace detail {
class SoundFileManager;
} // namespace nn::atk::detail

class SoundDataManager : detail::driver::DisposeCallback, detail::SoundArchiveLoader {
public:
    struct FileAddress {
        void* address;
    };

    using FileTable = detail::Util::Table<FileAddress>;

    constexpr static u32 BufferAlignSize = 8;

    SoundDataManager();
    ~SoundDataManager() override;

    size_t GetRequiredMemSize(const SoundArchive* arc) const;

    bool Initialize(const SoundArchive* pArchive, void* buffer, size_t size);
    
    bool CreateTables(void**, const SoundArchive* pArchive, void*);
    
    void Finalize();

    void InvalidateData(const void* start, const void* end) override;

    void* detail_GetFileAddress(SoundArchive::FileId fileId) const;
    void* GetFileAddressImpl(SoundArchive::FileId fileId) const override;

    void* SetFileAddressToTable(SoundArchive::FileId fileId, const void* address) override;
    void* GetFileAddressFromTable(SoundArchive::FileId fileId) const override;

    void* SetFileAddress(SoundArchive::FileId fileId, const void* address);

    SoundArchive::FileId detail_GetFileIdFromTable(const void*) const;

    bool SetFileAddressInGroupFile(const void*, size_t);
    void ClearFileAddressInGroupFile(const void*, size_t);

    void InvalidateSoundData(const void*, size_t);

private:
    FileTable* m_pFileTable;
    detail::SoundFileManager* m_pFileManager;
};
static_assert(sizeof(SoundDataManager) == 0x240);
}  // namespace nn::atk
