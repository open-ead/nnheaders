#pragma once

#include <nn/gfx/detail/gfx_Sampler-api.nvn.8.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Device.h>

namespace nn::gfx {

template <class TTarget>
class TSampler : public detail::SamplerImpl<TTarget> {
    NN_NO_COPY(TSampler);

public:
    typedef SamplerInfo InfoType;

    TSampler() = default;
    void Initialize(TDevice<TTarget>* pDevice, const InfoType& info) {
        detail::SamplerImpl<TTarget>::Initialize(pDevice, info);
    }
    void Finalize(TDevice<TTarget>* pDevice) { detail::SamplerImpl<TTarget>::Finalize(pDevice); }
    void SetUserPtr(void*);
    void* GetUserPtr();
    const void* GetUserPtr() const;
};

}  // namespace nn::gfx