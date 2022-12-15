#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_SyncData-api.nvn.8.h>

namespace nn {

class TimeSpan;

namespace gfx {

class FenceInfo;
class SemaphoreInfo;

namespace detail {

template <>
class FenceImpl<NvnApi> : public DataContainer<FenceImplData<NvnApi>> {
    NN_NO_COPY(FenceImpl);

public:
    typedef FenceInfo InfoType;

    FenceImpl();
    ~FenceImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
    bool IsSignaled() const;
    SyncResult Sync(TimeSpan) const;
};

template <>
class SemaphoreImpl<NvnApi> : public DataContainer<SemaphoreImplData<NvnApi>> {
    NN_NO_COPY(SemaphoreImpl);

public:
    typedef SemaphoreInfo InfoType;

    SemaphoreImpl();
    ~SemaphoreImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
};

}  // namespace detail
}  // namespace gfx
}  // namespace nn