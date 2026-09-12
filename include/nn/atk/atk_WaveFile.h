#pragma once

#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {
struct WaveFile {
    struct InfoBlock;
    struct DataBlock;
    struct FileHeader : Util::SoundFileHeader {
        const InfoBlock* GetInfoBlock() const;
        const DataBlock* GetDataBlock() const;
    };

    struct ChannelInfo;
    struct InfoBlockBody {
        u8 encoding;
        u8 isLoop;
        u8 padding[2];
        u32 sampleRate;
        u32 loopStartFrame;
        u32 loopEndFrame;
        u32 originalLoopStartFrame;
        Util::ReferenceTable channelInfoReferenceTable;

        int GetChannelCount() const { return channelInfoReferenceTable.count; };
        const ChannelInfo& GetChannelInfo(int channelIndex) const;
    };

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    struct DspAdpcmInfo;
    struct ChannelInfo {
        Util::Reference referToSamples;
        Util::Reference referToAdpcmInfo;
        u32 reserved;

        const void* GetSamplesAddress(const void* dataBlockBodyAddress) const;
        const DspAdpcmInfo& GetDspAdpcmInfo() const;
    };
    static_assert(sizeof(ChannelInfo) == 0x14);

    struct DspAdpcmInfo {
        DspAdpcmParam adpcmParam;
        DspAdpcmLoopParam adpcmLoopParam;
    };
    static_assert(sizeof(DspAdpcmInfo) == 0x2c);

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