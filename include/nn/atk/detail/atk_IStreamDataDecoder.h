#pragma once

#include <nn/os.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_Config.h>

namespace nn::atk::detail {
namespace driver {

class StreamSoundPlayer;

}  // namespace driver

class IStreamDataDecoder {
public:
    enum DecodeType { DecodeType_Normal, DecodeType_Loop, DecodeType_Idling, DecodeType_Count };

    struct DataInfo {
        s32 channelCount;
        s32 sampleRate;
        s32 blockSampleCount;
        size_t blockSize;
    };
    static_assert(sizeof(DataInfo) == 0x18);

    struct DecodeProfile {
        os::Tick decodeTick;
        s32 decodedSampleCount;
        os::Tick fsAccessTick;
        size_t fsReadSize;
    };
    static_assert(sizeof(DecodeProfile) == 0x20);

    struct CacheProfile {
        position_t cacheStartPosition;
        size_t cachedLength;
        position_t cacheCurrentPosition;
        driver::StreamSoundPlayer* player;
    };
    static_assert(sizeof(CacheProfile) == 0x20);

    virtual ~IStreamDataDecoder();
};

class IStreamDataDecoderManager {
public:
    virtual ~IStreamDataDecoderManager() = default;
    virtual void Impl1();
    virtual void FreeImpl(IStreamDataDecoder* pStreamDataDecoder);

    util::IntrusiveListNode m_Link;
};

}  // namespace nn::atk::detail
