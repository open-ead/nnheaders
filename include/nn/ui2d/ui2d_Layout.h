/**
 * @file Layout.h
 * @brief UI Layout implementation.
 */

#pragma once

#include <nn/font/font_Types.h>
#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/nn_Allocator.h>
#include <nn/types.h>
#include <nn/ui2d/ui2d_Animation.h>
#include <nn/ui2d/ui2d_Common.h>
#include <nn/ui2d/ui2d_Parts.h>
#include <nn/ui2d/ui2d_TextBox.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace ui2d {

class Animator;
class Pane;
class PaneAnimator;
class Group;
class GroupContainer;
class GroupAnimator;
class GroupArrayAnimator;
class DrawInfo;
struct ResPane;
class ShaderInfo;
class CaptureTexture;
struct ResParts;
struct ResPartsProperty;
struct ResVec2;
struct ResPicture;
class ResCaptureTexture;
class ResCaptureTextureOld;

typedef util::IntrusiveList<
    AnimTransform, util::IntrusiveListMemberNodeTraits<AnimTransform, &AnimTransform::m_Link>>
    AnimTransformList;

typedef util::IntrusiveList<Parts, util::IntrusiveListMemberNodeTraits<Parts, &Parts::m_Link>>
    PartsPaneList;

class Layout {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    static const int LayoutNameLengthMax = 64;
    static const size_t DefaultAligment = 4;
    static const size_t AlignmentForPaneInstance = 16;

    static AlignedAllocateFunctionWithUserData GetAllocateFunction();
    static FreeFunctionWithUserData GetFreeFunction();
    static void* GetUserDataForAllocator();
    static void SetAllocator(AlignedAllocateFunctionWithUserData, FreeFunctionWithUserData, void*);
    static void* AllocateMemory(size_t size, size_t alignment = DefaultAligment);

    // smo
    // static void* AllocateMemory(size_t size);
    static void FreeMemory(void*);

    Layout();
    Layout(gfx::Device*, const Layout&, Layout*, const char*);
    Layout(gfx::Device*, const Layout&, Layout*, const char*, const char*);
    Layout(gfx::Device*, const Layout&, Layout*);
    Layout(gfx::Device*, const Layout&);

    virtual ~Layout();
    void Finalize(gfx::Device*);

    struct BuildOption {
        bool isRootPaneParts;
        GetUserShaderInformationFromUserData pGetUserShaderInformationFromUserDataCallback;
        void* pGetUserShaderInformationFromUserDataCallbackUserData;

        void SetDefault();
    };

    /* newer
    struct LayoutBuildContext {
        BuildResSet buildResSet;
        BuildArgSet buildArgSet;
        int index;
        const void* pData;
        const ResExtUserDataList* pResExtUserDataList;
        uint32_t kind;
        int indexNext;
        const void* pDataNext;
        Pane* pLastBuiltPane;
        int groupNestLevel;
    };

    static void FindResPaneByName(const ResPane**, const ResExtUserDataList**, const void*,
                                      const char*, const ResPane*);
    */
    bool Build(BuildResultInformation*, gfx::Device*, ResourceAccessor*, ControlCreator*,
               TextSearcher*, const void*, const BuildOption&, bool);
    bool Build(BuildResultInformation*, gfx::Device*, ResourceAccessor*, ControlCreator*,
               TextSearcher*, const void*, const BuildOption&);
    bool Build(BuildResultInformation*, gfx::Device*, ResourceAccessor*, ControlCreator*,
               const void*);
    bool Build(BuildResultInformation*, gfx::Device*, ResourceAccessor*, const void*);
    bool BuildWithName(BuildResultInformation*, gfx::Device*, ResourceAccessor*, ControlCreator*,
                       TextSearcher*, const BuildOption&, const char*, bool);
    bool BuildWithName(BuildResultInformation*, gfx::Device*, ResourceAccessor*, ControlCreator*,
                       TextSearcher*, const BuildOption&, const char*);
    bool BuildWithName(BuildResultInformation*, gfx::Device*, ResourceAccessor*, const char*);
    void CalculateGlobalMatrix(DrawInfo&);
    void CalculateGlobalMatrix(DrawInfo&, bool);

    template <typename T>  // 594
    T* CreateAnimTransform() {
        T* const pAnimTrans = AllocateAndConstruct<T>();
        if (pAnimTrans) {
            m_AnimTransList.push_back(*pAnimTrans);
        }
        return pAnimTrans;
    }

    template <typename T>  // 625
    T* CreateAnimTransform(gfx::Device* pDevice, const void* pAnimResource) {
        return CreateAnimTransform<T>(pDevice, AnimResource(pAnimResource));
    }

    template <typename T>  // 649
    T* CreateAnimTransform(gfx::Device* pDevice, const AnimResource& animRes) {
        const ResAnimationBlock* const pAnimBlock = animRes.GetResourceBlock();
        if (!pAnimBlock) {
            return nullptr;
        }

        T* const pAnimTrans = CreateAnimTransform<T>();
        if (pAnimTrans) {
            pAnimTrans->SetResource(pDevice, m_pResourceAccessor, pAnimBlock);
        }
        return pAnimTrans;
    }

    template <typename T>  // 688
    T* CreateAnimTransform(gfx::Device* pDevice, const char* pTagName) {
        const void* pPtr = GetAnimResourceData(pTagName);
        return CreateAnimTransform<T>(pDevice, pPtr);
    }

    AnimTransformBasic* CreateAnimTransformBasic();
    AnimTransformBasic* CreateAnimTransformBasic(gfx::Device*, const void*);
    AnimTransformBasic* CreateAnimTransformBasic(gfx::Device*, const AnimResource&);
    AnimTransformBasic* CreateAnimTransformBasic(gfx::Device*, const char*);
    PaneAnimator* CreatePaneAnimator(gfx::Device*, const char*, Pane*, bool);
    PaneAnimator* CreatePaneAnimator(gfx::Device*, const char*, Pane*);
    PaneAnimator* CreatePaneAnimator(gfx::Device*, const char*, const char*, bool);
    PaneAnimator* CreatePaneAnimator(gfx::Device*, const char*, const char*);
    GroupAnimator* CreateGroupAnimator(gfx::Device*, const char*, Group*, bool);
    GroupAnimator* CreateGroupAnimator(gfx::Device*, const char*, Group*);
    GroupAnimator* CreateGroupAnimator(gfx::Device*, const char*, const char*, bool);
    GroupAnimator* CreateGroupAnimator(gfx::Device*, const char*, const char*);
    GroupAnimator* CreateGroupAnimator(gfx::Device*, const char*, bool);
    GroupAnimator* CreateGroupAnimator(gfx::Device*, const char*);
    GroupAnimator* CreateGroupAnimatorWithIndex(gfx::Device*, const char*, int, bool);
    GroupAnimator* CreateGroupAnimatorWithIndex(gfx::Device*, const char*, int);
    GroupArrayAnimator* CreateGroupArrayAnimator(gfx::Device*, const char*, bool);
    GroupArrayAnimator* CreateGroupArrayAnimator(gfx::Device*, const char*);
    Animator* CreateGroupAnimatorAuto(gfx::Device*, const char*, bool);
    Animator* CreateGroupAnimatorAuto(gfx::Device*, const char*);
    virtual void DeleteAnimTransform(AnimTransform*);
    virtual void BindAnimation(AnimTransform*);
    virtual void UnbindAnimation(AnimTransform*);
    virtual void UnbindAnimation(Pane*);
    virtual void UnbindAllAnimation();
    virtual bool BindAnimationAuto(gfx::Device*, const AnimResource&);
    virtual void Animate();
    virtual void UpdateAnimFrame(float);
    virtual void AnimateAndUpdateAnimFrame(float);
    const void* GetAnimResourceData(const char*) const;

    // removed in smo
    virtual void CalculateMtx(DrawInfo& info, bool b) { CalculateImpl(info, b); }

    void Calculate(DrawInfo&);
    void Calculate(DrawInfo&, bool);
    virtual void Draw(DrawInfo&, gfx::CommandBuffer&);
    void DrawCaptureTexture(gfx::Device*, DrawInfo&, gfx::CommandBuffer&);
    bool IsPartsLayout() const;

    Pane* GetRootPane() const { return m_pRootPane; }
    GroupContainer* GetGroupContainer() const { return m_pGroupContainer; }

    const Size& GetLayoutSize() const;
    const ResourceAccessor* GetResourceAccessor() const;
    ResourceAccessor* GetResourceAccessor();
    const font::Rectangle GetLayoutRect() const;
    const char* GetName() const;
    virtual void SetTagProcessor(TextBox::TagProcessor*);

    const AnimTransformList& GetAnimTransformList() const { return m_AnimTransList; }
    AnimTransformList& GetAnimTransformList() { return m_AnimTransList; }

    const PartsPaneList& GetPartsPaneList() const;
    PartsPaneList& GetPartsPaneList();
    const ResExtUserDataList* GetExtUserDataList() const;
    const void* GetLayoutResource() const;
    const ShaderInfo* AcquireArchiveShader(gfx::Device*, const char*) const;
    int GetCaptureTextureCount() const;
    Parts* FindPartsPaneByName(const char*);
    const Parts* FindPartsPaneByName(const char*) const;
    const CaptureTexture* FindCaptureTextureByPanePtr(const Pane*) const;
    void ResetFirstFrameCaptureUpdatdFlag();
    void SetGetUserShaderInformationCallback(GetUserShaderInformationFromUserData, void*);

    void SetGetUserShaderInformationCallback(GetUserShaderInformationFromUserData pGet) {
        m_pGetUserShaderInformationFromUserDataCallback = pGet;
    }

    static void SetRenderTargetTextureManagementCallback(CreateRenderTargetTextureCallback,
                                                         DestroyRenderTargetTextureCallback, void*);
    static void
    SetRenderTargetTextureResourceManagementCallback(CreateRenderTargetTextureResourceCallback,
                                                     DestroyRenderTargetTextureResourceCallback,
                                                     void*);
    static CreateRenderTargetTextureCallback GetCreateRenderTargetTextureCallback();
    static DestroyRenderTargetTextureCallback GetDestroyRenderTargetTextureCallback();
    static CreateRenderTargetTextureResourceCallback GetCreateRenderTargetTextureResourceCallback();
    static DestroyRenderTargetTextureResourceCallback
    GetDestroyRenderTargetTextureResourceCallback();
    static void* GetRenderTargetTextureCallbackUserData();
    bool CompareCopiedInstanceTest(const Layout&) const;

    template <typename T, typename... Params>
    static T* AllocateAndConstruct(Params... params) {
        if (void* pMem = AllocateMemory(sizeof(T))) {
            return new (pMem) T(params...);
        }
        return nullptr;
    }

    template <typename T, typename... Params>
    static T* AllocateAndConstructAligned(size_t alignment, Params... params) {
        if (void* pMem = AllocateMemory(sizeof(T), alignment)) {
            return new (pMem) T(params...);
        }
        return nullptr;
    }

    template <typename T>  // 2159
    static T* NewArray(int count) {
        if (void* pMem = AllocateMemory(sizeof(T) * count)) {
            auto objAry = static_cast<T* const>(pMem);

            for (int i = 0; i < count; ++i) {
                new (&objAry[i]) T();
            }

            return objAry;
        }

        return nullptr;
    }

    template <typename T>  // 2184
    static void DeleteObj(T* pObj) {
        if (pObj) {
            pObj->~T();
            FreeMemory(pObj);
        }
    }

    template <typename T>  // 2201
    static void DeleteArray(T*, int);

    template <typename T>  // 2220
    static void DeletePrimArray(T* objAry) {
        if (objAry) {
            FreeMemory(objAry);
        }
    }

    /* newer
    class PartsBuildDataAccessor {
    public:
        PartsBuildDataAccessor(const ResParts*);
        const ResPartsProperty* FindPartsPropertyFromName(const char*) const;
        const void* GetPropertyResBlock(const ResPartsProperty*) const;
        const ResExtUserDataList* GetExtUserDataListResBlock(bool*, const ResPartsProperty*) const;
        const ResPartsPaneBasicInfo* GetPartsPaneBasicInfoResBlock(const ResPartsProperty*) const;
        bool IsOverwriting() const;

    private:
        int32_t m_PropertyCount;
        const ResPartsProperty* m_pPropertyTable;
        const ResParts* m_pResParts;
    };
    */

    class PartsBuildDataSet {
    public:
        PartsBuildDataSet(Parts*, const ResParts*, const BuildResSet*, const ResVec2*);

        Parts* GetPartsPane() const { return m_pPartsPane; }

        const util::Float2& GetMagnify() const { return m_Magnify; }
        const BuildResSet* GetPropertyBuildResSet() const { return m_pPropertyBuildResSet; }
        const ResPartsProperty* FindPartsPropertyFromName(const char*) const;
        const void* GetPropertyResBlock(const ResPartsProperty*) const;
        const ResExtUserDataList* GetExtUserDataListResBlock(bool*, const ResPartsProperty*) const;
        const ResPartsPaneBasicInfo* GetPartsPaneBasicInfoResBlock(const ResPartsProperty*) const;
        // newer
        // bool IsOverwriting() const;

    private:
        // newer
        // PartsBuildDataAccessor m_PartsBuildDataAccessor;
        int32_t m_PropertyCount;
        const ResPartsProperty* m_pPropertyTable;
        Parts* m_pPartsPane;
        const ResParts* m_pResParts;
        const BuildResSet* m_pPropertyBuildResSet;
        util::Float2 m_Magnify;
    };

protected:
    virtual bool BuildImpl(BuildResultInformation*, gfx::Device*, const void*, ResourceAccessor*,
                           const BuildArgSet&, const PartsBuildDataSet*);
    /*newer
    void PrepareBuildArgSet(BuildArgSet&, const BuildArgSet&, const PartsBuildDataSet*);
    void SetByResLayout(LayoutBuildContext&);
    void BuildControl(LayoutBuildContext&, gfx::Device*);
    void BuildPaneByResPane(LayoutBuildContext&, BuildResultInformation*, gfx::Device*,
        const PartsBuildDataSet*);
    void BuildGroup(LayoutBuildContext&);
    void BuildAlignment(LayoutBuildContext&);
    */

    virtual Pane* BuildPartsImpl(BuildResultInformation*, gfx::Device*, const void*,
                                 const PartsBuildDataSet*, BuildArgSet&, BuildResSet&,
                                 const uint32_t);
    void CopyLayoutInstanceImpl(gfx::Device*, const Layout&, Layout*, const char*);
    // newer
    // void CopyLayoutInstanceImpl(gfx::Device*, const Layout&, Layout*, const char*, const char*);
    virtual Pane* BuildPaneObj(BuildResultInformation*, gfx::Device*, uint32_t, const void*,
                               const void*, const BuildArgSet&);
    virtual Layout* BuildPartsLayout(BuildResultInformation*, gfx::Device*, const char*,
                                     const PartsBuildDataSet&, const BuildArgSet&);

    void SetRootPane(Pane* pPane) { m_pRootPane = pPane; }
    void SetGroupContainer(GroupContainer* pGroupContainer) { m_pGroupContainer = pGroupContainer; }

    void SetLayoutSize(const Size&);
    void SetName(const char* pName) { m_Name = pName; }
    void SetResourceAccessor(ResourceAccessor*);
    const void* GetLayoutResourceData(const char*) const;
    GroupArrayAnimator* CreateGroupArrayAnimator(gfx::Device*, const AnimResource&, bool);
    virtual void CalculateImpl(DrawInfo&, bool);

private:
    /* newer
    template <typename T>  // 2493
    void AllocateCaptureTexture(const ResCaptureTextureList*, ResourceAccessor*, const char*);

    template <typename T>  // 2495
    void InitializeCaptureTextureList(gfx::Device*, const ResCaptureTextureList*,
                                      const PartsBuildDataSet*);
    */

    const char* GetCaptureTextureName(const ResCaptureTextureList*, const ResCaptureTexture*);
    const char* GetCaptureTextureName(const ResCaptureTextureList*, const ResCaptureTextureOld*);
    const char* GetCaptureTextureReferencedPaneName(const ResCaptureTextureList*,
                                                    const ResCaptureTexture*);
    const char* GetCaptureTextureReferencedPaneName(const ResCaptureTextureList*,
                                                    const ResCaptureTextureOld*);

    static AlignedAllocateFunctionWithUserData g_pAllocateFunction;
    static FreeFunctionWithUserData g_pFreeFunction;
    static void* g_pUserDataForAllocator;

    /* newer
    static CreateRenderTargetTextureCallback g_pCreateRenderTargetTextureCallback;
    static DestroyRenderTargetTextureCallback g_pDestroyRenderTargetTextureCallback;
    static CreateRenderTargetTextureResourceCallback g_pCreateRenderTargetTextureResourceCallback;
    static DestroyRenderTargetTextureResourceCallback g_pDestroyRenderTargetTextureResourceCallback;
    static void* g_pRenderTargetTextureCallbackUserData;
    static size_t g_CosntantBufferReservedSize;
    */

    AnimTransformList m_AnimTransList;
    Pane* m_pRootPane;
    GroupContainer* m_pGroupContainer;
    Size m_LayoutSize;
    const char* m_Name;
    const ResExtUserDataList* m_pResExtUserDataList;
    ResourceAccessor* m_pResourceAccessor;
    PartsPaneList m_PartsPaneList;

    GetUserShaderInformationFromUserData m_pGetUserShaderInformationFromUserDataCallback;

    /* smo
    void* m_pGetUserShaderInformationFromUserDataCallbackUserData;
    */

    /* later version
        struct CaptureTextureList {
            int count;
            CaptureTexture* pCaptureTextures;
        };

        CaptureTextureList* m_pCaptureTextureList;
    */
    const Layout& operator=(const Layout&);
};
}  // namespace ui2d
}  // namespace nn
