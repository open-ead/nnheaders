#pragma once

#include <nn/os.h>

namespace nn::atk::detail::fnd {
class TimeSpan {
public:
    using TickType = s64;

    os::Tick FromNanoSeconds(TickType);
    os::Tick FromMicroSeconds(TickType);
    os::Tick FromMilliSeconds(TickType);
    
    TickType ToNanoSeconds() const;
    TickType ToMicroSeconds() const;
    TickType ToMilliSeconds() const;

private:
    TickType m_TickSpan;
};
static_assert(sizeof(TimeSpan) == 0x8);

class Time {
public:
    s64 Current();
};
} // namespace nn::atk::detail::fnd