#pragma once

#include <nn/time.h>
#include <nn/util.h>

namespace nn::os {

class Tick;
s64 ConvertToTick(TimeSpan timeSpan);
TimeSpan ConvertToTimeSpan(Tick ticks);

class Tick {
public:
    Tick(s64 tick) : m_Tick(tick) {}

    Tick(TimeSpan timeSpan) : m_Tick(ConvertToTick(timeSpan)) {}

    s64 GetInt64Value() const { return m_Tick; }

    TimeSpan ToTimeSpan() const { return ConvertToTimeSpan(*this); }

    Tick& operator-=(Tick rhs) {
        *this = *this - rhs;
        return *this;
    }

    Tick operator-(Tick rhs) const { return m_Tick - rhs.m_Tick; }

    Tick& operator+=(Tick rhs) {
        *this = *this + rhs;
        return *this;
    }

    Tick operator+(Tick rhs) const { return m_Tick + rhs.m_Tick; }

private:
    s64 m_Tick;
};

}  // namespace nn::os