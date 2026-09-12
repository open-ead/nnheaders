#include <nn/atk/atk_SequenceTrack.h>

namespace nn::atk::detail::driver {

SequenceTrack::SequenceTrack()
    : m_OpenFlag{false}, m_pSequenceSoundPlayer{nullptr}, m_pChannelList{nullptr} {
    InitParam();
}

}  // namespace nn::atk::detail::driver