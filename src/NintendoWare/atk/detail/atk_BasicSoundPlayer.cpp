#include <nn/atk/atk_BasicSoundPlayer.h>

namespace nn::atk::detail::driver {

BasicSoundPlayer::BasicSoundPlayer() : m_Event(os::EventClearMode_ManualClear) {
    m_Event.Signal();
}

}  // namespace nn::atk::detail::driver
