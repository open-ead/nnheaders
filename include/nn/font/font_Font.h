#pragma once

#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_BufferInfo.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_MemoryPoolInfo.h>
#include <nn/gfx/gfx_Types.h>

namespace nn {
namespace font {

namespace detail {
inline size_t GetIndexBufferAlignment(gfx::Device* pDevice) {
    gfx::BufferInfo info{};
    info.SetDefault();

    info.SetGpuAccessFlags(gfx::GpuAccess_IndexBuffer);

    return gfx::Buffer::GetBufferAlignment(pDevice, info);
}

/*
size_t GetVertexBufferAlignment(gfx::Device* pDevice) {
    gfx::BufferInfo info;
}

size_t GetMemoryPoolAlignment(gfx::Device* pDevice) {
    gfx::MemoryPoolInfo info;
}
*/

inline size_t GetPoolMemorySizeGranularity(gfx::Device* pDevice) {
    gfx::MemoryPoolInfo info{};
    info.SetDefault();

    info.SetMemoryPoolProperty(gfx::MemoryPoolProperty_GpuCached |
                               gfx::MemoryPoolProperty_CpuUncached);

    return gfx::MemoryPool::GetPoolMemorySizeGranularity(pDevice, info);
}

}  // namespace detail

typedef uint16_t TexFmt;

union TextureSize {
    struct {
        uint16_t height;
        uint16_t width;
    };
    uint32_t size;
};

// reorder
class TextureObject {
    static const uint64_t InvalidDescriptorSlotValue;
    uintptr_t m_pImage;
    TextureSize m_Size;
    uint16_t m_Format;
    uint8_t m_SheetCount;
    bool m_ColorBlackWhiteInterpolationEnabled;
    bool m_isInitialized;
    gfx::DescriptorSlot m_DescriptorSlotForTexture;

public:
    TextureObject();
    virtual ~TextureObject();
    void Set(const void*, TexFmt, uint16_t, uint16_t, uint8_t, bool);
    void Reset();
    bool IsInitialized() const;
    uintptr_t GetImage() const;
    uint16_t GetFormat() const;
    uint16_t GetFormatAttribute() const;
    uint8_t GetSheetCount() const;
    const TextureSize GetSize() const;
    bool IsColorBlackWhiteInterpolationEnabled() const;
    void SetColorBlackWhiteInterpolationEnabled(bool);
    virtual const gfx::TextureView* GetTextureView() const;
    virtual gfx::TextureView* GetTextureView();
    const gfx::DescriptorSlot& GetDescriptorSlotForTexture() const;
    gfx::DescriptorSlot& GetDescriptorSlotForTexture();
    bool IsDescriptorSlotForTextureReady() const;

protected:
    uintptr_t GetImagePtr() const;
};

class PlacementTextureObject : public TextureObject {
public:
    PlacementTextureObject();
    virtual ~PlacementTextureObject();
    virtual const gfx::TextureView* GetTextureView() const;
    virtual gfx::TextureView* GetTextureView();
};

}  // namespace font
}  // namespace nn
