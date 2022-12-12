#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_QueueData-api.nvn.8.h>

namespace nn::gfx {

class QueueInfo;

namespace detail {

template <>
class QueueImpl<NvnApi> : public DataContainer<QueueImplData<NvnApi>> {
    NN_NO_COPY(QueueImpl);

public:
    typedef QueueInfo InfoType;

    QueueImpl();
    ~QueueImpl();
    void Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
    void ExecuteCommand(CommandBufferImpl<NvnApi>*, FenceImpl<NvnApi>*);
    void Flush() const;
    void Sync() const;
    void SetSemaphore(SemaphoreImpl<NvnApi>*);
    void SyncSemaphore(const SemaphoreImpl<NvnApi>*);
    void CopyToScanBuffer(SwapChainImpl<NvnApi>*, const ColorTargetViewImpl<NvnApi>*);
    void Present(SwapChainImpl<NvnApi>*, int);
};

}  // namespace detail
}  // namespace nn::gfx