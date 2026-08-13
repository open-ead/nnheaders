#include <nn/atk/atk_StreamSoundFileReader.h>

namespace nn::atk::detail {
const u32 SignatureFileStm {0x4d545346}; // FSTM
const u32 SignatureInfoBlockStm {0x4f464e49}; // INFO

const u32 SupportedFileVersionStm {0x10000};
const u32 CurrentFileVersionStm {0x60200};

const u32 IncludeTrackinfoVersionStm {0x20000};
const u32 IncludeOriginalloopVersionStm {0x40000};
const u32 IncludeCrc32CheckVersionStm {0x50000};
const u32 IncludeRegionIndexCheckVersionStm {0x60100};

StreamSoundFileReader::StreamSoundFileReader() = default;

void StreamSoundFileReader::Initialize(const void* streamSoundFile) {
    if (IsValidFileHeader(streamSoundFile)) {
        m_pHeader = util::ConstBytePtr(streamSoundFile).Get<StreamSoundFile::FileHeader>();
        const StreamSoundFile::InfoBlock* infoBlock {m_pHeader->GetInfoBlock()};
        if (infoBlock->header.kind == SignatureInfoBlockStm)
            m_pInfoBlockBody = &infoBlock->body;
    }
}

void StreamSoundFileReader::Finalize()  {
    m_pHeader = nullptr;
    m_pInfoBlockBody = nullptr;
}

bool StreamSoundFileReader::IsTrackInfoAvailable() const {
    auto& header {*util::ConstBytePtr(m_pHeader).Get<BinaryFileHeader>()};

    return header.version <= IncludeTrackinfoVersionStm;
}

bool StreamSoundFileReader::IsValidFileHeader(const void* streamSoundFile) {
    const BinaryFileHeader& header {*util::ConstBytePtr(streamSoundFile).Get<BinaryFileHeader>()};
    
    bool isSupportedVersion {header.signature == SignatureFileStm 
                             && header.byteOrder == BinaryFileHeader::ValidByteOrderMark 
                             && header.version >= SupportedFileVersionStm 
                             && header.version <= CurrentFileVersionStm};

    return isSupportedVersion;
}
} // namespace nn::atk::detail