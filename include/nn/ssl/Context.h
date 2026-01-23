#pragma once

#include <nn/ssl.h>
#include <nn/types.h>

namespace nn::ssl {
// TODO: find the size of this struct
struct Context {
public:
    enum SslVersion {
        Auto = 0x01,
        v10 = 0x08,
        v11 = 0x10,
        v12 = 0x20,
        v13 = 0x40,         // [11.0.0+]
        Auto24 = 0x1000000  // [11.0.0+]
    };

    enum InternalPki { DeviceClientCertDefault = 1 };

    enum ContextOption { CrlImportDateCheckEnable = 1 };

    Context();
    ~Context();

    Result Create(SslVersion version);
    Result Destroy();
    Result SetOption(ContextOption option, s32 value);
    Result GetOption(s32* outValue, ContextOption option);
    Result GetContextId(s64* outId);
    Result ImportServerPki(u64* outId, const char* certificateBuffer, u32 certificateBufferSize,
                           CertificateFormat certificateFormat);
    Result ImportClientPki(u64* outId, const char* certificateBuffer,
                           const char* certificatePassword, u32 certificateBufferSize,
                           u32 certificatePasswordSize);
    Result RemovePki(u64 id);
    Result RegisterInternalPki(u64* outId, InternalPki pki);
    Result AddPolicyOid(const char* policyOidBuffer, u32 policyOidBufferSize);
    Result ImportCrl(u64* outId, const char* certificateRevocationListBuffer,
                     u32 certificateRevocationListBufferSize);

private:
    unsigned char padding[8];
};

// TODO: I'm pretty sure there is inheritance between ContextPrivate and Context
struct ContextPrivate {
public:
    enum SslVersion {
        // TODO
    };

    Result Create(SslVersion version);

private:
    Context context;
};
}  // namespace nn::ssl
