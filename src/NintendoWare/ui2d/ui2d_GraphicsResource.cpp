#include <nn/ui2d/ui2d_GraphicsResource.h>

#include <nn/font/font_Font.h>
#include <nn/util/util_BitUtil.h>

namespace nn {
namespace ui2d {

namespace {
size_t GetSizeOfGraphicsResourceBuffers_(gfx::Device* pDevice) {
    const size_t IndexBufferAlignment = font::detail::GetIndexBufferAlignment(pDevice);

    const size_t PaddingSizeForIndexBuffer = util::align_up(32, IndexBufferAlignment) - 32;
    const size_t TotalMemoryPoolSize = util::align_up(
        44 + PaddingSizeForIndexBuffer, font::detail::GetPoolMemorySizeGranularity(pDevice));
    return TotalMemoryPoolSize;
}

}  // namespace

size_t GraphicsResource::CalculateMemoryPoolSize(gfx::Device* pDevice, uint32_t charMax) {
    size_t sizeOfRectDrawerPool = font::RectDrawer::CalculateMemoryPoolSize(pDevice, charMax);

    return GetSizeOfGraphicsResourceBuffers_(pDevice) + sizeOfRectDrawerPool;
}

size_t GraphicsResource::CalculateMemoryPoolAlignment(gfx::Device* pDevice) {
    return font::RectDrawer::CalculateMemoryPoolAlignment(pDevice);
}

/*
void GraphicsResource::Setup(gfx::Device*, int, gfx::MemoryPool*, ptrdiff_t, size_t,
                             font::RectDrawer*, float);
gfx::BlendState* GraphicsResource::GetPresetBlendState(PresetBlendStateId);
PresetBlendStateId GraphicsResource::GetPresetBlendStateId(const ResBlendMode*,
                                                           const ResBlendMode*);
size_t GraphicsResource::SetupBlendStateInfo(gfx::BlendStateInfo*, gfx::BlendTargetStateInfo*,
                                             const ResBlendMode*, const ResBlendMode*);
                                             */

void GraphicsResource::InitializeVertexBuffer(gfx::Device*, gfx::MemoryPool*, ptrdiff_t, size_t) {}
/*
void GraphicsResource::ActivateVertexBuffer(gfx::CommandBuffer*) const;
void GraphicsResource::GetPresetBlendModeArray(const ResBlendMode**, const ResBlendMode**);
GraphicsResource::GraphicsResource();
GraphicsResource::~GraphicsResource();
void GraphicsResource::Finalize(gfx::Device*);
*/

void GraphicsResource::RegisterCommonSamplerSlot(RegisterSamplerSlot pRegisterSamplerSlot,
                                                 void* pUserData) {
    m_pFontDrawer.RegisterSamplerToDescriptorPool(pRegisterSamplerSlot, pUserData);
}

/*
void GraphicsResource::UnregisterCommonSamplerSlot(UnregisterSamplerSlot, void*);
gfx::DescriptorSlot& GraphicsResource::GetSamplerDescriptorSlot(TexWrap, TexWrap, TexFilter,
                                                                TexFilter) const;
*/
}  // namespace ui2d
}  // namespace nn
