#pragma once

#include <nn/os.h>

namespace nn::atk::detail::fnd {
class Time {
public:
    s64 Current();
};

class TimeSpan {
public:
    using TickType = s64;

    static TimeSpan FromNanoSeconds(TickType);
    static TimeSpan FromMicroSeconds(TickType);
    static TimeSpan FromMilliSeconds(TickType);
    
    TickType ToNanoSeconds() const;
    TickType ToMicroSeconds() const;
    TickType ToMilliSeconds() const;

private:
    TickType m_TickSpan;
};
static_assert(sizeof(TimeSpan) == 0x8);
} // namespace nn::atk::detail::fnd