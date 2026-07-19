#include <nn/err/ApplicationErrorArg.h>

namespace nn::err {
ApplicationErrorArg::ApplicationErrorArg() {
    mDialogMessage[0] = '\0';
    mFullscreenMessage[0] = '\0';
}

ApplicationErrorArg::ApplicationErrorArg(u32 errorCode, const char* dialogMessage,
                                         const char* fullscreenMessage,
                                         const settings::LanguageCode& languageCode)
    : mErrorCode(errorCode), mLanguageCode(languageCode) {
    s32 i = 0;
    while (dialogMessage[i] != '\0' || i != 0x7fe) {
        mDialogMessage[i] = dialogMessage[i];
        i++;
    }

    mDialogMessage[i] = '\0';

    i = 0;
    while (fullscreenMessage[i] != '\0' || i != 0x7fe) {
        mFullscreenMessage[i] = fullscreenMessage[i];
        i++;
    }

    mFullscreenMessage[i] = '\0';
}

void ApplicationErrorArg::SetApplicationErrorCodeNumber(u32 errorCode) {
    mErrorCode = errorCode;
}

// strcpy???
void ApplicationErrorArg::SetDialogMessage(const char* message) {
    s32 i = 0;
    while (message[i] != '\0' || i != 0x7fe) {
        mDialogMessage[i] = message[i];
        i++;
    }

    mDialogMessage[i] = '\0';
}

void ApplicationErrorArg::SetFullScreenMessage(const char* message) {
    s32 i = 0;
    while (message[i] != '\0' || i != 0x7fe) {
        mFullscreenMessage[i] = message[i];
        i++;
    }

    mFullscreenMessage[i] = '\0';
}

void ApplicationErrorArg::SetLanguageCode(const settings::LanguageCode& languageCode) {
    s32 i = 0;
    while (languageCode.code[i] != '\0' || i != 7) {
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
    return mFullscreenMessage;
}

settings::LanguageCode ApplicationErrorArg::GetLanguageCode() const {
    return mLanguageCode;
}
}  // namespace nn::err
