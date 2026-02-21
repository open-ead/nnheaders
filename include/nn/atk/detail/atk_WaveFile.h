#pragma once

#include <nn/atk/detail/atk_Util.h>

namespace nn::atk::detail {
struct WaveFile {
    struct InfoBlock;
    struct DataBlock;
    struct FileHeader : Util::SoundFileHeader {
        InfoBlock* GetInfoBlock();
        DataBlock* GetDataBlock();
    };

    struct ChannelInfo;
    struct InfoBlockBody {
        u8 encoding;
        u8 isLoop;
        u8 padding;
        u32 sampleRate;
        u32 loopStartFrame;
        u32 loopEndFrame;
        u32 originalLoopStartFrame;
        Util::ReferenceTable channelInfoReferenceTable;

        ChannelInfo* GetChannelInfo(s32 channelIndex);
    };

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    struct DspAdpcmInfo {
        DspAdpcmParam adpcmParam;
        DspAdpcmLoopParam adpcmLoopParam;
    };
    static_assert(sizeof(DspAdpcmInfo) == 0x2C);

    struct ChannelInfo {
        Util::Reference referToSamples;
        Util::Reference referToAdpcmInfo;
        u32 reserved;

        void* GetSamplesAddress(void* dataBlockBodyAddress);
        DspAdpcmInfo* GetDspAdpcmInfo();
    };
    static_assert(sizeof(ChannelInfo) == 0x14);

    struct DataBlock {
        BinaryBlockHeader header;
        union {
            s8 pcm8[1];
            s16 pcm16[1];
            u8 byte[1];
        };
    };
};
} // namespace nn::atk::detail