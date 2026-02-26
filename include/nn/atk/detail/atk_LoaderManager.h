#pragma once

#include <nn/atk/detail/thread/atk_SoundThread.h>

namespace nn::atk::detail {
template <typename T>
class LoaderManager : driver::SoundThread::SoundFrameCallback {
public:
    using List = util::IntrusiveList<T, util::IntrusiveListMemberNodeTraits<T, &T::m_LinkForLoaderManager>>;
    using Iterator = typename List::iterator;

    LoaderManager();
    ~LoaderManager() override;

    void OnBeginSoundFrame() override;

    void Destroy();

private:
    void* m_pBuffer;
    std::size_t m_BufferSize;
    List m_FreeList;
    List m_FreeReqList;
};

} // namespace nn::atk::detail