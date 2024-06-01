#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace ui2d {

struct ResGroup;
class Pane;

struct PaneLink {
    util::IntrusiveListNode link;
    Pane* pTarget;
};

typedef util::IntrusiveList<PaneLink,
                            util::IntrusiveListMemberNodeTraits<PaneLink, &PaneLink::link>>
    PaneLinkList;

class Group {
public:
    Group();
    Group(const char*);
    Group(const ResGroup*, Pane*);
    Group(const Group&, Pane*);
    virtual ~Group();

    const char* GetName() const { return m_pName; }
    PaneLinkList& GetPaneList() { return m_PaneLinkList; }

    void AppendPane(Pane*);

    bool IsUserAllocated() const { return m_UserAllocated != 0; }
    void SetUserAllocated();

    bool CompareCopiedInstanceTest(const Group&) const;

    util::IntrusiveListNode m_Link;

private:
    PaneLinkList m_PaneLinkList;
    const char* m_pName;
    uint8_t m_UserAllocated;
    uint8_t m_Padding[3];

    const Group& operator=(const Group&);
};

typedef util::IntrusiveList<Group, util::IntrusiveListMemberNodeTraits<Group, &Group::m_Link>>
    GroupList;

class GroupContainer {
    NN_NO_COPY(GroupContainer);

public:
    GroupContainer() = default;
    ~GroupContainer();
    void AppendGroup(Group*);

    GroupList& GetGroupList() { return m_GroupList; }

    Group* FindGroupByName(const char*);
    const Group* FindGroupByName(const char*) const;

private:
    GroupList m_GroupList;
};

}  // namespace ui2d
}  // namespace nn
