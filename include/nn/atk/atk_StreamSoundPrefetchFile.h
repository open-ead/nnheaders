#pragma once

#include <nn/atk/atk_StreamSoundFile.h>
#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {
class StreamSoundPrefetchFile {
public:
    struct PrefetchDataBlock;
    struct FileHeader : Util::SoundFileHeader {

        const StreamSoundFile::InfoBlock* GetInfoBlock() const;
        const StreamSoundFile::RegionBlock* GetRegionBlock() const;
        const PrefetchDataBlock* GetPrefetchDataBlock() const;

        u32 GetPrefetchDataBlockSize() const;
        
        bool HasRegionBlock() const;
        u32 GetRegionBlockSize() const;
        u32 GetRegionBlockOffset() const;
    };

    struct PrefetchSample;
    struct PrefetchData {
        u32 startFrame;
        u32 prefetchSize;
        u32 reserved[1];
        Util::Reference toPrefetchSample;

        const PrefetchSample* GetPrefetchSample() const;
    };
    static_assert(sizeof(PrefetchData) == 0x14);

    struct PrefetchDataBlockBody {
        Util::Table<PrefetchData> prefetchDataTable;

        u32 GetPrefetchDataCount() const { return prefetchDataTable.count; }
        const PrefetchData* GetPrefetchData(u32 index) const { return &prefetchDataTable.item[index]; }
    };

    struct PrefetchDataBlock {
        BinaryBlockHeader header;
        PrefetchDataBlockBody body;
    };

    struct PrefetchSample {
        u8 data[1];

        const void* GetSampleAddress() const;
    };
};
} // namespace nn::atk::detail