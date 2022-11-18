#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_DescriptorPoolData-api.nvn.8.h>
#include <nn/gfx/gfx_Enum.h>

namespace nn::gfx {

class DescriptorPoolInfo;
class DescriptorSlot;
class GpuAddress;

namespace detail {

template <>
class DescriptorPoolImpl<NvnApi> : public DataContainer<DescriptorPoolImplData<NvnApi>> {
    NN_NO_COPY(DescriptorPoolImpl);

public:
    typedef DescriptorPoolInfo InfoType;

    static size_t CalculateDescriptorPoolSize(DeviceImpl<NvnApi>*, const InfoType&);
    static ptrdiff_t GetDescriptorSlotIncrementSize(DeviceImpl<NvnApi>*, DescriptorPoolType);
    static size_t GetDescriptorPoolAlignment(DeviceImpl<NvnApi>*, const InfoType&);

    DescriptorPoolImpl();
    ~DescriptorPoolImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&, MemoryPoolImpl<NvnApi>*, ptrdiff_t,
                    size_t);
    void Finalize(DeviceImpl<NvnApi>*);
    void BeginUpdate();
    void EndUpdate();
    void SetBufferView(int, const GpuAddress&, size_t);
    void SetSampler(int, const SamplerImpl<NvnApi>*);
    void SetTextureView(int, const TextureViewImpl<NvnApi>*);
    void SetImage(int, const TextureViewImpl<NvnApi>*);
    void SetBufferTextureView(int, const BufferTextureViewImpl<NvnApi>*);
    void SetBufferImage(int, const BufferTextureViewImpl<NvnApi>*);
    void GetDescriptorSlot(DescriptorSlot*, int) const;
    int GetDescriptorSlotIndex(const DescriptorSlot&) const;
};
}  // namespace detail
}  // namespace nn::gfx