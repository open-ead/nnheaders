#include <nn/atk/atk_DisposeCallbackManager.h>

namespace nn::atk::detail::driver {

DisposeCallbackManager& DisposeCallbackManager::GetInstance() {
    static DisposeCallbackManager instance;
    return instance;
}

DisposeCallbackManager::DisposeCallbackManager() = default;

}  // namespace nn::atk::detail::driver
