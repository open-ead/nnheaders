#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/nn_BitTypes.h>
#include <nn/util/util_BitPack.h>

namespace nn::gfx {

template <>
struct BufferImplData<NvnApi> {
    enum State { State_NotInitialized, State_Initialized };

    enum Flag { Flag_Shared, Flag_CpuCached };

    Bit8 state;
    util::BitPack8 flags;
    char reserved[6];
    detail::Ptr<void> pNvnBuffer;
    char nvnBuffer[48];
    detail::Ptr<void> userPtr;
};

}  // namespace nn::gfx