#pragma once

#include <cstddef>

namespace nn::atk::detail::fnd {

template <typename T>
inline T ClampMin(T x, T low) {
    return x < low ? low : x;
}

template <typename T>
inline T ClampMax(T x, T high) {
    return x > high ? high : x;
}

template <typename T>
inline T Clamp(T x, T low, T high) {
    return ClampMax(ClampMin(x, low), high);
}

inline float FloatClamp(float value, float min, float max);

template <typename ValueT>
inline ValueT RoundUp(ValueT x, int base);

// TODO: figure out if this is correct for
// void* RoundUp<void>(void* x, int base)
template <typename ValueT = void>
inline ValueT* RoundUp(ValueT* x, int base);

template <typename ValueT>
inline ValueT RoundDown(ValueT x, int base);

template <typename ValueT = void>
inline ValueT* RoundDown(ValueT* x, int base);

template <typename TDiff>
inline TDiff GetOffsetFromPtr(const void* start, const void* end) {
    return reinterpret_cast<TDiff>(end) - reinterpret_cast<TDiff>(start);
}

inline ptrdiff_t GetOffsetFromPtr(const void* start, const void* end) {
    return reinterpret_cast<ptrdiff_t>(end) - reinterpret_cast<ptrdiff_t>(start);
}

template <typename TOffset>
inline void* AddOffsetToPtr(void* ptr, TOffset offset);

}  // namespace nn::atk::detail::fnd