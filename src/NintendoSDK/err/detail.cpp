#include <nn/err/detail.h>

#include <nn/diag.h>
#include <nn/fs.h>
#include <nn/util.h>

namespace nn::err::detail {
static const char* sMessageKindStrs[] = {"DlgMsg", "DlgBtn", "FlvMsg", "FlvBtn"};

void MakeErrorCodeString(char* outErrorCodeString, size_t errorCodeStringBufferSize,
                         ErrorCode errorCode) {
    util::TSNPrintf(outErrorCodeString, errorCodeStringBufferSize, "%04d-%04d",
                    errorCode.GetCategory(), errorCode.GetNumber());
}

void MakeApplicationErrorCodeString(
    char* outErrorCodeString, size_t errorCodeStringBufferSize,
    const ns::ApplicationErrorCodeCategory& applicationErrorCodeCategory, u32 errorCodeNumber) {
    util::TSNPrintf(outErrorCodeString, errorCodeStringBufferSize, "2-%s-%04d",
                    applicationErrorCodeCategory.GetCategory(), errorCodeNumber);
}

// void ParseErrorCodeString(ErrorCode* outErrorCode, const char* errorCodeString) {}

// void ParseApplicationErrorCodeString(
//     ns::ApplicationErrorCodeCategory* outApplicationErrorCodeCategory,
//     u32* outErrorCodeCategoryNumber, const char* errorCodeString) {}

bool IsApplicationErrorCodeString(const char* errorCodeString) {
    char c1 = errorCodeString[0];
    char c2;
    if (c1 == '2') {
        c1 = errorCodeString[1];
        c2 = '-';
    } else {
        c2 = '2';
    }

    return c1 == c2;
}

// bool TryParseErrorCodeString(ErrorCode* outErrorCode, const char* errorCodeString) {}

// bool TryParseApplicationErrorCodeString(
//     ns::ApplicationErrorCodeCategory* outApplicationErrorCodeCategory,
//     u32* outErrorCodeCategoryNumber, const char* errorCodeString) {}

void MakeErrorInfoModuleDirectoryPath(char* outErrorInfoModuleDirectoryPath,
                                      size_t errorInfoModuleDirectoryPathBufferSize,
                                      u32 errorCodeCategory) {
    util::TSNPrintf(outErrorInfoModuleDirectoryPath, errorInfoModuleDirectoryPathBufferSize,
                    "%s:/%04d", "err", errorCodeCategory);
}

void MakeErrorInfoDirectoryPath(char* outErrorInfoDirectoryPath,
                                size_t errorInfoDirectoryPathBufferSize, ErrorCode errorCode) {
    util::TSNPrintf(outErrorInfoDirectoryPath, errorInfoDirectoryPathBufferSize, "%s:/%04d/%04d",
                    "err", errorCode.GetCategory(), errorCode.GetNumber());
}

void MakeErrorInfoCommonFilePath(char* outErrorInfoCommonFilePath,
                                 size_t errorInfoCommonFilePathBufferSize, ErrorCode errorCode) {
    util::TSNPrintf(outErrorInfoCommonFilePath, errorInfoCommonFilePathBufferSize,
                    "%s:/%04d/%04d/common", "err", errorCode.GetCategory(), errorCode.GetNumber());
}

void MakeErrorInfoMessageFilePath(char* outErrorInfoMessageFilePath,
                                  size_t errorInfoMessageFilePathBufferSize, ErrorCode errorCode,
                                  settings::LanguageCode languageCode, MessageKind messageKind) {
    u8 kind = static_cast<u8>(messageKind);
    if (kind >= 4) {
        nn::detail::UnexpectedDefaultImpl("", "", 0);
    }

    util::TSNPrintf(outErrorInfoMessageFilePath, errorInfoMessageFilePathBufferSize,
                    "%s:/%04d/%04d/%s_%s", "err", errorCode.GetCategory(), errorCode.GetNumber(),
                    languageCode.code, sMessageKindStrs[kind]);
}

bool ErrorMessageDataExists(ErrorCode errorCode) {
    char errorInfoDirectoryPath[32];
    MakeErrorInfoDirectoryPath(errorInfoDirectoryPath, sizeof(errorInfoDirectoryPath), errorCode);

    fs::DirectoryHandle handle;
    Result result =
        fs::OpenDirectory(&handle, errorInfoDirectoryPath, fs::OpenDirectoryMode_Directory);

    if (result.IsSuccess()) {
        fs::CloseDirectory(handle);

        return true;
    }

    if (result != fs::ResultPathNotFound()) {
        diag::detail::AbortImpl("", "", "", 0, result,
                                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
                                "result", result.GetModule(), result.GetDescription(),
                                result.GetInnerValueForDebug());
    }

    return false;
}

bool CategoryExists(u32 category) {
    char errorInfoModuleDirectoryPath[32];
    MakeErrorInfoModuleDirectoryPath(errorInfoModuleDirectoryPath,
                                     sizeof(errorInfoModuleDirectoryPath), category);

    fs::DirectoryHandle handle;
    Result result =
        fs::OpenDirectory(&handle, errorInfoModuleDirectoryPath, fs::OpenDirectoryMode_Directory);

    if (result.IsSuccess()) {
        fs::CloseDirectory(handle);

        return true;
    }

    if (result != fs::ResultPathNotFound()) {
        diag::detail::AbortImpl("", "", "", 0, result,
                                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
                                "result", result.GetModule(), result.GetDescription(),
                                result.GetInnerValueForDebug());
    }

    return false;
}

bool DefaultErrorMessageDataExists(u32 category) {
    char errorInfoDirectoryPath[32];
    MakeErrorInfoDirectoryPath(errorInfoDirectoryPath, sizeof(errorInfoDirectoryPath),
                               {category, 9999});

    fs::DirectoryHandle handle;
    Result result =
        fs::OpenDirectory(&handle, errorInfoDirectoryPath, fs::OpenDirectoryMode_Directory);

    if (result.IsSuccess()) {
        fs::CloseDirectory(handle);

        return true;
    }

    if (result != fs::ResultPathNotFound()) {
        diag::detail::AbortImpl("", "", "", 0, result,
                                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
                                "result", result.GetModule(), result.GetDescription(),
                                result.GetInnerValueForDebug());
    }

    return false;
}

void* ReadMessageFile(char16* outMessage, s32* outMessageLength, size_t messageBufferSize,
                      ErrorCode errorCode, settings::LanguageCode languageCode,
                      MessageKind messageKind) {
    char errorInfoMessageFilePath[32];
    MakeErrorInfoMessageFilePath(errorInfoMessageFilePath, sizeof(errorInfoMessageFilePath),
                                 errorCode, languageCode, messageKind);

    fs::FileHandle fileHandle;
    Result result =
        fs::OpenFile(&fileHandle, errorInfoMessageFilePath, fs::OpenDirectoryMode_Directory);

    if (result.IsSuccess()) {
        u64 fileSize;
        result = fs::GetFileSize(&fileSize, fileHandle);
    }

    if (result != fs::ResultPathNotFound()) {
        diag::detail::AbortImpl("", "", "", 0, result,
                                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
                                "result", result.GetModule(), result.GetDescription(),
                                result.GetInnerValueForDebug());
    }

    *outMessageLength = 0;
    *outMessage = u'\0';

    return nullptr;
}

void ReadVersion(ErrorMessageDatabaseVersion* outMessageDatabaseVersion) {}

void* ReadMessageFile(char16* outMessage, size_t messageBufferSize, const char* errorCodeString,
                      const settings::LanguageCode& languageCode) {}
}  // namespace nn::err::detail
