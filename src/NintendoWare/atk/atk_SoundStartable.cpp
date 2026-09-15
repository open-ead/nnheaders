#include <nn/atk/atk_SoundStartable.h>

namespace nn::atk {

SoundStartable::StartResult SoundStartable::StartSound(SoundHandle* handle,
                                                       SoundArchive::ItemId soundId,
                                                       const char* soundArchiveName,
                                                       const StartInfo* startInfo) {
    StartResult result{detail_SetupSound(handle, soundId, false, soundArchiveName, startInfo)};

    if (!result.IsSuccess() || !handle->IsAttachedSound())
        return result;

    handle->StartPrepared();
    StartResult result2{StartResult::ResultCode_Success};
    return result2;
}

SoundStartable::StartResult SoundStartable::StartSound(SoundHandle* handle, SoundArchive::ItemId soundId, const StartInfo* startInfo) {
    return StartSound(handle, soundId, nullptr, startInfo);
}

}  // namespace nn::atk
