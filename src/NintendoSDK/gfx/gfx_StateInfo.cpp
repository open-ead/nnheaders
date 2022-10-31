#include <nn/gfx/gfx_StateInfo.h>

#include "detail/gfx_NvnHelper.h"

namespace nn::gfx {

void MultisampleStateInfo::SetDefault() {
    SetAlphaToCoverageEnabled(false);
    SetSampleCount(1);
    SetSampleMask(0xFFFFFFFF);
}

void RasterizerStateInfo::SetDefault() {
    SetFillMode(FillMode_Solid);
    SetFrontFace(FrontFace_Ccw);
    SetCullMode(CullMode_Back);
    SetPrimitiveTopologyType(PrimitiveTopologyType_Triangle);

    SetRasterEnabled(true);
    SetMultisampleEnabled(false);
    SetDepthClipEnabled(false);
    SetScissorEnabled(false);

    SetSlopeScaledDepthBias(0.0f);
    SetDepthBias(0);
    SetDepthBiasClamp(0.0f);

    SetConservativeRasterizationMode(ConservativeRasterizationMode_Disable);

    // todo: figure out where to put this
    EditMultisampleStateInfo().SetDefault();
}

void BlendTargetStateInfo::SetDefault() {
    SetSourceColorBlendFactor(BlendFactor_One);
    SetSourceAlphaBlendFactor(BlendFactor_One);
    SetBlendEnabled(false);
    SetDestinationColorBlendFactor(BlendFactor_Zero);
    SetColorBlendFunction(BlendFunction_Add);
    SetDestinationAlphaBlendFactor(BlendFactor_Zero);
    SetAlphaBlendFunction(BlendFunction_Add);
    SetChannelMask(ChannelMask_Red | ChannelMask_Green | ChannelMask_Blue | ChannelMask_Alpha);
}

void BlendStateInfo::SetDefault() {
    SetLogicOperation(LogicOperation_NoOp);
    SetBlendConstant(0.0f, 0.0f, 0.0f, 1.0f);

    SetAlphaToCoverageEnabled(false);
    SetDualSourceBlendEnabled(false);
    SetIndependentBlendEnabled(false);
    SetLogicOperationEnabled(false);

    SetBlendTargetStateInfoArray(nullptr, 0);
}

void DepthStencilStateInfo::SetDefault() {}

}  // namespace nn::gfx