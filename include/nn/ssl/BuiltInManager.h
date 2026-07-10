#pragma once

#include <nn/types.h>

namespace nn::ssl {
enum class CaCertificateId;
}  // namespace nn::ssl

namespace nn::ssl::BuiltInManager {
struct BuiltInCertificateInfo;  // TODO

Result GetBuiltInCertificates(BuiltInCertificateInfo**, u8*, u32, CaCertificateId*, u32);
Result GetBuiltInCertificateBufSize(u32*, CaCertificateId*, u32);
}  // namespace nn::ssl::BuiltInManager
