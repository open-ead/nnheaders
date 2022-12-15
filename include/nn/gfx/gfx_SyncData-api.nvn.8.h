#pragma once

#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/nn_BitTypes.h>

namespace nn::gfx {

template <>
struct FenceImplData<NvnApi> {
    enum State { State_NotInitialized, State_Initialized };

    detail::Ptr<const detail::DeviceImpl<NvnApi>> pNnDevice;
    detail::Ptr<void> pNvnSync;
    char nvnSync[64];
    Bit8 state;
    char reserved[3];
};

template <>
struct SemaphoreImplData<NvnApi> {
    enum State { State_NotInitialized, State_Initialized };

    detail::Ptr<const detail::DeviceImpl<NvnApi>> pNnDevice;
    detail::Ptr<void> pNvnSync;
    char nvnSync[64];
    Bit8 state;
    char reserved[3];
};

}  // namespace nn::gfx