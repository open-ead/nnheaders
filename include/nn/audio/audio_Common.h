#pragma once

#include <nn/types.h>

namespace nn::audio {
using NodeId = uint32_t;

struct AudioDeviceName {
    char raw_name[0x100];
};
static_assert(sizeof(AudioDeviceName) == 0x100);

struct DelayType {
    u64* _0;
};

struct EffectInfo {};
struct MixInfo {};

class VoiceInfoManager {};
class MixManager {};
class EffectManager {};
class SinkManager {};
class PerformanceBufferManager {};
class MemoryPoolManager {};
class BehaviorManager {};
class SplitterInfoManager {};
} // namespace nn::audio