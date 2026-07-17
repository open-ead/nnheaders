#pragma once

#include <nn/types.h>

namespace nn::err {
class ErrorCode {
public:
    static ErrorCode GetInvalidErrorCode();

    ErrorCode() {}
    ErrorCode(s32 param_1, s32 param_2) : _0(param_1), _4(param_2) {}

    bool IsValid() const;

private:
    s32 _0 = 0;
    s32 _4 = 0;
};
}  // namespace nn::err
