#pragma once

#include <nn/err.h>
#include <nn/err/ErrorCode.h>
#include <nn/err/detail/MessageKind.h>
#include <nn/settings.h>
#include <nn/types.h>

namespace nn::ns {
// TODO: move
class ApplicationErrorCodeCategory {
public:
    const char* GetCategory() const { return mCategory; }

private:
    char mCategory[8];
};
}  // namespace nn::ns

namespace nn::err {
class ErrorMessageDatabaseVersion;
}  // namespace nn::err

namespace nn::err::detail {
bool CategoryExists(u32 category);
bool DefaultErrorMessageDataExists(u32 category);
bool ErrorMessageDataExists(ErrorCode errorCode);
bool IsApplicationErrorCodeString(const char* errorCodeString);
void MakeApplicationErrorCodeString(
    char* outErrorCodeString, size_t errorCodeStringBufferSize,
    const ns::ApplicationErrorCodeCategory& applicationErrorCodeCategory, u32 errorCodeNumber);
void MakeErrorCodeString(char* outErrorCodeString, size_t errorCodeStringBufferSize,
                         ErrorCode errorCode);
void MakeErrorInfoCommonFilePath(char* outErrorInfoCommonFilePath,
                                 size_t errorInfoCommonFilePathBufferSize, ErrorCode errorCode);
void MakeErrorInfoDirectoryPath(char* outErrorInfoDirectoryPath,
                                size_t errorInfoDirectoryPathBufferSize, ErrorCode errorCode);
void MakeErrorInfoMessageFilePath(char* outErrorInfoMessageFilePath,
                                  size_t errorInfoMessageFilePathBufferSize, ErrorCode errorCode,
                                  settings::LanguageCode languageCode, MessageKind messageKind);
void MakeErrorInfoModuleDirectoryPath(char* outErrorInfoModuleDirectoryPath,
                                      size_t errorInfoModuleDirectoryPathBufferSize,
                                      u32 errorCodeCategory);
void ParseApplicationErrorCodeString(
    ns::ApplicationErrorCodeCategory* outApplicationErrorCodeCategory,
    u32* outErrorCodeCategoryNumber, const char* errorCodeString);
void ParseErrorCodeString(ErrorCode* outErrorCode, const char* errorCodeString);
void* ReadMessageFile(char16* outBuffer, s32* outMessageLength, size_t messageBufferSize,
                      ErrorCode errorCode, settings::LanguageCode languageCode,
                      MessageKind messageKind);
void ReadMessageFile(char16* outMessage, size_t messageBufferSize, const char* errorCodeString,
                     const settings::LanguageCode& languageCode);
void ReadVersion(ErrorMessageDatabaseVersion* outVersion);
bool TryParseApplicationErrorCodeString(
    ns::ApplicationErrorCodeCategory* outApplicationErrorCodeCategory,
    u32* outErrorCodeCategoryNumber, const char* errorCodeString);
bool TryParseErrorCodeString(ErrorCode* outErrorCode, const char* errorCodeString);
}  // namespace nn::err::detail
