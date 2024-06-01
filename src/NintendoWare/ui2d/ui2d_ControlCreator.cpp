#include <nn/ui2d/ui2d_ControlCreator.h>

#include <nn/ui2d/ui2d_Resources.h>

namespace nn {
namespace ui2d {

ControlSrc::ControlSrc()
    : m_pControlName(nullptr), m_pControlUserName(nullptr), m_FunctionalPaneCount(0),
      m_FunctionalAnimCount(0), m_pFunctionalPaneNames(nullptr),
      m_pControlFunctionalAnimNameOffests(nullptr),
      m_pControlFunctionalPaneParameterNameOffsets(nullptr),
      m_pControlFunctionalAnimParameterNameOffsets(nullptr), m_pExtUserDataList(nullptr) {}

void ControlSrc::Initialize(const void* pData, const ResExtUserDataList* pUserDataBlock) {
    auto pResControl = static_cast<const ResControl*>(pData);
    m_pControlName = util::ConstBytePtr(pResControl, sizeof(ResControl)).Get<char>();
    m_pControlUserName =
        util::ConstBytePtr(pResControl, pResControl->controlUserNameOffset).Get<char>();
    m_FunctionalPaneCount = pResControl->controlFunctionalPaneCount;
    m_FunctionalAnimCount = pResControl->controlFunctionalAnimCount;
    m_pFunctionalPaneNames =
        util::ConstBytePtr(pResControl, pResControl->controlFunctionalPaneNamesOffset).Get<char>();
    m_pControlFunctionalAnimNameOffests =
        util::ConstBytePtr(pResControl, pResControl->controlFunctionalPaneNamesOffset +
                                            24 * m_FunctionalPaneCount)
            .Get();
    m_pControlFunctionalPaneParameterNameOffsets =
        util::ConstBytePtr(pResControl,
                           pResControl->controlFunctionalPaneParameterNameOffsetsOffset)
            .Get();
    m_pControlFunctionalAnimParameterNameOffsets =
        util::ConstBytePtr(pResControl,
                           pResControl->controlFunctionalAnimParameterNameOffsetsOffset)
            .Get();
    m_pExtUserDataList = pUserDataBlock;
}

const char* ControlSrc::GetFunctionalPaneName(int index) const {
    if (index < GetFunctionalPaneCount()) {
        return &m_pFunctionalPaneNames[24 * index];
    }

    return nullptr;
}

const char* ControlSrc::FindFunctionalPaneName(const char* pParameterName) const {
    for (uint32_t i = 0; i < m_FunctionalPaneCount; ++i) {
        const char* pName = util::ConstBytePtr(m_pControlFunctionalPaneParameterNameOffsets,
                                               static_cast<const uint32_t*>(
                                                   m_pControlFunctionalPaneParameterNameOffsets)[i])
                                .Get<char>();

        if (strcmp(pName, pParameterName) == 0) {
            return &m_pFunctionalPaneNames[24 * i];
        }
    }

    return nullptr;
}

const char* ControlSrc::GetFunctionalAnimName(int index) const {
    if (index < GetFunctionalAnimCount()) {
        return util::ConstBytePtr(
                   m_pControlFunctionalAnimNameOffests,
                   static_cast<const uint32_t*>(m_pControlFunctionalAnimNameOffests)[index])
            .Get<char>();
    }

    return nullptr;
}

const char* ControlSrc::FindFunctionalAnimName(const char* pParameterName) const {
    for (uint32_t i = 0; i < m_FunctionalAnimCount; ++i) {
        const char* pName = util::ConstBytePtr(m_pControlFunctionalAnimParameterNameOffsets,
                                               static_cast<const uint32_t*>(
                                                   m_pControlFunctionalAnimParameterNameOffsets)[i])
                                .Get<char>();

        if (strcmp(pName, pParameterName) == 0) {
            return util::ConstBytePtr(
                       m_pControlFunctionalAnimNameOffests,
                       static_cast<const uint32_t*>(m_pControlFunctionalAnimNameOffests)[i])
                .Get<char>();
        }
    }

    return nullptr;
}

uint16_t ControlSrc::GetExtUserDataCount() const {
    if (m_pExtUserDataList) {
        return m_pExtUserDataList->count;
    }

    return 0;
}

const ResExtUserData* ControlSrc::GetExtUserDataArray() const {
    if (m_pExtUserDataList) {
        return util::ConstBytePtr(m_pExtUserDataList, sizeof(ResExtUserDataList))
            .Get<ResExtUserData>();
    }

    return 0;
}

const ResExtUserData* ControlSrc::FindExtUserDataByName(const char* pName) const {
    const ResExtUserData* pExtUserData = GetExtUserDataArray();
    if (!pExtUserData) {
        return nullptr;
    }

    for (int i = 0; i < m_pExtUserDataList->count; ++i, ++pExtUserData) {
        if (strcmp(pName, pExtUserData->GetName()) == 0) {
            return pExtUserData;
        }
    }

    return nullptr;
}

}  // namespace ui2d
}  // namespace nn
