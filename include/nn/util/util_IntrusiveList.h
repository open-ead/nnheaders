#pragma once

#include <iterator>

namespace nn::util {
struct IntrusiveListNode {
    IntrusiveListNode* m_Prev;
    IntrusiveListNode* m_Next;
};

namespace detail {
class IntrusiveListImplementation {
public:
    using value_type = IntrusiveListNode;
    using reference = value_type&;
    using const_reference = const value_type&;

    class iterator {
    public:
        using value_type = value_type;
        using pointer = value_type*;
        using reference = value_type&;

    private:
        pointer m_Node;
    };

    class const_iterator {
    public:
        using value_type = value_type;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = int;

    private:
        pointer m_Node;
    };

private:
    IntrusiveListNode m_Root;
};
} // namespace nn::util::detail

template <typename T1, typename T2 = T1>
class IntrusiveListBaseNode : IntrusiveListNode {};

template <typename T1, typename T2 = T1>
class IntrusiveListBaseNodeTraits {
public:
    using BaseNodeType = IntrusiveListBaseNode<T1>;
};

template <typename T1, auto MemberNode, typename T2 = T1>
class IntrusiveListMemberNodeTraits {};

template <typename T, typename Traits>
class IntrusiveList {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;

    class iterator {
    public:
        using value_type = value_type;
        using reference = value_type&;
        using pointer = value_type*;

    private:
        detail::IntrusiveListImplementation::iterator m_Iterator;
    };
    using reverse_iterator = std::reverse_iterator<iterator>;

    class const_iterator {
    public:
        using value_type = value_type;
        using reference = value_type&;
        using pointer = value_type*;

    private:
        detail::IntrusiveListImplementation::const_iterator m_Iterator;
    };
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    detail::IntrusiveListImplementation m_Implementation;
};
} // namespace nn::util