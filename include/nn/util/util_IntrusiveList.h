#pragma once

namespace nn::util {
struct IntrusiveListNode {
    IntrusiveListNode* m_Prev;
    IntrusiveListNode* m_Next;
};
} // namespace nn::util