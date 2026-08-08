#pragma once

#include <nn/types.h>

namespace nn::atk {
class OutputReceiver {
public:
    enum ReceiverType {
        ReceiverType_SubMix,
        ReceiverType_FinalMix,
    };

    virtual ReceiverType GetReceiverType() const = 0;
    virtual s32 GetChannelCount() const = 0;
    virtual s32 GetBusCount() const = 0;
    virtual void AddReferenceCount(s32 value) = 0;
    virtual bool IsSoundSendClampEnabled(s32 bus) const = 0;
};
static_assert(sizeof(OutputReceiver) == 0x8);
} // namespace nn::atk