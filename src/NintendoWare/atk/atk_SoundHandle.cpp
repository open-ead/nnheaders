#include <nn/atk/atk_SoundHandle.h>

namespace nn::atk {

void SoundHandle::detail_DuplicateHandle(SoundHandle* handle) {

    detail::BasicSound* sound{m_pSound};

    DetachSound();

    // if (handle != nullptr)
    //     handle->detail_AttachSoundAsTempHandle(sound);
}

void SoundHandle::detail_AttachSoundAsTempHandle(detail::BasicSound* sound) {
    m_pSound = sound;

    if (m_pSound->IsAttachedTempGeneralHandle())
        m_pSound->DetachTempGeneralHandle();

    m_pSound->m_pTempGeneralHandle = this;
}

void SoundHandle::DetachSound() {
    if (IsAttachedSound()) {
        if (m_pSound->m_pGeneralHandle == this)
            m_pSound->m_pGeneralHandle = nullptr;

        if (m_pSound->m_pTempGeneralHandle == this)
            m_pSound->m_pTempGeneralHandle = nullptr;

        if (m_pSound != nullptr)
            m_pSound = nullptr;
    }
}

}  // namespace nn::atk