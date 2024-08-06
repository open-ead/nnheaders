#include <nn/ui2d/ui2d_Animation.h>

#include <nn/ui2d/ui2d_Group.h>
#include <nn/ui2d/ui2d_Layout.h>
#include <nn/ui2d/ui2d_Material.h>
#include <nn/ui2d/ui2d_ResourceAccessor.h>
#include <nn/ui2d/ui2d_Resources.h>
#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_Window.h>

namespace nn {
namespace ui2d {

#define SIG(name) const uint32_t name = MakeSig(#name)

SIG(FLPA);
SIG(FLVI);
SIG(FLVC);
SIG(FLCT);
// SIG(FLWN); added in smo

SIG(FLTS);
SIG(FLTP);
SIG(FLFS);
SIG(FLIM);
SIG(FLAC);
SIG(FLMC);

namespace {
bool RIsSame(float a, float b, float tolerance = 0.001f) {
    float c = a - b;
    return c > -tolerance && c < tolerance;
}

uint16_t GetStepCurveValue(float frame, const ResStepKey* pKeys, uint32_t keySize) {
    // doesn't match with using ikeyL and ikeyR
    if (keySize == 1 || frame <= pKeys[0].frame) {
        return pKeys[0].value;
    } else if (frame >= pKeys[keySize - 1].frame) {
        return pKeys[keySize - 1].value;
    }

    int ikeyL = 0;
    int ikeyR = keySize - 1;

    while (ikeyL != ikeyR && ikeyL != ikeyR - 1) {
        int ikeyCenter = (ikeyL + ikeyR) / 2;
        const ResStepKey& centerKey = pKeys[ikeyCenter];

        if (frame < centerKey.frame) {
            ikeyR = ikeyCenter;
        } else {
            ikeyL = ikeyCenter;
        }
    }

    if (RIsSame(frame, pKeys[ikeyR].frame)) {
        return pKeys[ikeyR].value;
    }

    return pKeys[ikeyL].value;
}

void AnimatePaneSrt(Pane* pPane, const ResAnimationInfo* pAnimInfo,
                    const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();

        pPane->SetSrtElement(pAnimTarget->target,
                             GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount));
    }
}

void AnimateVisibility(Pane* pPane, const ResAnimationInfo* pAnimInfo,
                       const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResStepKey>();

        pPane->SetVisible(GetStepCurveValue(frame, pKeys, pAnimTarget->keyCount));
    }
}

void AnimateVertexColor(Pane* pPane, const ResAnimationInfo* pAnimInfo,
                        const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();

        float value = GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount);
        uint8_t u8Val = std::min(std::max(value + 0.5f, 0.0f), 255.0f);
        pPane->SetColorElement(pAnimTarget->target, u8Val);
    }
}

void AnimatePerCharacterTransform(Pane* pPane, const ResAnimationInfo* pAnimInfo,
                                  const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();

        float value = GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount);
        static_cast<TextBox*>(pPane)->SetPerCharacterTransformOffset(value);
        // smo: static_cast<TextBox*>(pPane)->SetPerCharacterTransform(pAnimTarget->target, value);
    }
}

/* added in smo
void AnimateWindow(Pane* pPane, const ResAnimationInfo* pAnimInfo,
                   const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();

        float value = GetHermiteCurveValue(frame, pKeys, pAnimTarget->keysOffset);
        static_cast<Window*>(pPane)->SetWindowFrameSize(pAnimTarget->target, value);
    }
}

added after smo
void AnimateMaskTexSrt(Pane* pPane, const ResAnimationInfo* pAnimInfo,
                       const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();
    }
}
*/

void AnimateMaterialColor(Material* pMaterial, const ResAnimationInfo* pAnimInfo,
                          const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();
        float value = GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount);

        if (pAnimTarget->target <= AnimTargetMatColor_MaxAnimTargetMatColor) {
            value += 0.5f;
            uint8_t val = std::min(std::max(value, 0.0f), 255.0f);
            pMaterial->SetColorElement(pAnimTarget->target, val);
        } else {
            pMaterial->SetColorElementFloat(pAnimTarget->target, value);
        }
    }
}

void AnimateTextureSrt(Material* pMaterial, const ResAnimationInfo* pAnimInfo,
                       const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();

        pMaterial->SetTexSrtElement(pAnimTarget->id, pAnimTarget->target,
                                    GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount));
    }
}

void AnimateTexturePattern(Material* pMaterial, const ResAnimationInfo* pAnimInfo,
                           const uint32_t* pAnimTargetOffsets, float frame,
                           const TextureInfo** texInfos) {
    for (int j = 0; j < pAnimInfo->count; ++j) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[j]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResStepKey>();
        const uint16_t fileIdx = GetStepCurveValue(frame, pKeys, pAnimTarget->keyCount);

        if (texInfos[fileIdx]->IsValid())
            pMaterial->SetTexMap(pAnimTarget->id, texInfos[fileIdx]);
    }
}

void AnimateIndirectSrt(Material* pMaterial, const ResAnimationInfo* pAnimInfo,
                        const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();

        pMaterial->SetIndirectSrtElement(pAnimTarget->target,
                                         GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount));
    }
}

void AnimateAlphaCompare(Material* pMaterial, const ResAnimationInfo* pAnimInfo,
                         const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();
        float value = GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount);
        float val = std::min(std::max(value, 0.0f), 1.0f);

        ResAlphaCompare compare(pMaterial->GetAlphaCompare().GetFunc(), val);
        pMaterial->SetAlphaCompare(compare);
    }
}

void AnimateFontShadow(Material* pMaterial, const ResAnimationInfo* pAnimInfo,
                       const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();
        float value = GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount);

        uint8_t value8 = std::min(std::max(value + 0.5f, 0.0f), 255.0f);
        pMaterial->SetFontShadowParameterElement(pAnimTarget->target, value8);
    }
}

void AnimateExtUserData(ResExtUserData* pExtUserData, const ResAnimationInfo* pAnimInfo,
                        const uint32_t* pAnimTargetOffsets, float frame) {
    for (int i = 0; i < pAnimInfo->count; ++i) {
        auto pAnimTarget =
            util::ConstBytePtr(pAnimInfo, pAnimTargetOffsets[i]).Get<ResAnimationTarget>();
        auto pKeys = util::ConstBytePtr(pAnimTarget, pAnimTarget->keysOffset).Get<ResHermiteKey>();

        switch (pExtUserData->GetType()) {
        case ExtUserDataType_Int:
            pExtUserData->WriteIntValue(GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount),
                                        pAnimTarget->id);
            break;
        case ExtUserDataType_Float:
            pExtUserData->WriteFloatValue(GetHermiteCurveValue(frame, pKeys, pAnimTarget->keyCount),
                                          pAnimTarget->id);
            break;
        default:
            break;
        }
    }
}

// 427
const uint32_t* GetAnimInfoOffsets(const ResAnimationContent& animContent) {
    if (animContent.type == AnimContentType_ExtUserData) {
        const uint32_t* tableOffsets =
            util::ConstBytePtr(&animContent, sizeof(ResAnimationContent)).Get<uint32_t>();
        return util::ConstBytePtr(&animContent, tableOffsets[0]).Get<uint32_t>();
    }

    return util::ConstBytePtr(&animContent, sizeof(ResAnimationContent)).Get<uint32_t>();
}

// 443
const char* GetExtUserDataTargetName(const ResAnimationContent& animContent) {
    if (animContent.type == AnimContentType_ExtUserData) {
        const uint32_t* tableOffsets =
            util::ConstBytePtr(&animContent, sizeof(ResAnimationContent)).Get<uint32_t>();
        const uint32_t* fileNameOffsets =
            util::ConstBytePtr(&animContent, tableOffsets[1]).Get<uint32_t>();
        return detail::GetStrTableStr(fileNameOffsets, 0);
    }

    return nullptr;
}

}  // namespace

AnimTransform::AnimTransform() : m_pRes(nullptr), m_Frame(0.0f), m_IsEnabled(true) {}

AnimTransform::~AnimTransform() {}

uint16_t AnimTransform::GetFrameSize() const {
    return m_pRes->frameSize;
}

void AnimTransform::UpdateFrame(float progressFrame) {
    NN_UNUSED(progressFrame);
}

void AnimTransform::SetEnabled(bool bEnable) {
    m_IsEnabled = bEnable;
}

bool AnimTransform::IsLoopData() const {
    return m_pRes->loop;
}

bool AnimTransform::IsWaitData() const {
    return m_pRes->frameSize == 0;
}

AnimTransformBasic::AnimTransformBasic()
    : AnimTransform(), m_pTextures(nullptr), m_pBindPairs(nullptr), m_BindPairCount(0),
      m_BindPairCountMax(0) {}

AnimTransformBasic::~AnimTransformBasic() {
    Layout::DeletePrimArray(m_pBindPairs);
    Layout::DeletePrimArray(m_pTextures);
}

void AnimTransformBasic::SetResource(gfx::Device* pDevice, ResourceAccessor* pResAccessor,
                                     const ResAnimationBlock* pRes) {
    SetResource(pDevice, pResAccessor, pRes, pRes->animContCount);
}

void AnimTransformBasic::SetResource(gfx::Device* pDevice, ResourceAccessor* pResAccessor,
                                     const ResAnimationBlock* pRes, uint16_t animNum) {
    SetAnimResource(pRes);
    m_pTextures = nullptr;

    if (pRes->fileCount) {
        m_pTextures = Layout::NewArray<const TextureInfo*>(pRes->fileCount);
        if (m_pTextures) {
            auto fileNameOffsets =
                util::ConstBytePtr(pRes, sizeof(ResAnimationBlock)).Get<uint32_t>();
            for (int i = 0; i < pRes->fileCount; ++i) {
                const char* const fileName = detail::GetStrTableStr(fileNameOffsets, i);
                m_pTextures[i] = pResAccessor->AcquireTexture(pDevice, fileName);
            }
        }
    }

    m_pBindPairs = Layout::NewArray<BindPair>(animNum);
    if (m_pBindPairs) {
        m_BindPairCountMax = animNum;
    }
}

void AnimTransformBasic::BindPane(Pane* pPane, bool bRecursive) {
    auto pRes = GetAnimResource();
    const uint32_t* const pAnimContentOffsets =
        util::ConstBytePtr(pRes, pRes->animContOffsetsOffset).Get<uint32_t>();

    for (uint16_t i = 0; i < pRes->animContCount; ++i) {
        auto& animContent =
            *util::ConstBytePtr(pRes, pAnimContentOffsets[i]).Get<ResAnimationContent>();

        switch (animContent.type) {
        case AnimContentType_Pane: {
            Pane* const pFindPane = pPane->FindPaneByName(animContent.name, bRecursive);
            if (pFindPane) {
                // CheckBindAnimationDoubly();
                BindPaneImpl(pFindPane, &animContent);
            }
        } break;

        case AnimContentType_Material: {
            Material* const pFindMat = pPane->FindMaterialByName(animContent.name, bRecursive);
            if (pFindMat) {
                // CheckBindAnimationDoubly();
                BindMaterialImpl(pFindMat, &animContent);
            }
        } break;

        case AnimContentType_ExtUserData: {
            Pane* const pFindPane = pPane->FindPaneByName(animContent.name, bRecursive);
            if (pFindPane) {
                BindExtUserDataToPane(pFindPane, animContent);
            }
        } break;

        default:
            break;
        }
    }
}

void AnimTransformBasic::BindGroup(Group* pGroup) {
    auto pRes = GetAnimResource();
    const uint32_t* const pAnimContentOffsets =
        util::ConstBytePtr(pRes, pRes->animContOffsetsOffset).Get<uint32_t>();

    for (uint16_t i = 0; i < pRes->animContCount; ++i) {
        auto& animContent =
            *util::ConstBytePtr(pRes, pAnimContentOffsets[i]).Get<ResAnimationContent>();

        switch (animContent.type) {
        case AnimContentType_Pane: {
            auto paneEnd = pGroup->GetPaneList().end();
            Pane* targetPane = nullptr;
            for (auto iter = pGroup->GetPaneList().begin(); iter != paneEnd; ++iter) {
                if (detail::EqualsResName(animContent.name, iter->pTarget->GetName())) {
                    targetPane = iter->pTarget;
                    break;
                }
            }

            if (targetPane) {
                // CheckBindAnimationDoubly
                BindPaneImpl(targetPane, &animContent);
            }
        } break;

        case AnimContentType_Material: {
            auto& paneList = pGroup->GetPaneList();
            auto paneEnd = paneList.end();
            Material* pFindMat = nullptr;
            for (auto iter = paneList.begin(); iter != paneEnd; ++iter) {
                pFindMat = iter->pTarget->FindMaterialByName(animContent.name, false);
                if (pFindMat) {
                    BindMaterialImpl(pFindMat, &animContent);
                    break;
                }
            }
        } break;

        case AnimContentType_ExtUserData: {
            auto paneEnd = pGroup->GetPaneList().end();
            Pane* targetPane = nullptr;
            for (auto iter = pGroup->GetPaneList().begin(); iter != paneEnd; ++iter) {
                if (detail::EqualsResName(animContent.name, iter->pTarget->GetName())) {
                    targetPane = iter->pTarget;
                    break;
                }
            }

            if (targetPane) {
                // CheckBindAnimationDoubly
                BindExtUserDataToPane(targetPane, animContent);
            }
        } break;

        default:
            break;
        }
    }
}

void AnimTransformBasic::BindMaterial(Material* pMaterial) {
    auto pRes = GetAnimResource();
    const uint32_t* const pAnimContentOffsets =
        util::ConstBytePtr(pRes, pRes->animContOffsetsOffset).Get<uint32_t>();

    for (uint32_t i = 0; i < pRes->animContCount; ++i) {
        auto& animContent =
            *util::ConstBytePtr(pRes, pAnimContentOffsets[i]).Get<ResAnimationContent>();

        if (animContent.type == AnimContentType_Material &&
            detail::EqualsMaterialName(pMaterial->GetName(), animContent.name)) {
            // CheckBindAnimationDoubly()
            if (!BindMaterialImpl(pMaterial, &animContent)) {
                return;
            }
        }
    }
}

void AnimTransformBasic::ForceBindPane(Pane* pDstPane, const Pane* pSrcPane) {
    auto pRes = GetAnimResource();
    const uint32_t* const pAnimContentOffsets =
        util::ConstBytePtr(pRes, pRes->animContOffsetsOffset).Get<uint32_t>();

    for (uint16_t i = 0; i < pRes->animContCount; ++i) {
        auto& animContent =
            *util::ConstBytePtr(pRes, pAnimContentOffsets[i]).Get<ResAnimationContent>();

        switch (animContent.type) {
        case AnimContentType_Pane:
            if (detail::EqualsResName(pSrcPane->GetName(), animContent.name)) {
                // CheckBindAnimationDoubly
                if (!BindPaneImpl(pDstPane, &animContent)) {
                    return;
                }
            }
            break;

        case AnimContentType_Material: {
            uint32_t nbMaterial = pSrcPane->GetMaterialCount();
            for (uint32_t idx = 0; idx < nbMaterial; ++idx) {
                auto pSrcMaterial = pSrcPane->GetMaterial(idx);
                if (pSrcMaterial &&
                    detail::EqualsMaterialName(pSrcMaterial->GetName(), animContent.name)) {
                    Material* pDstMaterial = pDstPane->GetMaterial(idx);
                    if (pDstMaterial && !BindMaterialImpl(pDstMaterial, &animContent)) {
                        return;
                    }
                }
            }
        } break;

        case AnimContentType_ExtUserData:
            if (detail::EqualsResName(pSrcPane->GetName(), animContent.name)) {
                if (!BindExtUserDataToPane(pDstPane, animContent)) {
                    return;
                }
            }
            break;

        default:
            break;
        }
    }
}

bool AnimTransformBasic::BindExtUserDataToPane(Pane* pPane,
                                               const ResAnimationContent& animContent) {
    if (pPane) {
        const char* const pExtUserDataName = GetExtUserDataTargetName(animContent);
        auto pUserData = pPane->FindExtUserDataByName(pExtUserDataName);
        if (pUserData) {
            // CheckBindAnimationDoubly();
            return BindExtUserDataImpl(const_cast<ResExtUserData*>(pUserData), &animContent);
        }
    }

    return true;
}

void AnimTransformBasic::UnbindPane(const Pane* pPane) {
    int end = m_BindPairCount;
    for (int i = 0; i < end; ++i) {
        BindPair* pPair = &m_pBindPairs[i];

        if (pPair->target == pPane) {
            EraseBindPair(i);
            break;
        }

        uint32_t nbExtUserData = pPane->GetExtUserDataCount();
        for (uint32_t extUserDataIndex = 0; extUserDataIndex < nbExtUserData; ++extUserDataIndex) {
            const ResExtUserData* pUserData = &(pPane->GetExtUserDataArray()[extUserDataIndex]);
            if (pPair->target == pUserData) {
                EraseBindPair(i);
                break;
            }
        }
    }

    uint32_t nbMaterial = pPane->GetMaterialCount();
    for (uint32_t idx = 0; idx < nbMaterial; ++idx) {
        Material* pMaterial = pPane->GetMaterial(idx);
        if (pMaterial) {
            UnbindMaterial(pMaterial);
        }
    }
}

void AnimTransformBasic::UnbindGroup(const Group* pGroup) {
    PaneLinkList& paneList = const_cast<Group*>(pGroup)->GetPaneList();
    auto endIter = paneList.end();
    for (auto it = paneList.begin(); it != endIter; ++it) {
        UnbindPane(it->pTarget);
    }
}

void AnimTransformBasic::UnbindMaterial(const Material* pMaterial) {
    int end = m_BindPairCount;
    for (int i = 0; i < end; ++i) {
        BindPair* pPair = &m_pBindPairs[i];
        if (pPair->target == pMaterial) {
            EraseBindPair(i);
            break;
        }
    }
}

void AnimTransformBasic::UnbindAll() {
    m_BindPairCount = 0;
}

bool AnimTransformBasic::BindPaneImpl(Pane* pTarget, const ResAnimationContent* pAnimContent) {
    if (m_BindPairCount >= m_BindPairCountMax) {
        return false;
    }

    BindPair* pPair = &m_pBindPairs[m_BindPairCount++];
    pPair->target = pTarget;
    pPair->animCont = pAnimContent;
    return true;
}

#define IFTEST(statement, ...)                                                                     \
    if (statement) {                                                                               \
        isBinding = false;                                                                         \
        __VA_ARGS__                                                                                \
    }

bool AnimTransformBasic::BindMaterialImpl(Material* pTarget,
                                          const ResAnimationContent* pAnimContent) {
    if (m_BindPairCount >= m_BindPairCountMax) {
        return false;
    }

    const uint32_t* animInfoOffsets = GetAnimInfoOffsets(*pAnimContent);

    bool isBinding = true;
    if (pAnimContent) {
        for (int j = 0; j < pAnimContent->count; ++j) {
            const ResAnimationInfo* animInfo =
                util::ConstBytePtr(pAnimContent, animInfoOffsets[j]).Get<ResAnimationInfo>();
            const uint32_t* pAnimTargetOffsets =
                util::ConstBytePtr(animInfo, sizeof(ResAnimationInfo)).Get<uint32_t>();

            for (int k = 0; k < animInfo->count; ++k) {
                const ResAnimationTarget* pAnimTarget =
                    util::ConstBytePtr(animInfo, pAnimTargetOffsets[k]).Get<ResAnimationTarget>();

                switch (animInfo->kind) {
                case FLTS:
                    IFTEST(pAnimTarget->id >= TexMapMax)
                    IFTEST(pTarget->GetTexSrtCap() <= pAnimTarget->id)
                    IFTEST(pAnimTarget->target >= AnimTargetTexSrt_MaxAnimTargetTexSrt)
                    IFTEST(pAnimTarget->curveType != AnimCurve_Hermite)
                    break;

                case FLTP: {
                    IFTEST(!m_pTextures)
                    IFTEST(pTarget->GetTexMapCount() <= pAnimTarget->id)
                    IFTEST(pAnimTarget->curveType != AnimCurve_Step)
                    IFTEST(pAnimTarget->target >= AnimTargetTexPattern_MaxAnimTargetTexPattern)

                    for (int fileIdx = 0; fileIdx < GetAnimResource()->fileCount; ++fileIdx) {
                        IFTEST(!m_pTextures[fileIdx]->IsValid(), break;)
                    }
                } break;

                case FLFS:
                    IFTEST(!pTarget->IsFontShadowParameterCap())
                    IFTEST(AnimTargetFontShadow_MaxAnimTargetFontShadow <= pAnimTarget->target)
                    IFTEST(pAnimTarget->curveType != AnimCurve_Hermite)
                    break;

                case FLIM:
                    IFTEST(!pTarget->IsIndirectParameterCap())
                    IFTEST(pAnimTarget->target >= AnimTargetIndirectSrt_MaxAnimTargetIndirectSrt)
                    IFTEST(pAnimTarget->curveType != AnimCurve_Hermite)
                    break;

                case FLAC:
                    IFTEST(!pTarget->IsAlphaCompareCap())
                    IFTEST(pAnimTarget->target >= AnimTargetAlphaCompare_MaxAnimTargetAlphaCompare)
                    IFTEST(pAnimTarget->curveType != AnimCurve_Hermite)
                    break;

                case FLMC:
                    IFTEST(pAnimTarget->target >= AnimTargetMatColorFloat_MaxAnimTargetMatColor)
                    IFTEST(pAnimTarget->curveType != AnimCurve_Hermite)
                    break;

                default:
                    break;
                }
            }
        }
    }

    if (isBinding) {
        BindPair* pPair = &m_pBindPairs[m_BindPairCount];
        m_BindPairCount++;
        pPair->target = pTarget;
        pPair->animCont = pAnimContent;
    }

    return true;
}

bool AnimTransformBasic::BindExtUserDataImpl(ResExtUserData* pTarget,
                                             const ResAnimationContent* pAnimContent) {
    if (m_BindPairCount >= m_BindPairCountMax) {
        return false;
    }

    BindPair* pPair = &m_pBindPairs[m_BindPairCount++];
    pPair->target = pTarget;
    pPair->animCont = pAnimContent;
    return true;
}

void AnimTransformBasic::Animate() {
    if (IsEnabled()) {
        int end = m_BindPairCount;
        for (int i = 0; i < end; ++i) {
            BindPair* pPair = &m_pBindPairs[i];

            switch (pPair->animCont->type) {
            case AnimContentType_Pane:
                AnimatePaneImpl(static_cast<Pane*>(pPair->target), pPair->animCont);
                break;
            case AnimContentType_Material:
                AnimateMaterialImpl(static_cast<Material*>(pPair->target), pPair->animCont);
                break;
            case AnimContentType_ExtUserData:
                AnimateExtUserDataImpl(static_cast<ResExtUserData*>(pPair->target),
                                       pPair->animCont);
                break;
            }
        }
    }
}

void AnimTransformBasic::AnimatePane(Pane* pPane) {
    if (IsEnabled()) {
        int end = m_BindPairCount;
        for (int i = 0; i < end; ++i) {
            BindPair* pPair = &m_pBindPairs[i];

            if (pPair->target == pPane) {
                AnimatePaneImpl(pPane, pPair->animCont);

                uint32_t nbMaterial = pPane->GetMaterialCount();
                for (uint32_t idx = 0; idx < nbMaterial; ++idx) {
                    Material* pMaterial = pPane->GetMaterial(idx);
                    if (pMaterial) {
                        AnimateMaterial(pMaterial);
                    }
                }
                break;
            }
        }
    }
}

void AnimTransformBasic::AnimateMaterial(Material* pMaterial) {
    if (IsEnabled()) {
        int end = m_BindPairCount;
        for (int i = 0; i < end; ++i) {
            BindPair* pPair = &m_pBindPairs[i];

            if (pPair->target == pMaterial) {
                AnimateMaterialImpl(pMaterial, pPair->animCont);
                break;
            }
        }
    }
}

void AnimTransformBasic::AnimatePaneImpl(Pane* pPane, const ResAnimationContent* pAnimContent) {
    const uint32_t* animInfoOffsets = GetAnimInfoOffsets(*pAnimContent);

    for (int i = 0; i < pAnimContent->count; ++i) {
        const ResAnimationInfo* pAnimInfo =
            util::ConstBytePtr(pAnimContent, animInfoOffsets[i]).Get<ResAnimationInfo>();
        const uint32_t* pAnimTargetOffsets =
            util::ConstBytePtr(pAnimInfo, sizeof(ResAnimationInfo)).Get<uint32_t>();

        switch (pAnimInfo->kind) {
        case FLPA:
            AnimatePaneSrt(pPane, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

        case FLVI:
            AnimateVisibility(pPane, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

        case FLVC:
            AnimateVertexColor(pPane, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

        case FLCT:
            AnimatePerCharacterTransform(pPane, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

            /* added in smo
            case FLWN:
                AnimateWindow(pPane, pAnimInfo, pAnimTargetOffsets, GetFrame());
                break;
            */

            /* added after smo
            case FL:
                AnimateMaskTexSrt(pPane, pAnimInfo, pAnimTargetOffsets, GetFrame());
                break;
            */

        default:
            break;
        }
    }
}

void AnimTransformBasic::AnimateMaterialImpl(Material* pMaterial,
                                             const ResAnimationContent* pAnimContent) {
    const uint32_t* animInfoOffsets = GetAnimInfoOffsets(*pAnimContent);

    for (int i = 0; i < pAnimContent->count; ++i) {
        const ResAnimationInfo* pAnimInfo =
            util::ConstBytePtr(pAnimContent, animInfoOffsets[i]).Get<ResAnimationInfo>();
        const uint32_t* pAnimTargetOffsets =
            util::ConstBytePtr(pAnimInfo, sizeof(ResAnimationInfo)).Get<uint32_t>();

        switch (pAnimInfo->kind) {
        case FLMC:
            AnimateMaterialColor(pMaterial, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

        case FLTS:
            AnimateTextureSrt(pMaterial, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

        case FLTP:
            AnimateTexturePattern(pMaterial, pAnimInfo, pAnimTargetOffsets, GetFrame(),
                                  m_pTextures);
            break;

        case FLIM:
            AnimateIndirectSrt(pMaterial, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

        case FLAC:
            AnimateAlphaCompare(pMaterial, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

        case FLFS:
            AnimateFontShadow(pMaterial, pAnimInfo, pAnimTargetOffsets, GetFrame());
            break;

        default:
            break;
        }
    }
}

void AnimTransformBasic::AnimateExtUserDataImpl(ResExtUserData* pExtUserData,
                                                const ResAnimationContent* pAnimContent) {
    const uint32_t* animInfoOffsets = GetAnimInfoOffsets(*pAnimContent);

    for (int i = 0; i < pAnimContent->count; ++i) {
        const ResAnimationInfo* pAnimInfo =
            util::ConstBytePtr(pAnimContent, animInfoOffsets[i]).Get<ResAnimationInfo>();
        const uint32_t* pAnimTargetOffsets =
            util::ConstBytePtr(pAnimInfo, sizeof(ResAnimationInfo)).Get<uint32_t>();

        AnimateExtUserData(pExtUserData, pAnimInfo, pAnimTargetOffsets, GetFrame());
    }
}

bool AnimTransformBasic::CheckBindAnimationDoubly(const void* target,
                                                  const ResAnimationContent* pAnimContent) const {
    NN_UNUSED(target);
    NN_UNUSED(pAnimContent);
    // loop here, optimized out?
    return false;
}

void AnimTransformBasic::EraseBindPair(int pos) {
    if (pos + 1 < m_BindPairCount) {
        m_pBindPairs[pos] = m_pBindPairs[m_BindPairCount - 1];
    }
    m_BindPairCount -= 1;
}

AnimResource::AnimResource() {
    Initialize();
}

bool AnimResource::CheckResource() const {
    return m_pResBlock;
}

void AnimResource::Set(const void* pAnimResBuf) {
    Initialize();

    auto pFileHeader = static_cast<const font::detail::BinaryFileHeader* const>(pAnimResBuf);
    font::detail::IsValidBinaryFile(pFileHeader, FileSignatureFlan, 0, 0);
    m_pFileHeader = pFileHeader;

    auto pDataBlockHead = util::ConstBytePtr(pFileHeader, pFileHeader->headerSize)
                              .Get<font::detail::BinaryBlockHeader>();
    for (int i = 0; i < m_pFileHeader->dataBlocks; ++i) {
        uint32_t kind = pDataBlockHead->kind;

        switch (kind) {
        case DataBlockKindAnimTag:
            m_pTagBlock = reinterpret_cast<const ResAnimationTagBlock*>(pDataBlockHead);
            break;

        case DataBlockKindAnimShare:
            m_pShareBlock = reinterpret_cast<const ResAnimationShareBlock*>(pDataBlockHead);
            break;

        case DataBlockKindAnim:
            m_pResBlock = reinterpret_cast<const ResAnimationBlock*>(pDataBlockHead);
            break;

        default:
            break;
        }

        pDataBlockHead = util::ConstBytePtr(pDataBlockHead, pDataBlockHead->size)
                             .Get<font::detail::BinaryBlockHeader>();
    }
}

void AnimResource::Initialize() {
    m_pTagBlock = nullptr;
    m_pShareBlock = nullptr;
    m_pFileHeader = nullptr;
    m_pResBlock = nullptr;
}

uint16_t AnimResource::GetTagOrder() const {
    if (!m_pTagBlock) {
        return -1;
    }

    return m_pTagBlock->tagOrder;
}

const char* AnimResource::GetTagName() const {
    if (!m_pTagBlock) {
        return nullptr;
    }

    return util::ConstBytePtr(m_pTagBlock, m_pTagBlock->nameOffset).Get<char>();
}

uint16_t AnimResource::GetGroupCount() const {
    if (!m_pTagBlock) {
        return 0;
    }

    return m_pTagBlock->groupCount;
}

const ResAnimationGroupRef* AnimResource::GetGroupArray() const {
    if (!m_pTagBlock) {
        return nullptr;
    }

    const ResAnimationGroupRef* const groups =
        util::ConstBytePtr(m_pTagBlock, m_pTagBlock->groupsOffset).Get<ResAnimationGroupRef>();
    return groups;
}

const ResExtUserDataList* AnimResource::GetExtUserDataList() const {
    if (!m_pTagBlock) {
        return nullptr;
    }

    if (m_pTagBlock->userDataListOffset == 0) {
        return nullptr;
    }

    const ResExtUserDataList* const list =
        util::ConstBytePtr(m_pTagBlock, m_pTagBlock->userDataListOffset).Get<ResExtUserDataList>();
    return list;
}

bool AnimResource::IsDescendingBind() const {
    if (!m_pTagBlock) {
        return false;
    }

    return detail::TestBit(m_pTagBlock->flag, AnimTagFlag_DescendingBind);
}

uint16_t AnimResource::GetAnimationShareInfoCount() const {
    if (!m_pShareBlock) {
        return 0;
    }

    return m_pShareBlock->shareCount;
}

const ResAnimationShareInfo* AnimResource::GetAnimationShareInfoArray() const {
    if (!m_pShareBlock) {
        return nullptr;
    }

    return util::ConstBytePtr(m_pShareBlock, m_pShareBlock->animShareInfoOffset)
        .Get<ResAnimationShareInfo>();
}

uint16_t AnimResource::CalculateAnimationCount(Pane* pPane, bool bRecursive) const {
    if (!CheckResource()) {
        return 0;
    }

    uint16_t linkNum = 0;

    const uint32_t* const pAnimContentOffsets =
        util::ConstBytePtr(m_pResBlock, m_pResBlock->animContOffsetsOffset).Get<uint32_t>();

    for (uint16_t i = 0; i < m_pResBlock->animContCount; ++i) {
        const ResAnimationContent& animContent =
            *util::ConstBytePtr(m_pResBlock, pAnimContentOffsets[i]).Get<ResAnimationContent>();

        if (animContent.type == AnimContentType_Pane) {
            if (pPane->FindPaneByName(animContent.name, bRecursive))
                ++linkNum;
        } else {
            if (pPane->FindMaterialByName(animContent.name, bRecursive))
                ++linkNum;
        }
    }

    return linkNum;
}

uint16_t AnimResource::CalculateAnimationCount(Material* pMaterial) const {
    if (!CheckResource()) {
        return 0;
    }

    uint16_t linkNum = 0;

    auto const pAnimContentOffsets =
        util::ConstBytePtr(m_pResBlock, m_pResBlock->animContOffsetsOffset).Get<uint32_t>();

    for (uint16_t i = 0; i < m_pResBlock->animContCount; ++i) {
        auto& animContent =
            *util::ConstBytePtr(m_pResBlock, pAnimContentOffsets[i]).Get<ResAnimationContent>();

        if (animContent.type == AnimContentType_Material) {
            if (detail::EqualsMaterialName(pMaterial->GetName(), animContent.name)) {
                linkNum = 1;  // bug?
            }
        }
    }

    return linkNum;
}

uint16_t AnimResource::CalculateAnimationCount(Group* pGroup, bool bRecursive) const {
    uint16_t linkNum = 0;
    auto& paneList = pGroup->GetPaneList();

    auto enditer = paneList.end();
    for (auto iter = paneList.begin(); iter != enditer; ++iter) {
        linkNum += CalculateAnimationCount(iter->pTarget, bRecursive);
    }

    return linkNum;
}

namespace detail {

AnimPaneTree::AnimPaneTree() {
    Initialize();
}

AnimPaneTree::AnimPaneTree(Pane* pTargetPane, const AnimResource& animRes) {
    Initialize();
    Set(pTargetPane, animRes);
}

const ResAnimationContent* AnimPaneTree::FindAnimContent(const ResAnimationBlock* pAnimBlock,
                                                         const char* pAnimContentName,
                                                         uint8_t animContentType) {
    const uint32_t* const pAnimContentOffsets =
        util::ConstBytePtr(pAnimBlock, pAnimBlock->animContOffsetsOffset).Get<uint32_t>();

    for (uint16_t i = 0; i < pAnimBlock->animContCount; ++i) {
        const ResAnimationContent* const pAnimCont =
            util::ConstBytePtr(pAnimBlock, pAnimContentOffsets[i]).Get<ResAnimationContent>();

        if (pAnimCont->type == animContentType &&
            EqualsMaterialName(pAnimCont->name, pAnimContentName)) {
            return pAnimCont;
        }
    }

    return nullptr;
}

void AnimPaneTree::Initialize() {
    m_pPaneAnimContent = nullptr;

    for (uint8_t i = 0; i < MaterialCountMax; ++i) {
        m_pMatAnimContents[i] = nullptr;
    }

    m_LinkCount = 0;
    m_AnimMatCnt = 0;
}

void AnimPaneTree::Set(Pane* pTargetPane, const AnimResource& animRes) {
    uint16_t linkNum = 0;
    const ResAnimationBlock* pAnimBlock = animRes.GetResourceBlock();
    const ResAnimationContent* pAnimContent =
        FindAnimContent(pAnimBlock, pTargetPane->GetName(), AnimContentType_Pane);

    if (pAnimContent) {
        ++linkNum;
    }

    const uint8_t animMatCnt = pTargetPane->GetMaterialCount();
    const ResAnimationContent* animMatIdxs[MaterialCountMax];

    for (uint8_t i = 0; i < animMatCnt; ++i) {
        animMatIdxs[i] = FindAnimContent(pAnimBlock, pTargetPane->GetMaterial(i)->GetName(),
                                         AnimContentType_Material);
        if (animMatIdxs[i]) {
            ++linkNum;
        }
    }

    if (linkNum == 0) {
        return;
    }

    m_AnimRes = animRes;
    m_pPaneAnimContent = pAnimContent;
    m_AnimMatCnt = animMatCnt;
    for (uint8_t i = 0; i < animMatCnt; ++i) {
        m_pMatAnimContents[i] = animMatIdxs[i];
    }
    m_LinkCount = linkNum;
}

AnimTransform* AnimPaneTree::Bind(gfx::Device* pDevice, Layout* pLayout, Pane* pTargetPane,
                                  ResourceAccessor* pResAccessor) const {
    AnimTransformBasic* const pAnimTrans = pLayout->CreateAnimTransformBasic();
    pAnimTrans->SetResource(pDevice, pResAccessor, m_AnimRes.GetResourceBlock(), m_LinkCount);

    if (m_pPaneAnimContent) {
        pAnimTrans->BindPaneImpl(pTargetPane, m_pPaneAnimContent);
    }

    const uint32_t animMatMax = std::min(m_AnimMatCnt, pTargetPane->GetMaterialCount());
    for (uint32_t i = 0; i < animMatMax; ++i) {
        if (m_pMatAnimContents[i]) {
            Material* const pMaterial = pTargetPane->GetMaterial(i);
            pAnimTrans->BindMaterialImpl(pMaterial, m_pMatAnimContents[i]);
        }
    }

    return pAnimTrans;
}

}  // namespace detail
}  // namespace ui2d
}  // namespace nn
