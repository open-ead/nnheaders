/**
 * @file deviceimpl.h
 * @brief Device implementation for GFX.
 */

#pragma once

namespace nn::gfx {

class DeviceInfo;

namespace detail {

template <typename Api>
class DeviceImpl {
public:
    DeviceImpl();
    ~DeviceImpl();

    void Initialize(nn::gfx::DeviceInfo const& deviceInfo);
    void Finalize();
};

}  // namespace detail

}  // namespace nn::gfx
