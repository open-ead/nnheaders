#pragma once

#include <nn/ssl/sf/ISslConnection.h>
#include <nn/types.h>

namespace nn::ssl {
struct Context;

struct Connection : sf::ISslConnection {
public:
    enum class VerifyOption {
        PeerCa = 1 << 0,
        HostName = 1 << 1,
        DateCheck = 1 << 2,
        EvCertPartial = 1 << 3,
        EvPolicyOid = 1 << 4,       // [6.0.0+]
        EvCertFingerprint = 1 << 5  // [6.0.0+]
    };

    enum class IoMode { Blocking = 1, NonBlocking = 2 };

    enum class SessionCacheMode { None, SessionId, SessionTicket };

    enum class RenegotiationMode { None, Secure };

    enum class PollEvent { Read = 1 << 0, Write = 1 << 1, Except = 1 << 2 };

    enum class OptionType {
        DoNotCloseSocket,
        GetServerCertChain,  // [3.0.0+]
        SkipDefaultVerify,   // [5.0.0+]
        EnableAlpn           // [9.0.0+]
    };

    // TODO
    struct ServerCertDetail;

    Connection();
    ~Connection();

    Result Create(Context* context);
    Result Destroy();
    Result SetSocketDescriptor(s32 socketDescriptor);
    Result SetHostName(const char* hostName, u32 hostNameSize);
    Result SetVerifyOption(VerifyOption verifyOption);
    Result SetServerCertBuffer(const char* serverCertificateBuffer,
                               u32 serverCertificateBufferSize);
    Result SetIoMode(IoMode ioMode);
    Result SetSessionCacheMode(SessionCacheMode sessionCacheMode);
    Result SetRenegotiationMode(RenegotiationMode renegotiationMode);
    Result GetSocketDescriptor(s32* outSocketDescriptor);
    Result GetHostName(const char* outHostName, u32* outHostNameSize, u32 maxHostNameSize);
    Result GetVerifyOption(VerifyOption* outVerifyOption);
    Result GetIoMode(IoMode* outIoMode);
    Result GetSessionCacheMode(SessionCacheMode* outSessionCacheMode);
    Result GetRenegotiationMode(RenegotiationMode* outRenegotiationMode);
    Result FlushSessionCache();
    Result DoHandshake();
    Result DoHandshake(u32* outServerCertificateBufferSize, u32* outNumCertificates);
    Result DoHandshake(u32* outServerCertificateBufferSize, u32* outNumCertificates,
                       char* outServerCertificateBuffer, u32 serverCertificateBufferMaxSize);
    Result GetServerCertDetail(ServerCertDetail*, const char*, u32);
    Result Read(char* outBuffer, u32 maxBufferSize);
    Result Read(char* outBuffer, s32* outBufferSize, u32 maxBufferSize);
    Result Write(const char* outBuffer, u32 maxBufferSize);
    Result Write(const char* outBuffer, s32* outBufferSize, u32 maxBufferSize);
    Result Pending();
    Result Pending(s32*);
    Result Peek(char* outBuffer, s32* outBufferSize, u32 maxBufferSize);
    Result Poll(PollEvent*, PollEvent*, u32 timeout);
    Result GetLastError(Result* outErrorResult);
    Result GetVerifyCertError(Result* outErrorResult);
    Result GetVerifyCertErrors(Result* outErrorResults, u32*, u32*, u32 maxErrorResultCount);
    Result GetNeededServerCertBufferSize(u32* outNeededServerCertBufferSize);
    Result GetContextId(u64* outContextId);
    Result GetConnectionId(u64* outConnectionId);
    Result SetOption(OptionType option, bool value);
    Result GetOption(bool* outValue, OptionType option);

private:
    u64 connectionId;
    u64 contextId;
    unsigned char _10[8];
    const char* serverCertificateBuffer;
    u32 serverCertificateBufferSize;
    Result _24;
};
}  // namespace nn::ssl
