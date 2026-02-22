#pragma once

namespace nn::atk {
class OutputReceiver {
public:
    enum ReceiverType {
        ReceiverType_SubMix,
        ReceiverType_FinalMix,
    };

    virtual ~OutputReceiver() = 0;
};
static_assert(sizeof(OutputReceiver) == 0x8);
} // namespace nn::atk