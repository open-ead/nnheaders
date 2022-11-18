#pragma once

#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/nn_BitTypes.h>
#include <nn/util/util_BitPack.h>

namespace nn::gfx {

template <>
struct DescriptorPoolImplData<NvnApi> {
    Bit8 state;
    Bit8 descriptorPoolType;
    util::BitPack8 flags;
    char reserved;
    int32_t slotCount;
    int32_t reservedSlots;
    char reserved2[4];
    detail::Ptr<void> pDescriptorPool;
    char nvnDescriptorPool[32];
    detail::Ptr<void> userPtr;
};

}  // namespace nn::gfx