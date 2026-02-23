#pragma once

namespace nn::util {
struct IntrusiveListNode {
    IntrusiveListNode* m_Prev;
    IntrusiveListNode* m_Next;
};

namespace detail {
class IntrusiveListImplementation {
private:
    IntrusiveListNode m_Root;
};
} // namespace nn::util::detail

template <typename T1, typename T2 = T1>
class IntrusiveListBaseNode : IntrusiveListNode {};

template <typename T1, typename T2 = T1>
class IntrusiveListBaseNodeTraits {};

template <typename T1, auto MemberNode, typename T2 = T1>
class IntrusiveListMemberNodeTraits {};

template <typename T, typename Traits>
class IntrusiveList {
private:
    detail::IntrusiveListImplementation m_Implementation;
};
} // namespace nn::util