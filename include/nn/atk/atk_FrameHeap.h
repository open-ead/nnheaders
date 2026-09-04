#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/fnd/basis/atkfnd_FrameHeapImpl.h>

namespace nn::atk::detail {

class FrameHeap {
public:
    using DisposeCallback = void (*)(void*, size_t, void*);
    using HeapCallback = void (*)(void*);

    static const int HeapAlign{64};

    class Block {
    public:
        Block(void* buffer, size_t size, DisposeCallback callback, 
              void* callbackArg, HeapCallback heapCallback, void* heapCallbackArg)
              : m_pBuffer{buffer}, m_Size{size}, 
                m_Callback{callback}, m_pCallbackArg{callbackArg},
                m_HeapCallback{heapCallback}, m_pHeapCallbackArg{heapCallbackArg} {}

        void* GetBufferAddr() { return m_pBuffer; }
        const void* GetBufferAddr() const { return m_pBuffer; }

        size_t GetBufferSize() const { return m_Size; }
        DisposeCallback GetDisposeCallback() const { return m_Callback; }
        void* GetDisposeCallbackArg() const { return m_pCallbackArg; }
        HeapCallback GetHeapCallback() const { return m_HeapCallback; }
        void* GetHeapCallbackArg() const { return m_pHeapCallbackArg; }

        util::IntrusiveListNode m_Link;

    private:
        void* m_pBuffer;
        size_t m_Size;
        DisposeCallback m_Callback;
        void* m_pCallbackArg;
        HeapCallback m_HeapCallback;
        void* m_pHeapCallbackArg;
    };

    using BlockList =
        util::IntrusiveList<Block, util::IntrusiveListMemberNodeTraits<Block, &Block::m_Link>>;

    class Section {
    public:
        Section();
        ~Section();

        void AppendBlock(Block* block);

        const BlockList& GetBlockList() const { return m_BlockList; }
        BlockList& GetBlockList() { return m_BlockList; }

        void* GetAddr() { return this; }

        void SetUseCallback(bool use) { m_UseCallback = use; }

        void Dump(const SoundDataManager& mgr, const SoundArchive& arc) const;

        util::IntrusiveListNode m_Link;

    private:
        BlockList m_BlockList;
        bool m_UseCallback{true};
    };
    static_assert(sizeof(Section) == 0x28);

    using SectionList =
        util::IntrusiveList<Section,
                            util::IntrusiveListMemberNodeTraits<Section, &Section::m_Link>>;

    FrameHeap();
    ~FrameHeap();

    bool Create(void* startAddress, size_t size);
    void Destroy();

    void* Alloc(size_t size, DisposeCallback callback, void* callbackArg, 
                HeapCallback heapCallback, void* heapCallbackArg);

    void Clear();

    int SaveState();
    void LoadState(int level);

    int GetCurrentLevel() const;
    size_t GetSize() const;
    size_t GetFreeSize() const;

    bool IsValid() const { return m_pHeap != nullptr; }

    void Dump(const SoundDataManager& mgr, const SoundArchive& arc) const;

    bool ProcessCallback(int level);

private:
    bool NewSection();
    void ClearSection();

    fnd::FrameHeapImpl* m_pHeap{};
    SectionList m_SectionList;
};
static_assert(sizeof(FrameHeap) == 0x18);

}  // namespace nn::atk::detail