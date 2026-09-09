#pragma once

#include <nn/ssl/Connection.h>
#include <nn/ssl/Context.h>
#include <nn/types.h>

extern "C" {
nn::Result nnsslInitialize(void);
nn::Result nnsslInitializeWithConcurrencyLimit(u32 limit);
nn::Result nnsslFinialize(void);
nn::Result nnsslContextCreate(nn::ssl::Context* context, nn::ssl::Context::SslVersion version);
nn::Result nnsslContextDestroy(nn::ssl::Context* context);
nn::Result nnsslContextGetContextId(nn::ssl::Context* context, u64* outId);
nn::Result nnsslConnectionCreate(nn::ssl::Connection* connection, nn::ssl::Context* context);
nn::Result nnsslConnectionDestroy(nn::ssl::Connection* connection);
nn::Result nnsslConnectionSetSocketDescriptor(nn::ssl::Connection* connection,
                                              s32 socketDescriptor);
nn::Result nnsslConnectionSetHostName(nn::ssl::Connection* connection, const char* hostName,
                                      u32 hostNameSize);
nn::Result nnsslConnectionSetVerifyOption(nn::ssl::Connection* connection,
                                          nn::ssl::Connection::VerifyOption verifyOption);
nn::Result nnsslConnectionSetServerCertBuffer(nn::ssl::Connection* connection,
                                              const char* serverCertificateBuffer,
                                              u32 serverCertificateBufferSize);
nn::Result nnsslConnectionSetIoMode(nn::ssl::Connection* connection,
                                    nn::ssl::Connection::IoMode ioMode);
nn::Result
nnsslConnectionSetSessionCacheMode(nn::ssl::Connection* connection,
                                   nn::ssl::Connection::SessionCacheMode sessionCacheMode);
nn::Result nnsslConnectionGetSocketDescriptor(nn::ssl::Connection* connection,
                                              s32* outSocketDescriptor);
nn::Result nnsslConnectionGetHostName(nn::ssl::Connection* connection, const char* outHostName,
                                      u32* outHostNameSize, u32 maxHostNameSize);
nn::Result nnsslConnectionGetVerifyOption(nn::ssl::Connection* connection,
                                          nn::ssl::Connection::VerifyOption* outVerifyOption);
nn::Result nnsslConnectionGetIoMode(nn::ssl::Connection* connection,
                                    nn::ssl::Connection::IoMode* outIoMode);
nn::Result nnsslConnectionDoHandshake(nn::ssl::Connection* connection);
nn::Result nnsslConnectionDoHandshakeWithCertBuffer(nn::ssl::Connection* connection,
                                                    u32* outServerCertificateBufferSize,
                                                    u32* outNumCertificates);
nn::Result nnsslConnectionDoHandshakeWithBuffer(nn::ssl::Connection* connection,
                                                u32* outServerCertificateBufferSize,
                                                u32* outNumCertificates,
                                                char* outServerCertificateBuffer,
                                                u32 serverCertificateBufferMaxSize);
nn::Result nnsslConnectionGetServerCertDetail(nn::ssl::Connection* connection,
                                              nn::ssl::Connection::ServerCertDetail*, const char*,
                                              u32);
nn::Result nnsslConnectionRead(nn::ssl::Connection* connection, char* outBuffer, s32* outBufferSize,
                               u32 maxBufferSize);
nn::Result nnsslConnectionWrite(nn::ssl::Connection* connection, const char* buffer,
                                s32* outWrittenBufferSize, u32 maxBufferSize);
nn::Result nnsslConnectionPending(nn::ssl::Connection* connection, s32*);
nn::Result nnsslConnectionPeek(nn::ssl::Connection* connection, char* outBuffer, s32* outBufferSize,
                               u32 maxBufferSize);
nn::Result nnsslConnectionPoll(nn::ssl::Connection* connection, PollEvent*, PollEvent*,
                               u32 timeout);
nn::Result nnsslConnectionGetVerifyCertError(nn::ssl::Connection* connection,
                                             nn::Result* outErrorResult);
nn::Result nnsslConnectionGetContextId(nn::ssl::Connection* connection, u64* outContextId);
nn::Result nnsslConnectionGetConnectionId(nn::ssl::Connection* connection, u64* outConnectionId);
nn::Result nnsslConnectionSetOption(nn::ssl::Connection* connection,
                                    nn::ssl::Connection::OptionType option, bool value);
nn::Result nnsslConnectionGetOption(nn::ssl::Connection* connection, bool* outValue,
                                    nn::ssl::Connection::OptionType option);
}
