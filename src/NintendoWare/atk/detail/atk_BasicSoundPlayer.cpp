#include <nn/atk/atk_BasicSoundPlayer.h>

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

}  // namespace nn::atk::detail::driver
