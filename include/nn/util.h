/**
 * @file util.h
 * @brief Helper functions for OS functionality.
 */

#pragma once

#include <nn/types.h>

namespace nn {
namespace util {
struct Unorm8x4 {
    u8 elements[0x4];
};

enum CharacterEncodingResult { Success, BadLength, InvalidFormat };

CharacterEncodingResult PickOutCharacterFromUtf8String(char*, char const** str);
CharacterEncodingResult ConvertCharacterUtf8ToUtf32(u32* dest, char const* src);
CharacterEncodingResult ConvertStringUtf16NativeToUtf8(char*, s32, u16 const*, s32);
CharacterEncodingResult ConvertStringUtf8ToUtf16Native(u16*, s32, char const*, s32);

template <s32 size, typename T>
struct BitFlagSet {};

void ReferSymbol(const void*);
}  // namespace util
}  // namespace nn

#ifndef NN_VER
#define NN_VER 0
#endif

#define NN_MIDDLEWARE(var, company, name)                                                          \
    static const char var[] __attribute__((section(".api_info"))) = "SDK MW+" company "+" name

// todo: move with more info
#define NN_NO_COPY(CLASS)                                                                          \
    CLASS(const CLASS&) = delete;                                                                  \
    CLASS& operator=(const CLASS&) = delete

#define NN_NO_MOVE(CLASS)                                                                          \
    CLASS(CLASS&&) = delete;                                                                       \
    CLASS& operator=(CLASS&&) = delete

namespace nn::detail {

[[noreturn]] void UnexpectedDefaultImpl(const char*, const char*, int);

}  // namespace nn::detail

// todo: ifdef to support debug functionality?
#define NN_UNEXPECTED_DEFAULT nn::detail::UnexpectedDefaultImpl("", "", 0)