#include <nn/gfx/detail/gfx_Texture-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Device-api.nvn.8.h>
#include <nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h>
#include <nn/gfx/gfx_TextureInfo.h>
#include <nn/util/util_BitUtil.h>

#include <algorithm>

#include "gfx_CommonHelper.h"
#include "gfx_NvnHelper.h"

namespace nn::gfx::detail {

namespace {

void SetupTextureBuilder(NVNtextureBuilder* pBuilder, NVNdevice* pDevice, const TextureInfo& info) {
    ImageStorageDimension imageStorageDimension = info.GetImageStorageDimension();
    int arrayLength = info.GetArrayLength();
    NVNtextureTarget target = Nvn::GetImageTarget(
        GetImageDimension(imageStorageDimension, arrayLength > 1, info.GetMultisampleCount() > 1));

    nvnTextureBuilderSetDefaults(pBuilder);
    nvnTextureBuilderSetDevice(pBuilder, pDevice);
    nvnTextureBuilderSetWidth(pBuilder, info.GetWidth());
    nvnTextureBuilderSetHeight(
        pBuilder, (target == NVN_TEXTURE_TARGET_1D_ARRAY) ? arrayLength : info.GetHeight());
    nvnTextureBuilderSetDepth(pBuilder, (target == NVN_TEXTURE_TARGET_2D_ARRAY ||
                                         target == NVN_TEXTURE_TARGET_2D_MULTISAMPLE_ARRAY) ?
                                            arrayLength :
                                            info.GetDepth());
    nvnTextureBuilderSetLevels(pBuilder, info.GetMipCount());

    if (info.GetMultisampleCount() > 1) {
        nvnTextureBuilderSetSamples(pBuilder, info.GetMultisampleCount());
    }

    nvnTextureBuilderSetTarget(pBuilder, target);
    nvnTextureBuilderSetFormat(pBuilder, Nvn::GetImageFormat(info.GetImageFormat()));

    int flags = NVN_TEXTURE_FLAGS_IMAGE;
    flags |= (info.GetGpuAccessFlags() & (GpuAccess_DepthStencil | GpuAccess_ColorBuffer)) ?
                 NVN_TEXTURE_FLAGS_COMPRESSIBLE :
                 0;

    flags |= (info.GetGpuAccessFlags() & GpuAccess_ScanBuffer) ? NVN_TEXTURE_FLAGS_DISPLAY : 0;

    flags |= (info.ToData()->flags.GetBit(info.ToData()->Flag_SparseResidency)) ?
                 NVN_TEXTURE_FLAGS_SPARSE :
                 0;

    if (info.GetTileMode() == TileMode_Linear) {
        NVNdeviceInfo strideAlignmentInfo;
        if (info.GetGpuAccessFlags() & (GpuAccess_DepthStencil | GpuAccess_ColorBuffer)) {
            strideAlignmentInfo = NVN_DEVICE_INFO_LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT;
            flags |= NVN_TEXTURE_FLAGS_LINEAR_RENDER_TARGET;

        } else {
            strideAlignmentInfo = NVN_DEVICE_INFO_LINEAR_TEXTURE_STRIDE_ALIGNMENT;
            flags |= NVN_TEXTURE_FLAGS_LINEAR;
        }

        int strideAlignment;
        nvnDeviceGetInteger(pDevice, strideAlignmentInfo, &strideAlignment);
        ptrdiff_t stride = nn::util::align_up(
            CalculateRowSize(info.GetWidth(), GetChannelFormat(info.GetImageFormat())),
            strideAlignment);
        nvnTextureBuilderSetStride(pBuilder, stride);
    }

    nvnTextureBuilderSetFlags(pBuilder, flags);
    if (info.ToData()->flags.GetBit(info.ToData()->Flag_SpecifyTextureLayout)) {
        auto pLayout =
            reinterpret_cast<const NVNpackagedTextureLayout*>(info.ToData()->textureLayout);
        nvnTextureBuilderSetPackagedTextureLayout(pBuilder, pLayout);
    }
}

size_t CalculateMipDataOffsetSize(ptrdiff_t* pMipOffsets, const TextureInfo& info) {
    ImageStorageDimension imageStorageDimension = info.GetImageStorageDimension();
    int arrayLength = info.GetArrayLength();

    int width = info.GetWidth();
    int height = (imageStorageDimension == ImageStorageDimension_1d && arrayLength > 1) ?
                     arrayLength :
                     info.GetHeight();
    int depth = (imageStorageDimension == ImageStorageDimension_2d && arrayLength > 1) ?
                    arrayLength :
                    info.GetDepth();

    int minHeight = 1;
    int minDepth = 1;
    if (arrayLength > 1) {
        if (imageStorageDimension == ImageStorageDimension_1d) {
            minHeight = height;
        } else if (imageStorageDimension == ImageStorageDimension_2d) {
            minDepth = depth;
        }
    }

    ChannelFormat channelFormat = GetChannelFormat(info.GetImageFormat());
    size_t size = 0;

    for (size_t mipLevel = 0; mipLevel < info.GetMipCount(); ++mipLevel) {
        if (pMipOffsets) {
            pMipOffsets[mipLevel] = size;
        }

        size += CalculateImageSize(channelFormat, std::max(width >> mipLevel, 1),
                                   std::max(height >> mipLevel, minHeight),
                                   std::max(depth >> mipLevel, minDepth));
    }
    return size;
}

}  // namespace

size_t TextureImpl<NvnApi>::CalculateMipDataAlignment(DeviceImpl<NvnApi>* pNnDevice,
                                                      const TextureInfo& info) {
    NVNtextureBuilder builder;
    SetupTextureBuilder(&builder, pNnDevice->ToData()->pNvnDevice, info);
    return nvnTextureBuilderGetStorageAlignment(&builder);
}

size_t TextureImpl<NvnApi>::CalculateMipDataSize(DeviceImpl<NvnApi>* pNnDevice,
                                                 const TextureInfo& info) {
    NVNtextureBuilder builder;
    SetupTextureBuilder(&builder, pNnDevice->ToData()->pNvnDevice, info);
    return nvnTextureBuilderGetStorageSize(&builder);
}

void TextureImpl<NvnApi>::CalculateMipDataOffsets(ptrdiff_t* pMipOffsets,
                                                  DeviceImpl<NvnApi>* pNnDevice,
                                                  const TextureInfo& info) {
    CalculateMipDataOffsetSize(pMipOffsets, info);
}

size_t TextureImpl<NvnApi>::GetRowPitch(DeviceImpl<NvnApi>* pDevice, const TextureInfo& info) {
    int stride;
    nvnDeviceGetInteger(
        pDevice->ToData()->pNvnDevice,
        (info.GetGpuAccessFlags() & (GpuAccess_DepthStencil | GpuAccess_ColorBuffer)) ?
            NVN_DEVICE_INFO_LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT :
            NVN_DEVICE_INFO_LINEAR_TEXTURE_STRIDE_ALIGNMENT,
        &stride);

    size_t row = CalculateRowSize(info.GetWidth(), GetChannelFormat(info.GetImageFormat()));
    return nn::util::align_up(row, stride);
}

TextureImpl<NvnApi>::TextureImpl() {
    state = State_NotInitialized;
}

TextureImpl<NvnApi>::~TextureImpl() {}

void TextureImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>* pDevice, const TextureInfo& info,
                                     MemoryPoolImpl<NvnApi>* pMemoryPool,
                                     ptrdiff_t memoryPoolOffset, size_t memoryPoolSize) {
    NVNtextureBuilder textureBuilder;
    SetupTextureBuilder(&textureBuilder, pDevice->ToData()->pNvnDevice, info);
    nvnTextureBuilderSetStorage(&textureBuilder, pMemoryPool->ToData()->pNvnMemoryPool,
                                memoryPoolOffset);

    Nvn::SetPackagedTextureDataImpl(&textureBuilder, info, pMemoryPool, memoryPoolOffset,
                                    memoryPoolSize);

    pNvnTexture = &nvnTexture;

    NVNboolean isTextureOK = nvnTextureInitialize(pNvnTexture, &textureBuilder);

    flags.SetBit(Flag_Shared, false);
    state = State_Initialized;
}

void TextureImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    nvnTextureFinalize(pNvnTexture);
    pNvnTexture = nullptr;
    state = State_NotInitialized;
}

TextureViewImpl<NvnApi>::TextureViewImpl() {
    state = State_NotInitialized;
}

TextureViewImpl<NvnApi>::~TextureViewImpl() {}

void TextureViewImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>*, const TextureViewInfo& info) {
    const TextureImpl<NvnApi>* pSourceTexture = info.GetTexturePtr();

    pNvnTexture = pSourceTexture->ToData()->pNvnTexture;
    pNvnTextureView = &nvnTextureView;

    const NVNtextureSwizzle s_ChannelMappingTable[6] = {
        NVN_TEXTURE_SWIZZLE_ZERO, NVN_TEXTURE_SWIZZLE_ONE, NVN_TEXTURE_SWIZZLE_R,
        NVN_TEXTURE_SWIZZLE_G,    NVN_TEXTURE_SWIZZLE_B,   NVN_TEXTURE_SWIZZLE_A,
    };

    nvnTextureViewSetDefaults(pNvnTextureView);
    nvnTextureViewSetLevels(pNvnTextureView,
                            info.GetSubresourceRange().GetMipRange().GetMinMipLevel(),
                            info.GetSubresourceRange().GetMipRange().GetMipCount());

    if (info.GetImageDimension() != ImageDimension_3d) {
        nvnTextureViewSetLayers(pNvnTextureView,
                                info.GetSubresourceRange().GetArrayRange().GetBaseArrayIndex(),
                                info.GetSubresourceRange().GetArrayRange().GetArrayLength());
    }

    nvnTextureViewSetFormat(pNvnTextureView, Nvn::GetImageFormat(info.GetImageFormat()));
    nvnTextureViewSetSwizzle(pNvnTextureView,
                             s_ChannelMappingTable[info.GetChannelMapping(ColorChannel_Red)],
                             s_ChannelMappingTable[info.GetChannelMapping(ColorChannel_Green)],
                             s_ChannelMappingTable[info.GetChannelMapping(ColorChannel_Blue)],
                             s_ChannelMappingTable[info.GetChannelMapping(ColorChannel_Alpha)]);
    nvnTextureViewSetDepthStencilMode(pNvnTextureView, (info.GetDepthStencilTextureMode() !=
                                                        DepthStencilFetchMode_DepthComponent) ?
                                                           NVN_TEXTURE_DEPTH_STENCIL_MODE_STENCIL :
                                                           NVN_TEXTURE_DEPTH_STENCIL_MODE_DEPTH);

    nvnTextureViewSetTarget(pNvnTextureView, Nvn::GetImageTarget(info.GetImageDimension()));

    flags.SetBit(Flag_Shared, false);
    state = State_Initialized;
}

void TextureViewImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    state = State_NotInitialized;
    pNvnTexture = nullptr;
    pNvnTextureView = nullptr;
}

ColorTargetViewImpl<NvnApi>::ColorTargetViewImpl() {
    state = State_NotInitialized;
}

ColorTargetViewImpl<NvnApi>::~ColorTargetViewImpl() {}

void ColorTargetViewImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>*, const ColorTargetViewInfo& info) {
    const TextureImpl<NvnApi>* pSourceTexture = info.GetTexturePtr();

    pNvnTexture = pSourceTexture->ToData()->pNvnTexture;
    pNvnTextureView = &nvnTextureView;

    nvnTextureViewSetDefaults(pNvnTextureView);
    nvnTextureViewSetLevels(pNvnTextureView, info.GetMipLevel(), 1);
    nvnTextureViewSetLayers(pNvnTextureView, info.GetArrayRange().GetBaseArrayIndex(),
                            info.GetArrayRange().GetArrayLength());

    nvnTextureViewSetFormat(pNvnTextureView, Nvn::GetImageFormat(info.GetImageFormat()));
    nvnTextureViewSetTarget(pNvnTextureView, Nvn::GetImageTarget(info.GetImageDimension()));

    flags.SetBit(Flag_Shared, false);
    state = State_Initialized;
}

void ColorTargetViewImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    state = State_NotInitialized;
    pNvnTexture = nullptr;
    pNvnTextureView = nullptr;
}

DepthStencilViewImpl<NvnApi>::DepthStencilViewImpl() {
    state = State_NotInitialized;
}

DepthStencilViewImpl<NvnApi>::~DepthStencilViewImpl() {}

void DepthStencilViewImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>*,
                                              const DepthStencilViewInfo& info) {
    const TextureImpl<NvnApi>* pSourceTexture = info.GetTexturePtr();

    pNvnTexture = pSourceTexture->ToData()->pNvnTexture;
    pNvnTextureView = &nvnTextureView;

    nvnTextureViewSetDefaults(pNvnTextureView);
    nvnTextureViewSetLevels(pNvnTextureView, info.GetMipLevel(), 1);
    nvnTextureViewSetLayers(pNvnTextureView, info.GetArrayRange().GetBaseArrayIndex(),
                            info.GetArrayRange().GetArrayLength());

    nvnTextureViewSetTarget(pNvnTextureView, Nvn::GetImageTarget(info.GetImageDimension()));

    flags.SetBit(Flag_Shared, false);
    state = State_Initialized;
}

void DepthStencilViewImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    state = State_NotInitialized;
    pNvnTexture = nullptr;
    pNvnTextureView = nullptr;
}

template <>
void GetImageFormatProperty<NvnApi>(ImageFormatProperty* pOutImageFormatProperty,
                                    DeviceImpl<NvnApi>* pDevice, ImageFormat imageFormat) {
    NVNformat nvnFormat = Nvn::GetImageFormat(imageFormat);
    Nvn::GetImageFormatProperty(pOutImageFormatProperty, nvnFormat);
}

}  // namespace nn::gfx::detail