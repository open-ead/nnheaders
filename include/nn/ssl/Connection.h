#pragma once

#include <nn/ssl/sf/ISslConnection.h>
#include <nn/types.h>

namespace nn::ssl {
struct Context;

struct Connection : sf::ISslConnection {
public:
    Connection();
    ~Connection();

    Result Create(Context*);
    Result Destroy();
    Result SetSocketDescriptor(s32);
    Result SetHostName(const char*, u32);
    Result SetVerifyOption(VerifyOption);
    Result SetServerCertBuffer(const char*, u32);
    Result SetIoMode(IoMode);
    Result SetSessionCacheMode(SessionCacheMode);
    Result SetRenegotiationMode(RenegotiationMode);
    Result GetSocketDescriptor(s32*);
    Result GetHostName(const char*, u32*, u32);
    Result GetVerifyOption(VerifyOption*);
    Result GetIoMode(IoMode*);
    Result GetSessionCacheMode(SessionCacheMode*);
    Result GetRenegotiationMode(RenegotiationMode*);
    Result FlushSessionCache();
    Result DoHandshake();
    Result DoHandshake(u32*, u32*);
    Result DoHandshake(u32*, u32*, char*, u32);
    Result GetServerCertDetail(ServerCertDetail*, const char*, u32);
    Result Read(char*, u32);
    Result Read(char*, s32*, u32);
    Result Write(const char*, u32);
    Result Write(const char*, s32*, u32);
    Result Pending();
    Result Pending(s32*);
    Result Peek(char*, s32*, u32);

private:
    u64 connectionId;
    u64 contextId;
    unsigned char _10[8];
    const char* serverCertificateBuffer;
    u32 serverCertificateBufferSize;
    Result _24;
};
}  // namespace nn::ssl
