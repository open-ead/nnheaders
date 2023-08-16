#pragma once

#include <nn/font/font_Util.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn {
namespace font {
class Font;
}

namespace ui2d {

class ResourceAccessor {
public:
    typedef void (*FindResourceByTypeCallback)(const void*, size_t, const char*, void*);

    static const char* ArchiveShaderPrefix;
    static const char* ArchiveShaderSuffix;

    static void GetArchiveShaderResourceName(char* pBuffer, const char* pShaderName,
                                             int bufferSize);

protected:
    ResourceAccessor();

public:
    NN_RUNTIME_TYPEINFO_BASE();

    virtual ~ResourceAccessor();
    virtual void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot, void*) = 0;
    virtual void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot, void*) = 0;

    /* newer
    virtual RenderTargetTextureInfo* RegisterRenderTargetTexture(const char*);
    virtual void UnregisterRenderTargetTexture(TextureInfo*);
    */

    virtual void Finalize(gfx::Device*);
    virtual void* FindResourceByName(size_t*, ResType, const char*) = 0;

    virtual const void* FindResourceByName(size_t* pSize, ResType resType,
                                           const char* pName) const {
        return const_cast<ResourceAccessor*>(this)->FindResourceByName(pSize, resType, pName);
    }

    virtual void* FindResourceByName(ResType resType, const char* pName) {
        return FindResourceByName(nullptr, resType, pName);
    }

    virtual const void* FindResourceByName(ResType resType, const char* pName) const {
        return FindResourceByName(nullptr, resType, pName);
    }

    virtual void FindResourceByType(ResType, FindResourceByTypeCallback, void*) const = 0;
    virtual font::Font* AcquireFont(gfx::Device*, const char*) = 0;
    virtual const TextureInfo* AcquireTexture(gfx::Device*, const char*) = 0;
    virtual const ShaderInfo* AcquireShader(gfx::Device*, const char*) = 0;

protected:
    virtual bool LoadTexture(ResourceTextureInfo*, gfx::Device*, const char*) = 0;
    virtual font::Font* LoadFont(gfx::Device*, const char*);
    virtual bool LoadShader(ShaderInfo*, gfx::Device*, const char*) = 0;
};

}  // namespace ui2d
}  // namespace nn
