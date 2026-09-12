#pragma once

#include <nn/atk/atk_SoundThread.h>

namespace nn::atk::detail {

template <typename Instance>
class LoaderManager : public driver::SoundThread::SoundFrameCallback {
public:
    using List = util::IntrusiveList<
        Instance, util::IntrusiveListMemberNodeTraits<Instance, &Instance::m_LinkForLoaderManager>>;
    using Iterator = typename List::iterator;

    LoaderManager();
    ~LoaderManager() override;

    // UNCHECKED
    size_t GetRequiredMemSize(int count) { return sizeof(Instance) * count; }

    // UNCHECKED
    int Create(void* buffer, size_t size) {
        char* ptr{reinterpret_cast<char*>(buffer)};
        const int objectCount{size / sizeof(Instance)};

        for (int i{0}; i < objectCount; ++i) {
            Instance* instance{new (ptr) Instance};
            m_FreeList.push_back(*instance);

            ptr += i * sizeof(Instance);
        }

        m_pBuffer = buffer;
        m_BufferSize = size;
    }

    // UNCHECKED
    void Destroy() {
        char* ptr{reinterpret_cast<char*>(m_pBuffer)};
        const int objectCount{m_BufferSize / sizeof(Instance)};

        if (m_FreeList.empty())
            return;

        for (int i{0}; i < objectCount; ++i) {
            Instance* instance{reinterpret_cast<Instance*>(ptr)};
            m_FreeList.iterator_to(*instance)->Finalize();

            ptr += i * sizeof(Instance);
        }

        m_FreeList.clear();
    }

    Instance* Alloc() {
        if (m_FreeList.empty())
            return nullptr;

        m_FreeList.pop_front();

        Instance* instance{&m_FreeList.front()};

        return instance;
    }

    void Free(Instance* instance) {
        // TODO
    }

    void UpdateFreeReqList() {
        // TODO
    }

    int GetFreeCount() const { return m_FreeList.size(); }

    void OnBeginSoundFrame() override {
        // TODO
    }

private:
    void* m_pBuffer;
    size_t m_BufferSize;
    List m_FreeList;
    List m_FreeReqList;
};

}  // namespace nn::atk::detail
