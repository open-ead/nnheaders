#pragma once

#include <atomic>

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk {
namespace detail {
class ThreadInfoRecorder {
public:
    util::IntrusiveListNode m_List;

private:
    void* m_Buffer;
    std::size_t m_BufferSize;
    std::size_t m_WritePosition;
    std::atomic_ulong m_ReadPosition;
    std::atomic_int m_RecordFrameCount;
    std::atomic_bool m_IsAllocationFailed;
};
static_assert(sizeof(ThreadInfoRecorder) == 0x38);
} // namespace nn::atk::detail

using SoundThreadInfoRecorder = detail::ThreadInfoRecorder;
using SoundThreadInfoRecorderList = util::IntrusiveList<SoundThreadInfoRecorder, 
                                        util::IntrusiveListMemberNodeTraits<SoundThreadInfoRecorder,
                                            &SoundThreadInfoRecorder::m_List>>;
} // namespace nn::atk