#pragma once

#include <nn/types.h>

namespace nn::atk {

class OutputReceiver {
public:
    enum class ReceiverType : int {
        ReceiverType_SubMix,
        ReceiverType_FinalMix,
    };

    static const int BusCountMax{24};

    virtual ReceiverType GetReceiverType() const = 0;
    virtual int GetChannelCount() const = 0;
    virtual int GetBusCount() const = 0;
    virtual void AddReferenceCount(int value) = 0;
    virtual bool IsSoundSendClampEnabled(int bus) const = 0;
};
static_assert(sizeof(OutputReceiver) == 0x8);

}  // namespace nn::atk
