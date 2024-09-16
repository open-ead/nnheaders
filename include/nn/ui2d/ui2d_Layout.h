/**
 * @file Layout.h
 * @brief UI Layout implementation.
 */

#pragma once

#include <nn/font/font_TagProcessorBase.h>
#include <nn/font/font_Types.h>
#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/nn_Allocator.h>
#include <nn/types.h>
#include <nn/ui2d/ui2d_Animation.h>
#include <nn/ui2d/ui2d_Common.h>
#include <nn/ui2d/ui2d_Parts.h>
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
class ResPartsProperty;
class ResVec2;
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
    static void* AllocateMemory(size_t, size_t);
    static void* AllocateMemory(size_t);
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
    T* CreateAnimTransform();

    template <typename T>  // 625
    T* CreateAnimTransform(gfx::Device*, const void*);

    template <typename T>  // 649
    T* CreateAnimTransform(gfx::Device*, const AnimResource&);

    template <typename T>  // 688
    T* CreateAnimTransform(gfx::Device*, const char*);

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
    Pane* GetRootPane() const;
    GroupContainer* GetGroupContainer() const;
    const Size& GetLayoutSize() const;
    const ResourceAccessor* GetResourceAccessor() const;
    ResourceAccessor* GetResourceAccessor();
    const font::Rectangle GetLayoutRect() const;
    const char* GetName() const;
    virtual void SetTagProcessor(font::TagProcessorBase<uint16_t>*);
    const AnimTransformList& GetAnimTransformList() const;
    AnimTransformList& GetAnimTransformList();
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
    void SetGetUserShaderInformationCallback(GetUserShaderInformationFromUserData);
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

    template <typename T>  // 1731
    static T* AllocateAndConstruct();

    template <typename T, typename Param1, typename Param2>  // 1819
    static T* AllocateAndConstruct(Param1, Param2);

    template <typename T, typename Param1, typename Param2>  // 1843
    static T* AllocateAndConstruct(Param1, Param2, size_t);

    template <typename T, typename Param1, typename Param2, typename Param3>  // 1895
    static T* AllocateAndConstruct(Param1, Param2, Param3, size_t);

    template <typename T, typename Param1, typename Param2, typename Param3,
              typename Param4>  // 1951
    static T* AllocateAndConstruct(Param1, Param2, Param3, Param4, size_t);

    template <typename T, typename Param1, typename Param2, typename Param3, typename Param4,
              typename Param5>  // 2011
    static T* AllocateAndConstruct(Param1, Param2, Param3, Param4, Param5, size_t);

    template <typename T, typename Param1, typename Param2, typename Param3, typename Param4,
              typename Param5, typename Param6>  // 2074
    static T* AllocateAndConstruct(Param1, Param2, Param3, Param4, Param5, Param6, size_t);

    template <typename T>  // 2159
    static T* NewArray(int);

    template <typename T>  // 2184
    static void DeleteObj(T* pObj) {
        if (pObj) {
            pObj->~T();
            FreeMemory(pObj);
        }
    }

    template <typename T>  // 2201
    static void DeleteArray(T*, int);

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

    class PartsBuildDataSet {
    public:
        PartsBuildDataSet(Parts*, const ResParts*, const BuildResSet*, const ResVec2*);
        Parts* GetPartsPane() const;
        const util::Float2& GetMagnify() const;
        const BuildResSet* GetPropertyBuildResSet() const;
        const ResPartsProperty* FindPartsPropertyFromName(const char*) const;
        const void* GetPropertyResBlock(const ResPartsProperty*) const;
        const ResExtUserDataList* GetExtUserDataListResBlock(bool*, const ResPartsProperty*) const;
        const ResPartsPaneBasicInfo* GetPartsPaneBasicInfoResBlock(const ResPartsProperty*) const;
        bool IsOverwriting() const;

    private:
        PartsBuildDataAccessor m_PartsBuildDataAccessor;
        Parts* m_pPartsPane;
        const BuildResSet* m_pPropertyBuildResSet;
        util::Float2 m_Magnify;
    };

protected:
    virtual bool BuildImpl(BuildResultInformation*, gfx::Device*, const void*, ResourceAccessor*,
                           const BuildArgSet&, const PartsBuildDataSet*);
    void PrepareBuildArgSet(BuildArgSet&, const BuildArgSet&, const PartsBuildDataSet*);
    void SetByResLayout(LayoutBuildContext&);
    void BuildControl(LayoutBuildContext&, gfx::Device*);
    void BuildPaneByResPane(LayoutBuildContext&, BuildResultInformation*, gfx::Device*,
                            const PartsBuildDataSet*);
    void BuildGroup(LayoutBuildContext&);
    void BuildAlignment(LayoutBuildContext&);
    virtual Pane* BuildPartsImpl(BuildResultInformation*, gfx::Device*, const void*,
                                 const PartsBuildDataSet*, BuildArgSet&, BuildResSet&,
                                 const uint32_t);
    void CopyLayoutInstanceImpl(gfx::Device*, const Layout&, Layout*, const char*);
    void CopyLayoutInstanceImpl(gfx::Device*, const Layout&, Layout*, const char*, const char*);
    virtual Pane* BuildPaneObj(BuildResultInformation*, gfx::Device*, uint32_t, const void*,
                               const void*, const BuildArgSet&);
    virtual Layout* BuildPartsLayout(BuildResultInformation*, gfx::Device*, const char*,
                                     const PartsBuildDataSet&, const BuildArgSet&);
    void SetRootPane(Pane*);
    void SetGroupContainer(GroupContainer*);
    void SetLayoutSize(const Size&);
    void SetName(const char*);
    void SetResourceAccessor(ResourceAccessor*);
    const void* GetLayoutResourceData(const char*) const;
    GroupArrayAnimator* CreateGroupArrayAnimator(gfx::Device*, const AnimResource&, bool);
    virtual void CalculateImpl(DrawInfo&, bool);

private:
    template <typename T>  // 2493
    void AllocateCaptureTexture(const ResCaptureTextureList*, ResourceAccessor*, const char*);

    template <typename T>  // 2495
    void InitializeCaptureTextureList(gfx::Device*, const ResCaptureTextureList*,
                                      const PartsBuildDataSet*);

    const char* GetCaptureTextureName(const ResCaptureTextureList*, const ResCaptureTexture*);
    const char* GetCaptureTextureName(const ResCaptureTextureList*, const ResCaptureTextureOld*);
    const char* GetCaptureTextureReferencedPaneName(const ResCaptureTextureList*,
                                                    const ResCaptureTexture*);
    const char* GetCaptureTextureReferencedPaneName(const ResCaptureTextureList*,
                                                    const ResCaptureTextureOld*);

    static AlignedAllocateFunctionWithUserData g_pAllocateFunction;
    static FreeFunctionWithUserData g_pFreeFunction;
    static void* g_pUserDataForAllocator;
    static CreateRenderTargetTextureCallback g_pCreateRenderTargetTextureCallback;
    static DestroyRenderTargetTextureCallback g_pDestroyRenderTargetTextureCallback;
    static CreateRenderTargetTextureResourceCallback g_pCreateRenderTargetTextureResourceCallback;
    static DestroyRenderTargetTextureResourceCallback g_pDestroyRenderTargetTextureResourceCallback;
    static void* g_pRenderTargetTextureCallbackUserData;
    static size_t g_CosntantBufferReservedSize;

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
