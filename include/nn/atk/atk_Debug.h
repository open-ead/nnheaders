#pragma once

#include <nn/types.h>

namespace nn::atk {
enum DebugWarningFlag {
    DebugWarningFlag_NotEnoughInstance,
    DebugWarningFlag_NotEnoughSeqsound,
    DebugWarningFlag_NotEnoughStrmsound,
    DebugWarningFlag_NotEnoughWavesound,
    DebugWarningFlag_NotEnoughSeqtrack,
    DebugWarningFlag_NotEnoughStrmchannel,
};

void Debug_SetWarningFlag(DebugWarningFlag warning, bool enable);

namespace detail {
enum DebugSoundType {
    DebugSoundType_Seqsound,
    DebugSoundType_Strmsound,
    DebugSoundType_Wavesound,
};

static const char* gSoundTypeStrings[3] = {"seq", "strm", "wave"};

bool Debug_GetWarningFlag(DebugWarningFlag warning);
DebugWarningFlag Debug_GetDebugWarningFlagFromSoundType(DebugSoundType type);
char* Debug_GetSoundTypeString(DebugSoundType type);

using DebugLogFunc = void(*)(char*);
} // namespace nn::atk::detail
} // namespace nn::atk