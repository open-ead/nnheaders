#pragma once

#include <nn/gfx/detail/gfx_Shader-api.nvn.8.h>

namespace nn::gfx {

template <class TTarget>
class TShader : public detail::ShaderImpl<TTarget> {
    NN_NO_COPY(TShader);

public:
    typedef ShaderInfo InfoType;

    static size_t GetBinaryCodeAlignment(TDevice<TTarget>*);

    TShader();

    ShaderInitializeResult Initialize(TDevice<TTarget>* pDevice, const InfoType& info) {
        return detail::ShaderImpl<TTarget>::Initialize(pDevice, info);
    }

    void Finalize(TDevice<TTarget>*);

    int GetInterfaceSlot(ShaderStage stage, ShaderInterfaceType shaderInterfaceType,
                         const char* pName) const {
        return detail::ShaderImpl<TTarget>::GetInterfaceSlot(stage, shaderInterfaceType, pName);
    }

    void GetWorkGroupSize(int*, int*, int*) const;
    void SetUserPtr(void*);
    void* GetUserPtr();
    const void* GetUserPtr() const;
};

}  // namespace nn::gfx
