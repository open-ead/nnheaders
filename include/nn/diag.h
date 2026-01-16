/**
 * @file diag.h
 * @brief Module, logging, and symbol operations.
 */

#pragma once

#include <nn/types.h>

namespace nn {
namespace diag {
struct LogMetaData;

struct ModuleInfo {
    char* mPath;
    u64 mBaseAddr;
    u64 mSize;
};

enum AssertionType {};

namespace detail {
// LOG
void LogImpl(nn::diag::LogMetaData const&, char const*, ...);
void AbortImpl(char const*, char const*, char const*, s32);
void AbortImpl(char const*, char const*, char const*, int, Result);

void OnAssertionFailure(nn::diag::AssertionType, char const*, char const*, char const*, int);
}  // namespace detail

// MODULE / SYMBOL
size_t GetSymbolName(char* name, size_t nameSize, uintptr_t addr);
u64 GetRequiredBufferSizeForGetAllModuleInfo();
s32 GetAllModuleInfo(nn::diag::ModuleInfo** out, void* buffer, u64 bufferSize);
u64 GetSymbolSize(u64 addr);
}  // namespace diag
}  // namespace nn
