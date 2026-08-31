#include <nn/atk/atk_SequenceSoundFile.h>

#include <cstring>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const SequenceSoundFile::DataBlock* SequenceSoundFile::FileHeader::GetDataBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_SequenceSoundFile_DataBlock))
            .Get<DataBlock>();
}

const SequenceSoundFile::LabelBlock* SequenceSoundFile::FileHeader::GetLabelBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_SequenceSoundFile_LabelBlock))
            .Get<LabelBlock>();
}

const SequenceSoundFile::LabelInfo* SequenceSoundFile::LabelBlockBody::GetLabelInfo(int index) const {
    return util::ConstBytePtr(labelInfoReferenceTable.GetReferedItem(index)).Get<LabelInfo>();
}

const char* SequenceSoundFile::LabelBlockBody::GetLabel(int index) const {
    const LabelInfo* labelInfo {GetLabelInfo(index)};
    return labelInfo->label;
}

const char* SequenceSoundFile::LabelBlockBody::GetLabelByOffset(u32 offset) const {
    for (int i {0}; i < GetLabelCount(); ++i) {
        const LabelInfo* labelInfo {GetLabelInfo(i)};
        if (static_cast<u32>(labelInfo->referToSequenceData.offset) == offset)
            return labelInfo->label;
    }

    return nullptr;
}

bool SequenceSoundFile::LabelBlockBody::GetOffset(int index, u32* offsetPtr) const {
    const LabelInfo* labelInfo {GetLabelInfo(index)};
    *offsetPtr = labelInfo->referToSequenceData.offset;
    return true;
}

bool SequenceSoundFile::LabelBlockBody::GetOffsetByLabel(const char* label, u32* offsetPtr) const {
    const size_t labelLength {strlen(label)};
    for (int i {0}; i < GetLabelCount(); ++i) {
        const LabelInfo* labelInfo {GetLabelInfo(i)};
        if (strncmp(label, labelInfo->label, labelLength) == 0) {
            *offsetPtr = labelInfo->referToSequenceData.offset;
            return true;
        }
    }

    return false;
}
} // namespace nn::atk::detail