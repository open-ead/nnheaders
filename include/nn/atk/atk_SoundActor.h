#pragma once

#include <nn/atk/atk_ExternalSoundPlayer.h>

namespace nn::atk {
class SoundActor {
public:
    using ActorPlayer = detail::ExternalSoundPlayer;

    ActorPlayer* detail_GetActorPlayer(int actorPlayerId);

private:
    u8 _0[0x90];
    ActorPlayer* m_ActorPlayerList;
};

} // namespace nn::atk