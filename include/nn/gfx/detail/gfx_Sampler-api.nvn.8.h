#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_SamplerData-api.nvn.8.h>

namespace nn::gfx {

class SamplerInfo;

namespace detail {
template <>
class SamplerImpl<NvnApi> : public DataContainer<SamplerImplData<NvnApi>> {
    NN_NO_COPY(SamplerImpl);

public:
    typedef SamplerInfo InfoType;

    SamplerImpl();
    ~SamplerImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
};

}  // namespace detail
}  // namespace nn::gfx