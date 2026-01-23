#pragma once

#include <nn/types.h>

namespace nn::ssl {
enum CertificateFormat { PEM = 0x01, DER = 0x02 };

Result Initialize();
Result Initialize(u32 numSessions);
Result Finalize();
Result GetSslResultFromValue(Result*, const char*, u32);
}  // namespace nn::ssl
