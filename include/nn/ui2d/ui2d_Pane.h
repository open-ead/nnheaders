/**
 * @file Pane.h
 * @brief Base UI panel.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/types.h>
#include <nn/ui2d/ui2d_Types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {

namespace font {
class RectDrawer;
}

namespace ui2d {
class AnimTransform;
class Layout;
class DrawInfo;
class Pane;
struct ResPane;
struct SystemDataMaskTexture;
struct SystemDataAlignmentExInfo;

namespace detail {

class PaneBase {
    NN_NO_COPY(PaneBase);

public:
    PaneBase();
    virtual ~PaneBase();

    util::IntrusiveListNode m_Link;
};

}  // namespace detail

typedef util::IntrusiveList<
    Pane, util::IntrusiveListMemberNodeTraits<detail::PaneBase, &detail::PaneBase::m_Link, Pane>>
    PaneList;

class Pane : public detail::PaneBase {
    struct CalculateContext {
        font::RectDrawer* pRectDrawer;
        const util::MatrixT4x3fType* pViewMtx;
        util::Float2 locationAdjustScale;
        float influenceAlpha;
        bool isLocationAdjust;
        bool isInvisiblePaneCalculateMtx;
        bool isAlphaZeroPaneCalculateMtx;
        bool isInfluenceAlpha;
        const Layout* pLayout;

        void SetDefault();
        void Set(const DrawInfo&, const Layout*);
    };

    typedef CalculateContext CalculateMtxContext;

    Pane* m_pParent;
    PaneList m_ChildList;
    util::Float3 m_Translate;
    util::Float3 m_Rotate;
    util::Float2 m_Scale;
    Size m_Size;
    uint8_t m_Flag;
    uint8_t m_Alpha;
    uint8_t m_GlbAlpha;
    uint8_t m_BasePosition;
    uint8_t m_FlagEx;
    uint32_t m_SystemDataFlags;
    util::MatrixT4x3fType m_GlbMtx;
    const util::MatrixT4x3fType* m_pUserMtx;
    const ResExtUserDataList* m_pExtUserDataList;
    char m_Name[25];
    char m_UserData[9];

public:
    NN_RUNTIME_TYPEINFO_BASE();

    Pane();
    Pane(const ResPane*, const BuildArgSet&);
    Pane(BuildResultInformation*, gfx::Device*, const ResPane*, const BuildArgSet&);
    Pane(const Pane&);
    Pane(const Pane&, gfx::Device*, ResourceAccessor*, const char*);

    virtual ~Pane();
    virtual void Finalize(gfx::Device*);
    const char* GetName() const;
    void SetName(const char*);
    const char* GetUserData() const;
    void SetUserData(const char*);
    bool IsInfluencedAlpha() const;
    void SetInfluencedAlpha(bool);
    bool IsLocationAdjust() const;
    void SetLocationAdjust(bool);
    const util::Float3& GetTranslate() const;
    void SetTranslate(const util::Float3&);
    void SetTranslate(const util::Float2&);
    const util::Float3& GetRotate() const;
    void SetRotate(const util::Float3&);
    const util::Float2& GetScale() const;
    void SetScale(const util::Float2&);
    const Size& GetSize() const;
    void SetSize(const Size&);
    uint8_t GetBasePositionX() const;
    void SetBasePositionX(uint8_t);
    uint8_t GetBasePositionY() const;
    void SetBasePositionY(uint8_t);
    uint8_t GetParentRelativePositionX() const;
    void SetParentRelativePositionX(uint8_t);
    uint8_t GetParentRelativePositionY() const;
    void SetParentRelativePositionY(uint8_t);
    const util::MatrixT4x3fType* GetMtx() const;
    void SetMtx(const util::MatrixT4x3fType*);
    bool IsUserMtx() const;
    void ResetMtx();
    const util::MatrixT4x3fType& GetGlobalMtx() const;
    void SetGlobalMtx(const util::MatrixT4x3fType&);
    bool IsUserGlobalMtx() const;
    void ResetGlobalMtx();
    const font::Rectangle GetPaneRect() const;
    virtual const util::Unorm8x4 GetVertexColor(int) const;
    virtual void SetVertexColor(int, const util::Unorm8x4&);
    uint8_t GetAlpha() const;
    void SetAlpha(uint8_t);
    uint8_t GetGlobalAlpha() const;
    void SetGlobalAlpha(uint8_t);
    float GetSrtElement(int) const;
    void SetSrtElement(int, float);
    virtual uint8_t GetColorElement(int) const;
    virtual void SetColorElement(int, uint8_t);
    virtual uint8_t GetVertexColorElement(int) const;
    virtual void SetVertexColorElement(int, uint8_t);
    float GetMaskTexSrtElement(int) const;
    void SetMaskTexSrtElement(int, float);
    Material* GetMaterial() const;
    virtual Material* GetMaterial(int) const;
    virtual uint8_t GetMaterialCount() const;
    uint16_t GetExtUserDataCount() const;
    const ResExtUserData* GetExtUserDataArray() const;
    const ResExtUserData* FindExtUserDataByName(const char*) const;
    ResExtUserData* GetExtUserDataArrayForAnimation() const;
    ResExtUserData* FindExtUserDataByNameForAnimation(const char*) const;
    Pane* GetParent();
    const Pane* GetParent() const;
    const PaneList& GetChildList() const;
    PaneList& GetChildList();
    void AppendChild(Pane*);
    void PrependChild(Pane*);
    void InsertChild(Pane*, Pane*);
    void InsertChild(
        util::IntrusiveList<Pane, util::IntrusiveListMemberNodeTraits<
                                      detail::PaneBase, &detail::PaneBase::m_Link, Pane>>::iterator,
        Pane*);
    void RemoveChild(Pane*);
    virtual Pane* FindPaneByName(const char*, bool);
    virtual const Pane* FindPaneByName(const char*, bool) const;
    virtual Material* FindMaterialByName(const char*, bool);
    virtual const Material* FindMaterialByName(const char*, bool) const;
    virtual void BindAnimation(AnimTransform*, bool, bool);
    virtual void UnbindAnimation(AnimTransform*, bool);
    virtual void UnbindAnimationSelf(AnimTransform*);

    bool IsVisible() const;
    void SetVisible(bool);
    bool IsViewerInvisible() const;
    virtual void Calculate(DrawInfo&, CalculateMtxContext&, bool);

    bool IsUserAllocated() const { return m_Flag & (1 << PaneFlag_UserAllocated); }

    void SetUserAllocated();

    void SetExtUserDataList(const ResExtUserDataList*);
    void AddDynamicSystemExtUserData(SystemDataType, void*, int);
    virtual void Draw(DrawInfo&, gfx::CommandBuffer&);
    void DrawChildren(DrawInfo&, gfx::CommandBuffer&);
    virtual void DrawSelf(DrawInfo&, gfx::CommandBuffer&);
    bool IsDrawTreeReady() const;
    bool IsDrawSelfReady() const;
    bool IsGlbMtxDirty() const;
    uint8_t* GetFlagPtr();
    uint8_t* GetAlphaPtr();
    void CalculateGlobalMatrix(CalculateMtxContext&);
    void CalculateGlobalMatrix(CalculateMtxContext&, bool);
    bool IsMaskEnabled() const;
    bool CompareCopiedInstanceTest(const Pane&) const;
    bool IsAlignmentIgnore();
    bool IsAlignmentMarginEnabled();
    bool IsAlignmentNullPane();
    float GetAlignmentMargin();

protected:
    void CopyImpl(const Pane&, gfx::Device*, ResourceAccessor*, const char*);
    void InitializeByResourceBlock(BuildResultInformation*, gfx::Device*, const ResPane*,
                                   const BuildArgSet&);
    virtual void LoadMtx(DrawInfo&);
    const util::Float2 GetVertexPos() const;
    void SetGlbMtxDirty();
    void CleanGlbMtx();
    void SetConstantBufferReady(bool);
    bool IsConstantBufferReady() const;
    void SetConstantBufferReadySelf(bool);
    bool IsConstantBufferReadySelf() const;
    void SetCalculationFinishedSelf(bool);
    bool IsCalculationFinishedSelf() const;
    virtual Pane* FindPaneByNameRecursive(const char*);
    virtual const Pane* FindPaneByNameRecursive(const char*) const;
    virtual Material* FindMaterialByNameRecursive(const char*);
    virtual const Material* FindMaterialByNameRecursive(const char*) const;
    uint32_t GetSystemExtDataFlag() const;
    uint32_t GetSystemExtDataCount() const;
    const void* GetSystemExtData(int) const;
    const void* GetSystemExtData(SystemDataType) const;
    void UpdateSystemExtDataFlag(const ResExtUserDataList*);
    bool CheckInvisibleAndUpdateConstantBufferReady();
    void InitializeMaskFunction(BuildResultInformation*, gfx::Device*, const SystemDataMaskTexture*,
                                const BuildArgSet&);
    void CalculateMaskConstantBuffer(DrawInfo&);
    void DrawMaskedImage(DrawInfo&, gfx::CommandBuffer&);
    void CalculateCaptureRootMatrix(util::MatrixT4x3fType&, const DrawInfo&) const;
    void CalculateCaptureProjectionMatrix(util::MatrixT4x4fType&) const;
    void UpdateMaterialConstantBufferForEffectCapture(const DrawInfo&);
    int GetMaskShaderSecondBlend(const SystemDataMaskTexture*);

private:
    void InitializeParams();
    void AddDynamicSystemExtUserDataAllNewImpl(SystemDataType, void*, int);
    void AddDynamicSystemExtUserDataPartialImpl(SystemDataType, void*, int);
    const Pane& operator=(const Pane&);
    void CalculateScaleFromPartsRoot(util::Float2*, Pane*) const;
    void AllocateAndCopyAnimatedExtUserData(const ResExtUserDataList*);
    void CalculateGlobalMatrixSelf(CalculateMtxContext&);
    const SystemDataAlignmentExInfo* GetAlignmentExInfo() const;
};

}  // namespace ui2d
}  // namespace nn