/**
 * @file deviceimpl.h
 * @brief Device implementation for GFX.
 */

#pragma once

#include <nn/types.h>
#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nvn/nvn_types.h>

namespace nn::gfx {

class DeviceInfo;

namespace detail {

template <>
class DeviceImpl<NvnApi> {
public:
    DeviceImpl();
    ~DeviceImpl();

    void Initialize(const DeviceInfo& deviceInfo);
    void Finalize();

	NVNdevice* field_0;
	NVNdevice field_8;
	bool initialized; // might be a u8
	u8 field_3009;
	int deviceFeatures;
	char _3010[8]; // pointer?
};

}  // namespace detail

}  // namespace nn::gfx
