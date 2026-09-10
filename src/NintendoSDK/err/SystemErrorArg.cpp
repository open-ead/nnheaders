#include <nn/err/SystemErrorArg.h>
#include <nn/util/util_StringUtil.h>

namespace nn::err {
// NON_MATCHING
SystemErrorArg::SystemErrorArg() {
    mDialogMessage[0] = '\0';
    mFullScreenMessage[0] = '\0';
    mLanguageCode.code[0] = '\0';
}

// NON_MATCHING
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
