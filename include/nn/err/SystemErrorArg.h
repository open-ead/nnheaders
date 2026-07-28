#pragma once

#include <nn/err/ErrorCode.h>
#include <nn/settings.h>
#include <nn/types.h>

namespace nn::err {
class SystemErrorArg {
public:
    SystemErrorArg();
    SystemErrorArg(ErrorCode errorCode, const char* dialogMessage, const char* fullScreenMessage,
                   const settings::LanguageCode& languageCode);

    void SetErrorCode(ErrorCode errorCode);
    void SetDialogMessage(const char* message);
    void SetFullScreenMessage(const char* message);
    void SetLanguageCode(const settings::LanguageCode& languageCode);
    ErrorCode GetErrorCode() const;
    const char* GetDialogMessage() const;
    const char* GetFullScreenMessage() const;
    settings::LanguageCode GetLanguageCode() const;
    void GetStartupParam() const;

private:
    s8 _0 = 1;
    s8 _1 = 0;
    s8 _2 = 0;
    s8 _3 = 0;
    s8 _4 = 0;
    s8 _5 = 0;
    s8 _6 = 0;
    s8 _7 = 0;
    ErrorCode mErrorCode;
    settings::LanguageCode mLanguageCode;
    char mDialogMessage[2048];
    char mFullScreenMessage[2048];
};
}  // namespace nn::err
