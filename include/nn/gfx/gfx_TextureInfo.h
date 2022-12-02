#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_TextureInfoData.h>

namespace nn::gfx {

class TextureInfo;

class TextureViewInfo : public detail::DataContainer<TextureViewInfoData> {
public:
    TextureViewInfo();
    void SetDefault();
    void SetImageDimension(ImageDimension);
    void SetDepthStencilTextureMode(DepthStencilFetchMode);
    void SetImageFormat(ImageFormat);
    void SetTexturePtr(const void*);
    void SetChannelMapping(ChannelMapping, ChannelMapping, ChannelMapping, ChannelMapping);
    TextureSubresourceRange& EditSubresourceRange();
    ImageDimension GetImageDimension() const;
    DepthStencilFetchMode GetDepthStencilTextureMode() const;
    ImageFormat GetImageFormat() const;
    detail::Caster<const void> GetTexturePtr() const;
    ChannelMapping GetChannelMapping(ColorChannel) const;
    const TextureSubresourceRange& GetSubresourceRange() const;
};

class ColorTargetViewInfo : public detail::DataContainer<ColorTargetViewInfoData> {
public:
    ColorTargetViewInfo();
    void SetDefault();
    void SetImageDimension(ImageDimension);
    void SetImageFormat(ImageFormat);
    void SetMipLevel(int);
    void SetTexturePtr(const void*);
    TextureArrayRange& EditArrayRange();
    ImageDimension GetImageDimension() const;
    ImageFormat GetImageFormat() const;
    int GetMipLevel() const;
    detail::Caster<void const> GetTexturePtr() const;
    const TextureArrayRange& GetArrayRange() const;
};

class DepthStencilViewInfo : public detail::DataContainer<DepthStencilViewInfoData> {
public:
    DepthStencilViewInfo();
    void SetDefault();
    void SetImageDimension(ImageDimension);
    void SetMipLevel(int);
    void SetTexturePtr(const void*);
    TextureArrayRange& EditArrayRange();
    ImageDimension GetImageDimension() const;
    int GetMipLevel() const;
    detail::Caster<void const> GetTexturePtr() const;
    const TextureArrayRange& GetArrayRange() const;

    template <typename TTarget>
    void SetTexturePtr(const TTexture<TTarget>*);
};

class TextureMipRange;

class TextureArrayRange : public detail::DataContainer<TextureArrayRangeData> {
public:
    TextureArrayRange();
    void SetDefault();
    void SetBaseArrayIndex(int);
    void SetArrayLength(int);

    int GetBaseArrayIndex() const { return baseArrayIndex; }
    int GetArrayLength() const { return arrayLength; }
};

class TextureSubresourceRange;

class TextureSubresource : public detail::DataContainer<TextureSubresourceData> {
public:
    TextureSubresource();
    void SetDefault();
    void SetMipLevel(int);
    void SetArrayIndex(int);
    int GetMipLevel() const { return mipLevel; }
    int GetArrayIndex() const { return arrayIndex; }
};

class TextureCopyRegion : public detail::DataContainer<TextureCopyRegionData> {
public:
    TextureCopyRegion();
    void SetDefault();
    void SetOffsetU(int);
    void SetOffsetV(int);
    void SetOffsetW(int);
    void SetWidth(int);
    void SetHeight(int);
    void SetDepth(int);
    TextureSubresource& EditSubresource();
    void SetArrayLength(int);

    int GetOffsetU() const { return offsetU; }
    int GetOffsetV() const { return offsetV; }
    int GetOffsetW() const { return offsetW; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetDepth() const { return depth; }

    const TextureSubresource& GetSubresource() const {
        return nn::gfx::DataToAccessor(subresource);
    }

    int GetArrayLength() const { return arrayLength; }
};

class BufferTextureCopyRegion : public detail::DataContainer<BufferTextureCopyRegionData> {
public:
    BufferTextureCopyRegion();
    void SetDefault();
    void SetBufferOffset(int);
    void SetBufferImageWidth(int);
    void SetBufferImageHeight(int);
    TextureCopyRegion& EditTextureCopyRegion();
    int GetBufferOffset() const { return bufferOffset; }
    int GetBufferImageWidth() const { return bufferImageWidth; }
    int GetBufferImageHeight() const { return bufferImageHeight; }

    const TextureCopyRegion& GetTextureCopyRegion() const {
        return nn::gfx::DataToAccessor(textureCopyRegion);
    }
};

}  // namespace nn::gfx