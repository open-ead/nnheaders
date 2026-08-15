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

bool StreamSoundPrefetchFileReader::IsCrc32CheckAvailable() const {
    return m_pHeader->header.version >= IncludeCrc32CheckVersionStp;
}

bool StreamSoundPrefetchFileReader::IsRegionIndexCheckAvailable() const {
    return m_pHeader->header.version >= IncludeRegionIndexCheckVersionStp;
}

bool StreamSoundPrefetchFileReader::ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* strmInfo) const {
    *strmInfo = *m_pInfoBlockBody->GetStreamSoundInfo();
    return true;
}

bool StreamSoundPrefetchFileReader::ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam,
                                                            int channelIndex) const {
    auto* src {m_pInfoBlockBody->GetChannelInfoTable()->GetChannelInfo(channelIndex)->GetDspAdpcmChannelInfo()};
    if (src != nullptr) {
        *pParam = src->param;
        *pLoopParam = src->loopParam;
        return true;
    }

    return false;
}

bool StreamSoundPrefetchFileReader::ReadPrefetchDataInfo(PrefetchDataInfo* pDataInfo, int prefetchIndex) const {
    auto* data {m_pPrefetchDataBlockBody->GetPrefetchData(prefetchIndex)};
    pDataInfo->startFrame = data->startFrame;
    pDataInfo->prefetchSize = data->prefetchSize;

    auto* sample {data->GetPrefetchSample()};
    pDataInfo->dataAddress = sample->GetSampleAddress();

    return true;
}

bool StreamSoundPrefetchFileReader::ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo, u32 regionIndex) const { // 179
    if (m_RegionDataOffset != 0 && m_RegionInfoBytes != 0) {
        position_t offset = m_RegionDataOffset + static_cast<long>(m_RegionInfoBytes) * regionIndex;
        auto bytePtr {util::ConstBytePtr(m_pHeader)};
        *pInfo = *bytePtr.Advance(offset).Get<StreamSoundFile::RegionInfo>();
        return true;
    }

    return false;
}

} // namespace nn::atk::detail