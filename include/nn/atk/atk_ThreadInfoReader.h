#pragma once

#include <atomic>

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk {
namespace detail {
class ThreadInfoRecorder {
public:
    static const size_t BufferMinimumSize {};

    ThreadInfoRecorder();

    void Initialize(void* buffer, size_t bufferSize) {
        m_Buffer = buffer;
        m_BufferSize = bufferSize;
    }

    int GetRecordedFrameCount() const { return m_RecordFrameCount; }

    const void* GetRingBufferHeadAddress() const {
        // TODO
        return nullptr;
    }

    void MoveToNextFrame() {
        // TODO
    }

    void RecordFooter() {
        // TODO
    }
    
    util::IntrusiveListNode m_List;

private:
    void* AllocateInfoBuffer([[maybe_unused]] u32 count, [[maybe_unused]] size_t size) { return nullptr; }
    void* AllocateBuffer([[maybe_unused]] u32 count, [[maybe_unused]] size_t size1, [[maybe_unused]]size_t size2) { return nullptr; }

    void* m_Buffer;
    size_t m_BufferSize;
    size_t m_WritePosition;
    std::atomic_ulong m_ReadPosition;
    std::atomic_int m_RecordFrameCount;
    std::atomic_bool m_IsAllocationFailed;
};
static_assert(sizeof(ThreadInfoRecorder) == 0x38);
}  // namespace detail

using SoundThreadInfoRecorder = detail::ThreadInfoRecorder;
using SoundThreadInfoRecorderList = util::IntrusiveList<SoundThreadInfoRecorder, 
                                        util::IntrusiveListMemberNodeTraits<SoundThreadInfoRecorder,
                                            &SoundThreadInfoRecorder::m_List>>;
}  // namespace nn::atk