#include <nn/err/ErrorCode.h>

namespace nn::err {
bool ErrorCode::IsValid() const {
    return mCategory != 0;
}

ErrorCode ErrorCode::GetInvalidErrorCode() {
    return {};
}
}  // namespace nn::err
