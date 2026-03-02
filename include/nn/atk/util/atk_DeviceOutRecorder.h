#pragma once

#include <atomic>

#include <nn/os.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/util/util_BytePtr.h>

#include <nn/atk/detail/atk_WavOutFileStream.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>
#include <nn/atk/util/atk_Global.h>

namespace nn::atk {
class DeviceOutRecorder : detail::fnd::Thread::Handler {
public:
    enum State {
        State_NotInitialized,
        State_Initialized,
        State_Recording,
        State_Recorded,
    };

    enum Message {
        Message_Prepare,
        Message_WriteSamples,
        Message_RequestStop,
        Message_Exit,
    };

    constexpr static u32 RecordingBufferSize = 0x5a000;
    constexpr static u32 DefaultWriteBlockPerSamples = 0x10000;
    constexpr static u32 RequiredThreadStackSize = 0x10000;

    struct RecordingOptions {
        u32 m_Channels;
        bool m_IsLeadSilenceTrimmingEnabled;
        u32 m_MaxFrames;
        u32 m_WriteBlockPerSamples;
    };
    static_assert(sizeof(RecordingOptions) == 0x10);

    struct InitializationOptions {
        u32 m_Priority;
        s32 m_IdealCoreNumber;
    };
    static_assert(sizeof(InitializationOptions) == 0x8);

    struct RecorderBuffer {
        struct WriteState {
            u32 channelIndex;
            u32 writtenSampleCount;
        };
        static_assert(sizeof(WriteState) == 0x8);

        explicit RecorderBuffer(const char* deviceName);

        void Initialize(s16* sampleBuffer, u32 maxSamples);

        void UpdateMaxSamples();

        void Clear();

        void Finalize();

        u32 Push(const s16* sampleBuffer, u32 samples);
        u32 Pop(u32);
        s64 Peek();

        void SetReadBlockSamples(u32 readBlockSamples);

        s16* m_SampleBuffer;
        u32 m_MaxBufferSamples;
        u32 m_MaxSamples;
        std::atomic_uint m_ValidSamples;
        u32 m_ReadPosition;
        u32 m_WritePosition;
        u32 m_ReadBlockSamples;
        WriteState m_WriteState;
        char* m_DeviceName;
    };
    static_assert(sizeof(RecorderBuffer) == 0x30);

    explicit DeviceOutRecorder(const char* deviceName);
    ~DeviceOutRecorder() override;

    void Finalize();

    bool Initialize(void*, size_t, void*, size_t);
    bool Initialize(void*, size_t, void*, size_t, const InitializationOptions& options);

    bool StartThread(u32, s32);

    static size_t GetRequiredMemorySizeForRecording();

    void Stop(bool);
    void StopThread();

    bool Start(detail::fnd::FileStream&, const RecordingOptions& options);

    s32 GetReadBlockSamples(u32) const;

    s32 Prepare();

    bool SendMessage(Message message);

    void RecordSamples(const s16* sampleBuffer, u32 samples);

    bool PostMessage(Message message);

    u32 Run(void* param) override;

    s32 OnPrepare();
    bool OnWriteSamples(bool);
    void OnRequestStop();
    void OnExit();

    u64 GetLeadSilenceSamples(const s16* sampleBuffer, u32 sampleCount, u32) const;
    u32 GetWritableSamples(u32) const;

    bool IsNoMoreSamples() const;

    void OnStart();
    void OnStop();

    u32 OnProcessSamples(s16* sampleBuffer, u32 samples);

private:
    State m_State;
    u32 m_Channels;
    OutputMode m_OutputMode;
    bool m_IsLeadSilenceTrimming;
    u32 m_MaxSamples;
    u32 m_WrittenSamples;
    detail::fnd::Thread m_Thread;
    void* m_ThreadStack;
    os::MessageQueue m_MessageQueue;
    std::uintptr_t m_Message;
    s32 m_MessageResult;
    os::Event m_MessageDoneEvent;
    detail::fnd::FileStream* m_Stream;
    detail::WavOutFileStream m_WavOutStream;
    RecorderBuffer m_RecordingBuffer;
    util::BytePtr m_WorkBuffer;
    u32 m_WriteBlockPerSamples;
};
static_assert(sizeof(DeviceOutRecorder) == 0x310);
} // namespace nn::atk