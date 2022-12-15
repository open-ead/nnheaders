#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>

namespace nn::gfx {

class SwapChainInfo;

namespace detail {

template <>
class SwapChainImpl<NvnApi> : public DataContainer<SwapChainImplData<NvnApi>> {
    NN_NO_COPY(SwapChainImpl);

public:
    typedef SwapChainInfo InfoType;

    static const bool IsMemoryPoolRequired = true;

    static size_t GetScanBufferAlignment(DeviceImpl<NvnApi>*, const InfoType&);
    static size_t CalculateScanBufferSize(DeviceImpl<NvnApi>*, const InfoType&);

    SwapChainImpl();
    ~SwapChainImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&, MemoryPoolImpl<NvnApi>*, ptrdiff_t,
                    size_t);
    void Finalize(DeviceImpl<NvnApi>*);
    int GetScanBufferViews(TColorTargetView<NvnApi>**, int);
    int GetScanBuffers(TTexture<NvnApi>**, int);
    AcquireScanBufferResult AcquireNextScanBufferIndex(int*, SemaphoreImpl<NvnApi>*,
                                                       FenceImpl<NvnApi>*);
    int AcquireNextScanBufferIndex();
    ColorTargetViewImpl<NvnApi>* AcquireNextScanBufferView();
    TextureImpl<NvnApi>* AcquireNextScanBuffer();
};

}  // namespace detail
}  // namespace nn::gfx