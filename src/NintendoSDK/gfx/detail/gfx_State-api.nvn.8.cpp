#include <nn/gfx/detail/gfx_State-api.nvn.8.h>

#include <nn/gfx/gfx_StateInfo.h>

#include "gfx_NvnHelper.h"

namespace nn::gfx::detail {

RasterizerStateImpl<NvnApi>::RasterizerStateImpl() {}

RasterizerStateImpl<NvnApi>::~RasterizerStateImpl() {}

void RasterizerStateImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>* device, const InfoType& info) {
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

void RasterizerStateImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
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

void BlendStateImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>* device, const InfoType& info) {
    auto pnColorState = reinterpret_cast<NVNcolorState*>(&nvnColorState);
    auto pnChannelMaskState = reinterpret_cast<NVNchannelMaskState*>(&nvnChannelMaskState);
    NVNblendState* pnBlendState = pNvnBlendStateData;
    const BlendTargetStateInfo* pTargetInfo = info.GetBlendTargetStateInfoArray();

    nvnBlendConstant[0] = info.GetBlendConstant(ColorChannel_Red);
    nvnBlendConstant[1] = info.GetBlendConstant(ColorChannel_Green);
    nvnBlendConstant[2] = info.GetBlendConstant(ColorChannel_Blue);
    nvnBlendConstant[3] = info.GetBlendConstant(ColorChannel_Alpha);

    nvnColorStateSetDefaults(pnColorState);
    nvnChannelMaskStateSetDefaults(pnChannelMaskState);

    targetCount = info.GetBlendTargetCount();

    for (int i = 0; i < info.GetBlendTargetCount(); ++i) {
        int infoOffset = (info.IsIndependentBlendEnabled()) ? i : 0;

        nvnColorStateSetBlendEnable(pnColorState, i, pTargetInfo[i].IsBlendEnabled());
        nvnBlendStateSetDefaults(&pnBlendState[i]);
        nvnBlendStateSetBlendTarget(&pnBlendState[i], i);

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
            pnChannelMaskState, i,
            (pTargetInfo[infoOffset].GetChannelMask() & ChannelMask_Red) == ChannelMask_Red,
            (pTargetInfo[infoOffset].GetChannelMask() & ChannelMask_Green) == ChannelMask_Green,
            (pTargetInfo[infoOffset].GetChannelMask() & ChannelMask_Blue) == ChannelMask_Blue,
            (pTargetInfo[infoOffset].GetChannelMask() & ChannelMask_Alpha) == ChannelMask_Alpha);
    }

    if (info.IsLogicOperationEnabled()) {
        nvnColorStateSetLogicOp(pnColorState, Nvn::GetLogicOperation(info.GetLogicOperation()));
    }

    state = State_Initialized;
}

void BlendStateImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>* device) {
    state = State_NotInitialized;
}

DepthStencilStateImpl<NvnApi>::DepthStencilStateImpl() {}

DepthStencilStateImpl<NvnApi>::~DepthStencilStateImpl() {}

void DepthStencilStateImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>*,
                                               const DepthStencilStateInfo& info) {
    auto pnDepth = reinterpret_cast<NVNdepthStencilState*>(&nvnDepthStencilState);

    nvnDepthStencilStateSetDefaults(pnDepth);
    nvnDepthStencilStateSetDepthTestEnable(pnDepth, info.IsDepthTestEnabled());
    nvnDepthStencilStateSetDepthWriteEnable(pnDepth, info.IsDepthWriteEnabled());
    nvnDepthStencilStateSetStencilTestEnable(pnDepth, info.IsStencilTestEnabled());

    nvnDepthStencilStateSetDepthFunc(pnDepth,
                                     Nvn::GetDepthFunction(info.GetDepthComparisonFunction()));

    nvnDepthStencilStateSetStencilFunc(
        pnDepth, NVN_FACE_BACK,
        Nvn::GetStencilFunction(info.GetBackStencilStateInfo().GetComparisonFunction()));

    nvnDepthStencilStateSetStencilOp(
        pnDepth, NVN_FACE_BACK,
        Nvn::GetStencilOperation(info.GetBackStencilStateInfo().GetStencilFailOperation()),
        Nvn::GetStencilOperation(info.GetBackStencilStateInfo().GetDepthFailOperation()),
        Nvn::GetStencilOperation(info.GetBackStencilStateInfo().GetDepthPassOperation()));

    nvnDepthStencilStateSetStencilFunc(
        pnDepth, NVN_FACE_FRONT,
        Nvn::GetStencilFunction(info.GetFrontStencilStateInfo().GetComparisonFunction()));

    nvnDepthStencilStateSetStencilOp(
        pnDepth, NVN_FACE_FRONT,
        Nvn::GetStencilOperation(info.GetFrontStencilStateInfo().GetStencilFailOperation()),
        Nvn::GetStencilOperation(info.GetFrontStencilStateInfo().GetDepthFailOperation()),
        Nvn::GetStencilOperation(info.GetFrontStencilStateInfo().GetDepthPassOperation()));

    nvnStencilBackRef = info.GetBackStencilStateInfo().GetStencilRef();
    nvnStencilFrontRef = info.GetFrontStencilStateInfo().GetStencilRef();
    nvnStencilValueMask = info.GetStencilReadMask();
    nvnStencilMask = info.GetStencilWriteMask();

    state = State_Initialized;
}

void DepthStencilStateImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    state = State_NotInitialized;
}

size_t VertexStateImpl<NvnApi>::GetRequiredMemorySize(const InfoType& info) {
    const VertexAttributeStateInfo* pAttrib = info.GetVertexAttributeStateInfoArray();
    int num = -1;

    for (int i = 0; i < info.GetVertexAttributeCount(); ++i) {
        int shaderSlot = pAttrib[i].GetShaderSlot();

        if (num < shaderSlot) {
            num = shaderSlot;
        }

        if (shaderSlot < 0) {
            num = 15;
        }
    }

    // todo: figure out magic numbers
    return 4 * (num + 1) + 8 * info.GetVertexBufferCount();
}

VertexStateImpl<NvnApi>::VertexStateImpl() {}

VertexStateImpl<NvnApi>::~VertexStateImpl() {}

void VertexStateImpl<NvnApi>::SetMemory(void* p, size_t s) {
    pNvnVertexStreamState = p;
    memorySize = s;
}

void* VertexStateImpl<NvnApi>::GetMemory() {
    return pNvnVertexStreamState;
}

const void* VertexStateImpl<NvnApi>::GetMemory() const {
    return pNvnVertexStreamState;
}

void VertexStateImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>* device, const VertexStateInfo& info,
                                         const ShaderImpl<NvnApi>* shader) {
    // todo
}

void VertexStateImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>* device) {
    state = State_NotInitialized;
}

}  // namespace nn::gfx::detail