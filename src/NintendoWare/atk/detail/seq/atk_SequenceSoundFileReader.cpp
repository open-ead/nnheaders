#include <nn/atk/atk_SequenceSoundFileReader.h>

namespace nn::atk::detail {

namespace {

const u32 SignatureDataBlockSeq{0x41544144};   // DATA
const u32 SignatureLabelBlockSeq{0x4c42414c};  // LABL

const u32 SupportedFileVersionSeq{0x10000};
const u32 CurrentFileVersionSeq{0x20000};

bool IsValidFileHeaderSeq(const void* sequenceFile) {
    const BinaryFileHeader* header{util::ConstBytePtr(sequenceFile).Get<BinaryFileHeader>()};

    bool isSupportedVersion{header->signature == SequenceSoundFileReader::SignatureFile &&
                            header->byteOrder == BinaryFileHeader::ValidByteOrderMark &&
                            header->version >= SupportedFileVersionSeq &&
                            header->version <= CurrentFileVersionSeq};

    return isSupportedVersion;
}

}  // anonymous namespace

SequenceSoundFileReader::SequenceSoundFileReader(const void* sequenceFile) {
    if (!IsValidFileHeaderSeq(sequenceFile))
        return;

    m_pHeader = reinterpret_cast<const SequenceSoundFile::FileHeader*>(sequenceFile);

    const SequenceSoundFile::DataBlock* dataBlock{m_pHeader->GetDataBlock()};
    if (dataBlock->header.kind != SignatureDataBlockSeq)
        return;

    const SequenceSoundFile::LabelBlock* labelBlock{m_pHeader->GetLabelBlock()};
    if (labelBlock->header.kind != SignatureLabelBlockSeq)
        return;

    m_pDataBlockBody = &dataBlock->body;
    m_pLabelBlockBody = &labelBlock->body;
}

const void* SequenceSoundFileReader::GetSequenceData() const {
    return m_pDataBlockBody;
}

}  // namespace nn::atk::detail