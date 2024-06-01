#pragma once

#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>

namespace nn {
namespace ui2d {

class ResExtUserData;
struct ResExtUserDataList;
class Layout;

class ControlSrc {
public:
    ControlSrc();

    ControlSrc(const void* pData, const ResExtUserDataList* pUserDataBlock) {
        Initialize(pData, pUserDataBlock);
    }

    void Initialize(const void*, const ResExtUserDataList*);
    const char* GetControlName() const;
    const char* GetControlUserName() const;

    int GetFunctionalPaneCount() const { return m_FunctionalPaneCount; }
    int GetFunctionalAnimCount() const { return m_FunctionalAnimCount; }

    const char* GetFunctionalAnimParameterName(int) const;
    const char* GetFunctionalPaneName(int) const;
    const char* FindFunctionalPaneName(const char*) const;
    const char* GetFunctionalAnimName(int) const;
    const char* FindFunctionalAnimName(const char*) const;
    uint16_t GetExtUserDataCount() const;
    const ResExtUserData* GetExtUserDataArray() const;
    const ResExtUserData* FindExtUserDataByName(const char*) const;

private:
    const char* m_pControlName;
    const char* m_pControlUserName;
    uint16_t m_FunctionalPaneCount;
    uint16_t m_FunctionalAnimCount;
    const char* m_pFunctionalPaneNames;
    const void* m_pControlFunctionalAnimNameOffests;
    const void* m_pControlFunctionalPaneParameterNameOffsets;
    const void* m_pControlFunctionalAnimParameterNameOffsets;
    const ResExtUserDataList* m_pExtUserDataList;
};

class ControlCreator {
public:
    virtual ~ControlCreator() = default;

    virtual void CreateControl(gfx::Device*, Layout*, const ControlSrc&) = 0;
};

}  // namespace ui2d
}  // namespace nn
