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

static u8 gWarningFlag = 0b11111;
static const u32 gWarningBitFlags[6] = {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000};

void Debug_SetWarningFlag(DebugWarningFlag warning, bool);

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

static DebugLogFunc g_DebugLogHookFunc;
} // namespace nn::atk::detail
} // namespace nn::atk