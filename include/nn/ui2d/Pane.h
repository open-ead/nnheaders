/**
 * @file Pane.h
 * @brief Base UI panel.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/types.h>
#include "nn/util/MathTypes.h"

namespace nn::gfx {
template <typename TType, typename TVersion>
class ApiVariation;

template <typename TTarget>
class TCommandBuffer;

template <typename T>
class TDevice;

template <int T>
class ApiType;

template <int T>
class ApiVersion;
}  // namespace nn::gfx

namespace nn::util {
struct Unorm8x4;
}

namespace nn::ui2d {
class AnimTransform;
class Layout;
class DrawInfo;

class Pane {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    struct CalculateContext;

    Pane();
    Pane(const Pane&);

    virtual ~Pane();
    virtual void Finalize(gfx::TDevice<gfx::ApiVariation<gfx::ApiType<4>, gfx::ApiVersion<8>>>*);
    virtual s32 GetVertexColor(s32);
    virtual void SetVertexColor(s32, util::Unorm8x4 const&);
    virtual u8 GetColorElement(s32);
    virtual void SetColorElement(u32, u8);
    virtual u8 GetVertexColorElement(s32);
    virtual void SetVertexColorElement(u32, u8);
    virtual u32 GetMaterialCount() const;
    virtual u64* GetMaterial(s32) const;
    virtual Pane* FindPaneByName(char const*, bool);
    virtual Pane* FindPaneByName(char const*, bool) const;
    virtual void FindMaterialByName(char const*, bool);
    virtual void FindMaterialByName(char const*, bool) const;
    virtual void BindAnimation(AnimTransform*, bool, bool);
    virtual void UnbindAnimation(AnimTransform*, bool);
    virtual void UnbindAnimationSelf(AnimTransform*);
    virtual void Calculate(DrawInfo&, CalculateContext&, bool);
    virtual void Draw(DrawInfo&,
                      gfx::TCommandBuffer<gfx::ApiVariation<gfx::ApiType<4>, gfx::ApiVersion<8>>>&);
    virtual void
    DrawSelf(DrawInfo&,
             gfx::TCommandBuffer<gfx::ApiVariation<gfx::ApiType<4>, gfx::ApiVersion<8>>>&);
    virtual void LoadMtx(DrawInfo&);
    virtual Pane* FindPaneByNameRecursive(char const*);
    virtual Pane* FindPaneByNameRecursive(char const*) const;
    virtual void FindMaterialByNameRecursive(char const*);
    virtual void FindMaterialByNameRecursive(char const*) const;

    void Initialize();
    void SetName(const char*);
    void SetUserData(const char*);
    void AppendChild(Pane*);
    void PrependChild(Pane*);
    void InsertChild(Pane*, Pane*);
    void RemoveChild(Pane*);
    void GetVertexPos() const;

    void Show() { mFlags |= 0x01; }
    void Hide() { mFlags &= ~0x01; }
    bool IsShow() { return (mFlags & 0x01) != 0; };

    const util::Float3& GetPosition() const { return mPosition; }
    void SetPosition(const util::Float3& position) {
        mPosition = position;
        mFlags |= 0x10;
    }

    const util::Float3& GetRotation() const { return mRotation; }
    void SetRotation(const util::Float3& rotation) {
        mRotation = rotation;
        mFlags |= 0x10;
    }

    const util::Float2& GetScale() const { return mScale; }
    void SetScale(const util::Float2& scale) {
        mScale = scale;
        mFlags |= 0x10;
    }

    const util::Float2& GetSize() const { return mSize; }
    void SetSize(const util::Float2& size) {
        mSize = size;
        mFlags |= 0x10;
    }

    void SetAlpha(u8 alpha) { mAlpha = alpha; }

    u8 GetGlobalAlpha() const { return mGlobalAlpha; }

    const util::neon::MatrixColumnMajor4x3fType& GetMtx() const { return mMtx; }

private:
    Pane* mParent;
    u64 _10;
    u64 _18;
    u64 _20;
    u64 _28;
    util::Float3 mPosition;
    util::Float3 mRotation;
    util::Float2 mScale;
    util::Float2 mSize;
    u8 mFlags;
    u8 mAlpha;
    u8 mGlobalAlpha;
    u8 mOriginFlags;
    u32 _5C;
    u64 _60;
    Layout* mLayout;
    util::neon::MatrixColumnMajor4x3fType mMtx;
    u64 _A0;
    u64 _A8;
    void* mAnimExtUserData;
    char mPanelName[0x18];
    u8 _D0;
    char mUserData[8];
    u8 _D9;
    u16 _DA;
    u32 _DC;
};
}  // namespace nn::ui2d
