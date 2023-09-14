#include <nn/ui2d/ui2d_Layout.h>

#include <nn/ui2d/ui2d_Animator.h>
#include <nn/ui2d/ui2d_Bounding.h>
#include <nn/ui2d/ui2d_ControlCreator.h>
#include <nn/ui2d/ui2d_DrawInfo.h>
#include <nn/ui2d/ui2d_Group.h>
#include <nn/ui2d/ui2d_Pane.h>
#include <nn/ui2d/ui2d_Picture.h>
#include <nn/ui2d/ui2d_ResourceAccessor.h>
#include <nn/ui2d/ui2d_Resources.h>
#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_Window.h>

namespace nn {
namespace ui2d {

AlignedAllocateFunctionWithUserData Layout::g_pAllocateFunction = nullptr;
FreeFunctionWithUserData Layout::g_pFreeFunction = nullptr;
void* Layout::g_pUserDataForAllocator = nullptr;
/* newer
static CreateRenderTargetTextureCallback Layout::g_pCreateRenderTargetTextureCallback;
static DestroyRenderTargetTextureCallback Layout::g_pDestroyRenderTargetTextureCallback;
static CreateRenderTargetTextureResourceCallback
    Layout::g_pCreateRenderTargetTextureResourceCallback;
static DestroyRenderTargetTextureResourceCallback
    Layout::g_pDestroyRenderTargetTextureResourceCallback;
static void* Layout::g_pRenderTargetTextureCallbackUserData;
static size_t Layout::g_CosntantBufferReservedSize;
*/

namespace {
void SetTagProcessorImpl(Pane* pPane, TextBox::TagProcessor* pTagProcessor) {
    if (TextBox* pTextBox = DynamicCast<TextBox*>(pPane)) {
        pTextBox->SetTagProcessor(pTagProcessor);
    }

    PaneList::iterator endIter = pPane->GetChildList().end();
    for (PaneList::iterator iter = pPane->GetChildList().begin(); iter != endIter; ++iter) {
        SetTagProcessorImpl(&*iter, pTagProcessor);
    }
}

bool IsIncludeAnimationGroupRef(GroupContainer* pGroupContainer,
                                const ResAnimationGroupRef* pGroupRefs, uint16_t bindGroupNum,
                                bool bDescendingBind, Pane* pTargetPane) {
    for (uint16_t groupIndex = 0; groupIndex < bindGroupNum; ++groupIndex) {
        Group* const pGroup = pGroupContainer->FindGroupByName(pGroupRefs[groupIndex].GetName());
        PaneLinkList& paneList = pGroup->GetPaneList();

        auto endIter = paneList.end();
        for (auto iter = paneList.begin(); iter != endIter; ++iter) {
            if (iter->pTarget == pTargetPane) {
                return true;
            }

            if (bDescendingBind) {
                for (Pane* pParentPane = pTargetPane->GetParent(); pParentPane;
                     pParentPane = pParentPane->GetParent()) {
                    if (iter->pTarget == pParentPane) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

const ResExtUserDataList*
TryReadNextBlockUserDataIfExist(const font::detail::BinaryFileHeader* const pFileHead, int* pIndex,
                                const void** ppDataPtr,
                                const font::detail::BinaryBlockHeader** ppDataBlockHead) {
    if (*pIndex + 1 < pFileHead->dataBlocks) {
        auto pNextDataBlockHead = util::ConstBytePtr(*ppDataPtr, (*ppDataBlockHead)->size)
                                      .Get<font::detail::BinaryBlockHeader>();
        if (pNextDataBlockHead->kind == DataBlockKindUserDataList) {
            *pIndex += 1;
            *ppDataPtr = pNextDataBlockHead;
            *ppDataBlockHead = pNextDataBlockHead;

            return reinterpret_cast<const ResExtUserDataList*>(pNextDataBlockHead);
        }
    }
    return nullptr;
}

}  // namespace

void Layout::SetAllocator(AlignedAllocateFunctionWithUserData pAllocateFunction,
                          FreeFunctionWithUserData pFreeFunction, void* pUserData) {
    g_pAllocateFunction = pAllocateFunction;
    g_pFreeFunction = pFreeFunction;
    g_pUserDataForAllocator = pUserData;
}

void* Layout::AllocateMemory(size_t size, size_t alignment) {
    return g_pAllocateFunction(size, alignment, g_pUserDataForAllocator);
}

/* smo function
void* Layout::AllocateMemory(size_t size) {
    return g_pAllocateFunction(size, DefaultAligment, g_pUserDataForAllocator);
}
*/

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

bool Layout::Build(BuildResultInformation* pOutBuildResultInformation, gfx::Device* pDevice,
                   ResourceAccessor* pResAcsr, ControlCreator* pControlCreator,
                   TextSearcher* pTextSearcher, const void* pLayoutResource,
                   const Layout::BuildOption& buildOption, bool utf8) {
    ui2d::BuildArgSet buildArgSet;
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

bool Layout::BuildWithName(BuildResultInformation* pOutBuildResultInformation, gfx::Device* pDevice,
                           ResourceAccessor* pResAcsr, ControlCreator* pControlCreator,
                           TextSearcher* pTextSearcher, const Layout::BuildOption& buildOption,
                           const char* pLayoutFileName, bool utf8) {
    const void* pLayoutResource = pResAcsr->FindResourceByName(ResourceTypeLayout, pLayoutFileName);
    if (pLayoutResource) {
        return Build(pOutBuildResultInformation, pDevice, pResAcsr, pControlCreator, pTextSearcher,
                     pLayoutResource, buildOption, utf8);
    }

    return false;
}

Pane* Layout::BuildPartsImpl(BuildResultInformation* pOutBuildResultInformation,
                             gfx::Device* pDevice, const void* pData,
                             const PartsBuildDataSet* pPartsBuildDataSet, BuildArgSet& buildArgSet,
                             BuildResSet& buildResSet, const uint32_t kind) {
    Pane* pPane = nullptr;
    const void* pResBlock = pData;
    const void* pOverrideResBlock = nullptr;

    buildArgSet.overrideUsageFlag = 0;
    buildArgSet.overrideBasicUsageFlag = 0;
    buildArgSet.overrideMaterialUsageFlag = 0;
    buildArgSet.pOverridePartsPaneBasicInfo = nullptr;

    if (pPartsBuildDataSet) {
        if (!GetRootPane()) {
            pPane = pPartsBuildDataSet->GetPartsPane();
        } else {
            const ResPane* pResPane = static_cast<const ResPane*>(pResBlock);
            const ResPartsProperty* pResPartsProperty =
                pPartsBuildDataSet->FindPartsPropertyFromName(pResPane->name);

            if (pResPartsProperty) {
                pOverrideResBlock = pPartsBuildDataSet->GetPropertyResBlock(pResPartsProperty);
                buildArgSet.overrideUsageFlag = pResPartsProperty->usageFlag;
                buildArgSet.overrideBasicUsageFlag = pResPartsProperty->basicUsageFlag;
                buildArgSet.overrideMaterialUsageFlag = pResPartsProperty->materialUsageFlag;
                buildArgSet.pOverridePartsPaneBasicInfo =
                    pPartsBuildDataSet->GetPartsPaneBasicInfoResBlock(pResPartsProperty);

                bool isOverrideExtUserData = false;
                const ResExtUserDataList* pUserDataList =
                    pPartsBuildDataSet->GetExtUserDataListResBlock(&isOverrideExtUserData,
                                                                   pResPartsProperty);
                if (isOverrideExtUserData) {
                    buildArgSet.pExtUserDataList = pUserDataList;
                }
            }
        }
    } else if (!GetRootPane() && buildArgSet.isRootPaneParts) {
        ResParts resParts;
        *static_cast<ResPane*>(&resParts) = *static_cast<const ResPane*>(pResBlock);
        resParts.blockHeader.kind = DataBlockKindParts;
        resParts.propertyCount = 0;
        resParts.magnify.x = 1.0f;
        resParts.magnify.y = 1.0f;
        pPane = BuildPaneObj(pOutBuildResultInformation, pDevice, DataBlockKindParts, &resParts,
                             pOverrideResBlock, buildArgSet);
        static_cast<Parts*>(pPane)->SetLayout(this);
    }

    if (!pPane) {
        pPane = BuildPaneObj(pOutBuildResultInformation, pDevice, kind, pResBlock,
                             pOverrideResBlock, buildArgSet);
    }

    if (pPane) {
        if (!GetRootPane()) {
            SetRootPane(pPane);
        }

        if (buildArgSet.pParentPane) {
            buildArgSet.pParentPane->AppendChild(pPane);
        }

        if (kind == DataBlockKindParts) {
            Parts* pPartsPane = static_cast<Parts*>(pPane);
            const void* pLocalResBlock;
            const BuildResSet* pLocalBuildResSet;
            if (pOverrideResBlock) {
                pLocalResBlock = pOverrideResBlock;
                pLocalBuildResSet = buildArgSet.pOverrideBuildResSet;
            } else {
                pLocalResBlock = pData;
                pLocalBuildResSet = &buildResSet;
            }

            const ResParts* pResParts = static_cast<const ResParts*>(pLocalResBlock);
            PartsBuildDataSet localPartsBuildDataSet(
                pPartsPane, pResParts, pLocalBuildResSet,
                &static_cast<const ResParts*>(pResBlock)->size);

            Layout* pLayout = BuildPartsLayout(
                pOutBuildResultInformation, pDevice,
                util::ConstBytePtr(pResParts, sizeof(ResParts) + sizeof(ResPartsProperty) *
                                                                     pResParts->propertyCount)
                    .Get<char>(),
                localPartsBuildDataSet, buildArgSet);
            if (pLayout) {
                pPartsPane->SetLayout(pLayout);
                m_PartsPaneList.push_back(*pPartsPane);
            }
        }

        return pPane;
    }

    return nullptr;
}

/* smo
bool IsFlytOlderThanBinary8200(const font::detail::BinaryFileHeader* pFileHead) {
    const uint32_t BinaryFileFormatVersion8000 = 0x8000000;
    const uint32_t BinaryFileFormatVersion8100 = 0x8010000;

    return pFileHead->version == BinaryFileFormatVersion8000 ||
           pFileHead->version == BinaryFileFormatVersion8100;
}
*/

/* newer
void Layout::FindResPaneByName(const ResPane**, const ResExtUserDataList**, const void*, const
char*, const ResPane*);
void Layout::PrepareBuildArgSet(BuildArgSet& buildArgSet, const BuildArgSet& parentBuildArgSet,
const Layout::PartsBuildDataSet* pPartsBuildDataSet); void
Layout::SetByResLayout(Layout::LayoutBuildContext&); void
Layout::BuildControl(Layout::LayoutBuildContext&, gfx::Device*); void
Layout::BuildPaneByResPane(Layout::LayoutBuildContext&, BuildResultInformation*, gfx::Device*,
const Layout::PartsBuildDataSet*); void Layout::BuildGroup(Layout::LayoutBuildContext&);
*/

bool Layout::BuildImpl(BuildResultInformation* pOutBuildResultInformation, gfx::Device* pDevice,
                       const void* pLayoutResource, ResourceAccessor* pResAcsr,
                       const BuildArgSet& parentBuildArgSet,
                       const PartsBuildDataSet* pPartsBuildDataSet) {
    m_pResourceAccessor = pResAcsr;

    auto pFileHead = static_cast<const font::detail::BinaryFileHeader* const>(pLayoutResource);
    // last 2 parameters are unused
    font::detail::IsValidBinaryFile(pFileHead, FileSignatureFlyt, 0, 0);

    BuildResSet buildResSet{};
    buildResSet.pResAccessor = pResAcsr;
    buildResSet.pLayout = this;
    // smo
    // buildResSet.isFlytOlderThanBinary8200 = IsFlytOlderThanBinary8200(pFileHead);

    BuildArgSet buildArgSet;

    if (pPartsBuildDataSet) {
        buildArgSet.magnify = pPartsBuildDataSet->GetMagnify();
        buildArgSet.pOverrideBuildResSet = pPartsBuildDataSet->GetPropertyBuildResSet();
    } else {
        buildArgSet.magnify.x = 1.0f;
        buildArgSet.magnify.y = 1.0f;
        buildArgSet.pOverrideBuildResSet = nullptr;
    }

    buildArgSet.pControlCreator = parentBuildArgSet.pControlCreator;
    buildArgSet.pTextSearcher = parentBuildArgSet.pTextSearcher;
    buildArgSet.pLayout = this;
    buildArgSet.pBodyLayout = parentBuildArgSet.pBodyLayout;
    buildArgSet.pCurrentBuildResSet = &buildResSet;
    buildArgSet.overrideUsageFlag = 0;
    buildArgSet.overrideMaterialUsageFlag = 0;
    buildArgSet.pParentPane = nullptr;
    buildArgSet.isRootPaneParts = parentBuildArgSet.isRootPaneParts;
    buildArgSet.isUtf8 = parentBuildArgSet.isUtf8;
    buildArgSet.pGetUserShaderInformationFromUserDataCallback =
        m_pGetUserShaderInformationFromUserDataCallback;
    /* smo
    buildArgSet.pGetUserShaderInformationFromUserDataCallbackUserData =
        m_pGetUserShaderInformationFromUserDataCallbackData;
    buildArgSet.resourceVersion = version;
    */
    buildArgSet.partsSize.x = 0.0f;
    buildArgSet.partsSize.y = 0.0f;

    Pane* pLastBuiltPane = nullptr;
    bool firstGroup = false;
    int groupNestLevel = 0;

    const void* pData = static_cast<const uint8_t*>(pLayoutResource) + pFileHead->headerSize;
    for (int index = 0; index < pFileHead->dataBlocks; ++index) {
        auto pBlock = static_cast<const font::detail::BinaryBlockHeader*>(pData);
        uint32_t kind = pBlock->kind;

        switch (kind) {
        case DataBlockKindLyt: {
            auto pResLyt = static_cast<const ResLayout*>(pData);
            m_LayoutSize.width = pResLyt->layoutSize.x;
            m_LayoutSize.height = pResLyt->layoutSize.y;
            SetName(util::ConstBytePtr(pResLyt, sizeof(ResLayout)).Get<char>());
            buildArgSet.partsSize.x = pResLyt->partsSize.x;
            buildArgSet.partsSize.y = pResLyt->partsSize.y;

            m_pResExtUserDataList =
                TryReadNextBlockUserDataIfExist(pFileHead, &index, &pData, &pBlock);
        } break;

        case DataBlockKindControl:
            if (buildArgSet.pControlCreator) {
                auto pControlData = pData;
                auto pExtUserDataList =
                    TryReadNextBlockUserDataIfExist(pFileHead, &index, &pData, &pBlock);

                ControlSrc src(pControlData, pExtUserDataList);
                buildArgSet.pControlCreator->CreateControl(pDevice, this, src);
            }
            break;

        case DataBlockKindTextureList:
            buildResSet.pTextureList = static_cast<const ResTextureList*>(pData);
            break;

        case DataBlockKindFontList:
            buildResSet.pFontList = static_cast<const ResFontList*>(pData);
            break;

        case DataBlockKindMateriaList:
            buildResSet.pMaterialList = static_cast<const ResMaterialList*>(pData);
            break;

            /* smo
            case 0x31697073:  // 'spi1'
                buildResSet.pShapeInfoList = static_cast<const ResShapeInfoList*>(pData);
                break;
            */

        case DataBlockKindPane:
        case DataBlockKindPicture:
        case DataBlockKindTextBox:
        case DataBlockKindWindow:
        case DataBlockKindBounding:
        case DataBlockKindParts: {
            auto pPartsData = pData;

            buildArgSet.pExtUserDataList =
                TryReadNextBlockUserDataIfExist(pFileHead, &index, &pData, &pBlock);

            if (auto pane = BuildPartsImpl(pOutBuildResultInformation, pDevice, pPartsData,
                                           pPartsBuildDataSet, buildArgSet, buildResSet, kind))
                pLastBuiltPane = pane;
        } break;

        case DataBlockKindPaneStart:
            buildArgSet.pParentPane = pLastBuiltPane;
            break;

        case DataBlockKindPaneEnd:
            pLastBuiltPane = buildArgSet.pParentPane;
            buildArgSet.pParentPane = pLastBuiltPane->GetParent();
            break;

        case DataBlockKindGroup:
            if (!firstGroup) {
                firstGroup = true;
                SetGroupContainer(AllocateAndConstruct<GroupContainer>());
            } else {
                if (GetGroupContainer() && groupNestLevel == 1) {
                    if (auto group = AllocateAndConstruct<Group>(
                            static_cast<const ResGroup*>(pData), GetRootPane())) {
                        GetGroupContainer()->AppendGroup(group);
                    }
                }
            }
            break;

        case DataBlockKindGroupStart:
            groupNestLevel++;
            break;

        case DataBlockKindGroupEnd:
            groupNestLevel--;
            break;

        default:
            break;
        }

        pData = static_cast<const uint8_t*>(pData) + pBlock->size;
    }

    return true;
}

/* newer
template <typename T>  // 2493
void AllocateCaptureTexture(const ResCaptureTextureList*, ResourceAccessor*, const char*);

template <typename T>  // 2495
void InitializeCaptureTextureList(gfx::Device*, const ResCaptureTextureList*, const
Layout::PartsBuildDataSet*);
*/

void Layout::CopyLayoutInstanceImpl(gfx::Device* pDevice, const Layout& src, Layout* pParentLayout,
                                    const char* pRootPaneName) {
    m_pRootPane = nullptr;
    m_pGroupContainer = nullptr;
    m_LayoutSize = src.m_LayoutSize;
    m_Name = src.m_Name;
    m_pResExtUserDataList = nullptr;
    m_pResourceAccessor = src.m_pResourceAccessor;
    m_pGetUserShaderInformationFromUserDataCallback = nullptr;
    // smo
    // m_pGetUserShaderInformationFromUserDataCallbackData = nullptr;

    m_pRootPane = ClonePaneTreeWithPartsLayout(
        src.m_pRootPane, src.m_pRootPane->GetParent() ? this : nullptr, pDevice, this);

    if (pRootPaneName) {
        m_pRootPane->SetName(pRootPaneName);
    }

    m_pGroupContainer = AllocateAndConstruct<GroupContainer>();
    const GroupList& src_groups = src.GetGroupContainer()->GetGroupList();
    for (GroupList::const_iterator iter = src_groups.begin(), endIter = src_groups.end();
         iter != endIter; ++iter) {
        m_pGroupContainer->AppendGroup(AllocateAndConstruct<Group>(*iter, m_pRootPane));
    }

    if (pParentLayout) {
        Parts* pParts = DynamicCast<Parts*>(m_pRootPane);
        if (pParts) {
            pParentLayout->GetPartsPaneList().push_back(*pParts);
        }
    }
}

void Layout::CalculateGlobalMatrix(DrawInfo& drawInfo, bool forceGlobalMatrixDirty) {
    if (!GetRootPane()) {
        return;
    }

    Pane::CalculateContext context;
    context.Set(drawInfo, this);
    GetRootPane()->CalculateGlobalMatrix(context, forceGlobalMatrixDirty);
}

AnimTransformBasic* Layout::CreateAnimTransformBasic() {
    return CreateAnimTransform<AnimTransformBasic>();
}

void Layout::DeleteAnimTransform(AnimTransform* pAnimTransform) {
    m_AnimTransList.erase(m_AnimTransList.iterator_to(*pAnimTransform));
    DeleteObj(pAnimTransform);
}

AnimTransformBasic* Layout::CreateAnimTransformBasic(gfx::Device* pDevice,
                                                     const void* pAnimResource) {
    return CreateAnimTransform<AnimTransformBasic>(pDevice, pAnimResource);
}

AnimTransformBasic* Layout::CreateAnimTransformBasic(gfx::Device* pDevice,
                                                     const AnimResource& animRes) {
    return CreateAnimTransform<AnimTransformBasic>(pDevice, animRes);
}

AnimTransformBasic* Layout::CreateAnimTransformBasic(gfx::Device* pDevice, const char* pTagName) {
    return CreateAnimTransform<AnimTransformBasic>(pDevice, pTagName);
}

PaneAnimator* Layout::CreatePaneAnimator(gfx::Device* pDevice, const char* pTagName, Pane* pPane,
                                         bool enabled) {
    const void* pResData = GetAnimResourceData(pTagName);
    PaneAnimator* pAnimator = CreateAnimTransform<PaneAnimator>(pDevice, pResData);
    pAnimator->Setup(pPane, enabled);
    return pAnimator;
}

GroupAnimator* Layout::CreateGroupAnimator(gfx::Device* pDevice, const char* pTagName,
                                           Group* pGroup, bool enabled) {
    const void* pResData = GetAnimResourceData(pTagName);
    GroupAnimator* pAnimator = CreateAnimTransform<GroupAnimator>(pDevice, pResData);
    pAnimator->Setup(pGroup, enabled);
    return pAnimator;
}

GroupAnimator* Layout::CreateGroupAnimatorWithIndex(gfx::Device* pDevice, const char* pTagName,
                                                    int groupIndex, bool enabled) {
    const void* pResData = GetAnimResourceData(pTagName);
    AnimResource animRes(pResData);
    GroupAnimator* pAnimator = CreateAnimTransform<GroupAnimator>(pDevice, animRes);
    pAnimator->Setup(animRes, m_pGroupContainer, groupIndex, enabled);
    return pAnimator;
}

GroupArrayAnimator* Layout::CreateGroupArrayAnimator(gfx::Device* pDevice,
                                                     const AnimResource& animRes, bool enabled) {
    const ResAnimationBlock* const pAnimBlock = animRes.GetResourceBlock();
    if (!pAnimBlock) {
        return nullptr;
    }

    void* pPtr = AllocateMemory(88 + animRes.GetGroupCount() * 8);
    if (!pPtr) {
        return nullptr;
    }

    GroupArrayAnimator* pAnimator = new (pPtr) GroupArrayAnimator();
    pAnimator->SetResource(pDevice, m_pResourceAccessor, pAnimBlock);

    GetAnimTransformList().push_back(*pAnimator);

    pAnimator->Setup(animRes, m_pGroupContainer,
                     util::BytePtr(pAnimator, sizeof(GroupArrayAnimator)).Get<Group*>(), enabled);
    return pAnimator;
}

GroupArrayAnimator* Layout::CreateGroupArrayAnimator(gfx::Device* pDevice, const char* pTagName,
                                                     bool enabled) {
    const void* pResData = GetAnimResourceData(pTagName);
    AnimResource animRes(pResData);
    return CreateGroupArrayAnimator(pDevice, animRes, enabled);
}

Animator* Layout::CreateGroupAnimatorAuto(gfx::Device* pDevice, const char* pTagName,
                                          bool enabled) {
    const void* pResData = GetAnimResourceData(pTagName);
    AnimResource animRes(pResData);

    if (animRes.GetGroupCount() > 1) {
        return CreateGroupArrayAnimator(pDevice, animRes, enabled);
    } else {
        GroupAnimator* pAnimator = CreateAnimTransform<GroupAnimator>(pDevice, animRes);
        pAnimator->Setup(animRes, m_pGroupContainer, 0, enabled);
        return pAnimator;
    }

    return nullptr;
}

void Layout::BindAnimation(AnimTransform* pAnimTrans) {
    if (GetRootPane()) {
        pAnimTrans->BindPane(GetRootPane(), true);
    }
}

void Layout::UnbindAnimation(AnimTransform* pAnimTrans) {
    pAnimTrans->UnbindAll();
}

void Layout::UnbindAnimation(Pane* pPane) {
    auto endIter = m_AnimTransList.end();
    for (auto iter = m_AnimTransList.begin(); iter != endIter; ++iter) {
        iter->UnbindPane(pPane);
    }
}

void Layout::UnbindAllAnimation() {
    auto endIter = m_AnimTransList.end();
    for (auto iter = m_AnimTransList.begin(); iter != endIter; ++iter) {
        iter->UnbindAll();
    }
}

bool Layout::BindAnimationAuto(gfx::Device* pDevice, const AnimResource& animRes) {
    if (!GetRootPane() || !animRes.GetResourceBlock()) {
        return false;
    }

    auto pAnimTrans = CreateAnimTransformBasic();
    if (!pAnimTrans) {
        return false;
    }

    bool bResult = true;

    const uint16_t bindGroupNum = animRes.GetGroupCount();
    uint16_t animNum = 0;

    if (bindGroupNum == 0) {
        animNum = animRes.GetResourceBlock()->animContCount;
        pAnimTrans->SetResource(pDevice, m_pResourceAccessor, animRes.GetResourceBlock(), animNum);
        const bool bRecursive = true;
        GetRootPane()->BindAnimation(pAnimTrans, bRecursive, false);
    } else {
        const ResAnimationGroupRef* const pGroupRefs = animRes.GetGroupArray();

        for (int groupIndex = 0; groupIndex < bindGroupNum; ++groupIndex) {
            Group* const pGroup =
                GetGroupContainer()->FindGroupByName(pGroupRefs[groupIndex].GetName());
            if (pGroup) {
                animNum += animRes.CalculateAnimationCount(pGroup, animRes.IsDescendingBind());
            } else {
                bResult = false;
            }
        }

        pAnimTrans->SetResource(pDevice, m_pResourceAccessor, animRes.GetResourceBlock(), animNum);

        for (int groupIndex = 0; groupIndex < bindGroupNum; ++groupIndex) {
            Group* const pGroup =
                GetGroupContainer()->FindGroupByName(pGroupRefs[groupIndex].GetName());
            if (pGroup) {
                ui2d::BindAnimation(pAnimTrans, pGroup, false);
            }
        }
    }

    const uint16_t animShareInfoNum = animRes.GetAnimationShareInfoCount();

    if (animShareInfoNum != 0) {
        const ResAnimationShareInfo* const pAnimShareInfos = animRes.GetAnimationShareInfoArray();

        for (int i = 0; i < animShareInfoNum; ++i) {
            Pane* const pSrcPane =
                GetRootPane()->FindPaneByName(pAnimShareInfos[i].GetSrcPaneName(), true);
            if (!pSrcPane) {
                bResult = false;
                continue;
            }

            detail::AnimPaneTree animPaneTree(pSrcPane, animRes);
            if (!animPaneTree.IsEnabled()) {
                continue;
            }

            Group* const pGroup =
                GetGroupContainer()->FindGroupByName(pAnimShareInfos[i].GetTargetGroupName());
            if (!pGroup) {
                bResult = false;
                continue;
            }

            PaneLinkList& paneList = pGroup->GetPaneList();
            for (auto iter = paneList.begin(); iter != paneList.end(); ++iter) {
                if (iter->pTarget != pSrcPane) {
                    if (bindGroupNum != 0) {
                        const bool bInclude = IsIncludeAnimationGroupRef(
                            GetGroupContainer(), animRes.GetGroupArray(), bindGroupNum,
                            animRes.IsDescendingBind(), iter->pTarget);

                        if (!bInclude) {
                            continue;
                        }
                    }

                    animPaneTree.Bind(pDevice, this, iter->pTarget, m_pResourceAccessor);
                }
            }
        }
    }

    return bResult;
}

void Layout::CalculateImpl(DrawInfo& drawInfo, bool forceGlbMtxDirty) {
    if (!GetRootPane()) {
        return;
    }

    Pane::CalculateContext context;
    context.Set(drawInfo, this);

    drawInfo.SetLayout(this);
    GetRootPane()->Calculate(drawInfo, context, forceGlbMtxDirty);
    drawInfo.SetLayout(nullptr);
}

// newer
// void Layout::DrawCaptureTexture(gfx::Device*, DrawInfo&, gfx::CommandBuffer&);

void Layout::Draw(DrawInfo& drawInfo, gfx::CommandBuffer& commandBuffer) {
    if (!GetRootPane()) {
        return;
    }

    drawInfo.ResetVertexBufferState();
    // todo: throw in DrawInfo header
    drawInfo.filler[0] = 6;
    drawInfo.filler[1] = 0;
    drawInfo.filler[2] = 0;
    // smo
    // drawInfo.ResetCurrentShader();

    drawInfo.SetLayout(this);
    GetRootPane()->Draw(drawInfo, commandBuffer);
    drawInfo.SetLayout(nullptr);
}

void Layout::Animate() {
    for (auto iter = m_AnimTransList.begin(), endIter = m_AnimTransList.end(); iter != endIter;
         ++iter) {
        iter->Animate();
    }

    for (auto iter = m_PartsPaneList.begin(), endIter = m_PartsPaneList.end(); iter != endIter;
         ++iter) {
        iter->GetLayout()->Animate();
    }
}

void Layout::UpdateAnimFrame(float progressFrame) {
    for (auto iter = m_AnimTransList.begin(), endIter = m_AnimTransList.end(); iter != endIter;
         ++iter) {
        iter->UpdateFrame(progressFrame);
    }

    for (auto iter = m_PartsPaneList.begin(), endIter = m_PartsPaneList.end(); iter != endIter;
         ++iter) {
        iter->GetLayout()->UpdateAnimFrame(progressFrame);
    }
}

void Layout::AnimateAndUpdateAnimFrame(float progressFrame) {
    for (auto iter = m_AnimTransList.begin(), endIter = m_AnimTransList.end(); iter != endIter;
         ++iter) {
        iter->Animate();
        iter->UpdateFrame(progressFrame);
    }

    for (auto iter = m_PartsPaneList.begin(), endIter = m_PartsPaneList.end(); iter != endIter;
         ++iter) {
        iter->GetLayout()->AnimateAndUpdateAnimFrame(progressFrame);
    }
}

const font::Rectangle Layout::GetLayoutRect() const {
    return font::Rectangle(m_LayoutSize.width * -0.5f, m_LayoutSize.height * 0.5f,
                           m_LayoutSize.width * 0.5f, m_LayoutSize.height * -0.5f);
}

void Layout::SetTagProcessor(TextBox::TagProcessor* pTagProcessor) {
    SetTagProcessorImpl(GetRootPane(), pTagProcessor);
}

Parts* Layout::FindPartsPaneByName(const char* pFindName) {
    for (auto iter = m_PartsPaneList.begin(), endIter = m_PartsPaneList.end(); iter != endIter;
         ++iter) {
        if (detail::EqualsResName(pFindName, iter->GetName())) {
            return &*iter;
        }
    }

    return nullptr;
}

/* newer
const CaptureTexture* Layout::FindCaptureTextureByPanePtr(const Pane*) const;
void Layout::ResetFirstFrameCaptureUpdatdFlag();
*/

const Parts* Layout::FindPartsPaneByName(const char* pFindName) const {
    return const_cast<Layout*>(this)->FindPartsPaneByName(pFindName);
}

// bool Layout::CompareCopiedInstanceTest(const Layout&) const;

const ShaderInfo* Layout::AcquireArchiveShader(gfx::Device* pDevice, const char* pName) const {
    return m_pResourceAccessor->AcquireShader(pDevice, pName);
}

Pane* Layout::BuildPaneObj(BuildResultInformation* pOutBuildResultInformation, gfx::Device* pDevice,
                           uint32_t kind, const void* pBlock, const void* pOverrideBlock,
                           const BuildArgSet& buildArgSet) {
    switch (kind) {
    case DataBlockKindPane: {
        auto pResPane = static_cast<const ResPane*>(pBlock);
        return AllocateAndConstructAligned<Pane>(16, pResPane, buildArgSet);
    }

    case DataBlockKindPicture: {
        auto pResPic = static_cast<const ResPicture*>(pBlock);
        auto pResPicOverride = static_cast<const ResPicture*>(pOverrideBlock);
        return AllocateAndConstructAligned<Picture>(16, pOutBuildResultInformation, pDevice,
                                                    pResPic, pResPicOverride, buildArgSet);
    }

    case DataBlockKindTextBox: {
        auto pResTextBox = static_cast<const ResTextBox*>(pBlock);
        auto pResTextBoxOverride = static_cast<const ResTextBox*>(pOverrideBlock);
        TextBox::InitializeStringParam initStringParam;
        TextBox* textBox = AllocateAndConstructAligned<TextBox>(
            16, pOutBuildResultInformation, pDevice, &initStringParam, pResTextBox,
            pResTextBoxOverride, buildArgSet);
        textBox->InitializeString(pOutBuildResultInformation, pDevice, buildArgSet,
                                  initStringParam);
        return textBox;
    }

    case DataBlockKindWindow: {
        auto pResWindow = static_cast<const ResWindow*>(pBlock);
        auto pResWindowOverride = static_cast<const ResWindow*>(pOverrideBlock);
        return AllocateAndConstructAligned<Window>(16, pOutBuildResultInformation, pDevice,
                                                   pResWindow, pResWindowOverride, buildArgSet);
    }

    case DataBlockKindBounding: {
        auto pResBounding = static_cast<const ResBounding*>(pBlock);
        auto pResBoundingOverride = static_cast<const ResBounding*>(pOverrideBlock);
        return AllocateAndConstructAligned<Bounding>(16, pResBounding, pResBoundingOverride,
                                                     buildArgSet);
    }

    case DataBlockKindParts: {
        auto pResParts = static_cast<const ResParts*>(pBlock);
        auto pResPartsOverride = static_cast<const ResParts*>(pOverrideBlock);
        return AllocateAndConstructAligned<Parts>(16, pResParts, pResPartsOverride, buildArgSet);
    }

    default:
        break;
    }

    return nullptr;
}

Layout* Layout::BuildPartsLayout(BuildResultInformation* pOutBuildResultInformation,
                                 gfx::Device* pDevice, const char* pName,
                                 const PartsBuildDataSet& partsBuildDataSet,
                                 const BuildArgSet& buildArgSet) {
    const void* pResPtr = GetLayoutResourceData(pName);

    auto pLayout = AllocateAndConstruct<Layout>();
    // auto pLayout = new (AllocateMemory(sizeof(Layout))) Layout();

    pLayout->SetGetUserShaderInformationCallback(m_pGetUserShaderInformationFromUserDataCallback);
    //  pLayout->m_pGetUserShaderInformationFromUserDataCallback =
    //      m_pGetUserShaderInformationFromUserDataCallback;
    /* smo
    pLayout->m_pGetUserShaderInformationFromUserDataCallbackData =
        m_pGetUserShaderInformationFromUserDataCallbackData;
    */
    pLayout->BuildImpl(pOutBuildResultInformation, pDevice, pResPtr, m_pResourceAccessor,
                       buildArgSet, &partsBuildDataSet);
    return pLayout;
}

const void* Layout::GetAnimResourceData(const char* pTagName) const {
    char brlanPath[136];
    util::SNPrintf(brlanPath, sizeof(brlanPath), "%s_%s.bflan", m_Name, pTagName);
    return m_pResourceAccessor->FindResourceByName(ResourceTypeAnimation, brlanPath);
}

const void* Layout::GetLayoutResourceData(const char* pLayoutName) const {
    char bclytPath[72];
    util::SNPrintf(bclytPath, sizeof(bclytPath), "%s.bflyt", pLayoutName);
    return m_pResourceAccessor->FindResourceByName(ResourceTypeLayout, bclytPath);
}

Layout::PartsBuildDataSet::PartsBuildDataSet(Parts* pPartsPane, const ResParts* pResParts,
                                             const BuildResSet* pBuildResSet,
                                             const ResVec2* pOriginalSize)
    : m_PropertyCount(pResParts->propertyCount),
      m_pPropertyTable(util::ConstBytePtr(pResParts, sizeof(ResParts)).Get<ResPartsProperty>()),
      m_pPartsPane(pPartsPane), m_pResParts(pResParts), m_pPropertyBuildResSet(pBuildResSet) {
    m_Magnify.x = (pPartsPane->GetSize().width / pOriginalSize->x) * pResParts->magnify.x;
    m_Magnify.y = (pPartsPane->GetSize().height / pOriginalSize->y) * pResParts->magnify.y;
}

const ResPartsProperty*
Layout::PartsBuildDataSet::FindPartsPropertyFromName(const char* pName) const {
    for (int32_t i = 0; i < m_PropertyCount; ++i) {
        if (detail::EqualsResName(pName, m_pPropertyTable[i].name)) {
            return &m_pPropertyTable[i];
        }
    }

    return nullptr;
}

/* newer
bool Layout::PartsBuildDataSet::IsOverwriting() const;
*/

const void*
Layout::PartsBuildDataSet::GetPropertyResBlock(const ResPartsProperty* pResPartsProperty) const {
    if (pResPartsProperty->propertyOffset != 0) {
        return util::ConstBytePtr(m_pResParts, pResPartsProperty->propertyOffset).Get<void>();
    }

    return nullptr;
}

const ResExtUserDataList* Layout::PartsBuildDataSet::GetExtUserDataListResBlock(
    bool* pIsOverride, const ResPartsProperty* pResPartsProperty) const {
    if (pResPartsProperty->extUserDataOffset == ExtUserDataOverrideOffset_NoOverride) {
        *pIsOverride = false;
        return nullptr;
    } else if (pResPartsProperty->extUserDataOffset == ExtUserDataOverrideOffset_NoData) {
        *pIsOverride = true;
        return nullptr;
    } else {
        *pIsOverride = true;
        return util::ConstBytePtr(m_pResParts, pResPartsProperty->extUserDataOffset)
            .Get<ResExtUserDataList>();
    }
}

const ResPartsPaneBasicInfo* Layout::PartsBuildDataSet::GetPartsPaneBasicInfoResBlock(
    const ResPartsProperty* pResPartsProperty) const {
    if (pResPartsProperty->paneBasicInfoOffset != 0) {
        return util::ConstBytePtr(m_pResParts, pResPartsProperty->paneBasicInfoOffset)
            .Get<ResPartsPaneBasicInfo>();
    }

    return nullptr;
}

}  // namespace ui2d
}  // namespace nn
