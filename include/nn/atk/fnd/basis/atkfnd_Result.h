#pragma once

#include <nn/types.h>

namespace nn::atk::detail::fnd {
enum FndResultType {
    FndResultType_CodeMask = 0xffffff,
    FndResultType_CategoryMask = 0x7f000000,
    FndResultType_ErrorFlagMask = ~(FndResultType_CategoryMask | FndResultType_CodeMask),

    FndResultType_CategorySystem = 0x0,
    FndResultType_CategoryBitOffset = 0x18,
    FndResultType_CategoryIo = 0x1000000,

    FndResultType_ErrorFlag = FndResultType_ErrorFlagMask,
    
    FndResultType_Failed = FndResultType_ErrorFlag,
    FndResultType_NotInitialized,
    FndResultType_NotSupported,
    FndResultType_NotOpened,
    FndResultType_OutOfMemory,
    FndResultType_InvalidArgument,
    FndResultType_InvalidStatus,

    FndResultType_IoError = -0x7f000000,
    FndResultType_IoFileNotFound,
    FndResultType_IoInvalidAccess,
    FndResultType_IoTargetLocked,

    FndResultType_True = 0,
    FndResultType_False,
};

struct FndResult {
    u32 value;
};
static_assert(sizeof(FndResult) == 0x4);
} // namespace nn::atk::detail::fnd