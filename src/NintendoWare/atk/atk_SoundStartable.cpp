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

SoundStartable::StartResult SoundStartable::HoldSound(SoundHandle* handle,
                                                      SoundArchive::ItemId soundId,
                                                      const char* soundArchiveName,
                                                      const StartInfo* startInfo) {
    if (handle->IsAttachedSound() && handle->GetId() == soundId) {
        handle->detail_GetAttachedSound()->SetAutoStopCounter(1);
    } else {
        StartResult result{detail_SetupSound(handle, soundId, true, soundArchiveName, startInfo)};

        if (!result.IsSuccess())
            return result;

        handle->StartPrepared();
        handle->detail_GetAttachedSound()->SetAutoStopCounter(1);
    }

    StartResult result{StartResult::ResultCode_Success};

    if (startInfo != nullptr && (startInfo->enableFlag & StartInfo::EnableFlagBit_FadeFrame) != 0) {
        handle->Stop(startInfo->fadeFrame);
        StartResult result2{StartResult::ResultCode_Success};
        return result2;
    }

    return result;
}

SoundStartable::StartResult SoundStartable::HoldSound(SoundHandle* handle,
                                                      SoundArchive::ItemId soundId,
                                                      const StartInfo* startInfo) {
    StartResult result{HoldSound(handle, soundId, nullptr, startInfo)};
    return result;
}

SoundStartable::StartResult SoundStartable::HoldSound(SoundHandle* handle, const char* soundLabel,
                                                      const StartInfo* startInfo) {
    return HoldSound(handle, soundLabel, nullptr, startInfo);
}

SoundStartable::StartResult SoundStartable::HoldSound(SoundHandle* handle, const char* soundLabel,
                                                      const char* soundArchiveName,
                                                      const StartInfo* startInfo) {
    u32 soundId{detail_GetItemId(soundLabel, soundArchiveName)};

    if (soundId == InvalidSoundId)
        return StartResult{StartResult::ResultCode_ErrorInvalidLabelString};

    return HoldSound(handle, soundId, soundArchiveName, startInfo);
}

SoundStartable::StartResult SoundStartable::PrepareSound(SoundHandle* handle,
                                                         SoundArchive::ItemId soundId,
                                                         const char* soundArchiveName,
                                                         const StartInfo* startInfo) {
    return detail_SetupSound(handle, soundId, false, soundArchiveName, startInfo);
}

SoundStartable::StartResult SoundStartable::PrepareSound(SoundHandle* handle,
                                                         SoundArchive::ItemId soundId,
                                                         const StartInfo* startInfo) {
    return detail_SetupSound(handle, soundId, false, nullptr, startInfo);
}

SoundStartable::StartResult SoundStartable::PrepareSound(SoundHandle* handle, const char* soundName,
                                                         const char* soundArchiveName,
                                                         const StartInfo* startInfo) {
    u32 soundId{detail_GetItemId(soundName, soundArchiveName)};

    if (soundId == InvalidSoundId)
        return StartResult{StartResult::ResultCode_ErrorInvalidLabelString};

    return PrepareSound(handle, soundId, soundArchiveName, startInfo);
}

SoundStartable::StartResult SoundStartable::PrepareSound(SoundHandle* handle, const char* soundName,
                                                         const StartInfo* startInfo) {
    return PrepareSound(handle, soundName, nullptr, startInfo);
}

}  // namespace nn::atk
