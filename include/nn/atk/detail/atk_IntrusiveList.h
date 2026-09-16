#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail {

using IntrusiveListNode = util::IntrusiveListNode;

template <typename Element>
class IntrusiveList {
public:
    using ElementList =
        util::IntrusiveList<Element, util::IntrusiveListMemberNodeTraits<Element, &Element::m_ElementLink>>;
    using Iterator = typename ElementList::iterator;
    using ConstIterator = typename ElementList::const_iterator;

private:
    ElementList m_ListImpl;
};

}  // namespace nn::atk::detail
