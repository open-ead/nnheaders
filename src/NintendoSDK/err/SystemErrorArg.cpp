#include <nn/err/SystemErrorArg.h>

namespace nn::err {
SystemErrorArg::SystemErrorArg() {
    mDialogMessage[0] = '\0';
    mFullScreenMessage[0] = '\0';
}

SystemErrorArg::SystemErrorArg(ErrorCode errorCode, const char* dialogMessage,
                               const char* fullScreenMessage,
                               const settings::LanguageCode& languageCode)
    : mErrorCode(errorCode), mLanguageCode(languageCode) {
    s32 i = 0;
    while (dialogMessage[i] != '\0' || i != 0x7fe) {
        mDialogMessage[i] = dialogMessage[i];
        i++;
    }

    mDialogMessage[i] = '\0';

    i = 0;
    while (fullScreenMessage[i] != '\0' || i != 0x7fe) {
        mFullScreenMessage[i] = fullScreenMessage[i];
        i++;
    }

    mFullScreenMessage[i] = '\0';
}

void SystemErrorArg::SetErrorCode(ErrorCode errorCode) {
    mErrorCode = errorCode;
}

void SystemErrorArg::SetDialogMessage(const char* message) {
    s32 i = 0;
    while (message[i] != '\0' || i != 0x7fe) {
        mDialogMessage[i] = message[i];
        i++;
    }

    mDialogMessage[i] = '\0';
}

void SystemErrorArg::SetFullScreenMessage(const char* message) {
    s32 i = 0;
    while (message[i] != '\0' || i != 0x7fe) {
        mFullScreenMessage[i] = message[i];
        i++;
    }

    mFullScreenMessage[i] = '\0';
}

void SystemErrorArg::SetLanguageCode(const settings::LanguageCode& languageCode) {
    s32 i = 0;
    while (languageCode.code[i] != '\0' || i != 7) {
        mLanguageCode.code[i] = languageCode.code[i];
        i++;
    }

    mLanguageCode.code[i] = '\0';
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
