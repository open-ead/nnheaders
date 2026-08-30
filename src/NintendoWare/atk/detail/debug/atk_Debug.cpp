#include <nn/atk/atk_Debug.h>

namespace nn::atk {
namespace {
const u32 NotEnoughSeqSound     {0b00001};
const u32 NotEnoughStrmSound    {0b00010};
const u32 NotEnoughWaveSound    {0b00100};
const u32 NotEnoughSeqTrack     {0b01000};
const u32 NotEnoughStrmChannel  {0b10000};
const u32 NotEnoughInstance     {0b11111};

u32 gWarningFlag = NotEnoughInstance;

u32 GetWarningBitFlag(DebugWarningFlag warning) {
    u32 bitFlag {0};

    switch (warning) {
    case DebugWarningFlag_NotEnoughInstance:
        bitFlag = NotEnoughInstance;
        break;

    case DebugWarningFlag_NotEnoughSeqsound:
        bitFlag = NotEnoughSeqSound;
        break;

    case DebugWarningFlag_NotEnoughStrmsound:
        bitFlag = NotEnoughStrmSound;
        break;

    case DebugWarningFlag_NotEnoughWavesound:
        bitFlag = NotEnoughWaveSound;
        break;
    
    case DebugWarningFlag_NotEnoughSeqtrack:
        bitFlag = NotEnoughSeqTrack;
        break;

    case DebugWarningFlag_NotEnoughStrmchannel:
        bitFlag = NotEnoughStrmChannel;
        break;
    }

    return bitFlag;
}
} // anonymous namespace

void Debug_SetWarningFlag(DebugWarningFlag warning, bool enable) {
    u32 bitFlag {GetWarningBitFlag(warning)};

    if (enable)
        gWarningFlag |= bitFlag;
    else
        gWarningFlag &= ~bitFlag;
}

namespace detail {
DebugLogFunc g_DebugLogHookFunc;

bool Debug_GetWarningFlag(DebugWarningFlag warning) {
    u32 bitFlag {GetWarningBitFlag(warning)};

    return (bitFlag & ~gWarningFlag) == 0;
}
} // namespace nn::atk::detail 
} // namespace nn::atk