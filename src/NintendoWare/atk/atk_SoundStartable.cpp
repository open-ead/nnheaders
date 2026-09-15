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

SoundStartable::StartResult SoundStartable::StartSound(SoundHandle* handle,
                                                       SoundArchive::ItemId soundId,
                                                       const StartInfo* startInfo) {
    return StartSound(handle, soundId, nullptr, startInfo);
}

SoundStartable::StartResult SoundStartable::StartSound(SoundHandle* handle, const char* soundName,
                                                       const char* soundArchiveName,
                                                       const StartInfo* startInfo) {
    u32 soundId{detail_GetItemId(soundName, soundArchiveName)};

    if (soundId == InvalidSoundId)
        return StartResult{StartResult::ResultCode_ErrorInvalidLabelString};

    StartResult result{StartSound(handle, soundId, soundArchiveName, startInfo)};
    return result;
}

SoundStartable::StartResult SoundStartable::StartSound(SoundHandle* handle, const char* soundName,
                                                       const StartInfo* startInfo) {
    return StartSound(handle, soundName, nullptr, startInfo);
}

}  // namespace nn::atk
