#pragma once

#include <nn/types.h>
#include <nn/util.h>

namespace nn::atk::detail {

struct BinaryTypes {
    static const u32 InvalidOffset{0xffffffff};
    static const u32 InvalidSize{0xffffffff};

    struct Reference {
        u32 offset;

        static const u32 InvalidOffset{BinaryTypes::InvalidOffset};

        bool IsValidOffset() const { return offset != InvalidOffset; }
    };
    static_assert(sizeof(Reference) == 0x4);

    template <typename ItemType, typename CountType = int>
    struct Table {
        CountType count;
        ItemType item[1];
    };

    struct ReferenceTable : Table<Reference> {
        const void* GetReferedItem(int index) const;
    };
};

};  // namespace nn::atk::detail
