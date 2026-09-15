#pragma once

#include <nn/atk/atk_OutputReceiver.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundHandle.h>
#include <nn/atk/atk_StreamBufferPool.h>
#include <nn/atk/detail/atk_RegionManager.h>

namespace nn::atk {

class SoundStartable {
public:
    class StartResult {
    public:
        enum ResultCode {
            ResultCode_Success,
            ResultCode_ErrorLowPriority,
            ResultCode_ErrorInvalidLabelString,
            ResultCode_ErrorInvalidSoundId,
            ResultCode_CanceledBySinglePlay,

            ResultCode_ErrorCanceledForPrioritizingOldest = ResultCode_CanceledBySinglePlay,
            ResultCode_ErrorNotDataLoaded,
            ResultCode_ErrorNotSequenceLoaded,
            ResultCode_ErrorNotBankLoaded,
            ResultCode_ErrorNotWsdLoaded,
            ResultCode_ErrorNotWarcLoaded,
            ResultCode_ErrorNotEnoughPlayerHeap,
            ResultCode_ErrorCannotOpenFile,
            ResultCode_ErrorNotAvailable,
            ResultCode_ErrorCannotAllocateTrack,
            ResultCode_ErrorNotEnoughInstance,
            ResultCode_ErrorInvalidParameter,
            ResultCode_ErrorInvalidSequenceStartLocationLabel,
            ResultCode_ErrorActorNotInitialized,
            ResultCode_ErrorInvalidWarcId,
            ResultCode_ErrorInvalidBankData,
            ResultCode_ErrorInvalidStreamFileId,
            ResultCode_ErrorInvalidStreamFilePath,

            ResultCode_ErrorUser = 128,
            ResultCode_ErrorUnknown = 255,
        };

        StartResult() = default;
        explicit StartResult(ResultCode code) : m_Code{code} {}

        bool IsSuccess() const { return m_Code == ResultCode_Success; }

        ResultCode GetCode() const { return m_Code; }

    private:
        ResultCode m_Code;
    };

    struct StartInfo {
        enum EnableFlagBit {
            EnableFlagBit_StartOffset = 1 << 0,
            EnableFlagBit_PlayerId = 1 << 1,
            EnableFlagBit_PlayerPriority = 1 << 2,
            EnableFlagBit_ActorPlayerId = 1 << 3,
            EnableFlagBit_SequenceSoundInfo = 1 << 4,
            EnableFlagBit_StreamSoundInfo = 1 << 5,
            EnableFlagBit_WaveSoundInfo = 1 << 6,
            EnableFlagBit_VoiceRendererType = 1 << 7,
            EnableFlagBit_FadeFrame = 1 << 8,
            EnableFlagBit_SoundStopCallback = 1 << 9,
            EnableFlagBit_StreamSoundMetaInfo = 1 << 10,
            EnableFlagBit_StreamSoundMetaInfo2 = 1 << 11,
            EnableFlagBit_DelayTime = 1 << 12,
            EnableFlagBit_DelayCount = 1 << 13,
            EnableFlagBit_UpdateType = 1 << 14,
            EnableFlagBit_SubMixIndex = 1 << 15,
            EnableFlagBit_OutputReceiver = 1 << 16,
        };

        enum StartOffsetType {
            StartOffsetType_MilliSeconds,
            StartOffsetType_Tick,
            StartOffsetType_Sample,
        };

        struct SequenceSoundInfo {
            const void* sequenceDataAddress;
            const char* startLocationLabel;
            SoundArchive::ItemId bankIds[4];

            SequenceSoundInfo() = default;
        };
        static_assert(sizeof(SequenceSoundInfo) == 0x20);

        struct StreamSoundInfo {
            const char* externalPath;
            const void* pExternalData;
            size_t externalDataSize;
            StreamRegionCallback regionCallback;
            void* regionCallbackArg;
            const void* prefetchData;
            bool forcePlayPrefetchFlag;
            detail::driver::StreamBufferPool* pStreamBufferPool;

            StreamSoundInfo() = default;
        };
        static_assert(sizeof(StreamSoundInfo) == 0x40);

        struct WaveSoundInfo {
            enum EnableParameterFlagBit {
                EnableParameterFlagBit_Release = 1,
                EnableParameterFlagBit_ContextCalculationSkipMode,
            };

            const void* waveAddress;
            s8 waveType;
            u8 m_Padding[3];
            int enableParameterFlag;
            int release;
            bool isContextCalculationSkipMode;

            WaveSoundInfo() = default;
        };
        static_assert(sizeof(WaveSoundInfo) == 0x18);

        u32 enableFlag;
        StartOffsetType startOffsetType;
        int startOffset;
        SoundArchive::ItemId playerId;
        int playerPriority;
        int actorPlayerId;
        SequenceSoundInfo sequenceSoundInfo;
        StreamSoundInfo streamSoundInfo;
        SoundArchive::StreamSoundInfo streamSoundMetaInfo;
        SoundArchive::StreamSoundInfo2 streamSoundMetaInfo2;
        WaveSoundInfo waveSoundInfo;
        u8 voiceRendererType;
        int fadeFrame;
        SoundStopCallback soundStopCallback;
        int delayTime;
        int delayCount;
        UpdateType updateType;
        int subMixIndex;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        OutputReceiver* pOutputReceiver;
#endif

        StartInfo() = default;
    };

    virtual ~SoundStartable() = default;

    StartResult StartSound(SoundHandle* handle, SoundArchive::ItemId soundId,
                           const StartInfo* startInfo);

    StartResult StartSound(SoundHandle* handle, SoundArchive::ItemId soundId,
                           const char* soundArchiveName, const StartInfo* startInfo);

    StartResult StartSound(SoundHandle* handle, const char* soundName, const StartInfo* startInfo);

    StartResult StartSound(SoundHandle* handle, const char* soundName, const char* soundArchiveName,
                           const StartInfo* startInfo);

    StartResult HoldSound(SoundHandle* handle, SoundArchive::ItemId soundId,
                          const StartInfo* startInfo);

    StartResult HoldSound(SoundHandle* handle, SoundArchive::ItemId soundId,
                          const char* soundArchiveName, const StartInfo* startInfo);

    StartResult HoldSound(SoundHandle* handle, const char* soundLabel, const StartInfo* startInfo);

    StartResult HoldSound(SoundHandle* handle, const char* soundLabel, const char* soundArchiveName,
                          const StartInfo* startInfo);

    StartResult PrepareSound(SoundHandle* handle, SoundArchive::ItemId soundId,
                             const StartInfo* startInfo);

    StartResult PrepareSound(SoundHandle* handle, SoundArchive::ItemId soundId,
                             const char* soundArchiveName, const StartInfo* startInfo);

    StartResult PrepareSound(SoundHandle* handle, const char* soundName,
                             const StartInfo* startInfo);

    StartResult PrepareSound(SoundHandle* handle, const char* soundName,
                             const char* soundArchiveName, const StartInfo* startInfo);

protected:
    virtual StartResult detail_SetupSound(SoundHandle* handle, u32 soundId, bool holdFlag,
                                          const char* soundArchiveName,
                                          const StartInfo* startInfo) = 0;

    virtual SoundArchive::ItemId detail_GetItemId(char* pString) = 0;
    virtual SoundArchive::ItemId detail_GetItemId(char* pString, const char* soundArchiveName) = 0;
};
static_assert(sizeof(SoundStartable) == 0x8);

}  // namespace nn::atk
