#include <nn/err/ApplicationErrorArg.h>

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
    s32 i = 0;
    while (message[i] != '\0' && i < 0x7fe) {
        mDialogMessage[i] = message[i];
        i++;
    }

    mDialogMessage[i] = '\0';
}

void ApplicationErrorArg::SetFullScreenMessage(const char* message) {
    s32 i = 0;
    while (message[i] != '\0' && i < 0x7fe) {
        mFullScreenMessage[i] = message[i];
        i++;
    }

    mFullScreenMessage[i] = '\0';
}

void ApplicationErrorArg::SetLanguageCode(const settings::LanguageCode& languageCode) {
    s32 i = 0;
    while (languageCode.code[i] && i < 7) {
        mLanguageCode.code[i] = languageCode.code[i];
        i++;
    }

    mLanguageCode.code[i] = '\0';
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
