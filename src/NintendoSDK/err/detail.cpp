#include <nn/err/detail.h>

#include <nn/diag.h>
#include <nn/err/ErrorMessageDatabaseVersion.h>
#include <nn/fs.h>
#include <nn/util.h>
#include <nn/util/util_StringUtil.h>

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
    return util::Strncmp(errorCodeString, "2-", 2) == 0;
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
    if (messageKind > MessageKind::FlvButton) {
        NN_UNEXPECTED_DEFAULT;
    }

    const char* messageKindStr = sMessageKindStrs[static_cast<s8>(messageKind)];
    util::TSNPrintf(outErrorInfoMessageFilePath, errorInfoMessageFilePathBufferSize,
                    "%s:/%04d/%04d/%s_%s", "err", errorCode.GetCategory(), errorCode.GetNumber(),
                    languageCode.code, messageKindStr);
}

// NON_MATCHING: str w0, [sp] after bl nn::fs::OpenDirectory is in the wrong place
bool ErrorMessageDataExists(ErrorCode errorCode) {
    char errorInfoDirectoryPath[32];
    MakeErrorInfoDirectoryPath(errorInfoDirectoryPath, sizeof(errorInfoDirectoryPath), errorCode);

    fs::DirectoryHandle handle;
    if (Result result =
            fs::OpenDirectory(&handle, errorInfoDirectoryPath, fs::OpenDirectoryMode_Directory);
        result.IsFailure()) {
        if (result != fs::ResultPathNotFound()) {
            diag::detail::AbortImpl(
                "", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x", "result",
                result.GetModule(), result.GetDescription(), result.GetInnerValueForDebug()
#else
                ""
#endif
            );
        }

        return false;
    }

    fs::CloseDirectory(handle);

    return true;
}

// NON_MATCHING: str w0, [sp] after bl nn::fs::OpenDirectory is in the wrong place
bool CategoryExists(u32 category) {
    char errorInfoModuleDirectoryPath[32];
    MakeErrorInfoModuleDirectoryPath(errorInfoModuleDirectoryPath,
                                     sizeof(errorInfoModuleDirectoryPath), category);

    fs::DirectoryHandle handle;
    if (Result result = fs::OpenDirectory(&handle, errorInfoModuleDirectoryPath,
                                          fs::OpenDirectoryMode_Directory);
        result.IsFailure()) {
        if (result != fs::ResultPathNotFound()) {
            diag::detail::AbortImpl(
                "", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x", "result",
                result.GetModule(), result.GetDescription(), result.GetInnerValueForDebug()
#else
                ""
#endif
            );
        }

        return false;
    }

    fs::CloseDirectory(handle);

    return true;
}

// NON_MATCHING: str w0, [sp] after bl nn::fs::OpenDirectory is in the wrong place
bool DefaultErrorMessageDataExists(u32 category) {
    char errorInfoDirectoryPath[32];
    MakeErrorInfoDirectoryPath(errorInfoDirectoryPath, sizeof(errorInfoDirectoryPath),
                               {category, 9999});

    fs::DirectoryHandle handle;
    if (Result result =
            fs::OpenDirectory(&handle, errorInfoDirectoryPath, fs::OpenDirectoryMode_Directory);
        result.IsFailure()) {
        if (result != fs::ResultPathNotFound()) {
            diag::detail::AbortImpl(
                "", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x", "result",
                result.GetModule(), result.GetDescription(), result.GetInnerValueForDebug()
#else
                ""
#endif
            );
        }

        return false;
    }

    fs::CloseDirectory(handle);

    return true;
}

// NON_MATCHING: str w0, [sp, #8] after bl nn::fs::OpenFile is in the wrong place
void* ReadMessageFile(char16* outBuffer, s32* outMessageLength, size_t messageBufferSize,
                      ErrorCode errorCode, settings::LanguageCode languageCode,
                      MessageKind messageKind) {
    char errorInfoMessageFilePath[32];

    MakeErrorInfoMessageFilePath(errorInfoMessageFilePath, sizeof(errorInfoMessageFilePath),
                                 errorCode, languageCode, messageKind);

    fs::FileHandle fileHandle;
    if (Result result = fs::OpenFile(&fileHandle, errorInfoMessageFilePath, fs::OpenMode_Read);
        result.IsFailure()) {
        if (result == fs::ResultPathNotFound()) {
            *outMessageLength = 0;
            *outBuffer = u'\0';

            return nullptr;
        }

        diag::detail::AbortImpl("", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
                                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue:
                                    0x %
                                    08x ", " result ", result.GetModule(),
                                    result.GetDescription(),
                                result.GetInnerValueForDebug()
#else
                                ""
#endif
        );
    }

    s64 fileSize;
    if (Result result = fs::GetFileSize(&fileSize, fileHandle); result.IsFailure()) {
        diag::detail::AbortImpl("", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
                                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
                                "fs::GetFileSize(&fileSize, fileHandle)", result.GetModule(),
                                result.GetDescription(), result.GetInnerValueForDebug()
#else
                                ""
#endif
        );
    }

    if (fileSize % 2 != 0) {
        diag::detail::AbortImpl("", "", "", 0);
    }

    *outMessageLength = static_cast<s32>(fileSize >> 1);

    if (messageBufferSize <= *outMessageLength) {
        diag::detail::AbortImpl("", "", "", 0);
    }

    if (Result result = fs::ReadFile(fileHandle, 0, outBuffer, static_cast<size_t>(fileSize));
        result.IsFailure()) {
        diag::detail::AbortImpl(
            "", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
            "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
            "fs::ReadFile(fileHandle, 0, outBuffer, static_cast<size_t>(fileSize))",
            result.GetModule(), result.GetDescription(), result.GetInnerValueForDebug()
#else
            ""
#endif
        );
    }

    outBuffer[*outMessageLength] = u'\0';
    fs::CloseFile(fileHandle);

    return nullptr;
}

void ReadVersion(ErrorMessageDatabaseVersion* outVersion) {
    if (fs::MountSystemData("err", ncm::SystemDataId::Err).IsFailure()) {
        return;
    }

    char infoFilePath[32];
    util::TSNPrintf(infoFilePath, sizeof(infoFilePath), "%s:/DatabaseInfo", "err");

    fs::FileHandle fileHandle;

    if (Result result = fs::OpenFile(&fileHandle, infoFilePath, fs::OpenMode_Read);
        result.IsFailure()) {
        diag::detail::AbortImpl(
            "", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
            "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
            "nn::fs::OpenFile(&fileHandle, infoFilePath, nn::fs::OpenMode_Read)",
            result.GetModule(), result.GetDescription(), result.GetInnerValueForDebug()
#else
            ""
#endif
        );
    }

    if (Result result =
            fs::ReadFile(fileHandle, 0, outVersion, sizeof(ErrorMessageDatabaseVersion));
        result.IsFailure()) {
        diag::detail::AbortImpl(
            "", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
            "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
            "nn::fs::ReadFile(fileHandle, 0, outVersion, sizeof(ErrorMessageDatabaseVersion))",
            result.GetModule(), result.GetDescription(), result.GetInnerValueForDebug()
#else
            ""
#endif
        );
    }

    fs::CloseFile(fileHandle);
    fs::Unmount("err");
}

void ReadMessageFile(char16* outBuffer, size_t bufferSize, const char* errorCodeString,
                     const settings::LanguageCode& languageCode) {
    if (fs::MountSystemData("err", ncm::SystemDataId::Err).IsFailure()) {
        *outBuffer = u'\0';

        return;
    }

    char filePath[32];
    util::TSNPrintf(filePath, sizeof(filePath), "%s:/Messages/%s_%s", "err", languageCode.code,
                    errorCodeString);

    fs::FileHandle fileHandle;

    if (Result result = fs::OpenFile(&fileHandle, filePath, fs::OpenMode_Read);
        result.IsFailure()) {
        diag::detail::AbortImpl("", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
                                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
                                "nn::fs::OpenFile(&fileHandle, filePath, nn::fs::OpenMode_Read)",
                                result.GetModule(), result.GetDescription(),
                                result.GetInnerValueForDebug()
#else
                                ""
#endif
        );
    }

    u64 bytesRead;

    if (Result result = fs::ReadFile(&bytesRead, fileHandle, 0, outBuffer, bufferSize);
        result.IsFailure()) {
        diag::detail::AbortImpl("", "", "", 0, &result,
#if NN_SDK_VER < NN_MAKE_VER(1, 0, 0)
                                "Failed: %s\n  Module: %d\n  Description: %d\n  InnerValue: 0x%08x",
                                "nn::fs::ReadFile(&readSize, fileHandle, 0, outBuffer, bufferSize)",
                                result.GetModule(), result.GetDescription(),
                                result.GetInnerValueForDebug()
#else
                                ""
#endif
        );
    }

    u64 fileRead = bufferSize - 1;
    if (fileRead >= bytesRead)
        fileRead = bytesRead;

    outBuffer[fileRead] = u'\0';

    fs::CloseFile(fileHandle);
    fs::Unmount("err");
}
}  // namespace nn::err::detail
