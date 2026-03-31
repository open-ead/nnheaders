#pragma once

#include <nn/sf/ISharedObject.h>

namespace nn::sf {
class IServiceObject : public ISharedObject {
public:
    virtual void* GetInterfaceTypeInfo() { return nullptr; }
    virtual void* GetProxyInfo() { return nullptr; }  // CmifProxyInfo* ?
};
}  // namespace nn::sf
