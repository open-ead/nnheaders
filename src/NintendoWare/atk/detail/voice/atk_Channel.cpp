#include <nn/atk/atk_Channel.h>

#include <nn/atk/atk_DisposeCallbackManager.h>

namespace nn::atk::detail::driver {

namespace {

u8 GetNwInterpolationTypeFromHardwareManager() {
    // TODO
    u8 result;
}

}  // anonymous namespace

Channel::Channel() {
    m_Disposer.Initialize(this);
    DisposeCallbackManager::GetInstance()->RegisterDisposeCallback(&m_Disposer);
}

Channel::~Channel() {
    DisposeCallbackManager::GetInstance()->UnregisterDisposeCallback(&m_Disposer);
}

void Channel::CallChannelCallback(ChannelCallbackStatus status) {
    if (m_Callback == nullptr)
        return;

    m_Callback(this, status, m_CallbackData);
    m_Callback = nullptr;
    m_CallbackData = nullptr;
}

}  // namespace nn::atk::detail::driver