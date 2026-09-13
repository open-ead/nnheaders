#include <nn/atk/atk_BasicSoundPlayer.h>

#include <nn/atk/atk_OutputReceiver.h>

namespace nn::atk::detail::driver {

BasicSoundPlayer::BasicSoundPlayer() : m_Event(os::EventClearMode_ManualClear) {
    m_Event.Signal();
}

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
void BasicSoundPlayer::Initialize()
#else
void BasicSoundPlayer::Initialize(OutputReceiver* pOutputReceiver)
#endif
{
    m_ActiveFlag = false;
    m_StartedFlag = false;
    m_PauseFlag = false;
    m_FinishFlag = false;
    m_IsFinalizedForCannotAllocateResource = false;

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    m_pOutputReceiver = pOutputReceiver;
#endif

    m_PlayerParamSet.Initialize();
    m_pPlayerHeapDataManager = nullptr;
}

void BasicSoundPlayer::Finalize() {
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    if (m_pOutputReceiver != nullptr) {
        m_pOutputReceiver->AddReferenceCount(-1);
        m_pOutputReceiver = nullptr;
    }
#endif
    
    m_Event.Signal();
}

void BasicSoundPlayer::SetBiquadFilter(int type, float value) {
    m_PlayerParamSet.biquadType = type;
    m_PlayerParamSet.biquadValue = value;
}

}  // namespace nn::atk::detail::driver
