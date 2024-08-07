#include <nn/ui2d/ui2d_Pane.h>

#include <nn/ui2d/ui2d_DrawInfo.h>
#include <nn/ui2d/ui2d_Layout.h>
#include <nn/ui2d/ui2d_Material.h>
#include <nn/ui2d/ui2d_Resources.h>
#include <nn/util/util_MatrixApi.h>
#include <nn/util/util_StringUtil.h>

namespace nn {
namespace ui2d {

namespace detail {

PaneBase::PaneBase() {}
PaneBase::~PaneBase() {}

}  // namespace detail

Pane::Pane() {
    Initialize();

    m_Translate.x = 0.0f;
    m_Translate.y = 0.0f;
    m_Translate.z = 0.0f;
    m_Rotate.x = 0.0f;
    m_Rotate.y = 0.0f;
    m_Rotate.z = 0.0f;
    m_BasePosition = 0;
    memset(m_UserData, 0, sizeof(m_UserData));
    memset(m_Name, 0, sizeof(m_Name));
    m_Scale.x = 0.0f;
    m_Scale.y = 0.0f;
    m_GlbAlpha = -1;
    m_Alpha = -1;

    m_Size.Set(0.0f, 0.0f);
    SetGlbMtxDirty();
    SetVisible(true);
}

Pane::Pane(const ResPane* pBlock, const BuildArgSet& buildArgSet) : detail::PaneBase() {
    Initialize();

    const ResVec3* translate = &pBlock->translate;

    if (buildArgSet.pOverridePartsPaneBasicInfo) {
        if (detail::TestBit(buildArgSet.overrideBasicUsageFlag,
                            BasicOverrideUsageFlag_TranslateEnabled))
            translate = &buildArgSet.pOverridePartsPaneBasicInfo->translate;
    }
}

// Pane::Pane(const Pane&);

void Pane::Initialize() {
    m_pExtUserDataList = nullptr;
    m_pAnimatedExtUserDataList = nullptr;
    m_pParent = nullptr;
    m_Flag = 0;
    m_FlagEx = 0;
    nn::util::MatrixIdentity(&m_GlbMtx);
    m_pUserMtx = nullptr;
}

Pane::~Pane() {}

void Pane::Finalize(gfx::Device* pDevice) {
    for (auto it = m_ChildList.begin(); it != m_ChildList.end();) {
        auto currIt = it++;
        m_ChildList.erase(currIt);
        if (!(*currIt).IsUserAllocated()) {
            Parts* pParts = DynamicCast<Parts*>(&(*currIt));
            if (pParts && pParts->GetLayout()) {
                Layout* pLayout = pParts->GetLayout();
                pLayout->Finalize(pDevice);
                Layout::DeleteObj(pLayout);
            } else {
                (*currIt).Finalize(pDevice);
                Layout::DeleteObj(&(*currIt));
            }
        }
    }

    if (detail::TestBit(m_FlagEx, PaneFlagEx_ExtUserDataAnimationEnabled)) {
        Layout::FreeMemory(m_pAnimatedExtUserDataList);
    }
}

void Pane::SetName(const char* pName) {
    util::Strlcpy(m_Name, pName, sizeof(m_Name));
}

void Pane::SetUserData(const char* pUserData) {
    util::Strlcpy(m_UserData, pUserData, sizeof(m_UserData));
}

void Pane::AppendChild(Pane* pChild) {
    InsertChild(m_ChildList.end(), pChild);
}

void Pane::PrependChild(Pane* pChild) {
    InsertChild(m_ChildList.begin(), pChild);
}

void Pane::InsertChild(Pane* pNext, Pane* pChild) {
    InsertChild(m_ChildList.iterator_to(*pNext), pChild);
}

void Pane::InsertChild(PaneList::iterator next, Pane* pChild) {
    m_ChildList.insert(next, *pChild);
    pChild->m_pParent = this;
    pChild->SetGlbMtxDirty();
}

void Pane::RemoveChild(Pane* pChild) {
    m_ChildList.erase(m_ChildList.iterator_to(*pChild));
    pChild->m_pParent = nullptr;
}
/*
const font::Rectangle Pane::GetPaneRect() const;
*/

const util::Unorm8x4 Pane::GetVertexColor(int idx) const {
    NN_UNUSED(idx);
    const util::Unorm8x4 result = {{0xFF, 0xFF, 0xFF, 0xFF}};
    return result;
}

void Pane::SetVertexColor(int idx, const util::Unorm8x4& value) {
    NN_UNUSED(idx);
    NN_UNUSED(value);
}

uint8_t Pane::GetColorElement(int idx) const {
    if (idx == AnimTargetPaneColor_PaneAlpha) {
        return m_Alpha;
    } else {
        return GetVertexColorElement(idx);
    }
}

void Pane::SetColorElement(int idx, uint8_t value) {
    if (idx == AnimTargetPaneColor_PaneAlpha) {
        m_Alpha = value;
    } else {
        SetVertexColorElement(idx, value);
    }
}

uint8_t Pane::GetVertexColorElement(int idx) const {
    NN_UNUSED(idx);
    return 0xFF;
}

void Pane::SetVertexColorElement(int idx, uint8_t value) {
    NN_UNUSED(idx);
    NN_UNUSED(value);
}

Pane* Pane::FindPaneByName(const char* pFindName, bool bRecursive) {
    if (detail::EqualsResName(m_Name, pFindName)) {
        return this;
    }

    if (bRecursive) {
        PaneList::iterator endIter = m_ChildList.end();
        for (PaneList::iterator iter = m_ChildList.begin(); iter != endIter; ++iter) {
            Pane* pPane = iter->FindPaneByNameRecursive(pFindName);
            if (pPane)
                return pPane;
        }
    }

    return nullptr;
}

const Pane* Pane::FindPaneByName(const char* pFindName, bool bRecursive) const {
    return const_cast<Pane*>(this)->FindPaneByName(pFindName, bRecursive);
}

Material* Pane::FindMaterialByName(const char* pFindName, bool bRecursive) {
    int nbMaterial = GetMaterialCount();
    for (int idx = 0; idx < nbMaterial; ++idx) {
        Material* pMaterial = GetMaterial(idx);
        if (pMaterial) {
            if (detail::EqualsMaterialName(pMaterial->GetName(), pFindName)) {
                return pMaterial;
            }
        }
    }

    if (bRecursive) {
        PaneList::iterator endIter = m_ChildList.end();
        for (PaneList::iterator iter = m_ChildList.begin(); iter != endIter; ++iter) {
            Material* pMat = iter->FindMaterialByNameRecursive(pFindName);
            if (pMat)
                return pMat;
        }
    }

    return nullptr;
}

const Material* Pane::FindMaterialByName(const char* pFindName, bool bRecursive) const {
    return const_cast<Pane*>(this)->FindMaterialByName(pFindName, bRecursive);
}

/*
void Pane::Calculate(DrawInfo& drawInfo, Pane::CalculateContext& context, bool isDirtyParentMtx) {
    if (!IsVisible() && !context.isInvisiblePaneCalculateMtx) {
        if (isDirtyParentMtx) {
            SetGlbMtxDirty();
        }

        SetConstantBufferReady(false);
        return;
    }

    if (context.isInfluenceAlpha && m_pParent) {
        m_GlbAlpha = m_Alpha * context.influenceAlpha;
    } else {
        m_GlbAlpha = m_Alpha;
    }

    if (m_GlbAlpha == 0 && IsInfluencedAlpha() && !context.isAlphaZeroPaneCalculateMtx) {
        if (isDirtyParentMtx) {
            SetGlbMtxDirty();
        }

        SetConstantBufferReady(false);
        return;
    }

    bool isGlbMtxDirty = IsGlbMtxDirty() || isDirtyParentMtx;
    if (IsUserGlobalMtx()) {
        isGlbMtxDirty = true;
    } else if (IsUserMtx()) {
        nn::util::MatrixMultiply()
    }
}
*/

void Pane::Draw(DrawInfo& drawInfo, gfx::CommandBuffer& commandBuffer) {
    if (IsDrawTreeReady()) {
        if (detail::TestBit(m_FlagEx, PaneFlagEx_IsConstantBufferReadySelf)) {
            if (m_GlbAlpha) {
                DrawSelf(drawInfo, commandBuffer);
            }
        }

        if (!IsInfluencedAlpha() || m_GlbAlpha) {
            PaneList::iterator endIter = m_ChildList.end();
            for (PaneList::iterator iter = m_ChildList.begin(); iter != endIter; ++iter) {
                iter->Draw(drawInfo, commandBuffer);
            }
        }
    }
}

void Pane::DrawSelf(DrawInfo& drawInfo, gfx::CommandBuffer& commandBuffer) {
    NN_UNUSED(drawInfo);
    NN_UNUSED(commandBuffer);
}

void Pane::BindAnimation(AnimTransform* pAnimTrans, bool bRecursive, bool bEnable) {
    pAnimTrans->BindPane(this, bRecursive);
    pAnimTrans->SetEnabled(bEnable);
}

// bool Pane::CompareCopiedInstanceTest(const Pane&) const;

void Pane::UnbindAnimation(AnimTransform* pAnimTrans, bool bRecursive) {
    UnbindAnimationSelf(pAnimTrans);

    if (bRecursive) {
        PaneList::iterator endIter = m_ChildList.end();
        for (PaneList::iterator iter = m_ChildList.begin(); iter != endIter; ++iter) {
            iter->UnbindAnimation(pAnimTrans, bRecursive);
        }
    }
}

void Pane::UnbindAnimationSelf(AnimTransform* pAnimTrans) {
    int nbMaterial = GetMaterialCount();
    for (int idx = 0; idx < nbMaterial; ++idx) {
        Material* pMaterial = GetMaterial(idx);
        if (pMaterial) {
            pAnimTrans->UnbindMaterial(pMaterial);
        }
    }

    pAnimTrans->UnbindPane(this);
}

void Pane::LoadMtx(DrawInfo& drawInfo) {
    drawInfo.SetModelViewMtx(GetGlobalMtx());
}

/*
void Pane::CalculateGlobalMatrix(Pane::CalculateContext&, bool);
*/

const util::Float2 Pane::GetVertexPos() const {
    // todo: figure out magic numbers
    util::Float2 basePt{};
    switch (GetBasePositionX()) {
    default:
        basePt.x = 0.0f;
        break;
    case 0:
        basePt.x = -m_Size.width * .5f;
        break;
    case 2:
        basePt.x = -m_Size.width;
        break;
    }

    switch (GetBasePositionY()) {
    default:
        basePt.y = 0.0f;
        break;
    case 0:
        basePt.y = m_Size.height * .5f;
        break;
    case 2:
        basePt.y = m_Size.height;
        break;
    }
    return basePt;
}

Material* Pane::GetMaterial() const {
    if (GetMaterialCount() != 0) {
        return GetMaterial(0);
    }

    return nullptr;
}

uint8_t Pane::GetMaterialCount() const {
    return 0;
}

Material* Pane::GetMaterial(int idx) const {
    NN_UNUSED(idx);
    GetMaterialCount();  // leftover from assert?
    return nullptr;
}

uint16_t Pane::GetExtUserDataCount() const {
    if (m_pExtUserDataList) {
        return m_pExtUserDataList->count;
    }

    return 0;
}

const ResExtUserData* Pane::GetExtUserDataArray() const {
    if (detail::TestBit(m_FlagEx, PaneFlagEx_ExtUserDataAnimationEnabled)) {
        if (m_pAnimatedExtUserDataList) {
            return util::ConstBytePtr(m_pAnimatedExtUserDataList, sizeof(ResExtUserDataList))
                .Get<ResExtUserData>();
        }
    } else {
        if (m_pExtUserDataList) {
            return util::ConstBytePtr(m_pExtUserDataList, sizeof(ResExtUserDataList))
                .Get<ResExtUserData>();
        }
    }

    return nullptr;
}

// const ResExtUserData* Pane::FindExtUserDataByName(const char*) const;

ResExtUserData* Pane::GetExtUserDataArrayForAnimation() const {
    if (m_pAnimatedExtUserDataList) {
        return util::BytePtr(m_pAnimatedExtUserDataList, sizeof(ResExtUserDataList))
            .Get<ResExtUserData>();
    }

    return nullptr;
}

/*
ResExtUserData* Pane::FindExtUserDataByNameForAnimation(const char*) const;
Pane* Pane::FindPaneByNameRecursive(const char*);
const Pane* Pane::FindPaneByNameRecursive(const char*) const;
Material* Pane::FindMaterialByNameRecursive(const char*);
const Material* Pane::FindMaterialByNameRecursive(const char*) const;
*/

void Pane::CalculateScaleFromPartsRoot(util::Float2* scale, Pane* pane) const {
    scale->x = 1.0f;
    scale->y = 1.0f;

    while (pane && !IsDerivedFrom<Parts>(pane)) {
        scale->x = scale->x * pane->m_Scale.x;
        scale->y = scale->y * pane->m_Scale.y;
        pane = pane->GetParent();
    }
}

void Pane::CalculateContext::SetDefault() {
    pRectDrawer = nullptr;
    pViewMtx = nullptr;
    locationAdjustScale.x = 1.0f;
    locationAdjustScale.y = 1.0f;
    influenceAlpha = 1.0f;
    isLocationAdjust = false;
    isInvisiblePaneCalculateMtx = false;
    isAlphaZeroPaneCalculateMtx = false;
    isInfluenceAlpha = false;
    pLayout = nullptr;
}

void Pane::CalculateContext::Set(const DrawInfo& drawInfo, const Layout* pLayoutData) {
    pRectDrawer = &drawInfo.GetGraphicsResource()->GetFontDrawer();
    pViewMtx = &drawInfo.GetViewMtx();
    locationAdjustScale = drawInfo.GetLocationAdjustScale();
    influenceAlpha = 1.0f;
    isLocationAdjust = drawInfo.IsLocationAdjustEnabled();
    isInvisiblePaneCalculateMtx = drawInfo.IsInvisiblePaneCalculated();
    isAlphaZeroPaneCalculateMtx = drawInfo.IsAlphaZeroPaneCalculated();
    isInfluenceAlpha = false;
    pLayout = pLayoutData;
}

void Pane::AllocateAndCopyAnimatedExtUserData(const ResExtUserDataList* pExtUserDataList) {
    auto pDstExtUserDataList = static_cast<ResExtUserDataList*>(
        Layout::AllocateMemory(pExtUserDataList->blockHeader.size));
    m_pAnimatedExtUserDataList = pDstExtUserDataList;
    memcpy(m_pAnimatedExtUserDataList, pExtUserDataList, pExtUserDataList->blockHeader.size);
}

// void Pane::CalculateGlobalMatrixSelf(Pane::CalculateContext&);

}  // namespace ui2d
}  // namespace nn
