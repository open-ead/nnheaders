#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail::driver {
class DisposeCallback {
public:
    virtual ~DisposeCallback() = default;

private:
    util::IntrusiveListNode m_DisposeLink;
};
static_assert(sizeof(DisposeCallback) == 0x18);

} // namespace nn::atk::detail::driver