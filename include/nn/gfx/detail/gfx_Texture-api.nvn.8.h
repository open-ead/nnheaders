#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_TextureData-api.nvn.8.h>

namespace nn::gfx {

class TextureInfo;
class TextureViewInfo;
class ColorTargetViewInfo;
class DepthStencilViewInfo;

namespace detail {
template <>
class TextureImpl<NvnApi> : public DataContainer<TextureImplData<NvnApi>> {
    NN_NO_COPY(TextureImpl);

public:
    typedef TextureInfo InfoType;

    static const bool IsMemoryPoolRequired;

    static size_t CalculateMipDataAlignment(DeviceImpl<NvnApi>*, const InfoType&);
    static size_t CalculateMipDataSize(DeviceImpl<NvnApi>*, const InfoType&);
    static void CalculateMipDataOffsets(ptrdiff_t*, DeviceImpl<NvnApi>*, const InfoType&);
    static size_t GetRowPitch(DeviceImpl<NvnApi>*, const InfoType&);

    TextureImpl();
    ~TextureImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&, MemoryPoolImpl<NvnApi>*, ptrdiff_t,
                    size_t);
    void Finalize(DeviceImpl<NvnApi>*);
};

template <>
class TextureViewImpl<NvnApi> : public DataContainer<TextureViewImplData<NvnApi>> {
    NN_NO_COPY(TextureViewImpl);

public:
    typedef TextureViewInfo InfoType;

    TextureViewImpl();

    ~TextureViewImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
};

template <>
class ColorTargetViewImpl<NvnApi> : public DataContainer<ColorTargetViewImplData<NvnApi>> {
    NN_NO_COPY(ColorTargetViewImpl);

public:
    typedef ColorTargetViewInfo InfoType;

    ColorTargetViewImpl();

    ~ColorTargetViewImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
};

template <>
class DepthStencilViewImpl<NvnApi> : public DataContainer<DepthStencilViewImplData<NvnApi>> {
    NN_NO_COPY(DepthStencilViewImpl);

public:
    typedef DepthStencilViewInfo InfoType;

    DepthStencilViewImpl();
    ~DepthStencilViewImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
};

}  // namespace detail

}  // namespace nn::gfx