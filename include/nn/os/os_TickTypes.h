#pragma once

#include <nn/time.h>
#include <nn/util.h>

namespace nn::os {

class Tick;

Tick GetSystemTick();
Tick GetSystemTickFrequency();
Tick ConvertToTick(TimeSpan timeSpan);
TimeSpan ConvertToTimeSpan(Tick ticks);

class Tick {
public:
    Tick(s64 tick) : value(tick) {}

    Tick(TimeSpan timeSpan) : value(ConvertToTick(timeSpan).GetInt64Value()) {}

    s64 GetInt64Value() const { return value; }

    TimeSpan ToTimeSpan() const { return ConvertToTimeSpan(*this); }

    Tick& operator-=(Tick rhs) {
        *this = *this - rhs;
        return *this;
    }

    Tick operator-(Tick rhs) const { return value - rhs.value; }

    Tick& operator+=(Tick rhs) {
        *this = *this + rhs;
        return *this;
    }

    Tick operator+(Tick rhs) const { return value + rhs.value; }

    s64 value;

// TODO: DWARF shows value as private and named m_Tick.
// It's being kept as it was for now to keep sead
// from not compiling. Fix this before merging to master.
// private:
//     s64 m_Tick;
};

}  // namespace nn::os