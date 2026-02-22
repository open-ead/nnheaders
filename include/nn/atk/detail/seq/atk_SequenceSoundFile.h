#pragma once

#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk::detail {
struct SequenceSoundFile {
    struct DataBlock;
    struct LabelBlock;
    struct FileHeader : Util::SoundFileHeader {
        DataBlock* GetDataBlock();
        LabelBlock* GetLabelBlock();
    };

    struct LabelInfo {
        Util::Reference referToSequenceData;
        u32 labelStringLength;
        char label[1];
    };

    struct LabelBlockBody {
        Util::ReferenceTable labelInfoReferenceTable;

        LabelInfo* GetLabelInfo(s32 index) const;
        char* GetLabel(s32 index) const;
        char* GetLabelByOffest(s32 labelOffset) const;

        u32 GetOffset(s32 index, u32* offsetPtr) const;
        bool GetOffsetByLabel(char* label, u32* offsetPtr) const;
    };

    struct LabelBlock {
        BinaryBlockHeader header;
        LabelBlockBody body;
    };

    struct DataBlockBody {
        u8 sequenceData[1];
    };

    struct DataBlock {
        BinaryBlockHeader header;
        DataBlockBody body;
    };
};
} // namespace nn::atk::detail