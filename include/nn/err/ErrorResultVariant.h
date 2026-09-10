#pragma once

#include <nn/err/ErrorCode.h>
#include <nn/types.h>

namespace nn::err {
class ErrorResultVariant {
public:
    enum class State : u32 {
        Undefined,
        Result,
        ErrorCode,
    };

    ErrorResultVariant();
    ErrorResultVariant(const Result& result);
    ErrorResultVariant(const ErrorCode& errorCode);

    State GetState() const;

    operator ErrorCode() const;
    operator Result() const;

    void operator=(const Result& result);
    void operator=(const ErrorCode& errorCode);

private:
    State mState = State::Undefined;
    union {
        Result mResult;
        ErrorCode mErrorCode;
    } mValue;
};
}  // namespace nn::err
