#pragma once

#include <atomic>

#include <nn/os.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/util/util_BytePtr.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_WavOutFileStream.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>

namespace nn::atk {
class DeviceOutRecorder : public detail::fnd::Thread::Handler {
    NN_NO_COPY(DeviceOutRecorder);
public:
    static const u32 RecordingBufferSize = 0x5a000;
    static const u32 DefaultWriteBlockPerSamples = 0x10000;
    static const u32 RequiredThreadStackSize = 0x10000;

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

    class InitializationOptions {
    public:
        InitializationOptions() = default;

        u32 GetPrioritiy() const { return m_Priority; }
        void SetPriority(u32 value) { m_Priority = value; }

        int GetIdealCoreNumber() const { return m_IdealCoreNumber; }
        void SetIdealCoreNumber(int value) { m_IdealCoreNumber = value; }

    private:
        u32 m_Priority;
        int m_IdealCoreNumber;
    };
    static_assert(sizeof(InitializationOptions) == 0x8);

    class RecordingOptions {
    public:
        RecordingOptions() = default;

        u32 GetChannels() const { return m_Channels; }
        void SetChannels(u32 value) { m_Channels = value; }

        bool IsLeadSilenceTrimmingEnabled() const { return m_IsLeadSilenceTrimmingEnabled; }
        void SetLeadSilenceTrimmingEnabled(bool value) { m_IsLeadSilenceTrimmingEnabled = value; }

        u32 GetMaxFrames() const { return m_MaxFrames; }
        void SetMaxFrames(u32 value) { m_MaxFrames = value; }

        u32 GetWriteBlockPerSamples() const { return m_WriteBlockPerSamples; }
        void SetWriteBlockPerSamples(u32 value) { m_WriteBlockPerSamples = value; }

    private:
        u32 m_Channels;
        bool m_IsLeadSilenceTrimmingEnabled;
        u32 m_MaxFrames;
        u32 m_WriteBlockPerSamples;
    };
    static_assert(sizeof(RecordingOptions) == 0x10);

    class RecorderBuffer {
        struct WriteState {
            u32 channelIndex;
            u32 writtenSampleCount;
        };
        static_assert(sizeof(WriteState) == 0x8);

        explicit RecorderBuffer(const char* deviceName);

        void Initialize(s16* sampleBuffer, u32 maxSamples);
        void Finalize();
        
        u32 Push(const s16* sampleBuffer, u32 samples);
        u32 Pop(u32 samples);
        s16* Peek();

        void SetReadBlockSamples(u32 value);
        void Clear();

        u32 GetReadableCount() const;
        u32 GetWritableCount() const;
        u32 GetContiguousReadableCount() const;
        const char* GetDeviceName() const;
        
        void UpdateMaxSamples();

    private:
        void Skip(u32 samples);
        void Write(const s16* sampleBuffer, u32 samples);
        u32 IncrementPosition(u32 position, u32 length) const;

        s16* m_SampleBuffer {nullptr};
        u32 m_MaxBufferSamples {0};
        u32 m_MaxSamples {0};
        std::atomic_uint m_ValidSamples {0};
        u32 m_ReadPosition {0};
        u32 m_WritePosition {0};
        u32 m_ReadBlockSamples {1};
        WriteState m_WriteState;
        const char* m_DeviceName;
    };
    static_assert(sizeof(RecorderBuffer) == 0x30);

protected:
    explicit DeviceOutRecorder(const char* deviceName);

public:
    ~DeviceOutRecorder() override;

    bool Initialize(void* recordingBuffer, size_t recordingBufferSize, 
                    void* pThreadStack, size_t threadStackSize);
    bool Initialize(void* recordingBuffer, size_t recordingBufferSize, 
                    void* pThreadStack, size_t threadStackSize, const InitializationOptions& options);

    void Finalize();

    size_t GetRequiredMemorySizeForRecording();

    bool Start(detail::fnd::FileStream& fileStream);
    bool Start(detail::fnd::FileStream& fileStream, const RecordingOptions& options);

    void Stop(bool isBlocking);

    bool IsInitialized() const;

    State GetState() const;

    bool IsLeadSilenceTrimming() const;

    u32 GetRecordingChannels() const;

    void RecordSamples(const s16* sampleBuffer, u32 samples);

protected:
    OutputMode GetOutputMode() const;

    virtual u32 GetMaxFrameLength() const = 0;
    virtual u32 GetSamplesPerSec() const = 0;
    virtual u32 GetValidChannels() const = 0;

    virtual void OnStart();
    virtual void OnStop();
    virtual u32 OnProcessSamples(s16* sampleBuffer, u32 samples);

    s16 ResolveSampleEndian(s16 sample);

    u32 Run(void* param) override;

private:
    s32 GetReadBlockSamples(u32 channels) const;
    u32 GetLeadSilenceSamples(const s16* sampleBuffer, u32 samples, u32 channels) const;
    u32 GetWritableSamples(u32 samples) const;
    bool IsNoMoreSamples() const;

    bool StartThread(u32, s32);
    void StopThread();

    s32 Prepare();
    bool SendMessage(Message message);
    bool PostMessage(Message message);

    s32 OnPrepare();
    void OnRequestStop();
    void OnExit();
    bool OnWriteSamples(bool isForceWriteMode);

    volatile State m_State {State_NotInitialized};
    u32 m_Channels {0};
    OutputMode m_OutputMode {OutputMode_Stereo};
    bool m_IsLeadSilenceTrimming {false};
    u32 m_MaxSamples {0};
    u32 m_WrittenSamples {0};
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