#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/fnd/basis/atkfnd_FrameHeapImpl.h>

namespace nn::atk::detail {
class FrameHeap {
public:
    using DisposeCallback = void(*)(void*, size_t, void*);
    using HeapCallback = void(*)(void*);

    class Block {
    public:
        Block();

    private:
        friend FrameHeap;
        
        util::IntrusiveListNode m_Link;
        void* m_pBuffer;
        size_t m_Size;
        DisposeCallback m_Callback;
        void* m_pCallbackArg;
        HeapCallback m_HeapCallback;
        void* m_pHeapCallbackArg;
    };

    using BlockList = util::IntrusiveList<Block, 
                        util::IntrusiveListMemberNodeTraits<Block, &Block::m_Link>>;

    class Section {
    public:
        Section();
        ~Section();

        void* AppendBlock(Block* block);
        
        void Dump(const SoundDataManager&, const SoundArchive&) const;

    private:
        friend FrameHeap;

        util::IntrusiveListNode m_Link;
        BlockList m_BlockList;
        bool m_UseCallback;
    };
    
    using SectionList = util::IntrusiveList<Section, 
                            util::IntrusiveListMemberNodeTraits<Section, &Section::m_Link>>;

    FrameHeap();
    ~FrameHeap();

    void Destroy();

    bool Create(void* startAddress, size_t size);

    bool NewSection();
    void ClearSection();

    void Clear();

    void* Alloc(size_t size, DisposeCallback callback, void* callbackArg, 
                HeapCallback heapCallback, void* heapCallbackArg);

    s32 SaveState();

    bool ProcessCallback(s32 level);

    void LoadState(s32 state);

    s32 GetCurrentLevel() const;
    size_t GetSize() const;
    size_t GetFreeSize() const;

    void Dump(const SoundDataManager&, const SoundArchive&) const;

private:
    fnd::FrameHeapImpl* m_pHeap;
    SectionList m_SectionList;
};
static_assert(sizeof(FrameHeap) == 0x18);
} // namespace nn::atk::detail