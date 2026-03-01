#pragma once

#include <nn/audio/audio_MemoryPoolTypes.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundMemoryAllocatable.h>
#include <nn/atk/detail/util/atk_FrameHeap.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>

namespace nn::atk::detail {
class SoundHeap : SoundMemoryAllocatable {
public:
    SoundHeap();
    ~SoundHeap() override;

    void Destroy();

    bool Create(void* startAddress, size_t size);
    bool Create(void* startAddress, size_t size, bool);

    void Clear();

    void* Allocate(size_t size) override;

    static void DisposeCallbackFunc(void* mem, size_t size, void* arg);

    void* Allocate(size_t size, 
                   SoundMemoryAllocatable::DisposeCallback heapCallback, 
                   void* heapCallbackArg) override;

    size_t GetAllocateSize(size_t size, bool needMemoryPool) override;

    s32 SaveState();
    void LoadState(s32 state);

private:
    fnd::CriticalSection m_CriticalSection;
    FrameHeap m_FrameHeap;
    audio::MemoryPoolType m_MemoryPool;
    bool m_IsAutoMemoryPoolManagementEnabled;
};
static_assert(sizeof(SoundHeap) == 0x50);
} // namespace nn::atk::detail