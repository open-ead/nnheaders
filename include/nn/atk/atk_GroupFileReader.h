#pragma once

#include <nn/atk/atk_GroupFile.h>

namespace nn::atk::detail {

struct GroupItemLocationInfo {
    u32 fileId;
    const void* address;
};
static_assert(sizeof(GroupItemLocationInfo) == 0x10);

class GroupFileReader {
public:
    static const u32 SignatureFile{0x50524746};  // FGRP

    explicit GroupFileReader(const void* groupFile);

    u32 GetGroupItemCount() const { return m_pInfoBlockBody->GetGroupItemInfoCount(); }

    bool ReadGroupItemLocationInfo(GroupItemLocationInfo* out, u32 index) const;

    u32 GetGroupItemExCount() const;

    bool ReadGroupItemInfoEx(GroupFile::GroupItemInfoEx* out, u32 index) const;

private:
    const GroupFile::InfoBlockBody* m_pInfoBlockBody{};
    const GroupFile::FileBlockBody* m_pFileBlockBody{};
    const GroupFile::InfoExBlockBody* m_pInfoExBlockBody{};
};
static_assert(sizeof(GroupFileReader) == 0x18);

}  // namespace nn::atk::detail