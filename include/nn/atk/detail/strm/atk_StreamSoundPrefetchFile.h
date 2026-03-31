#pragma once

#include <nn/atk/detail/strm/atk_StreamSoundFile.h>
#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk::detail {
class StreamSoundPrefetchFile {
public:
    struct PrefetchDataBlock;
    struct FileHeader : Util::SoundFileHeader {
        
        StreamSoundFile::InfoBlock* GetInfoBlock() const;
        StreamSoundFile::RegionBlock* GetRegionBlock() const;
        PrefetchDataBlock* GetPrefetchDataBlock() const;

        u32 GetPrefetchDataBlockSize() const;
        
        bool HasRegionBlock() const;
        u32 GetRegionBlockSize() const;
        u32 GetRegionBlockOffset() const;

    };

    struct PrefetchSample {

        void* GetSampleAddress();

        u8 data[1];
    };

    struct PrefetchData {
        
        PrefetchSample* GetPrefetchSample();

        u32 startFrame;
        u32 prefetchSize;
        u32 reserved[1];
        Util::Reference toPrefetchSample;
    };
    static_assert(sizeof(PrefetchData) == 0x14);

    struct PrefetchDataBlockBody {
        Util::Table<PrefetchData> prefetchDataTable;
    };

    struct PrefetchDataBlock {
        BinaryBlockHeader header;
        PrefetchDataBlockBody body;
    };
};
} // namespace nn::atk::detail