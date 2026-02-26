#pragma once

namespace nn::atk::detail {
class IRegionInfoReadable {
public:
    virtual ~IRegionInfoReadable() = default;
};
static_assert(sizeof(IRegionInfoReadable) == 0x8);
} // namespace nn::atk::detail