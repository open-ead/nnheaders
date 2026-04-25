#pragma once

#include <cstddef>
#include <cstdint>
#include <vapours/results.hpp>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
#ifdef __aarch64__
using u128 = __uint128_t;
#endif

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;
#ifdef __aarch64__
using s128 = __int128_t;
#endif

using f32 = float;
using f64 = double;
#ifdef __aarch64__
using f128 = long double;
#endif

using char16 = char16_t;
using char32 = char32_t;

using ptrdiff = std::ptrdiff_t;
using uintptr = std::uintptr_t;
using intptr = std::intptr_t;
using size = std::size_t;
