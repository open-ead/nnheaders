#pragma once

#include <nn/atk/detail/atk_Util.h>
#include <nn/atk/detail/atk_BinaryFileFormat.h>
#include <nn/atk/detail/atk_StreamSoundFile.h>

namespace nn::atk::detail {
class StreamSoundPrefetchFile {
public:
    struct PrefetchDataBlock;
    struct FileHeader : Util::SoundFileHeader {
        
        StreamSoundFile::InfoBlock* GetInfoBlock();

        u32 GetPrefetchDataBlockSize();
        PrefetchDataBlock* GetPrefetchDataBlock();
        
        bool HasRegionBlock();
        u32 GetRegionBlockOffset();
        u32 GetRegionBlockSize();
        StreamSoundFile::RegionBlock* GetRegionBlock();
    };

    struct PrefetchSample {
        u8 data[1];

        void* GetSampleAddress();
    };

    struct PrefetchData {
        u32 startFrame;
        u32 prefetchSize;
        u32 reserved[1];
        Util::Reference toPrefetchSample;

        PrefetchSample* GetPrefetchSample();
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