#include <nn/err.h>

#include <nn/err/detail.h>

namespace nn::err {
ErrorCode MakeErrorCode(u32 category, u32 number) {
    return {category, number};
}

// TODO: needs `la` stuff before I can implement these
// void ShowError(Result result) {}
// void ShowError(ErrorCode errorCode) {}
// void ShowError(const ErrorResultVariant& errorResultVariant) {}
// void ShowUnacceptableApplicationVersionError() {}
// void ShowUnacceptableAddOnContentVersionError() {}
// void ShowApplicationError(const ApplicationErrorArg& arg) {}
// void ShowError(Result result, ResultBacktrace& backtrace) {}
// void ShowSystemError(const SystemErrorArg& arg){}
// void ShowErrorWithoutJump(Result result){}
// void ShowErrorWithoutJump(ErrorCode errorCode){}
// void ShowEula(settings::system::RegionCode regionCode) {}
// void ShowSystemUpdateEula(settings::system::RegionCode regionCode, EulaData& data) {}
// void ShowErrorRecord(Result result, time::PosixTime timestamp) {}
// void ShowErrorRecord(ErrorCode errorCode, time::PosixTime timestamp) {}

// bool CreateErrorViewerStartupParamForRecordedError(void*, u64*, u64, const char*, const char*,
//                                                    time::PosixTime) {}

// void ShowErrorRecord(const void*, u64) {}

ErrorCode ConvertResultToErrorCode(const Result& result) {
    return {result.GetModule() + 2000, result.GetDescription()};
}

void GetErrorCodeString(char* outErrorCodeString, size_t errorCodeStrBufferSize,
                        ErrorCode errorCode) {
    detail::MakeErrorCodeString(outErrorCodeString, errorCodeStrBufferSize, errorCode);
}

void* GetErrorMessageDatabaseVersion(ErrorMessageDatabaseVersion* outVersion) {
    detail::ReadVersion(outVersion);

    return nullptr;
}

// Seems to be stubbed in Release so I can't implement it
void ExecuteJump(ErrorViewerJumpDestination destination) {}
}  // namespace nn::err
