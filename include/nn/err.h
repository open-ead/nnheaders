#pragma once

#include <nn/err/ApplicationErrorArg.h>
#include <nn/err/ErrorCode.h>
#include <nn/err/ErrorResultVariant.h>
#include <nn/err/ResultBacktrace.h>
#include <nn/err/SystemErrorArg.h>
#include <nn/settings.h>
#include <nn/time.h>
#include <nn/types.h>

namespace nn::err {
// TODO
class ErrorMessageDatabaseVersion;
class EulaData;

ErrorCode ConvertResultToErrorCode(const Result& result);
bool CreateErrorViewerStartupParamForRecordedError(void*, u64*, u64, const char*, const char*,
                                                   time::PosixTime);
void GetErrorCodeString(char* outErrorCodeStr, size_t errorCodeStrBufferSize, ErrorCode errorCode);
void* GetErrorMessageDatabaseVersion(ErrorMessageDatabaseVersion* outErrorMessageDatabaseVersion);
ErrorCode MakeErrorCode(u32 category, u32 number);
void ShowApplicationError(const ApplicationErrorArg& arg);
void ShowError(Result result);
void ShowError(ErrorCode errorCode);
void ShowError(const ErrorResultVariant& errorResultVariant);
void ShowError(Result result, ResultBacktrace& backtrace);
void ShowErrorRecord(Result result, time::PosixTime timestamp);
void ShowErrorRecord(ErrorCode errorCode, time::PosixTime timestamp);
void ShowErrorRecord(const void*, u64);
void ShowErrorWithoutJump(Result result);
void ShowErrorWithoutJump(ErrorCode errorCode);
void ShowEula(settings::system::RegionCode regionCode);
void ShowSystemError(const SystemErrorArg& arg);
void ShowSystemUpdateEula(settings::system::RegionCode regionCode, EulaData& data);
void ShowUnacceptableAddOnContentVersionError();
void ShowUnacceptableApplicationVersionError();
}  // namespace nn::err
