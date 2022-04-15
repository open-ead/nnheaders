#pragma once

#include "nn/gfx/ResUserDataData.h"
#include "nn/util/AccessorBase.h"

namespace nn::gfx {

class ResUserData : util::AccessorBase<ResUserDataData> {
public:
    enum Type {
        Type_Int,
        Type_Float,
        Type_String,
        Type_Stream,
    };

    ResUserData(const ResUserData&) = delete;
    auto operator=(const ResUserData&) = delete;

    const char* GetName() const { return pName.Get()->GetData(); }
    int GetCount() const { return static_cast<int>(count); }
    Type GetType() const { return static_cast<Type>(type); }

    int32_t* GetInt() { return static_cast<int32_t*>(pData.Get()); }
    const int32_t* GetInt() const { return static_cast<const int32_t*>(pData.Get()); }

    float* GetFloat() { return static_cast<float*>(pData.Get()); }
    const float* GetFloat() const { return static_cast<const float*>(pData.Get()); }

    const char* GetString(int index) const {
        return static_cast<const util::BinPtrToString*>(pData.Get())[index].Get()->GetData();
    }

    void* GetStream() { return pData.Get(); }
    const void* GetStream() const { return pData.Get(); }
    size_t GetStreamSize() const { return static_cast<size_t>(count); }
};

}  // namespace nn::gfx
