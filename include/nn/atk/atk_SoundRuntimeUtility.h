#pragma once

#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundInstanceManager.h>

namespace nn::atk::detail {

class SoundRuntimeUtility {
public:
#if NN_WARE_VER < NN_MAKE_VER(4, 0, 0)
    Sound* AllocSound(SoundInstanceManager<Sound>* manager, SoundArchive::ItemId soundId,
                      int playerPriority, int ambientPriority,
                      BasicSound::AmbientInfo* ambientArgInfo)
#else
    template <typename Sound>
    Sound* AllocSound(SoundInstanceManager<Sound>* manager, SoundArchive::ItemId soundId,
                      int playerPriority, int ambientPriority,
                      BasicSound::AmbientInfo* ambientArgInfo, OutputReceiver* pOutputReceiver)
#endif
    {
#if NN_WARE_VER < NN_MAKE_VER(4, 0, 0)
        Sound* sound{manager->Alloc(playerPriority, ambientPriority)};
#else
        Sound* sound{manager->Alloc(playerPriority, ambientPriority, pOutputReceiver)};
#endif

        if (sound != nullptr) {
            sound->SetId(soundId);

            if (ambientArgInfo != nullptr)
                sound->SetAmbientInfo(*ambientArgInfo);
        }

        return sound;
    }
};
static_assert(sizeof(SoundRuntimeUtility) == 0x1);

}  // namespace nn::atk::detail
