#pragma once

#include <nn/atk/atk_Global.h>
#include <nn/atk/detail/atk_BinaryFileFormat.h>
#include <nn/atk/detail/atk_Util.h>

namespace nn::atk::detail {
class StreamSoundFile {
public:
    struct FileHeader : BinaryFileHeader {
        Util::ReferenceWithSize toBlocks[4];

        bool HasSeekBlock();
        bool HasRegionBlock();
        
        u32 GetInfoBlockSize();
        u32 GetInfoBlockOffset();
        u32 GetSeekBlockOffset();
        u32 GetDataBlockOffset();
        u32 GetRegionBlockOffset();
    };
    static_assert(sizeof(FileHeader) == 0x44);

    struct StreamSoundInfo {
        u8 encodeMethod;
        bool isLoop;
        u8 channelCount;
        u8 regionCount;
        u32 sampleRate;
        u32 loopStart;
        u32 frameCount;
        u32 blockCount;
        u32 oneBlockBytes;
        u32 oneBlockSamples;
        u32 lastBlockBytes;
        u32 lastBlockSamples;
        u32 lastBlockPaddedBytes;
        u32 sizeofSeekInfoAtom;
        u32 seekInfoIntervalSamples;
        Util::Reference sampleDataOffset;
        u16 regionInfoBytes;
        u8 padding[2];
        Util::Reference regionDataOffset;
        u32 originalLoopStart;
        u32 originalLoopEnd;
        u32 crc32Value;
    };
    static_assert(sizeof(StreamSoundInfo) == 0x50);

    struct TrackInfo {
        u8 volume;
        u8 pan;
        u8 span;
        u8 flags;

        Util::Reference toGlobalChannelIndexTable;
    };

    struct TrackInfoTable {
        Util::ReferenceTable table;

        TrackInfo* GetTrackInfo(u32 index);
    };

    struct DspAdpcmChannelInfo {
        DspAdpcmParam param;
        DspAdpcmLoopParam loopParam;
    };
    static_assert(sizeof(DspAdpcmChannelInfo) == 0x2c);

    struct ChannelInfo {
        Util::Reference toDetailChannelInfo;
        
        DspAdpcmChannelInfo* GetDspAdpcmChannelInfo();
    };
    static_assert(sizeof(ChannelInfo) == 0x8);

    struct ChannelInfoTable {
        Util::ReferenceTable table;
    
        ChannelInfo* GetChannelInfo(u32 index);
    };

    struct InfoBlockBody {
        Util::Reference toStreamSoundInfo;
        Util::Reference toTrackInfoTable;
        Util::Reference toChannelInfoTable;

        StreamSoundInfo* GetStreamSoundInfo();
        TrackInfoTable* GetTrackInfoTable();
        ChannelInfoTable* GetChannelInfoTable();
    };
    static_assert(sizeof(InfoBlockBody) == 0x18);

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };
    static_assert(sizeof(InfoBlock) == 0x20);

    struct RegionInfo {
        u32 start;
        u32 end;
        DspAdpcmLoopParam adpcmContext[16];
        bool isEnabled;
        u8 padding[87];
        char regionName[64];
    };
    static_assert(sizeof(RegionInfo) == 0x100);

    struct RegionBlock {
        BinaryBlockHeader header;
        RegionInfo info;
    };
    static_assert(sizeof(RegionBlock) == 0x108);

    struct GlobalChannelIndexTable {
        Util::Table<u8> table;
    };
};
} // namespace nn::atk::detail