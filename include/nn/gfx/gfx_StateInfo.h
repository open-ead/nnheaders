#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_StateInfoData.h>
#include <nvn/nvn_api.h>
#include <nvn/nvn_types.h>

namespace nn::gfx {

class MultisampleStateInfo : public detail::DataContainer<MultisampleStateInfoData> {
public:
    MultisampleStateInfo() {}

    void SetDefault();

    void SetSampleCount(int count) { sampleCount = count; }

    void SetSampleMask(int mask) { sampleMask = mask; }

    void SetAlphaToCoverageEnabled(bool alpha) { flag.SetBit(Flag_AlphaToCoverageEnable, alpha); }

    bool IsAlphaToCoverageEnabled() const { return flag.GetBit(Flag_AlphaToCoverageEnable); }

    int GetSampleCount() const { return sampleCount; }

    int GetSampleMask() const { return sampleMask; }
};

class RasterizerStateInfo : public detail::DataContainer<RasterizerStateInfoData> {
public:
    RasterizerStateInfo() {}

    void SetDefault();

    void SetFillMode(FillMode fill) { fillMode = fill; }

    void SetFrontFace(FrontFace face) { frontFace = face; }

    void SetCullMode(CullMode cull) { cullMode = cull; }

    void SetPrimitiveTopologyType(PrimitiveTopologyType type) { primitiveTopologyType = type; }

    void SetRasterEnabled(bool b) { flag.SetBit(Flag_RasterDisable, !b); }

    void SetMultisampleEnabled(bool b) { flag.SetBit(Flag_MultisampleEnable, b); }

    void SetDepthClipEnabled(bool b) { flag.SetBit(Flag_DepthClipDisable, !b); }

    void SetScissorEnabled(bool b) { flag.SetBit(Flag_ScissorEnable, b); }

    void SetSlopeScaledDepthBias(float bias) { slopeScaledDepthBias = bias; }

    void SetDepthBias(int bias) { depthBias = bias; }

    void SetDepthBiasClamp(float clamp) { depthBiasClamp = clamp; }

    void SetConservativeRasterizationMode(ConservativeRasterizationMode mode) {
        conservativeRasterizationMode = mode;
    }

    MultisampleStateInfo& EditMultisampleStateInfo() {
        return detail::DataContainer<MultisampleStateInfoData>::DataToAccessor(multisample);
    }

    FillMode GetFillMode() const { return static_cast<FillMode>(fillMode); }

    FrontFace GetFrontFace() const { return static_cast<FrontFace>(frontFace); }

    CullMode GetCullMode() const { return static_cast<CullMode>(cullMode); }

    PrimitiveTopologyType GetPrimitiveTopologyType() const {
        return static_cast<PrimitiveTopologyType>(primitiveTopologyType);
    }

    bool IsRasterEnabled() const { return !flag.GetBit(Flag_RasterDisable); }

    bool IsMultisampleEnabled() const { return flag.GetBit(Flag_MultisampleEnable); }

    bool IsDepthClipEnabled() const { return !flag.GetBit(Flag_DepthClipDisable); }

    bool IsScissorEnabled() const { return flag.GetBit(Flag_ScissorEnable); }

    float GetSlopeScaledDepthBias() const { return slopeScaledDepthBias; }

    int GetDepthBias() const { return depthBias; }

    float GetDepthBiasClamp() const { return depthBiasClamp; }

    ConservativeRasterizationMode GetConservativeRasterizationMode() const {
        return static_cast<ConservativeRasterizationMode>(conservativeRasterizationMode);
    }

    const MultisampleStateInfo& GetMultisampleStateInfo() const {
        return detail::DataContainer<MultisampleStateInfoData>::DataToAccessor(multisample);
    }
};

class BlendTargetStateInfo : public detail::DataContainer<BlendTargetStateInfoData> {
public:
    BlendTargetStateInfo();
    void SetDefault();

    void SetBlendEnabled(bool b) { flag.SetBit(Flag_BlendEnable, b); }

    void SetSourceColorBlendFactor(BlendFactor factor) { sourceColorBlendFactor = factor; }

    void SetDestinationColorBlendFactor(BlendFactor factor) {
        destinationColorBlendFactor = factor;
    }

    void SetColorBlendFunction(BlendFunction function) { colorBlendFunction = function; }

    void SetSourceAlphaBlendFactor(BlendFactor factor) { sourceAlphaBlendFactor = factor; }

    void SetDestinationAlphaBlendFactor(BlendFactor factor) {
        destinationAlphaBlendFactor = factor;
    }

    void SetAlphaBlendFunction(BlendFunction function) { alphaBlendFunction = function; }

    void SetChannelMask(int mask) { channelMask = mask; }

    bool IsBlendEnabled() const { return flag.GetBit(Flag_BlendEnable); }

    BlendFactor GetSourceColorBlendFactor() const {
        return static_cast<BlendFactor>(sourceColorBlendFactor);
    }

    BlendFactor GetDestinationColorBlendFactor() const {
        return static_cast<BlendFactor>(destinationColorBlendFactor);
    }

    BlendFunction GetColorBlendFunction() const {
        return static_cast<BlendFunction>(colorBlendFunction);
    }

    BlendFactor GetSourceAlphaBlendFactor() const {
        return static_cast<BlendFactor>(sourceAlphaBlendFactor);
    }

    BlendFactor GetDestinationAlphaBlendFactor() const {
        return static_cast<BlendFactor>(destinationAlphaBlendFactor);
    }

    BlendFunction GetAlphaBlendFunction() const {
        return static_cast<BlendFunction>(alphaBlendFunction);
    }

    int GetChannelMask() const { return channelMask; }
};

class BlendStateInfo : public detail::DataContainer<BlendStateInfoData> {
public:
    BlendStateInfo() {}

    void SetDefault();

    void SetLogicOperation(LogicOperation op) { logicOperation = op; }

    void SetAlphaToCoverageEnabled(bool b) { flag.SetBit(Flag_AlphaToCoverageEnable, b); }

    void SetDualSourceBlendEnabled(bool b) { flag.SetBit(Flag_DualSourceBlendEnable, b); }

    void SetIndependentBlendEnabled(bool b) { flag.SetBit(Flag_IndependentBlendEnable, b); }

    void SetLogicOperationEnabled(bool b) { flag.SetBit(Flag_LogicOperationEnable, b); }

    void SetBlendConstant(float r, float g, float b, float alpha) {
        blendConstant[0] = r;
        blendConstant[1] = g;
        blendConstant[2] = b;
        blendConstant[3] = alpha;
    }

    void SetBlendTargetStateInfoArray(const BlendTargetStateInfo* p, int c) {
        blendTargetCount = c;
        pBlendTargetArray = p->ToData();
    }

    int GetBlendTargetCount() const { return blendTargetCount; }

    LogicOperation GetLogicOperation() const { return static_cast<LogicOperation>(logicOperation); }

    bool IsAlphaToCoverageEnabled() const { return flag.GetBit(Flag_AlphaToCoverageEnable); }

    bool IsDualSourceBlendEnabled() const { return flag.GetBit(Flag_DualSourceBlendEnable); }

    bool IsIndependentBlendEnabled() const { return flag.GetBit(Flag_IndependentBlendEnable); }

    bool IsLogicOperationEnabled() const { return flag.GetBit(Flag_LogicOperationEnable); }

    float GetBlendConstant(ColorChannel channel) const { return blendConstant[channel]; }

    const BlendTargetStateInfo* GetBlendTargetStateInfoArray() const { return pBlendTargetArray; }
};

class DepthStencilStateInfo {
public:
    void SetDefault();

    enum Flag {
        Flag_1 = 1,
        Flag_2 = 2,
        Flag_4 = 4,
        Flag_DepthBoundsEnable = 8,
    };
};

}  // namespace nn::gfx