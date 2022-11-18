#pragma once

#include "nn/util/BinaryTypes.h"

namespace nn::gfx {

struct ResUserDataData {
    util::BinPtrToString pName;
    util::BinPtr pData;
    uint32_t count;
    uint8_t type;
    char reserved[43];
};

}  // namespace nn::gfx
