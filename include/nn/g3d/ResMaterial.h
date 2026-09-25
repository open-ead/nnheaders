/**
 * @file ResMaterial.h
 * @brief Resource material for models.
 */

#pragma once

#include <cstring>

#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>

namespace nn {
namespace gfx {
class SamplerInfo;
}
namespace g3d {

struct TextureRef {
    void* pTexture;
    void* pUserData;
};

class ResShaderParam {
public:
    enum Type {
        kTypeBool1 = 0x00,
        kTypeBool2,
        kTypeBool3,
        kTypeBool4,
        kTypeS321,
        kTypeS322,
        kTypeS323,
        kTypeS324,
        kTypeU321,
        kTypeU322,
        kTypeU323,
        kTypeU324,
        kTypeF321,
        kTypeF322,
        kTypeF323,
        kTypeF324,
        kTypeF32_2x1,
        kTypeF32_2x2,
        kTypeF32_2x3,
        kTypeF32_2x4,
        kTypeF32_3x1,
        kTypeF32_3x2,
        kTypeF32_3x3,
        kTypeF32_3x4,
        kTypeF32_4x1,
        kTypeF32_4x2,
        kTypeF32_4x3,
        kTypeF32_4x4,
        kTypeSrt2d = 0x1c,
        kTypeSrt3d,
        kTypeTexSrt,
        kTypeTexSrtEx,
    };

    typedef u64 (*ConvertFunc)(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                               const void* pUserData);

    template <bool T>
    void Convert(void* pDst, const void* pSrc) const {
        u32 type = m_Type;
        if (type <= kTypeF324) {
            memcpy(pDst, pSrc, ((type & 3) + 1) * sizeof(f32));
            return;
        }
        if (type <= kTypeF32_4x4) {
            size_t srcRowSize = ((type & 3) + 1) * sizeof(f32);
            s32 rowCount = ((static_cast<s32>(type) - kTypeF32_2x1) >> 2) + 2;
            for (s32 row = 0; row < rowCount; row++) {
                memcpy(pDst, pSrc, srcRowSize);
                pDst = static_cast<u8*>(pDst) + 0x10;
                pSrc = static_cast<const u8*>(pSrc) + srcRowSize;
            }
        }
    }

    static u64 GetSize(Type type);
    static u64 GetSrcSize(Type type);
    bool SetDependPointer(void* pDependPointer, const void* pSrc) const;
    bool GetDependPointer(void** ppDependPointer, const void* pSrc) const;

    static u64 ConvertSrt2dCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                    const void* pUserData);
    static u64 ConvertSrt3dCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                    const void* pUserData);
    static u64 ConvertSrt2dExCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                      const void* pUserData);
    static u64 ConvertTexSrtCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                     const void* pUserData);
    static u64 ConvertTexSrtExCallback(void* pDst, const void* pSrc, const ResShaderParam* pParam,
                                       const void* pUserData);

private:
    u8 _0[0x10];
    u8 m_Type;  // 0x10
    u8 _11;     // 0x11
    u16 _12;
    u32 _14;
    u32 _18;  // 0x18
    u32 _1c;
};

class ResMaterial {
public:
    struct ShaderParamEntry {
        ResShaderParam::ConvertFunc pConvertFunc;  // 0x00
        u64 _8;
        u8 type;  // 0x10
        u8 _11[3];
        s32 _14;  // 0x14
        u32 _18;
        u8 _1c;  // 0x1c
        u8 _1d[3];
    };
    static_assert(sizeof(ShaderParamEntry) == 0x20);

    u64 BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    bool ForceBindTexture(nn::g3d::TextureRef const&, char const*);
    void ReleaseTexture();
    void Setup(gfx::Device*);
    void Cleanup(gfx::Device*);
    void Reset();
    void Reset(u32);

    u8 _0[0x30];
    void** m_pTextureRefs;               // 0x30
    const char* const* m_pTextureNames;  // 0x38
    gfx::Sampler* m_pSamplers;           // 0x40
    gfx::SamplerInfo* m_pSamplerInfos;   // 0x48
    u64 _50;
    ShaderParamEntry* m_pShaderParams;  // 0x58
    u8 _60[0x20];
    void* m_pShaderParamFlags;  // 0x80
    void* _88;                  // 0x88
    u8 _90[8];
    s64* m_pTextureStatus;  // 0x98
    u8 _a0[8];
    u8 m_TextureCount;  // 0xa8
    u8 _a9;
    u16 m_ShaderParamCount;  // 0xaa
    u16 _ac;
    u16 _ae;
    u16 _b0;
    u16 _b2;
};
}  // namespace g3d
}  // namespace nn
