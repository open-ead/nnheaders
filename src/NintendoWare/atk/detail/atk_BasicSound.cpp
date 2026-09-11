#include <nn/atk/atk_BasicSound.h>

namespace nn::atk::detail {

// NON_MATCHING on versions lower than 4.0.0
BasicSound::BasicSound() = default;

void BasicSound::StartPrepared() {
    m_StartFlag = true;
}

}  // namespace nn::atk::detail