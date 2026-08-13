#pragma once

#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {
struct StreamSoundFile {
    struct InfoBlock;

    struct FileHeader : BinaryFileHeader {
    private:
        static const int BlockCount = 4;
    
    public:
        Util::ReferenceWithSize toBlocks[BlockCount];

        bool HasSeekBlock() const;
        bool HasRegionBlock() const;

        u32 GetInfoBlockSize() const;
        u32 GetSeekBlockSize() const;
        u32 GetDataBlockSize() const;
        u32 GetRegionBlockSize() const;

        u32 GetInfoBlockOffset() const;
        u32 GetSeekBlockOffset() const;
        u32 GetDataBlockOffset() const;
        u32 GetRegionBlockOffset() const;

        const InfoBlock* GetInfoBlock() const {
            return util::ConstBytePtr(this).Advance(GetInfoBlockOffset()).Get<InfoBlock>();
        };

    private:
        const Util::ReferenceWithSize* GetReferenceBy(u16 typeId) const;
    };
    static_assert(sizeof(FileHeader) == 0x44);

    struct StreamSoundInfo;
    struct TrackInfoTable;
    struct ChannelInfoTable;
    struct InfoBlockBody {
        Util::Reference toStreamSoundInfo;
        Util::Reference toTrackInfoTable;
        Util::Reference toChannelInfoTable;

        const StreamSoundInfo* GetStreamSoundInfo() const;
        const TrackInfoTable* GetTrackInfoTable() const;
        const ChannelInfoTable* GetChannelInfoTable() const;
    };
    static_assert(sizeof(InfoBlockBody) == 0x18);

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };
    static_assert(sizeof(InfoBlock) == 0x20);

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

    struct TrackInfo;
    struct TrackInfoTable {
        Util::ReferenceTable table;

        u32 GetTrackCount() const { return table.count; }

        const TrackInfo* GetTrackInfo(u32 index) const;
    };

    struct GlobalChannelIndexTable;
    struct TrackInfo {
        u8 volume;
        u8 pan;
        u8 span;
        u8 flags;

        Util::Reference toGlobalChannelIndexTable;

        u32 GetTrackChannelCount() const {
            return GetGlobalChannelIndexTable().GetCount();
        }
        
        u8 GetGlobalChannelIndex(u32 index) const {
            return GetGlobalChannelIndexTable().GetGlobalIndex(index);
        }

    private:
        const GlobalChannelIndexTable& GetGlobalChannelIndexTable() const {
            return *util::ConstBytePtr(this)
                    .Advance(toGlobalChannelIndexTable.offset)
                    .Get<GlobalChannelIndexTable>();
        }
    };
    static_assert(sizeof(TrackInfo) == 0xc);

    struct GlobalChannelIndexTable {
        Util::Table<u8> table;

        u32 GetCount() const { return table.count; }
        u8 GetGlobalIndex(u32 index) const { return table.item[index]; }
    };

    struct ChannelInfo;
    struct ChannelInfoTable {
        Util::ReferenceTable table;
        
        u32 GetChannelCount() const;
        const ChannelInfo* GetChannelInfo(u32 index) const;
    };

    struct DspAdpcmChannelInfo;
    struct ChannelInfo {
        Util::Reference toDetailChannelInfo;
        
        const DspAdpcmChannelInfo* GetDspAdpcmChannelInfo() const;
    };
    static_assert(sizeof(ChannelInfo) == 0x8);

    struct DspAdpcmChannelInfo {
        DspAdpcmParam param;
        DspAdpcmLoopParam loopParam;
    };
    static_assert(sizeof(DspAdpcmChannelInfo) == 0x2c);

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
};
} // namespace nn::atk::detail