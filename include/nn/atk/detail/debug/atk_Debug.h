#pragma once

namespace nn::atk {
enum DebugWarningFlag {
    DebugWarningFlag_NotEnoughInstance,
    DebugWarningFlag_NotEnoughSeqsound,
    DebugWarningFlag_NotEnoughStrmsound,
    DebugWarningFlag_NotEnoughWavesound,
    DebugWarningFlag_NotEnoughSeqtrack,
    DebugWarningFlag_NotEnoughStrmchannel,
};

namespace detail {
enum DebugSoundType {
    DebugSoundType_Seqsound,
    DebugSoundType_Strmsound,
    DebugSoundType_Wavesound,
};

using DebugLogFunc = void(*)(char*);
} // namespace nn::atk::detail
} // namespace nn::atk