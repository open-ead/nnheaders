#pragma once

#include <nn/audio/audio_Common.h>

namespace nn::audio {
enum AudioRendererRenderingDevice {
    AudioRendererRenderingDevice_AudioCoprocessor,
    AudioRendererRenderingDevice_Cpu,
};

enum AudioRendererExecutionMode {
    AudioRendererExecutionMode_AutoExecution,
    AudioRendererExecutionMode_ManualExecution,
};

struct AudioRendererHandle {
    void* _handle;
    void* _context;
};
static_assert(sizeof(AudioRendererHandle) == 0x10);

struct AudioRendererParameter {
    s32 sampleRate;
    s32 sampleCount;
    s32 mixBufferCount;
    s32 subMixCount;
    s32 voiceCount;
    s32 sinkCount;
    s32 effectCount;
    s32 performanceFrameCount;
    bool isVoiceDropEnabled;
    s32 splitterCount;
    s32 splitterSendChannelCount;
    AudioRendererRenderingDevice renderingDevice;
    AudioRendererExecutionMode executionMode;
    u32 _magic;

    static const s32 MixBufferCountMax = 256;
    static const s32 SubMixCountMax = 256;
    static const s32 VoiceCountMax = 1024;
    static const s32 SinkCountMax = 256;
    static const s32 EffectCountMax = 256;
    static const s32 PerformanceFrameCountMax = 4096;
    static const s32 SplitterCountMax = 256;
};
static_assert(sizeof(AudioRendererParameter) == 0x38);

struct AudioRendererConfig {
    VoiceInfoManager* _pVoiceInfoManager;
    MixManager* _pMixManager;
    EffectManager* _pEffectManager;
    SinkManager* _pSinkManager;
    PerformanceBufferManager* _pPerformanceBufferManager;
    MemoryPoolManager* _pMemoryPoolManager;
    BehaviorManager* _pBehaviorManager;
    SplitterInfoManager* _pSplitterInfoManager;
    void* _pInParameter;
    size_t _pInParameterSize;
    void* _pOutStatus;
    size_t _pOutStatusSize;
    void* _pConfigBuffer;
    size_t _configBufferSize;
};
static_assert(sizeof(AudioRendererConfig) == 0x70);
} // namespace nn::audio