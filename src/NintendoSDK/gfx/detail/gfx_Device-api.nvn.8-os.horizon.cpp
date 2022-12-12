#include <nn/gfx/detail/gfx_Device-api.nvn.8.h>

#include "gfx_CommonHelper.h"
#include "gfx_NvnHelper.h"

namespace nn::gfx::detail {

DeviceImpl<NvnApi>::DeviceImpl() {
    state = State_NotInitialized;
}

DeviceImpl<NvnApi>::~DeviceImpl() {}

void DeviceImpl<NvnApi>::Initialize(const DeviceInfo& info) {
    UseMiddleWare();

    PFNNVNDEVICEGETPROCADDRESSPROC getProcAddress =
        reinterpret_cast<PFNNVNDEVICEGETPROCADDRESSPROC>(
            nvnBootstrapLoader("nvnDeviceGetProcAddress"));

    nvnLoadCProcs(nullptr, getProcAddress);

    int deviceFlags = 0;

    NVNdeviceBuilder builder;
    nvnDeviceBuilderSetDefaults(&builder);
    nvnDeviceBuilderSetFlags(&builder, deviceFlags);

    pNvnDevice = &nvnDevice;

    NVNboolean result = nvnDeviceInitialize(pNvnDevice, &builder);
    nvnLoadCProcs(pNvnDevice, getProcAddress);

    supportedFeatures = Nvn::GetDeviceFeature(pNvnDevice);
    flags.SetBit(Flag_Shared, false);
    state = State_Initialized;
}

void DeviceImpl<NvnApi>::Finalize() {
    nvnDeviceFinalize(pNvnDevice);
    pNvnDevice = nullptr;
    state = State_NotInitialized;
}

}  // namespace nn::gfx::detail