#pragma once

#include <nn/gfx/detail/gfx_Misc.h>
#include <nn/nn_BitTypes.h>

namespace nn::gfx {

struct TextureMipRangeData {
    uint16_t minMipLevel;
    uint16_t mipCount;
    char reserved[12];
};

struct TextureArrayRangeData {
    uint32_t baseArrayIndex;
    uint32_t arrayLength;
    char reserved[8];
};

struct TextureSubresourceRangeData {
    TextureMipRangeData mipRange;
    TextureArrayRangeData arrayRange;
};

struct TextureInfoData;

struct TextureViewInfoData {
    Bit8 imageDimension;
    Bit8 depthStencilTextureMode;
    char reserved2[2];
    Bit32 imageFormat;
    Bit8 channelMapping[4];
    char reserved3[4];
    TextureSubresourceRangeData subresourceRange;
    detail::Ptr<const void> pTexture;
    char reserved[8];
};

struct ColorTargetViewInfoData {
    Bit8 imageDimension;
    char reserved2;
    uint16_t mipLevel;
    Bit32 imageFormat;
    TextureArrayRangeData arrayRange;
    detail::Ptr<const void> pTexture;
    char reserved[16];
};

struct DepthStencilViewInfoData {
    Bit8 imageDimension;
    char reserved2;
    uint16_t mipLevel;
    char reserved3[4];
    TextureArrayRangeData arrayRange;
    detail::Ptr<const void> pTexture;
    char reserved[16];
};

struct TextureSubresourceData {
    uint16_t mipLevel;
    char reserved[2];
    uint32_t arrayIndex;
};

struct TextureCopyRegionData {
    uint32_t offsetU;
    uint32_t offsetV;
    uint32_t offsetW;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    TextureSubresourceData subresource;
    uint32_t arrayLength;
};

struct BufferTextureCopyRegionData {
    uint32_t bufferOffset;
    uint32_t bufferImageWidth;
    uint32_t bufferImageHeight;
    TextureCopyRegionData textureCopyRegion;
};

}  // namespace nn::gfx