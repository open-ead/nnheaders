#pragma once

#include <nn/util.h>

#include <nn/font/font_RectDrawer.h>
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/ui2d/ui2d_ShaderInfo.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn {

namespace gfx {
class BlendTargetStateInfo;
}  // namespace gfx

namespace ui2d {

class ResBlendMode;

enum ShaderId {
    ShaderId_Undefined = -1,
    ShaderId_NullTexture,
    ShaderId_SingleTexture,
    ShaderId_DoubleTexture1,
    ShaderId_DoubleTexture2,
    ShaderId_DoubleTexture3,
    ShaderId_DoubleTexture4,
    ShaderId_DoubleTexture5,
    ShaderId_DoubleTexture6,
    ShaderId_DoubleTexture7,
    ShaderId_DoubleTexture8,
    ShaderId_DoubleTexture9,
    ShaderId_DoubleTexture10,
    ShaderId_DoubleTexture11,
    ShaderId_DoubleIndirectTexture,
    ShaderId_MaxShaderId,
    ShaderId_ArchiveShader,
    ShaderId_DoubleTextureBaseIdx = 2
};

enum PresetBlendStateId {
    PresetBlendStateId_Default,
    PresetBlendStateId_OpaqueOrAlphaTest,
    PresetBlendStateId_Addition,
    PresetBlendStateId_Subtraction,
    PresetBlendStateId_Multiplication,
    PresetBlendStateId_SemitransparencyMaxAlpha,
    PresetBlendStateId_MaxPresetBlendStateId,
    PresetBlendStateId_None
};

enum ArchiveShaderId {
    ArchiveShaderId_DetailedCombinerNonSource = 100,
    ArchiveShaderId_DetailedCombinerSingleSource,
    ArchiveShaderId_DetailedCombinerDoubleSource,
    ArchiveShaderId_DetailedCombinerTripleSource,
    ArchiveShaderId_DetailedCombinerBaseIdx = 100
};

class GraphicsResource {
    NN_NO_COPY(GraphicsResource);

public:
    static const int VertexBufferIndexCount = 6;
    static const int VertexAttributeCount = 1;

    GraphicsResource();

    static size_t CalculateMemoryPoolSize(gfx::Device*, uint32_t);
    static size_t CalculateMemoryPoolAlignment(gfx::Device*);
    ~GraphicsResource();
    void Setup(gfx::Device*, int);
    void Setup(gfx::Device*, int, gfx::MemoryPool*, ptrdiff_t, size_t);
    void Setup(gfx::Device*, int, gfx::MemoryPool*, ptrdiff_t, size_t, font::RectDrawer*);
    void Setup(gfx::Device*, int, gfx::MemoryPool*, ptrdiff_t, size_t, font::RectDrawer*, float);
    void Finalize(gfx::Device*);
    bool Initialized() const;
    void RegisterCommonSamplerSlot(RegisterSamplerSlot, void*);
    void UnregisterCommonSamplerSlot(UnregisterSamplerSlot, void*);
    gfx::DescriptorSlot& GetSamplerDescriptorSlot(TexWrap, TexWrap, TexFilter, TexFilter) const;
    font::RectDrawer& GetFontDrawer() { return m_pFontDrawer; }
    const ShaderInfo* GetCommonShaderInfo() const;
    const gfx::Buffer* GetIndexBuffer() const;
    const gfx::GpuAddress* GetIndexBufferGpuAddress() const;
    size_t GetConstantBufferAlignment() const;
    size_t GetVertexBufferAlignment() const;
    size_t GetIndexBufferAlignment() const;
    gfx::BlendState* GetPresetBlendState(PresetBlendStateId);
    static PresetBlendStateId GetPresetBlendStateId(const ResBlendMode*, const ResBlendMode*);
    static size_t SetupBlendStateInfo(gfx::BlendState::InfoType*, gfx::BlendTargetStateInfo*,
                                      const ResBlendMode*, const ResBlendMode*);

private:
    void InitializeVertexBuffer(gfx::Device*, gfx::MemoryPool*, ptrdiff_t, size_t);
    void ActivateVertexBuffer(gfx::CommandBuffer*) const;
    static void GetPresetBlendModeArray(const ResBlendMode**, const ResBlendMode**);

    ShaderInfo m_CommonShaderInfo;
    void* m_pUi2dBuildinShader;
    void** m_pConstantBufferMemories;
    void* m_pBufferMemory;
    // removed in smo
    void* _unk;

    void* m_pRectShaderBinary;
    int32_t m_RectShaderBinarySize;
    size_t m_ConstantBufferAlignment;
    /* added in smo
    size_t m_VertexBufferAlignment;
    size_t m_IndexBufferAlignment;
    */
    font::RectDrawer m_pFontDrawer;
    gfx::MemoryPool m_MemoryPoolForBuffers;
    gfx::Buffer m_VertexBuffer;
    gfx::GpuAddress m_VertexBufferGpuAddress;
    gfx::Buffer m_IndexBuffer;
    gfx::GpuAddress m_IndexBufferGpuAddress;
    gfx::Sampler* m_pSamplerTable;
    gfx::DescriptorSlot* m_pSamplerDescriptorSlotTable;
    gfx::BlendState m_PresetBlendState[6];
    bool m_Initialized;
    bool m_IsDefaultRectDrawerUsed;
};
}  // namespace ui2d
}  // namespace nn
