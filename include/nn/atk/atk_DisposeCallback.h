#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail::driver {

class DisposeCallback {
public:
    util::IntrusiveListNode m_DisposeLink;

    DisposeCallback() = default;

    virtual ~DisposeCallback() = default;

    virtual void InvalidateData(const void* start, const void* end) = 0;
};
static_assert(sizeof(DisposeCallback) == 0x18);

}  // namespace nn::atk::detail::driver
