#include <nn/atk/atk_StreamSoundPrefetchFileReader.h>

namespace nn::atk::detail {
const u32 SignatureFileStp {0x50545346}; // FSTP
const u32 SignatureInfoBlockStp {0x4f464e49}; // INFO
const u32 SignaturePrefetchdataBlockStp {0x54414450}; // PDAT

const int SupportedFileVersionStp {0x10000};
const int CurrentFileVersionStp {0x50200};

const int IncludeRegionInfoVersionStp {0x30000};
const int IncludeCrc32CheckVersionStp {0x40000};
const int IncludeRegionIndexCheckVersionStp {0x50100};

StreamSoundPrefetchFileReader::StreamSoundPrefetchFileReader() = default;

StreamSoundPrefetchFileReader::~StreamSoundPrefetchFileReader() {
    Finalize();
}

bool StreamSoundPrefetchFileReader::IsValidFileHeader(const void* streamSoundPrefetchFile) const {
    const BinaryFileHeader& header {*util::ConstBytePtr(streamSoundPrefetchFile).Get<BinaryFileHeader>()};
    
    bool isSupportedVersion {header.signature == SignatureFileStp 
                             && header.byteOrder == BinaryFileHeader::ValidByteOrderMark 
                             && header.version >= SupportedFileVersionStp 
                             && header.version <= CurrentFileVersionStp};

    return isSupportedVersion;
}

void StreamSoundPrefetchFileReader::Initialize(const void* streamSoundPrefetchFile) {
    if (IsValidFileHeader(streamSoundPrefetchFile)) {
        m_pHeader = util::ConstBytePtr(streamSoundPrefetchFile).Get<StreamSoundPrefetchFile::FileHeader>();
        auto* infoBlock {m_pHeader->GetInfoBlock()};
        if (infoBlock->header.kind == SignatureInfoBlockStp) {
            auto* dataBlock {m_pHeader->GetPrefetchDataBlock()};
            if (dataBlock->header.kind == SignaturePrefetchdataBlockStp) {
                m_pInfoBlockBody = &infoBlock->body;
                m_pPrefetchDataBlockBody = &dataBlock->body;

                m_RegionDataOffset = GetRegionDataOffset();
                m_RegionInfoBytes = GetRegionInfoBytes();
            }
        }
    }
}

bool StreamSoundPrefetchFileReader::IsIncludeRegionInfo() const {
    return m_pHeader->header.version >= IncludeRegionInfoVersionStp;
}

} // namespace nn::atk::detail