#pragma once

#include <iterator>
#include <nn/util.h>

namespace nn::util {

namespace detail {

class IntrusiveListImplementation;

}

class IntrusiveListNode {
    NN_NO_COPY(IntrusiveListNode);

public:
    IntrusiveListNode() : m_Prev(this), m_Next(this) {}

    bool IsLinked() const { return m_Next != this; }

private:
    friend class detail::IntrusiveListImplementation;

    IntrusiveListNode* GetPrev() { return m_Prev; }
    const IntrusiveListNode* GetPrev() const { return m_Prev; }
    IntrusiveListNode* GetNext() { return m_Next; }
    const IntrusiveListNode* GetNext() const { return m_Next; }

    void LinkPrev(IntrusiveListNode* node) { LinkPrev(node, node); }

    void LinkPrev(IntrusiveListNode* first, IntrusiveListNode* last) {
        IntrusiveListNode* node = last->m_Prev;
        first->m_Prev = m_Prev;
        node->m_Next = this;
        m_Prev->m_Next = first;
        m_Prev = node;
    }

    void LinkNext(IntrusiveListNode*);
    void LinkNext(IntrusiveListNode*, IntrusiveListNode*);

    void Unlink() { Unlink(m_Next); }

    void Unlink(IntrusiveListNode* last) {
        IntrusiveListNode* node = last->m_Prev;
        m_Prev->m_Next = last;
        last->m_Prev = m_Prev;
        node->m_Next = this;
        m_Prev = node;
    }

    IntrusiveListNode* m_Prev;
    IntrusiveListNode* m_Next;
};

namespace detail {

class IntrusiveListImplementation {
    NN_NO_COPY(IntrusiveListImplementation);

public:
    typedef IntrusiveListNode value_type;
    typedef int difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef int size_type;

    class const_iterator {
    public:
        typedef const value_type value_type;
        typedef difference_type difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;

        const_iterator(pointer p) : m_Node(p) {}

        reference operator*() const { return *m_Node; }

        pointer operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
        bool operator==(const const_iterator&) const;
        bool operator!=(const const_iterator& ci) const { return m_Node != ci.m_Node; }

    private:
        pointer m_Node;
    };

    class iterator {
    public:
        typedef value_type value_type;
        typedef value_type* pointer;
        typedef value_type& reference;

        iterator(pointer p) : m_Node(p) {}

        operator const_iterator() const { return m_Node; }

        reference operator*() const { return *m_Node; }
        pointer operator->() const { return m_Node; }

        iterator& operator++() {
            m_Node = m_Node->GetNext();
            return *this;
        }

        iterator operator++(int) {
            iterator temporary(*this);
            ++(*this);
            return temporary;
        }

        iterator& operator--();
        iterator operator--(int);
        bool operator==(const iterator&) const;

        bool operator!=(const iterator& i) const { return m_Node != i.m_Node; }

    private:
        pointer m_Node;
    };

    IntrusiveListImplementation() : m_Root() {}

    void push_back(reference node) { m_Root.LinkPrev(&node); }

    void push_front(reference);
    void pop_back();

    void pop_front() { m_Root.GetNext()->Unlink(); }

    reference back();
    reference back() const;
    reference front();
    reference front() const;

    iterator begin() { return m_Root.GetNext(); }
    const_iterator begin() const { return m_Root.GetNext(); }

    iterator end() { return &m_Root; }
    const_iterator end() const { return &m_Root; }

    iterator iterator_to(reference value) { return iterator(&value); }
    const_iterator iterator_to(reference value) const { return iterator(&value); }

    size_type size() const;

    bool empty() const { return !m_Root.IsLinked(); }

    iterator erase(const_iterator position) {
        iterator temporary(ToMutable(position));
        if (temporary != end()) {
            (temporary++)->Unlink();
        }
        return temporary;
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    iterator insert(const_iterator position, reference node) {
        ToMutable(position)->LinkPrev(&node);
        return iterator(&node);
    }

    void splice(const_iterator, IntrusiveListImplementation&);
    void splice(const_iterator, IntrusiveListImplementation&, const_iterator);
    void splice(const_iterator, IntrusiveListImplementation&, const_iterator, const_iterator);

private:
    void SpliceImplementation(const_iterator, const_iterator, const_iterator);

    iterator ToMutable(const_iterator i) const { return const_cast<value_type*>(&*i); }

    value_type m_Root;
};

}  // namespace detail

template <typename T, typename NodeTraits>
class IntrusiveList {
    NN_NO_COPY(IntrusiveList);

public:
    class iterator;
    class const_iterator;

    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef int size_type;

    class const_iterator {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;

        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
        bool operator==(const const_iterator&) const;
        bool operator!=(const const_iterator&) const;

    private:
        friend class IntrusiveList;

        const_iterator(detail::IntrusiveListImplementation::const_iterator ci) : m_Iterator(ci) {}

        detail::IntrusiveListImplementation::const_iterator GetImplementationIterator() const {
            return m_Iterator;
        }

        detail::IntrusiveListImplementation::const_iterator m_Iterator;
    };

    class iterator {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;

        operator const_iterator() const {
            return static_cast<detail::IntrusiveListImplementation::const_iterator>(m_Iterator);
        }

        reference operator*() const { return NodeTraits::GetItem(*m_Iterator); }

        pointer operator->() const { return &NodeTraits::GetItem(*m_Iterator); }

        iterator& operator++() {
            ++m_Iterator;
            return *this;
        }

        iterator operator++(int) {
            iterator temporary(*this);
            ++m_Iterator;
            return temporary;
        }

        iterator& operator--();
        iterator operator--(int);
        bool operator==(const iterator&) const;

        bool operator!=(const iterator& i) const { return m_Iterator != i.m_Iterator; }

    private:
        friend class IntrusiveList;

        iterator(detail::IntrusiveListImplementation::iterator iter) : m_Iterator(iter) {}

        detail::IntrusiveListImplementation::iterator GetImplementationIterator() const;

        detail::IntrusiveListImplementation::iterator m_Iterator;
    };

    IntrusiveList() : m_Implementation() {}

    void push_back(reference value) { m_Implementation.push_back(ToNode(value)); }

    void push_front(reference);
    void pop_back();
    void pop_front();
    reference front();
    reference front() const;
    reference back();
    reference back() const;

    iterator begin() { return m_Implementation.begin(); }
    const_iterator begin() const { return m_Implementation.begin(); }

    const_iterator cbegin() const;

    iterator end() { return m_Implementation.end(); }
    const_iterator end() const { return m_Implementation.end(); }

    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    iterator iterator_to(reference value) { return m_Implementation.iterator_to(ToNode(value)); }
    const_iterator iterator_to(const_reference value) const {
        return m_Implementation.iterator_to(ToNode(value));
    }

    size_type size() const;
    bool empty() const;

    iterator erase(const_iterator position) {
        detail::IntrusiveListImplementation::iterator result =
            m_Implementation.erase(position.GetImplementationIterator());
        return result;
    }

    void clear() { m_Implementation.clear(); }

    iterator insert(const_iterator position, reference value) {
        detail::IntrusiveListImplementation::iterator result =
            m_Implementation.insert(position.GetImplementationIterator(), ToNode(value));
        return result;
    }

    void splice(const_iterator, IntrusiveList&);
    void splice(const_iterator, IntrusiveList&, const_iterator);
    void splice(const_iterator, IntrusiveList&, const_iterator, const_iterator);

private:
    IntrusiveListNode& ToNode(reference ref) const { return NodeTraits::GetNode(ref); }

    const IntrusiveListNode& ToNode(const_reference) const;
    reference ToReference(IntrusiveListNode&) const;
    const_reference ToReference(const IntrusiveListNode&) const;

    detail::IntrusiveListImplementation m_Implementation;
};

template <class HolderT, IntrusiveListNode HolderT::*Member, class T = HolderT>
class IntrusiveListMemberNodeTraits {
    friend class IntrusiveList<T, IntrusiveListMemberNodeTraits>;

    static IntrusiveListNode& GetNode(T& ref) { return ref.*Member; }

    static const IntrusiveListNode& GetNode(const T& ref) { return ref.*Member; }

    static T& GetItem(IntrusiveListNode& node) {
        return *reinterpret_cast<T*>(reinterpret_cast<char*>(&node) - GetOffset());
    }

    static const T& GetItem(const IntrusiveListNode& node) {
        return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(&node) - GetOffset());
    }

    static uintptr_t GetOffset() {
        return reinterpret_cast<uintptr_t>(&(reinterpret_cast<T*>(0)->*Member));
    }
};

}  // namespace nn::util
