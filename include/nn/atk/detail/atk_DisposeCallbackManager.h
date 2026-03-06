#pragma once

#include <nn/types.h>

#include <nn/atk/detail/atk_DisposeCallback.h>

namespace nn::atk::detail::driver {
class DisposeCallbackManager {
public:
    using CallbackList = util::IntrusiveList<DisposeCallback, 
                            util::IntrusiveListMemberNodeTraits<DisposeCallback, 
                                &DisposeCallback::m_DisposeLink>>;

    DisposeCallbackManager();

    void RegisterDisposeCallback(DisposeCallback* callback);
    void UnregisterDisposeCallback(DisposeCallback* callback);

    u64 GetCallbackCount() const;

    void Dispose(const void* mem, size_t size);

private:
    CallbackList m_CallbackList;
};
static_assert(sizeof(DisposeCallbackManager) == 0x10);
} // namespace nn::atk::detail