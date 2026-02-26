#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail {
using IntrusiveListNode = util::IntrusiveListNode;

template <typename T>
class IntrusiveList {
public:
    using ElementList = util::IntrusiveList<T, util::IntrusiveListMemberNodeTraits<T, &T::m_ElementLink>>;
    using Iterator = typename ElementList::iterator;
    using ConstIterator = typename ElementList::const_iterator;

private:
    ElementList m_ListImpl;
};
} // namespace nn::atk::detail