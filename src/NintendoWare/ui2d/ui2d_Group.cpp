#include <nn/ui2d/ui2d_Group.h>

#include <nn/ui2d/ui2d_Layout.h>

namespace nn {
namespace ui2d {

GroupContainer::~GroupContainer() {
    for (GroupList::iterator it = m_GroupList.begin(); it != m_GroupList.end();) {
        GroupList::iterator currIt = it++;
        m_GroupList.erase(currIt);
        if (!currIt->IsUserAllocated()) {
            Layout::DeleteObj(&*currIt);
        }
    }
}

/*
void GroupContainer::AppendGroup(Group*);
Group* GroupContainer::FindGroupByName(const char*);
const Group* GroupContainer::FindGroupByName(const char*) const;
*/
}  // namespace ui2d
}  // namespace nn
