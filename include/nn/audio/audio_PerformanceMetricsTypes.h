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

enum PerformanceDetailType : u8 {
    PerformanceDetailType_Unknown,
    PerformanceDetailType_PcmInt16,
    PerformanceDetailType_Adpcm,
    PerformanceDetailType_VolumeRamp,
    PerformanceDetailType_BiquadFilter,
    PerformanceDetailType_Mix,
    PerformanceDetailType_Delay,
    PerformanceDetailType_Aux,
    PerformanceDetailType_Reverb,
    PerformanceDetailType_Reverb3d,
    PerformanceDetailType_PcmFloat
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