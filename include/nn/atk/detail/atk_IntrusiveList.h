#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail {

using IntrusiveListNode = util::IntrusiveListNode;

template <typename Element>
class IntrusiveList {
public:
    using ElementList =
        util::IntrusiveList<Element,
                            util::IntrusiveListMemberNodeTraits<Element, &Element::m_ElementLink>>;
    using Iterator = typename ElementList::iterator;
    using ConstIterator = typename ElementList::const_iterator;

    IntrusiveList() = default;
    ~IntrusiveList() = default;

    void PushFront(Element& element) { m_ListImpl.push_front(element); }

    void PushBack(Element& element) { m_ListImpl.push_back(element); }

    void PopFront() { m_ListImpl.pop_front(); }

    void PopBack() { m_ListImpl.pop_back(); }

    Iterator Begin() { return m_ListImpl.begin(); }

    ConstIterator Begin() const { return m_ListImpl.begin(); }

    Iterator End() { return m_ListImpl.end(); }

    ConstIterator End() const { return m_ListImpl.end(); }

    bool IsEmpty() const { return m_ListImpl.empty(); }

    void Remove(Element& element) {
        for (Iterator iterator{Begin()}; iterator != End(); ++iterator) {
            if (&*iterator == &element) {
                m_ListImpl.erase(iterator);
                return;
            }
        }
    }

    void Clear() { m_ListImpl.clear(); }

    int Count() const { return m_ListImpl.size(); }

private:
    ElementList m_ListImpl;
};

}  // namespace nn::atk::detail
