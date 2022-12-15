#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_PipelineData-api.nvn.8.h>

namespace nn::gfx {

class GraphicsPipelineInfo;
class ComputePipelineInfo;

namespace detail {

template <>
class PipelineImpl<NvnApi> : public DataContainer<PipelineImplData<NvnApi>> {
    NN_NO_COPY(PipelineImpl);

public:
    typedef GraphicsPipelineInfo GraphicsInfoType;
    typedef ComputePipelineInfo ComputeInfoType;

    static size_t GetRequiredMemorySize(const GraphicsInfoType&);
    static size_t GetRequiredMemorySize(const ComputeInfoType&);

    PipelineImpl();
    ~PipelineImpl();
    void SetMemory(void*, size_t);
    void* GetMemory();
    const void* GetMemory() const;
    void Initialize(DeviceImpl<NvnApi>*, const GraphicsInfoType&);
    void Initialize(DeviceImpl<NvnApi>*, const ComputeInfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
};

}  // namespace detail

}  // namespace nn::gfx