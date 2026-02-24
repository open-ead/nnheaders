#pragma once

#include <nn/atk/detail/atk_ExternalSoundPlayer.h>

namespace nn::atk {
class SoundActor {
public:
    using ActorPlayer = detail::ExternalSoundPlayer;
};
} // namespace nn::atk