#pragma once

namespace nn {
namespace font {
namespace detail {

// reorder
class FreeList {
    void* m_pFree;
    void* m_pWork;

public:
    FreeList();
    void Initialize(void*, int, int);
    void* Get();
    void Put(void*);
    void* GetWork() const;
    void Cleanup();
    bool IsEmpty() const;
};

}  // namespace detail
}  // namespace font
}  // namespace nn
