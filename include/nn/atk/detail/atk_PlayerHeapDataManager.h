#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/atk_DisposeCallback.h>
#include <nn/atk/detail/atk_SoundArchiveLoader.h>

namespace nn::atk::detail {
class PlayerHeapDataManager : driver::DisposeCallback, SoundArchiveLoader {
public:
    constexpr static u8 FileAddressCount = 9;

    struct FileAddress {
        SoundArchive::FileId fileId;
        void* address;
    };
    static_assert(sizeof(FileAddress) == 0x10);

    PlayerHeapDataManager();
    ~PlayerHeapDataManager() override;

    void Finalize();
    void Initialize(const SoundArchive* arc);

    void* SetFileAddress(SoundArchive::FileId fileId, const void* address);
    void* GetFileAddress(SoundArchive::FileId fileId) const;

    void InvalidateData(const void* start, const void* end) override;

    void* SetFileAddressToTable(SoundArchive::FileId fileId, const void* address) override;
    void* GetFileAddressFromTable(SoundArchive::FileId fileId) const override;
    void* GetFileAddressImpl(SoundArchive::FileId fileId) const override;

private:
    FileAddress m_FileAddress[9];
    bool m_IsInitialized;
    bool m_IsFinalized;
};
static_assert(sizeof(PlayerHeapDataManager) == 0x2c8);
} // namespace nn::atk::detail