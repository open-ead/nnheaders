#pragma once

#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/nn_BitTypes.h>
#include <nn/util/util_BitPack.h>

namespace nn::gfx {

struct MultisampleStateInfoData {
    enum Flag { Flag_AlphaToCoverageEnable };

    uint8_t sampleCount;
    char reserved2;
    util::BitPack16 flag;
    uint32_t sampleMask;
    char reserved[8];
};

struct RasterizerStateInfoData {
    enum Flag {
        Flag_RasterDisable,
        Flag_MultisampleEnable,
        Flag_DepthClipDisable,
        Flag_ScissorEnable
    };

    Bit8 fillMode;
    Bit8 frontFace;
    Bit8 cullMode;
    Bit8 primitiveTopologyType;
    util::BitPack16 flag;
    Bit8 conservativeRasterizationMode;
    char reserved2;
    float slopeScaledDepthBias;
    int32_t depthBias;
    float depthBiasClamp;
    char reserved3[4];
    MultisampleStateInfoData multisample;
    char reserved[24];
};

struct BlendTargetStateInfoData {
    enum Flag { Flag_BlendEnable };

    util::BitPack8 flag;
    Bit8 sourceColorBlendFactor;
    Bit8 destinationColorBlendFactor;
    Bit8 colorBlendFunction;
    Bit8 sourceAlphaBlendFactor;
    Bit8 destinationAlphaBlendFactor;
    Bit8 alphaBlendFunction;
    Bit8 channelMask;
    char reserved[24];
};

struct BlendStateInfoData {
    enum Flag {
        Flag_AlphaToCoverageEnable,
        Flag_DualSourceBlendEnable,
        Flag_IndependentBlendEnable,
        Flag_LogicOperationEnable
    };

    uint8_t blendTargetCount;
    Bit8 logicOperation;
    util::BitPack16 flag;
    float blendConstant[4];
    char reserved2[4];
    detail::Ptr<const BlendTargetStateInfoData> pBlendTargetArray;
    char reserved[16];
};

}  // namespace nn::gfx