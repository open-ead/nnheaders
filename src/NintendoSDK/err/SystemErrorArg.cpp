#include <nn/err/SystemErrorArg.h>

namespace nn::util {
template <typename T>
inline int Strlcpy(T* pOutDst, const T* pSrc, int count) {
    int length = 0;

    if (count > 0) {
        while (--count && *pSrc) {
            *pOutDst++ = *pSrc++;
            ++length;
        }
        *pOutDst++ = '\0';
    }

    while (*pSrc++)
        ++length;

    return length;
}
}  // namespace nn::util

namespace nn::err {
SystemErrorArg::SystemErrorArg() {
    mDialogMessage[0] = '\0';
    mFullScreenMessage[0] = '\0';
    mLanguageCode.code[0] = '\0';
}

SystemErrorArg::SystemErrorArg(ErrorCode errorCode, const char* dialogMessage,
                               const char* fullScreenMessage,
                               const settings::LanguageCode& languageCode)
    : mErrorCode(errorCode) {
    SetDialogMessage(dialogMessage);
    SetFullScreenMessage(fullScreenMessage);
    SetLanguageCode(languageCode);
}

void SystemErrorArg::SetErrorCode(ErrorCode errorCode) {
    mErrorCode = errorCode;
}

void SystemErrorArg::SetDialogMessage(const char* message) {
    util::Strlcpy(mDialogMessage, message, sizeof(mDialogMessage));
}

void SystemErrorArg::SetFullScreenMessage(const char* message) {
    util::Strlcpy(mFullScreenMessage, message, sizeof(mFullScreenMessage));
}

void SystemErrorArg::SetLanguageCode(const settings::LanguageCode& languageCode) {
    util::Strlcpy(mLanguageCode.code, languageCode.code, sizeof(mLanguageCode.code));
}

ErrorCode SystemErrorArg::GetErrorCode() const {
    return mErrorCode;
}

const char* SystemErrorArg::GetDialogMessage() const {
    return mDialogMessage;
}

const char* SystemErrorArg::GetFullScreenMessage() const {
    return mFullScreenMessage;
}

settings::LanguageCode SystemErrorArg::GetLanguageCode() const {
    return mLanguageCode;
}

void SystemErrorArg::GetStartupParam() const {}
}  // namespace nn::err
