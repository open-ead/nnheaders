#include <nn/err/ErrorResultVariant.h>

namespace nn::err {
ErrorResultVariant::ErrorResultVariant() : mValue({}) {}

ErrorResultVariant::ErrorResultVariant(const Result& result)
    : mState(State::Result), mValue({.mResult = result}) {}

ErrorResultVariant::ErrorResultVariant(const ErrorCode& errorCode)
    : mState(State::ErrorCode), mValue({.mErrorCode = errorCode}) {}

void ErrorResultVariant::operator=(const Result& result) {
    mState = State::Result;
    mValue.mResult = result;
}

void ErrorResultVariant::operator=(const ErrorCode& errorCode) {
    mState = State::ErrorCode;
    mValue.mErrorCode = errorCode;
}

ErrorResultVariant::operator Result() const {
    return mValue.mResult;
}

ErrorResultVariant::operator ErrorCode() const {
    return mValue.mErrorCode;
}

ErrorResultVariant::State ErrorResultVariant::GetState() const {
    return mState;
}
}  // namespace nn::err
