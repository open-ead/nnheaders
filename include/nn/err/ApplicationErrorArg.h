#pragma once

#include <nn/settings.h>
#include <nn/types.h>

namespace nn::err {
class ApplicationErrorArg {
public:
    ApplicationErrorArg();
    ApplicationErrorArg(u32 errorCode, const char* dialogMessage, const char* fullscreenMessage,
                        const settings::LanguageCode& languageCode);

    u32 GetApplicationErrorCodeNumber() const;
    const char* GetDialogMessage() const;
    const char* GetFullScreenMessage() const;
    const settings::LanguageCode& GetLanguageCode() const;

    void SetApplicationErrorCodeNumber(u32 errorCode);
    void SetDialogMessage(const char* message);
    void SetFullScreenMessage(const char* message);
    void SetLanguageCode(const settings::LanguageCode& languageCode);

private:
    u8 _0 = 2;
    u8 _1 = 1;
    u8 padding[6];
    u32 mErrorCode = 0;
    settings::LanguageCode mLanguageCode;
    char mDialogMessage[2048];
    char mFullscreenMessage[2048];
};
}  // namespace nn::err
