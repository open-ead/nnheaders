#pragma once

#include <nn/font/font_DispStringBuffer.h>
#include <nn/font/font_Types.h>
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_ResShader.h>
#include <nn/gfx/gfx_Sampler.h>
#include <nn/gfx/gfx_State.h>
#include <nn/gfx/gfx_Types.h>

namespace nn {
namespace font {

class RectDrawer {
public:
    static const int IndexCountByLetter = 6;
    static const int VertexAttributeCount = 3;

    static size_t GetWorkBufferSize(gfx::Device*, uint32_t);
    static size_t CalculateMemoryPoolSize(gfx::Device*, uint32_t);
    static size_t CalculateMemoryPoolAlignment(gfx::Device*);
    static size_t GetWorkBufferAlignment();

    RectDrawer();
    virtual ~RectDrawer();
    bool Initialize(gfx::Device*, void*, uint32_t);
    virtual bool Initialize(gfx::Device*, void*, uint32_t, gfx::MemoryPool*, ptrdiff_t, size_t);
    bool IsInitialized() const;
    virtual void Finalize(gfx::Device*);
    void RegisterSamplerToDescriptorPool(RegisterSamplerSlot, void*);
    void UnregisterSamplerFromDescriptorPool(UnregisterSamplerSlot, void*);
    virtual void Draw(gfx::CommandBuffer&, const DispStringBuffer&) const;
    void* GetBuffer() const;

protected:
    void LoadTexture(const TextureObject*);
    static void CreateIndices(uint16_t*, uint32_t);

private:
    enum ShaderVariation {
        ShaderVariation_NormalShader,
        ShaderVariation_BorderShader,
        ShaderVariation_InvisibleBorderShader,
        ShaderVariation_NormalShaderWithPerCharacterTransform,
        ShaderVariation_BorderShaderWithPerCharacterTransform,
        ShaderVariation_InvisibleBorderShaderWithPerCharacterTransform,
        ShaderVariation_ShaderQuantity,
        ShaderVariation_Invalid
    };

    struct ShaderParamBlackWhiteInterpolation {
        int32_t flags;
    };

    void AddDrawCommand(gfx::CommandBuffer&, const DispStringBuffer::VertexBufferData&,
                        const uint32_t, const gfx::GpuAddress&, const gfx::GpuAddress&) const;
    gfx::Shader* GetVertexShader(int) const;
    gfx::Shader* GetPixelShader(int) const;

    gfx::ResShaderFile* m_pResShaderFile;
    gfx::ShaderCodeType m_CodeType;
    int m_VertexShaderSlots[6];
    int m_VertexShaderPerCharacterParamsSlots[6];
    int m_PixelShaderSlots[6];
    int m_BlackWhiteInterpolationSlots[6];
    int m_TextureSlots[6];
    int32_t m_CharCountMax;
    gfx::VertexState m_VertexStates[6];
    gfx::MemoryPool m_MemoryPoolForBuffers;
    gfx::Buffer m_VertexBuffer;
    gfx::Buffer m_IndexBuffer;
    gfx::Buffer m_ShaderParamBlackWhiteInterpolationEnabledBuffer;
    gfx::Buffer m_ShaderParamBlackWhiteInterpolationDisabledBuffer;
    gfx::Sampler m_Sampler;
    gfx::DescriptorSlot m_DescriptorSlotForSampler;
    void* m_WorkMemory;
};
}  // namespace font
}  // namespace nn
