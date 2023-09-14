#include <nn/ui2d/ui2d_Group.h>

#include <nn/ui2d/ui2d_Common.h>
#include <nn/ui2d/ui2d_Layout.h>
#include <nn/ui2d/ui2d_Resources.h>

namespace nn {
namespace ui2d {

Group::Group() : m_pName(""), m_UserAllocated(false) {}

Group::Group(const char* pName) : m_pName(pName), m_UserAllocated(false) {}

Group::Group(const ResGroup* pResGroup, Pane* pRootPane)
    : m_pName(pResGroup->name), m_UserAllocated(false) {
    const char* const pPaneNameBase = util::ConstBytePtr(pResGroup, sizeof(ResGroup)).Get<char>();

    for (int i = 0; i < pResGroup->paneCount; ++i) {
        if (Pane* pFindPane =
                pRootPane->FindPaneByName(pPaneNameBase + i * ResourceNameStrMax, true)) {
            AppendPane(pFindPane);
        }
    }
}

Group::Group(const Group& group, Pane* pRootPane) : m_pName(group.m_pName), m_UserAllocated(false) {
    const PaneLinkList& list = group.m_PaneLinkList;
    if (pRootPane) {
        for (PaneLinkList::const_iterator iter = list.begin(), endIter = list.end();
             iter != endIter; ++iter) {
            AppendPane(pRootPane->FindPaneByName(iter->pTarget->GetName(), true));
        }
    } else {
        for (PaneLinkList::const_iterator iter = list.begin(), endIter = list.end();
             iter != endIter; ++iter) {
            AppendPane(iter->pTarget);
        }
    }
}

Group::~Group() {
    for (PaneLinkList::iterator iter = m_PaneLinkList.begin(); iter != m_PaneLinkList.end();) {
        PaneLinkList::iterator currIter = iter++;
        m_PaneLinkList.erase(currIter);
        Layout::DeleteObj(&*currIter);
    }
}

void Group::AppendPane(Pane* pPane) {
    if (PaneLink* pPaneLink = Layout::AllocateAndConstruct<PaneLink>()) {
        pPaneLink->pTarget = pPane;
        m_PaneLinkList.push_back(*pPaneLink);
    }
}

bool Group::CompareCopiedInstanceTest(const Group& target) const {
    NN_UNUSED(target);
    return true;
}

GroupContainer::~GroupContainer() {
    for (GroupList::iterator it = m_GroupList.begin(); it != m_GroupList.end();) {
        GroupList::iterator currIt = it++;
        m_GroupList.erase(currIt);
        if (!currIt->IsUserAllocated()) {
            Layout::DeleteObj(&*currIt);
        }
    }
}

void GroupContainer::AppendGroup(Group* pGroup) {
    m_GroupList.push_back(*pGroup);
}

Group* GroupContainer::FindGroupByName(const char* pFindName) {
    GroupList::iterator endIter = m_GroupList.end();
    for (GroupList::iterator iter = m_GroupList.begin(); iter != endIter; ++iter) {
        if (detail::EqualsGroupName(iter->GetName(), pFindName)) {
            return &*iter;
        }
    }

    return nullptr;
}

const Group* GroupContainer::FindGroupByName(const char* pFindName) const {
    return const_cast<GroupContainer*>(this)->FindGroupByName(pFindName);
}

}  // namespace ui2d
}  // namespace nn
