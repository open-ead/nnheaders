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

bool StreamSoundFileReader::IsOriginalLoopAvailable() const {
    return IsOriginalLoopAvailableImpl(m_pHeader);
}

bool StreamSoundFileReader::IsOriginalLoopAvailableImpl(const StreamSoundFile::FileHeader* pHeader) {
    return pHeader->version >= IncludeOriginalloopVersionStm;
}

bool StreamSoundFileReader::IsCrc32CheckAvailable() const {
    auto& header {*util::ConstBytePtr(m_pHeader).Get<BinaryFileHeader>()};

    return header.version >= IncludeCrc32CheckVersionStm;
}

bool StreamSoundFileReader::IsRegionIndexCheckAvailable() const {
    auto& header {*util::ConstBytePtr(m_pHeader).Get<BinaryFileHeader>()};

    return header.version >= IncludeRegionIndexCheckVersionStm;
}

bool StreamSoundFileReader::IsValidFileHeader(const void* streamSoundFile) {
    const BinaryFileHeader& header {*util::ConstBytePtr(streamSoundFile).Get<BinaryFileHeader>()};
    
    bool isSupportedVersion {header.signature == SignatureFileStm 
                             && header.byteOrder == BinaryFileHeader::ValidByteOrderMark 
                             && header.version >= SupportedFileVersionStm 
                             && header.version <= CurrentFileVersionStm};

    return isSupportedVersion;
}

bool StreamSoundFileReader::ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* strmInfo) const {
    auto* info {m_pInfoBlockBody->GetStreamSoundInfo()};

    strmInfo->encodeMethod = info->encodeMethod;
    strmInfo->isLoop = info->isLoop;
    strmInfo->channelCount = info->channelCount;
    strmInfo->regionCount = info->regionCount;
    strmInfo->sampleRate = info->sampleRate;
    strmInfo->loopStart = info->loopStart;
    strmInfo->frameCount = info->frameCount;
    strmInfo->blockCount = info->blockCount;
    strmInfo->oneBlockBytes = info->oneBlockBytes;
    strmInfo->oneBlockSamples = info->oneBlockSamples;
    strmInfo->lastBlockBytes = info->lastBlockBytes;
    strmInfo->lastBlockSamples = info->lastBlockSamples;
    strmInfo->lastBlockPaddedBytes = info->lastBlockPaddedBytes;
    strmInfo->sizeofSeekInfoAtom = info->sizeofSeekInfoAtom;
    strmInfo->seekInfoIntervalSamples = info->seekInfoIntervalSamples;
    strmInfo->sampleDataOffset = info->sampleDataOffset;
    strmInfo->regionInfoBytes = info->regionInfoBytes;
    strmInfo->regionDataOffset = info->regionDataOffset;
    
    if (IsOriginalLoopAvailable()) {
        strmInfo->originalLoopStart = info->originalLoopStart;
        strmInfo->originalLoopEnd = info->originalLoopEnd;
    }
    else {
        strmInfo->originalLoopStart = info->loopStart;
        strmInfo->originalLoopEnd = info->frameCount;
    }
    
    if (IsCrc32CheckAvailable()) {
        strmInfo->crc32Value = info->crc32Value;
    }
    else {
        strmInfo->crc32Value = 0;
    }

    return true;
}

bool StreamSoundFileReader::ReadStreamTrackInfo(TrackInfo* pTrackInfo, int trackIndex) const {
    auto* table {m_pInfoBlockBody->GetTrackInfoTable()};
    if (table != nullptr && trackIndex < static_cast<int>(table->GetTrackCount())) {
        auto* src {table->GetTrackInfo(trackIndex)};
        pTrackInfo->volume = src->volume;
        pTrackInfo->pan = src->pan;
        pTrackInfo->span = src->span;
        pTrackInfo->flags = src->flags;
        pTrackInfo->channelCount = src->GetTrackChannelCount();
        
        u32 count {pTrackInfo->channelCount};
        if (count > 1)
            count = 2;
        
        for (u32 i {0}; i < count; ++i)
            pTrackInfo->globalChannelIndex[i] = src->GetGlobalChannelIndex(i);

        return true;
    }

    return false;
}

bool StreamSoundFileReader::ReadDspAdpcmChannelInfo(DspAdpcmParam* pParam, DspAdpcmLoopParam* pLoopParam,
                                                    int channelIndex) const {
    auto* src {m_pInfoBlockBody->GetChannelInfoTable()->GetChannelInfo(channelIndex)->GetDspAdpcmChannelInfo()};
    if (src != nullptr) {
        *pParam = src->param;
        *pLoopParam = src->loopParam;
        return true;
    }

    return false;
}
} // namespace nn::atk::detail