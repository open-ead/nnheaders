#include <nn/atk/atk_SoundStartable.h>

namespace nn::atk {

SoundStartable::StartResult SoundStartable::StartSound(SoundHandle* handle, SoundArchive::ItemId soundId, const char* soundArchiveName, const StartInfo* startInfo) {
    StartResult result{detail_SetupSound(handle, soundId, false, soundArchiveName, startInfo)};
    
    if (!result.IsSuccess()) 
        return result;
    
    // StartResult result2{handle->StartPrepared()};
    // return result2;
}

}  // namespace nn::atk
