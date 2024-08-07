#pragma once

#include <nn/util.h>

namespace nn {
namespace ui2d {

class TexMap {
public:
    TexMap();
    TexMap(const TextureInfo*);
    ~TexMap();

    void Finalize();
    TexWrap GetWrapModeS() const;
    TexWrap GetWrapModeT() const;
    void SetWrapMode(TexWrap, TexWrap);
    TexFilter GetMinFilter() const;
    TexFilter GetMagFilter() const;
    void SetFilter(TexFilter, TexFilter);
    void Set(const TextureInfo*);

    const TextureInfo* GetTextureInfo() const { return m_pTextureInfo; }
    void SetTextureInfo(const TextureInfo* pTextureInfo) { m_pTextureInfo = pTextureInfo; }

    void CopySamplerSettings(const TexMap&);

private:
    void ResetSamplerSettings();

    struct Bits {
        uint32_t wrapS : 2;
        uint32_t wrapT : 2;
        uint32_t minFilter : 3;
        uint32_t magFilter : 1;
    };

    Bits m_Bits;
    const TextureInfo* m_pTextureInfo;

    NN_NO_COPY(TexMap);
};

}  // namespace ui2d
}  // namespace nn
