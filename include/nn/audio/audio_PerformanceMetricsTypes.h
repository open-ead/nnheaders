#pragma once

#include <nn/audio/audio_Common.h>

namespace nn::audio {
enum PerformanceEntryType {
    PerformanceEntryType_Unknown,
    PerformanceEntryType_Voice,
    PerformanceEntryType_SubMix,
    PerformanceEntryType_FinalMix,
    PerformanceEntryType_Sink,
    PerformanceEntryType_Count
};

struct PerformanceEntry {
    NodeId id;
    s32 startTime;
    s32 processingTime;
    s8 entryType;
    s8 _padding[3];
};
static_assert(sizeof(PerformanceEntry) == 0x10);

struct PerformanceFrameHeader {};
struct PerformanceDetail {};
} // namespace nn::audio