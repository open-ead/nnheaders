/**
 * @file diag.h
 * @brief Module, logging, and symbol operations.
 */

#pragma once

#include <nn/types.h>

namespace nn::diag {
struct LogMetaData;

struct ModuleInfo {
    char* mPath;
    u64 mBaseAddr;
    u64 mSize;
};

enum AssertionType {};

namespace detail {
void LogImpl(const LogMetaData&, char const*, ...);
[[noreturn]] void AbortImpl(char const*, char const*, char const*, s32);

[[gnu::format(printf, 6, 7)]] [[noreturn]] void AbortImpl(char const*, char const*, char const*,
                                                          s32, const Result*, const char*, ...);

void OnAssertionFailure(AssertionType, char const*, char const*, char const*, s32);
}  // namespace detail

// MODULE / SYMBOL
size_t GetSymbolName(char* name, size_t nameSize, uintptr_t addr);
u64 GetRequiredBufferSizeForGetAllModuleInfo();
s32 GetAllModuleInfo(ModuleInfo** out, void* buffer, u64 bufferSize);
u64 GetSymbolSize(u64 addr);
}  // namespace nn::diag
