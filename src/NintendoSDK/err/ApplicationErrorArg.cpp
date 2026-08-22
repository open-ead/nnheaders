#include <nn/err/ApplicationErrorArg.h>

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
ApplicationErrorArg::ApplicationErrorArg() {
    mDialogMessage[0] = '\0';
    mFullScreenMessage[0] = '\0';
}

ApplicationErrorArg::ApplicationErrorArg(u32 errorCode, const char* dialogMessage,
                                         const char* fullScreenMessage,
                                         const settings::LanguageCode& languageCode)
    : mErrorCode(errorCode) {
    SetDialogMessage(dialogMessage);
    SetFullScreenMessage(fullScreenMessage);
    SetLanguageCode(languageCode);
}

void ApplicationErrorArg::SetApplicationErrorCodeNumber(u32 errorCode) {
    mErrorCode = errorCode;
}

void ApplicationErrorArg::SetDialogMessage(const char* message) {
    util::Strlcpy(mDialogMessage, message, sizeof(mDialogMessage));
}

void ApplicationErrorArg::SetFullScreenMessage(const char* message) {
    util::Strlcpy(mFullScreenMessage, message, sizeof(mFullScreenMessage));
}

void ApplicationErrorArg::SetLanguageCode(const settings::LanguageCode& languageCode) {
    util::Strlcpy(mLanguageCode.code, languageCode.code, sizeof(mLanguageCode.code));
}

u32 ApplicationErrorArg::GetApplicationErrorCodeNumber() const {
    return mErrorCode;
}

const char* ApplicationErrorArg::GetDialogMessage() const {
    return mDialogMessage;
}

const char* ApplicationErrorArg::GetFullScreenMessage() const {
    return mFullScreenMessage;
}

settings::LanguageCode ApplicationErrorArg::GetLanguageCode() const {
    return mLanguageCode;
}
}  // namespace nn::err
