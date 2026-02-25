#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/submix/atk_OutputReceiver.h>

namespace nn::atk::detail {
template <typename T>
class SoundInstanceManager {
public:
    using PriorityList = util::IntrusiveList<T, 
                            util::IntrusiveListMemberNodeTraits<T, &T::m_PriorityLink>>;
    using Iterator = typename PriorityList::iterator;

    T* Alloc(s32 priority, s32 ambientPriority);
    T* Alloc(s32 priority, s32 ambientPriority, OutputReceiver* pOutputReceiver);

    s32 Create(void* buffer, std::size_t size, const SoundInstanceConfig& config);

    void SortPriorityList();

private:
    void* m_pBuffer;
    std::size_t m_BufferSize;
    SoundInstanceConfig m_SoundInstanceConfig;
    PriorityList m_PriorityList;
    PriorityList m_FreeList;
};
} // namespace nn::atk::detail