#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

class PoolImpl {
public:
    static const int DefaultAlignment{4};

    PoolImpl() = default;

protected:
    int CreateImpl(void* buffer, size_t size, size_t objSize);
    int CreateImpl(void* buffer, size_t size, size_t objSize, size_t alignment);

    void DestroyImpl();

    int CountImpl() const;

    void* AllocImpl();

    void FreeImpl(void* ptr);

private:
    PoolImpl* m_pNext;
    void* m_pBuffer;
    size_t m_BufferSize;
};
static_assert(sizeof(PoolImpl) == 0x18);

class BufferPool : PoolImpl {
public:
    static const int DefaultAlignment{4};

    BufferPool() = default;

    int Create(void* buffer, size_t size, size_t bufferSizePerAlloc);
    int Create(void* buffer, size_t size, size_t bufferSizePerAlloc, size_t alignment);

    void Destroy() { DestroyImpl(); }

    int Count() const { return CountImpl(); }

    void* Alloc() { return AllocImpl(); }

    void Free(void* ptr) { FreeImpl(ptr); }

};

template <typename T>
class InstancePool : PoolImpl {
public:
    static const int DefaultAlignment{4};

    InstancePool() = default;

    int Create(void* buffer, size_t size) {
        size_t objSize{sizeof(T)};

        return CreateImpl(buffer, size, objSize);
    }

    int Create(void* buffer, size_t size, size_t alignment) {
        size_t objSize{sizeof(T)};

        return CreateImpl(buffer, size, objSize, alignment);
    }

    void Destroy() { DestroyImpl(); }

    int Count() const { return Count(); }

    T* Alloc() { return reinterpret_cast<T*>(AllocImpl()); }

    void Free(T* obj) { FreeImpl(obj); }
};

} // namespace nn::atk::detail