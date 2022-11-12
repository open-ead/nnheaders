/**
 * @file deviceimpl.h
 * @brief Device implementation for GFX.
 */

#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/types.h>
#include <nvn/nvn.h>

namespace nn::gfx {

class DeviceInfo;

namespace detail {

template <>
class DeviceImpl<NvnApi> {
public:
    // todo: switch to enum class with bitflag class
    enum DeviceFeatures {
        _1 = 1,
        _2 = 2,
        _4 = 4,
        Feature_SupportsConservativeRaster = 8,
    };

    DeviceImpl();
    ~DeviceImpl();

    void Initialize(const DeviceInfo& deviceInfo);
    void Finalize();

    NVNdevice* pnDevice;
    NVNdevice nDevice;
    bool initialized;  // might be a u8
    u8 field_3009;
    int deviceFeatures;
    char _3010[8];  // pointer?
};

}  // namespace detail

}  // namespace nn::gfx
