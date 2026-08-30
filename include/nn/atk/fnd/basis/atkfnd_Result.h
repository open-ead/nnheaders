#pragma once

#include <nn/types.h>

namespace nn::atk::detail::fnd {
enum FndResultType {
    FndResultType_ErrorFlagMask = 1 << 31,
    FndResultType_CategoryMask = 0b1111111 << 24,
    FndResultType_CodeMask = 0xffffff,

    FndResultType_CategoryBitOffset = 24,
    FndResultType_ErrorFlag = FndResultType_ErrorFlagMask,

    FndResultType_CategorySystem = 0,
    FndResultType_CategoryIo = 1 << FndResultType_CategoryBitOffset,
    
    FndResultType_True = FndResultType_CategorySystem,
    FndResultType_False,

    FndResultType_Failed = FndResultType_ErrorFlag | FndResultType_CategorySystem,
    FndResultType_NotInitialized,
    FndResultType_NotSupported,
    FndResultType_NotOpened,
    FndResultType_OutOfMemory,
    FndResultType_InvalidArgument,
    FndResultType_InvalidStatus,

    FndResultType_IoError = FndResultType_ErrorFlag | FndResultType_CategoryIo,
    FndResultType_IoFileNotFound,
    FndResultType_IoInvalidAccess,
    FndResultType_IoTargetLocked,
};

class FndResult {
public:
    FndResult() = default;
    
    explicit FndResult(u32 value)
        : value{value} {};

    explicit FndResult(FndResultType value)
        : value(value) {};

    bool IsSucceeded() const {
        return !IsFailed();
    }

    bool IsTrue() const {
        return value == FndResultType_True;
    }

    bool IsFalse() const {
        return value == FndResultType_False;
    }

    bool IsFailed() const {
        return value == static_cast<u32>(FndResultType_Failed);
    }

    const char* ToString();

    void PrintResult();

    explicit operator u32() const { 
        return value; 
    }

    explicit operator FndResultType() const {
        return FndResultType(value);
    }
private:
    u32 value;

};
static_assert(sizeof(FndResult) == 0x4);
} // namespace nn::atk::detail::fnd