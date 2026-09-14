#pragma once

#include <nn/types.h>

namespace nn::err {
class ErrorCode {
public:
    static ErrorCode GetInvalidErrorCode();

    ErrorCode() {}
    ErrorCode(u32 category, u32 number) : mCategory(category), mNumber(number) {}

    bool IsValid() const;

    u32 GetCategory() const { return mCategory; }
    u32 GetNumber() const { return mNumber; }

private:
    u32 mCategory = 0;
    u32 mNumber = 0;
};
}  // namespace nn::err
