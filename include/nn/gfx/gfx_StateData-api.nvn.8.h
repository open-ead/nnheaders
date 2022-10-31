#pragma once

#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/nn_BitTypes.h>
#include <nn/util/util_BitPack.h>

namespace nn::gfx {

template <>
struct RasterizerStateImplData<NvnApi> {
    enum Flag {
        Flag_MultisampleEnabled,
        Flag_DepthClipEnabled,
        Flag_RasterEnabled,
        Flag_ConservativeRasterEnabled
    };

    enum State { State_NotInitialized, State_Initialized };

    Bit32 nvnPolygonState;
    char nvnMultisampleState[24];
    Bit32 nvnSampleMask;
    float nvnDepthBias;
    float nvnDepthBiasClamp;
    float nvnSlopeScaledDepthBias;
    Bit8 state;
    util::BitPack8 flags;
    char reserved[2];
};

template <>
struct BlendStateImplData<NvnApi> {
    enum State { State_NotInitialized, State_Initialized };

    detail::Ptr<void> pNvnBlendStateData;
    Bit32 nvnColorState;
    Bit32 nvnChannelMaskState;
    float nvnBlendConstant[4];
    Bit32 memorySize;
    Bit8 state;
    Bit8 targetCount;
    char reserved[2];
};

}  // namespace nn::gfx