#pragma once

#include <nn/os.h>

#include <nn/atk/detail/atk_Config.h>

namespace nn::atk::detail {
namespace driver {
class StreamSoundPlayer;
} // namespace nn::atk::detail::driver

class IStreamDataDecoder {
public:
    enum DecodeType {
        DecodeType_Normal,
        DecodeType_Loop,
        DecodeType_Idling,
        DecodeType_Count
    };

    struct DataInfo {
        s32 channelCount;
        s32 sampleRate;
        s32 blockSampleCount;
        std::size_t blockSize;
    };
    static_assert(sizeof(DataInfo) == 0x18);

    struct DecodeProfile {
        os::Tick decodeTick;
        s32 decodedSampleCount;
        os::Tick fsAccessTick;
        std::size_t fsReadSize;
    };
    static_assert(sizeof(DecodeProfile) == 0x20);

    struct CacheProfile {
        position_t cacheStartPosition;
        std::size_t cachedLength;
        position_t cacheCurrentPosition;
        driver::StreamSoundPlayer* player; 
    };
    static_assert(sizeof(CacheProfile) == 0x20);

    virtual ~IStreamDataDecoder();
};
} // namespace nn::atk::detail