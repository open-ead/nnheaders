#pragma once

#include <nn/svc.h>

namespace nn::sf::hipc {
void* GetMessageBufferOnTls();

nn::Result InitializeHipcServiceResolution();
nn::Result ConnectToHipcService(nn::svc::Handle*, char const*);
nn::Result FinalizeHipcServiceResolution();

nn::Result SendSyncRequest(nn::svc::Handle, void*, u64);
nn::Result CloseClientSessionHandle(nn::svc::Handle);

namespace detail {}
};  // namespace nn::sf::hipc
