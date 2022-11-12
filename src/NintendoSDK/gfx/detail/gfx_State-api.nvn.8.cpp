#include <nn/gfx/detail/gfx_State-api.nvn.8.h>

#include "gfx_NvnHelper.h"

namespace nn::gfx::detail {

RasterizerStateImpl<NvnApi>::RasterizerStateImpl() {}

RasterizerStateImpl<NvnApi>::~RasterizerStateImpl() {}

void RasterizerStateImpl<NvnApi>::Initialize(Device* device, const InfoType& info) {
    NVNpolygonState* pnPolygonState = reinterpret_cast<NVNpolygonState*>(&nvnPolygonState);
    NVNmultisampleState* pnMultisampleState =
        reinterpret_cast<NVNmultisampleState*>(&nvnMultisampleState);

    nvnPolygonStateSetDefaults(pnPolygonState);
    nvnPolygonStateSetCullFace(pnPolygonState, Nvn::GetCullMode(info.GetCullMode()));
    nvnPolygonStateSetFrontFace(pnPolygonState, Nvn::GetFrontFace(info.GetFrontFace()));
    nvnPolygonStateSetPolygonMode(pnPolygonState, Nvn::GetFillMode(info.GetFillMode()));

    nvnDepthBias = info.GetDepthBias();
    nvnDepthBiasClamp = info.GetDepthBiasClamp();
    nvnSlopeScaledDepthBias = info.GetSlopeScaledDepthBias();

    bool polyEnables =
        (nvnDepthBias == 0.0f && nvnDepthBiasClamp == 0.0f && nvnSlopeScaledDepthBias == 0);
    nvnPolygonStateSetPolygonOffsetEnables(pnPolygonState, (polyEnables) ?
                                                               (NVN_POLYGON_OFFSET_ENABLE_POINT |
                                                                NVN_POLYGON_OFFSET_ENABLE_LINE |
                                                                NVN_POLYGON_OFFSET_ENABLE_FILL) :
                                                               NVN_POLYGON_OFFSET_ENABLE_NONE);

    nvnSampleMask = info.GetMultisampleStateInfo().GetSampleMask();
    nvnMultisampleStateSetDefaults(pnMultisampleState);
    nvnMultisampleStateSetMultisampleEnable(pnMultisampleState, info.IsMultisampleEnabled());
    nvnMultisampleStateSetSamples(pnMultisampleState,
                                  info.GetMultisampleStateInfo().GetSampleCount());
    nvnMultisampleStateSetAlphaToCoverageEnable(
        pnMultisampleState, info.GetMultisampleStateInfo().IsAlphaToCoverageEnabled());

    flags.SetBit(Flag_MultisampleEnabled, info.IsMultisampleEnabled());
    flags.SetBit(Flag_DepthClipEnabled, info.IsDepthClipEnabled());
    flags.SetBit(Flag_RasterEnabled, info.IsRasterEnabled());
    flags.SetBit(Flag_ConservativeRasterEnabled,
                 info.GetConservativeRasterizationMode() == ConservativeRasterizationMode_Enable);

    state = State_Initialized;
}

void RasterizerStateImpl<NvnApi>::Finalize(Device*) {
    state = State_NotInitialized;
}

size_t BlendStateImpl<NvnApi>::GetRequiredMemorySize(const InfoType& info) {
    return 8 * info.GetBlendTargetCount();
}

BlendStateImpl<NvnApi>::BlendStateImpl() {}

BlendStateImpl<NvnApi>::~BlendStateImpl() {}

void BlendStateImpl<NvnApi>::SetMemory(void* p, size_t s) {
    pNvnBlendStateData = p;
    memorySize = s;
}

void* BlendStateImpl<NvnApi>::GetMemory() {
    return pNvnBlendStateData;
}

void* BlendStateImpl<NvnApi>::GetMemory() const {
    return pNvnBlendStateData;
}

void BlendStateImpl<NvnApi>::Initialize(Device* device, const InfoType& info) {
    auto pnColorState = reinterpret_cast<NVNcolorState*>(nvnColorState);
    auto pnChannelMaskState = reinterpret_cast<NVNchannelMaskState*>(nvnChannelMaskState);
    NVNblendState* pnBlendState = pNvnBlendStateData;
    const BlendTargetStateInfo* pTargetInfo = info.GetBlendTargetStateInfoArray();

    nvnBlendConstant[0] = info.GetBlendConstant(ColorChannel_Red);
    nvnBlendConstant[1] = info.GetBlendConstant(ColorChannel_Green);
    nvnBlendConstant[2] = info.GetBlendConstant(ColorChannel_Blue);
    nvnBlendConstant[3] = info.GetBlendConstant(ColorChannel_Alpha);

    nvnColorStateSetDefaults(pnColorState);
    nvnChannelMaskStateSetDefaults(pnChannelMaskState);

    targetCount = info.GetBlendTargetCount();

    for (int i = 0; i < targetCount; ++i) {
        nvnColorStateSetBlendEnable(pnColorState, i, pTargetInfo[i].IsBlendEnabled());
        nvnBlendStateSetDefaults(&pnBlendState[i]);
        nvnBlendStateSetBlendTarget(&pnBlendState[i], i);

        int infoOffset = (info.IsIndependentBlendEnabled()) ? i : 0;

        nvnBlendStateSetBlendFunc(
            &pnBlendState[i],
            Nvn::GetBlendFunction(pTargetInfo[infoOffset].GetSourceColorBlendFactor()),
            Nvn::GetBlendFunction(pTargetInfo[infoOffset].GetDestinationColorBlendFactor()),
            Nvn::GetBlendFunction(pTargetInfo[infoOffset].GetSourceAlphaBlendFactor()),
            Nvn::GetBlendFunction(pTargetInfo[infoOffset].GetDestinationAlphaBlendFactor()));

        nvnBlendStateSetBlendEquation(
            &pnBlendState[i],
            Nvn::GetBlendEquation(pTargetInfo[infoOffset].GetColorBlendFunction()),
            Nvn::GetBlendEquation(pTargetInfo[infoOffset].GetAlphaBlendFunction()));

        nvnChannelMaskStateSetChannelMask(
            pnChannelMaskState, i, pTargetInfo[infoOffset].GetChannelMask() & ChannelMask_Red,
            pTargetInfo[infoOffset].GetChannelMask() & ChannelMask_Green,
            pTargetInfo[infoOffset].GetChannelMask() & ChannelMask_Blue,
            pTargetInfo[infoOffset].GetChannelMask() & ChannelMask_Alpha);
    }

    if (info.IsLogicOperationEnabled()) {
        nvnColorStateSetLogicOp(pnColorState, Nvn::GetLogicOperation(info.GetLogicOperation()));
    }

    state = State_Initialized;
}

void BlendStateImpl<NvnApi>::Finalize(Device* device) {
    state = State_NotInitialized;
}

DepthStencilStateImpl<NvnApi>::DepthStencilStateImpl() {}

DepthStencilStateImpl<NvnApi>::~DepthStencilStateImpl() {}

/*
void DepthStencilStateImpl<NvnApi>::Initialize(Device*, const DepthStencilStateInfo& info) {
    nvnDepthStencilStateSetDefaults(nDepthStencilState);
    nvnDepthStencilStateSetDepthTestEnable(nDepthStencilState, a3->field_4 & 1);
    nvnDepthStencilStateSetDepthWriteEnable(nDepthStencilState, (a3->field_4 >> 1) & 1);
    nvnDepthStencilStateSetStencilTestEnable(nDepthStencilState, (a3->field_4 >> 2) & 1);

    initialized = true;
}
*/

void DepthStencilStateImpl<NvnApi>::Finalize(Device*) {
    /*
    initialized = false;
    */
}

}  // namespace nn::gfx::detail