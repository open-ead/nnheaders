#pragma once

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util.h>

namespace nn {

namespace gfx {

class ResShaderFile;

}

namespace ui2d {

class ShaderInfo {
    NN_NO_COPY(ShaderInfo);

public:
    ShaderInfo();

    void Initialize(gfx::Device*, void*);
    void InitializeWithVariationTable(gfx::Device*, void*, const void*);
    void InitializeWithVariationTable(gfx::Device*, void*, const void*, gfx::MemoryPool*, ptrdiff_t,
                                      size_t);
    void Finalize(gfx::Device*, bool);
    void SetShader(gfx::CommandBuffer&, int) const;
    int GetVertexShaderSlot(int) const;
    int GetGeometryShaderSlot(int) const;
    int GetPixelShaderSlot(int) const;
    int GetTextureSlot(int, int) const;
    const void* GetVariationTable() const;

private:
    gfx::Shader* GetVertexShader(int) const;
    gfx::Shader* GetGeometryShader(int) const;
    gfx::Shader* GetPixelShader(int) const;
    int GetVariationCount() const;
    void InitializeVertexStates(gfx::Device*);

    gfx::ResShaderFile* m_pResShaderFile;
    gfx::ShaderCodeType m_CodeType;
    gfx::VertexState* m_pVertexStates;
    int* m_pVertexShaderSlots;
    int* m_pGeometryShaderSlots;
    int* m_pPixelShaderSlots;
    int (*m_pTextureSlots)[3];
    const void* m_pVariationTable;
};

}  // namespace ui2d
}  // namespace nn
