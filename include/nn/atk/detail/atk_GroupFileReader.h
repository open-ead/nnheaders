#pragma once

#include <nn/atk/detail/atk_GroupFile.h>

namespace nn::atk::detail {
struct GroupItemLocationInfo {
    u32 fileId;
    void* address;
};
static_assert(sizeof(GroupItemLocationInfo) == 0x10);

class GroupFileReader {
public:
    constexpr static u64 SignatureFile = 0x50524746; // FGRP

    explicit GroupFileReader(const void* groupFile);

    bool ReadGroupItemLocationInfo(GroupItemLocationInfo* out, u32 index) const;

    u32 GetGroupItemExCount() const;

    bool ReadGroupItemInfoEx(GroupFile::GroupItemInfoEx* out, u32 index) const;

private:
    GroupFile::InfoBlockBody* m_pInfoBlockBody;
    GroupFile::FileBlockBody* m_pFileBlockBody;
    GroupFile::InfoExBlockBody* m_pInfoExBlockBody;
};
static_assert(sizeof(GroupFileReader) == 0x18);
} // namespace nn::atk::detail