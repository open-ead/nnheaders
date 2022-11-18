#pragma once

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/time.h>
#include <nn/util/util_BitPack.h>
#include <nvn/nvn.h>
#include <nvn/nvn_FuncPtrInline.h>

namespace nn::gfx {

struct ImageFormatProperty;
class TextureInfo;
struct SwapChainInfo;

namespace detail {

enum NvnDeviceFeature {
    NvnDeviceFeature_SupportMinMaxFiltering,
    NvnDeviceFeature_SupportStencil8Format,
    NvnDeviceFeature_SupportAstcFormat,
    NvnDeviceFeature_SupportConservativeRaster,
    NvnDeviceFeature_SupportZeroFromUnmappedVirtualPoolPage,
    NvnDeviceFeature_SupportPassthroughGeometryShader,
    NvnDeviceFeature_SupportViewportSwizzle,
    NvnDeviceFeature_SupportSparseTiledPackagedTexture,
    NvnDeviceFeature_AdvancedBlendModes,
    NvnDeviceFeature_DrawTexture,
    NvnDeviceFeature_TargetIndependentRasterization,
    NvnDeviceFeature_FragmentCoverageToColor,
    NvnDeviceFeature_PostDepthCoverage,
    NvnDeviceFeature_ImagesUsingTextureHandles,
    NvnDeviceFeature_SampleLocations,
    NvnDeviceFeature_SupportFragmentShaderInterlock
};

class Nvn {
public:
    static NVNformat GetImageFormat(ImageFormat);
    static NVNformat GetAttributeFormat(AttributeFormat);
    static NVNtextureTarget GetImageTarget(ImageDimension);
    static NVNdepthFunc GetDepthFunction(ComparisonFunction);
    static NVNstencilOp GetStencilOperation(StencilOperation);
    static NVNstencilFunc GetStencilFunction(ComparisonFunction);
    static NVNblendEquation GetBlendEquation(BlendFunction);
    static NVNblendFunc GetBlendFunction(BlendFactor);
    static NVNlogicOp GetLogicOperation(LogicOperation);
    static NVNfrontFace GetFrontFace(FrontFace);
    static NVNpolygonMode GetFillMode(FillMode);
    static NVNdrawPrimitive GetDrawPrimitive(PrimitiveTopology);
    static NVNminFilter GetMinFilter(FilterMode);
    static NVNmagFilter GetMagFilter(FilterMode);
    static NVNwrapMode GetWrapMode(TextureAddressMode);
    static NVNshaderStage GetShaderStage(ShaderStage);
    static int GetShaderStageBits(int);
    static NVNcompareFunc GetRComparisonFunction(ComparisonFunction);
    static NVNindexType GetIndexFormat(IndexFormat);
    static NVNface GetCullMode(CullMode);
    static int GetMemoryPoolFlags(int);
    static NVNbufferAddress GetBufferAddress(const GpuAddress);
    static void SetupScanBufferTextureInfo(TextureInfo*, const SwapChainInfo&);
    static NVNcounterType GetCounterType(QueryTarget);
    static util::BitPack32 GetDeviceFeature(const NVNdevice*);
    static void GetImageFormatProperty(ImageFormatProperty*, NVNformat);
    static ImageFormat GetGfxImageFormat(NVNformat);
    static void DebugCallback(NVNdebugCallbackSource, NVNdebugCallbackType, int,
                              NVNdebugCallbackSeverity, const char*, void*);
    static int GetFirstScanBufferIndex();
    static void SetPackagedTextureDataImpl(NVNtextureBuilder*, const TextureInfo&);
    static TimeSpan ToTimeSpan(int64_t);
};

}  // namespace detail

}  // namespace nn::gfx