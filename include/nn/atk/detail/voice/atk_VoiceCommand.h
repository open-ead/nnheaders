#pragma once

#include <nn/util.h>

#include <nn/atk/detail/thread/atk_CommandManager.h>
#include <nn/atk/detail/voice/atk_LowLevelVoice.h>

namespace nn::atk::detail {
struct VoiceCommandPlay : Command {
    u32 voiceId;
    SampleFormat sampleFormat;
    u32 sampleRate;
    AdpcmParam adpcmParam;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* pOutputReceiver;
#endif
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(VoiceCommandPlay) == 0x48);
#else
static_assert(sizeof(VoiceCommandPlay) == 0x50);
#endif

struct VoiceCommandPause : Command {
    u32 voiceId;
};
static_assert(sizeof(VoiceCommandPause) == 0x20);

struct VoiceCommandFree : Command {
    u32 voiceId;
};
static_assert(sizeof(VoiceCommandFree) == 0x20);

struct VoiceCommandParam : Command {
    u32 voiceId;
    VoiceParam voiceParam;
};
static_assert(sizeof(VoiceCommandParam) == 0x98);

struct VoiceCommandPriority : Command {
    u32 voiceId;
    u32 priority;
};
static_assert(sizeof(VoiceCommandPriority) == 0x20);

struct VoiceCommandAlloc : Command {
    u32 voiceId;
    u32 priority;
    void* userId;
};
static_assert(sizeof(VoiceCommandAlloc) == 0x28);

struct VoiceCommandAppendWaveBuffer : Command {
    u32 voiceId;
    void* tag;
    void* bufferAddress;
    size_t bufferSize;
    size_t sampleLength;
    position_t sampleOffset;
    bool adpcmContextEnable;
    AdpcmContext adpcmContext;
    bool loopFlag;
};
static_assert(sizeof(VoiceCommandAppendWaveBuffer) == 0x100);

class VoiceReplyCommand : CommandManager {
public:
    static void ProcessCommandList(Command* commandList);

};
static_assert(sizeof(VoiceReplyCommand) == 0x310);

class LowLevelVoiceCommand : CommandManager {
public:
    struct WaveBufferPacket {
        AdpcmContext adpcmContext;
        WaveBuffer waveBuffer;
    };
    static_assert(sizeof(WaveBufferPacket) == 0x80);

    void Initialize(void*, size_t, void*, size_t, s32);

    static void ProcessCommandList(Command* commandList);

    static size_t GetRequiredWaveBufferMemSize(s32);

    static void LowLevelVoiceDisposeCallback(LowLevelVoice*, void*);

    void* GetFreeWaveBuffer();

private:
    WaveBufferPacket* m_pWaveBufferPacket;
    s32 m_WaveBufferPacketCount;
};
static_assert(sizeof(LowLevelVoiceCommand) == 0x320);
} // namespace nn::atk::detail