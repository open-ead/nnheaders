#pragma once

#include <nn/types.h>
#include <nn/util.h>

namespace nn::atk::detail {
struct BinaryTypes {
    NN_NO_COPY(BinaryTypes);
    NN_NO_MOVE(BinaryTypes);

    template <typename T, typename CountType = u32>
    struct Table {
        CountType count;
        T item[1];
    };

    struct Reference {
        u32 offset;
    };
    static_assert(sizeof(Reference) == 0x4);

    struct ReferenceTable : Table<Reference> {};
};

};