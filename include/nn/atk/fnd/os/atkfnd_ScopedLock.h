#pragma once

namespace nn::atk::detail::fnd {
template <typename LockObj>
class ScopedLock {
public:
    LockObj* m_LockObj;
};
} // namespace nn::atk::detail::fnd