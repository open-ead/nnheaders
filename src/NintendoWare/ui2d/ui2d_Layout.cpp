#include <nn/ui2d/ui2d_Layout.h>

#include <nn/ui2d/ui2d_Group.h>
#include <nn/ui2d/ui2d_Pane.h>
#include <nn/ui2d/ui2d_ResourceAccessor.h>
#include <nn/ui2d/ui2d_Resources.h>

namespace nn {
namespace ui2d {

AlignedAllocateFunctionWithUserData Layout::g_pAllocateFunction = nullptr;
FreeFunctionWithUserData Layout::g_pFreeFunction = nullptr;
void* Layout::g_pUserDataForAllocator = nullptr;
/*
static CreateRenderTargetTextureCallback Layout::g_pCreateRenderTargetTextureCallback;
static DestroyRenderTargetTextureCallback Layout::g_pDestroyRenderTargetTextureCallback;
static CreateRenderTargetTextureResourceCallback
    Layout::g_pCreateRenderTargetTextureResourceCallback;
static DestroyRenderTargetTextureResourceCallback
    Layout::g_pDestroyRenderTargetTextureResourceCallback;
static void* Layout::g_pRenderTargetTextureCallbackUserData;
static size_t Layout::g_CosntantBufferReservedSize;
*/

void Layout::SetAllocator(AlignedAllocateFunctionWithUserData pAllocateFunction,
                          FreeFunctionWithUserData pFreeFunction, void* pUserData) {
    g_pAllocateFunction = pAllocateFunction;
    g_pFreeFunction = pFreeFunction;
    g_pUserDataForAllocator = pUserData;
}

void* Layout::AllocateMemory(size_t size, size_t alignment) {
    return g_pAllocateFunction(size, alignment, g_pUserDataForAllocator);
}

void* Layout::AllocateMemory(size_t size) {
    return g_pAllocateFunction(size, DefaultAligment, g_pUserDataForAllocator);
}

void Layout::FreeMemory(void* pMemory) {
    g_pFreeFunction(pMemory, g_pUserDataForAllocator);
}

Layout::Layout()
    : m_pRootPane(nullptr), m_pGroupContainer(nullptr), m_Name(nullptr),
      m_pResourceAccessor(nullptr), m_pGetUserShaderInformationFromUserDataCallback(nullptr) {
    m_LayoutSize.Set(0.0f, 0.0f);
}

Layout::~Layout() {
    ;  // hack to force vtable store
}

void Layout::Finalize(gfx::Device* pDevice) {
    m_PartsPaneList.clear();

    DeleteObj(m_pGroupContainer);
    m_pGroupContainer = nullptr;

    if (m_pRootPane && !m_pRootPane->IsUserAllocated()) {
        m_pRootPane->Finalize(pDevice);
        DeleteObj(m_pRootPane);
        m_pRootPane = nullptr;
    }

    for (AnimTransformList::iterator iter = m_AnimTransList.begin();
         iter != m_AnimTransList.end();) {
        AnimTransformList::iterator currIter = iter++;
        m_AnimTransList.erase(currIter);
        DeleteObj(&*currIter);
    }

    m_pResExtUserDataList = nullptr;
    m_pResourceAccessor = nullptr;
    m_LayoutSize.Set(0.0f, 0.0f);
    m_Name = nullptr;
    m_pGetUserShaderInformationFromUserDataCallback = nullptr;
}

bool Layout::Build(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice,
                   ResourceAccessor* pResAcsr, ControlCreator* pControlCreator,
                   TextSearcher* pTextSearcher, const void* pLayoutResource,
                   const Layout::BuildOption& buildOption, bool utf8) {
    nn::ui2d::BuildArgSet buildArgSet;
    buildArgSet.magnify.x = 1.0f;
    buildArgSet.magnify.y = 1.0f;
    buildArgSet.partsSize.x = 0.0f;
    buildArgSet.partsSize.y = 0.0f;
    buildArgSet.pControlCreator = pControlCreator;
    buildArgSet.pTextSearcher = pTextSearcher;
    buildArgSet.pLayout = nullptr;
    buildArgSet.pBodyLayout = this;
    buildArgSet.isRootPaneParts = buildOption.isRootPaneParts;
    buildArgSet.isUtf8 = utf8;
    buildArgSet.pGetUserShaderInformationFromUserDataCallback =
        m_pGetUserShaderInformationFromUserDataCallback;

    return BuildImpl(pOutBuildResultInformation, pDevice, pLayoutResource, pResAcsr, buildArgSet,
                     nullptr);
}

bool Layout::BuildWithName(BuildResultInformation* pOutBuildResultInformation,
                           nn::gfx::Device* pDevice, ResourceAccessor* pResAcsr,
                           ControlCreator* pControlCreator, TextSearcher* pTextSearcher,
                           const Layout::BuildOption& buildOption, const char* pLayoutFileName,
                           bool utf8) {
    const void* pLayoutResource = pResAcsr->FindResourceByName(ResourceTypeLayout, pLayoutFileName);
    if (pLayoutResource) {
        return Build(pOutBuildResultInformation, pDevice, pResAcsr, pControlCreator, pTextSearcher,
                     pLayoutResource, buildOption, utf8);
    }

    return false;
}
/* clang-format off
Pane* Layout::BuildPartsImpl(BuildResultInformation*, nn::gfx::Device*, const void*, const Layout::PartsBuildDataSet*, BuildArgSet&, BuildResSet&, unsigned int);
void Layout::FindResPaneByName(const ResPane**, const ResExtUserDataList**, const void*, const char*, const ResPane*);
void Layout::PrepareBuildArgSet(BuildArgSet&, const BuildArgSet&, const Layout::PartsBuildDataSet*);
void Layout::SetByResLayout(Layout::LayoutBuildContext&);
void Layout::BuildControl(Layout::LayoutBuildContext&, nn::gfx::Device*);
void Layout::BuildPaneByResPane(Layout::LayoutBuildContext&, BuildResultInformation*, nn::gfx::Device*, const Layout::PartsBuildDataSet*);
void Layout::BuildGroup(Layout::LayoutBuildContext&);
bool Layout::BuildImpl(BuildResultInformation*, nn::gfx::Device*, const void*, ResourceAccessor*, const BuildArgSet&, const Layout::PartsBuildDataSet*);

template <typename T>  // 2493
void AllocateCaptureTexture(const ResCaptureTextureList*, ResourceAccessor*, const char*);

template <typename T>  // 2495
void InitializeCaptureTextureList(gfx::Device*, const ResCaptureTextureList*, const Layout::PartsBuildDataSet*);

void Layout::CopyLayoutInstanceImpl(nn::gfx::Device*, const Layout&, Layout*, const char*, const char*);
void Layout::CalculateGlobalMatrix(DrawInfo&, bool);
AnimTransformBasic* Layout::CreateAnimTransformBasic();
void Layout::DeleteAnimTransform(AnimTransform*);
AnimTransformBasic* Layout::CreateAnimTransformBasic(nn::gfx::Device*, const void*);
AnimTransformBasic* Layout::CreateAnimTransformBasic(nn::gfx::Device*, const AnimResource&);
AnimTransformBasic* Layout::CreateAnimTransformBasic(nn::gfx::Device*, const char*);
PaneAnimator* Layout::CreatePaneAnimator(nn::gfx::Device*, const char*, Pane*, bool);
GroupAnimator* Layout::CreateGroupAnimator(nn::gfx::Device*, const char*, Group*, bool);
GroupAnimator* Layout::CreateGroupAnimatorWithIndex(nn::gfx::Device*, const char*, int, bool);
GroupArrayAnimator* Layout::CreateGroupArrayAnimator(nn::gfx::Device*, const AnimResource&, bool);
GroupArrayAnimator* Layout::CreateGroupArrayAnimator(nn::gfx::Device*, const char*, bool);
Animator* Layout::CreateGroupAnimatorAuto(nn::gfx::Device*, const char*, bool);
void Layout::BindAnimation(AnimTransform*);
void Layout::UnbindAnimation(AnimTransform*);
void Layout::UnbindAnimation(Pane*);
void Layout::UnbindAllAnimation();
bool Layout::BindAnimationAuto(nn::gfx::Device*, const AnimResource&);
void Layout::CalculateImpl(DrawInfo&, bool);
void Layout::DrawCaptureTexture(nn::gfx::Device*, DrawInfo&, nn::gfx::TCommandBuffer<nn::gfx::ApiVariation<nn::gfx::ApiType<4>, nn::gfx::ApiVersion<8>>>&);
void Layout::Draw(DrawInfo&, nn::gfx::TCommandBuffer<nn::gfx::ApiVariation<nn::gfx::ApiType<4>, nn::gfx::ApiVersion<8>>>&);
void Layout::Animate();
void Layout::UpdateAnimFrame(float);
void Layout::AnimateAndUpdateAnimFrame(float);
const nn::font::Rectangle Layout::GetLayoutRect() const;
void Layout::SetTagProcessor(nn::font::TagProcessorBase<unsigned short>*);
Parts* Layout::FindPartsPaneByName(const char*);
const CaptureTexture* Layout::FindCaptureTextureByPanePtr(const Pane*) const;
void Layout::ResetFirstFrameCaptureUpdatdFlag();
const Parts* Layout::FindPartsPaneByName(const char*) const;
bool Layout::CompareCopiedInstanceTest(const Layout&) const;
const ShaderInfo* Layout::AcquireArchiveShader(nn::gfx::Device*, const char*) const;
Pane* Layout::BuildPaneObj(BuildResultInformation*, nn::gfx::Device*, unsigned int, const void*, const void*, const BuildArgSet&);
Layout* Layout::BuildPartsLayout(BuildResultInformation*, nn::gfx::Device*, const char*, const Layout::PartsBuildDataSet&, const BuildArgSet&);
const void* Layout::GetAnimResourceData(const char*) const;
const void* Layout::GetLayoutResourceData(const char*) const;
void Layout::PartsBuildDataAccessor::PartsBuildDataAccessor(const ResParts*);
const ResPartsProperty* Layout::PartsBuildDataAccessor::FindPartsPropertyFromName(const char*) const;
bool Layout::PartsBuildDataAccessor::IsOverwriting() const;
const void* Layout::PartsBuildDataAccessor::GetPropertyResBlock(const ResPartsProperty*) const;
const ResExtUserDataList* Layout::PartsBuildDataAccessor::GetExtUserDataListResBlock(bool*, const ResPartsProperty*) const;
const ResPartsPaneBasicInfo* Layout::PartsBuildDataAccessor::GetPartsPaneBasicInfoResBlock(const ResPartsProperty*) const;
void Layout::PartsBuildDataSet::PartsBuildDataSet(Parts*, const ResParts*, const BuildResSet*, const ResVec2*);
*/
}  // namespace ui2d
}  // namespace nn
