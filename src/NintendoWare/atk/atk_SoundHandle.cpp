#include <nn/atk/atk_SoundHandle.h>

namespace nn::atk {

void SoundHandle::detail_DuplicateHandle(SoundHandle* handle) {
    DetachSound();

    if (handle == nullptr)
        return;

    detail::BasicSound* sound{handle->m_pSound};

    if (handle->IsAttachedSound())
        detail_AttachSoundAsTempHandle(sound);
}

void SoundHandle::detail_AttachSoundAsTempHandle(detail::BasicSound* sound) {
    m_pSound = sound;

    if (m_pSound->IsAttachedTempGeneralHandle())
        m_pSound->DetachTempGeneralHandle();

    m_pSound->m_pTempGeneralHandle = this;
}

void SoundHandle::detail_AttachSound(detail::BasicSound* sound) {
    m_pSound = sound;

    if (m_pSound->IsAttachedGeneralHandle())
        m_pSound->DetachGeneralHandle();

    m_pSound->m_pGeneralHandle = this;
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

bool SoundHandle::CalculateSoundParamCalculationValues(
    SoundParamCalculationValues* pOutValue) const {
    if (!IsAttachedSound())
        return false;

    m_pSound->CalculateSoundParamCalculationValues(pOutValue);
    return true;
}

}  // namespace nn::atk
