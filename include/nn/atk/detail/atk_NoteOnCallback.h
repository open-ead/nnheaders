#pragma once

#include <nn/atk/detail/voice/atk_Channel.h>

namespace nn::atk::detail::driver {
class SequenceSoundPlayer;

struct NoteOnInfo {
    s32 prgNo;
    s32 key;
    s32 velocity;
    s32 length;
    s32 initPan;
    s32 priority;
    driver::Channel::ChannelCallback channelCallback;
    void* channelCallbackData;
    OutputReceiver* pOutputReceiver;
    UpdateType updateType;
};
static_assert(sizeof(NoteOnInfo) == 0x38);

class NoteOnCallback {
public:
    virtual ~NoteOnCallback();
    virtual Channel* NoteOn(SequenceSoundPlayer* seqPlayer, u8 bankIndex, 
                            const NoteOnInfo& noteOnInfo) = 0;
};
static_assert(sizeof(NoteOnCallback) == 0x8);
} // namespace nn::atk::detail