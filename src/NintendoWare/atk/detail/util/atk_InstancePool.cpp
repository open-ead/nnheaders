#include <nn/atk/atk_InstancePool.h>

#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

// NON_MATCHING: incomplete implementation
int PoolImpl::CreateImpl(void* buffer, size_t size, size_t objSize, size_t alignment) {
    if (buffer == nullptr)
        return 0;

    char* ptr {util::BytePtr(buffer).AlignUp(alignment).Get<char>()};
    objSize = util::align_up(objSize, alignment);
    int objectCount = (reinterpret_cast<ptrdiff_t>(buffer) + size - reinterpret_cast<ptrdiff_t>(ptr)) / objSize;

    for (int i{0}; i < objectCount; ++i) {
        PoolImpl* head{m_pNext};
    }

    m_pBuffer = buffer;
    m_BufferSize = size;

    return objectCount;
}

void* PoolImpl::AllocImpl() {
    PoolImpl* head{m_pNext};

    if (head != nullptr) {
        m_pNext = head->m_pNext;
    }

    return head;
}

}  // namespace nn::atk::detail