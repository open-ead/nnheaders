#pragma once

#include <nn/atk/atk_Channel.h>

namespace nn::atk::detail::driver {

class SequenceSoundPlayer;

struct NoteOnInfo {
    int prgNo;
    int key;
    int velocity;
    int length;
    int initPan;
    int priority;
    Channel::ChannelCallback channelCallback;
    void* channelCallbackData;
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    UpdateType updateType;
    int subMixIndex;
    int finalMixIndex;
#else
    OutputReceiver* pOutputReceiver;
    UpdateType updateType;
#endif
};
static_assert(sizeof(NoteOnInfo) == 0x38);

class NoteOnCallback {
public:
    virtual ~NoteOnCallback() = default;

    virtual Channel* NoteOn(SequenceSoundPlayer* seqPlayer, u8 bankIndex,
                            const NoteOnInfo& noteOnInfo) = 0;
};
static_assert(sizeof(NoteOnCallback) == 0x8);

}  // namespace nn::atk::detail::driver
