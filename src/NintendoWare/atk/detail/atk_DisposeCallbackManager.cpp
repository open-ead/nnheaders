#include <nn/atk/atk_DisposeCallbackManager.h>

namespace nn::atk::detail::driver {

DisposeCallbackManager& DisposeCallbackManager::GetInstance() {
    static DisposeCallbackManager instance;
    return instance;
}

DisposeCallbackManager::DisposeCallbackManager() = default;

void DisposeCallbackManager::RegisterDisposeCallback(DisposeCallback* callback) {
    m_CallbackList.push_back(*callback);
}

// NON_MATCHING
void DisposeCallbackManager::Dispose(const void* mem, size_t size) {
    const void* start{mem};
    const void* end{reinterpret_cast<const void*>(reinterpret_cast<ptrdiff_t>(mem) + size)};

    for (auto itr{GetInstance().m_CallbackList.begin()}; itr != GetInstance().m_CallbackList.end();) {
        auto curItr{itr++};
        curItr->InvalidateData(start, end);
    }
}


}  // namespace nn::atk::detail::driver
