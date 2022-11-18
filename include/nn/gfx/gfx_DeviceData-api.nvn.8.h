#pragma once

#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/nn_BitTypes.h>
#include <nn/util/util_BitPack.h>

namespace nn::gfx {

template <>
struct DeviceImplData<NvnApi> {
    enum State { State_NotInitialized, State_Initialized };
    enum Flag { Flag_Shared };

    detail::Ptr<void> pNvnDevice;
    char nvnDevice[12288];
    Bit8 state;
    util::BitPack8 flags;
    char reserved[2];
    util::BitPack32 supportedFeatures;
    detail::Ptr<void> userPtr;
};

}  // namespace nn::gfx