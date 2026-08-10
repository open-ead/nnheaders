#pragma once

#include <cstddef>

namespace nn::atk::detail::fnd {
template <typename T>
inline T Clamp(T x, T low, T high);

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
inline TDiff GetOffsetFromPtr(const void* start, const void* end);

inline ptrdiff_t GetOffsetFromPtr(const void* start, const void* end);

template <typename TOffset>
inline void* AddOffsetToPtr(void* ptr, TOffset offset);
} // namespace nn::atk::detail::fnd