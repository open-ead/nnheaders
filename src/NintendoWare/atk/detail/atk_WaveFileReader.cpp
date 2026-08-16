#include <nn/atk/atk_WaveFileReader.h>

namespace nn::atk::detail {
const u32 SignatureInfoBlockWav {0x4f464e49}; // INFO
const u32 SignatureDataBlockWav {0x41544144}; // DATA

const u32 SupportedFileVersionWav {0x10000};
const u32 CurrentFileVersionWav {0x10200};

const u32 IncludeOriginalLoopVersionWav {0x10200};

namespace {
bool IsValidFileHeaderWav(const void* waveFile) {
    const BinaryFileHeader& header {*util::ConstBytePtr(waveFile).Get<BinaryFileHeader>()};
    
    bool isSupportedVersion {header.signature == WaveFileReader::SignatureFile 
                                && header.byteOrder == BinaryFileHeader::ValidByteOrderMark 
                                && header.version >= SupportedFileVersionWav 
                                && header.version <= CurrentFileVersionWav};

    return isSupportedVersion;
}
}

SampleFormat WaveFileReader::GetSampleFormat(u8 format) {
    switch (format) {
    case 0:
        return SampleFormat_PcmS8;
    case 1:
        return SampleFormat_PcmS16;
    case 2:
        return SampleFormat_DspAdpcm;
    default:
        return SampleFormat_DspAdpcm;
    }
}

WaveFileReader::WaveFileReader(const void* waveFile, s8 waveType) {
    m_WaveType = waveType;
    switch (m_WaveType) {
    case WaveType::WaveType_Nwwav:
        if (IsValidFileHeaderWav(waveFile)) {
            m_pHeader = util::ConstBytePtr(waveFile).Get<WaveFile::FileHeader>();
            const WaveFile::InfoBlock* infoBlock {m_pHeader->GetInfoBlock()};
            const WaveFile::DataBlock* dataBlock {m_pHeader->GetDataBlock()};

            if (infoBlock != nullptr && infoBlock->header.kind == SignatureInfoBlockWav
                && dataBlock != nullptr && dataBlock->header.kind == SignatureDataBlockWav) {
                m_pInfoBlockBody = &infoBlock->body;
                m_pDataBlockBody = &dataBlock->byte;
            }
        }
        break;
    case WaveType::WaveType_Dspadpcm:
        m_DspadpcmReader.Initialize(waveFile);
        break;
    case WaveType::WaveType_Invalid: 
        break;
    }
}

bool WaveFileReader::IsOriginalLoopAvailable() const {
    const BinaryFileHeader& header {*util::ConstBytePtr(m_pHeader).Get<BinaryFileHeader>()};
    
    return header.version >= IncludeOriginalLoopVersionWav;
}

bool WaveFileReader::ReadWaveInfo(WaveInfo* info, const void* waveDataOffsetOrigin) const {
    switch (m_WaveType) {
    case WaveType_Nwwav: {
        const SampleFormat format {GetSampleFormat(m_pInfoBlockBody->encoding)};
        const int channelCount {m_pInfoBlockBody->GetChannelCount()};

        info->sampleFormat = format;
        info->channelCount = channelCount;
        info->sampleRate = m_pInfoBlockBody->sampleRate;
        info->loopFlag = m_pInfoBlockBody->isLoop == 1;
        info->loopStartFrame = m_pInfoBlockBody->loopStartFrame;
        info->loopEndFrame = m_pInfoBlockBody->loopEndFrame;
        info->dataSize = m_pHeader->header.fileSize - sizeof(Util::SoundFileHeader);

        if (IsOriginalLoopAvailable()) 
            info->originalLoopStartFrame = m_pInfoBlockBody->originalLoopStartFrame;
        else
            info->originalLoopStartFrame = m_pInfoBlockBody->loopStartFrame;

        for (int i {0}; i < channelCount; ++i) {
            if (i < 2) {
                WaveInfo::ChannelParam& channelParam {info->channelParam[i]};
                const WaveFile::ChannelInfo& channelInfo {m_pInfoBlockBody->GetChannelInfo(i)};
                
                if (channelInfo.referToAdpcmInfo.offset != 0) {
                    const WaveFile::DspAdpcmInfo& adpcmInfo {channelInfo.GetDspAdpcmInfo()};
                    channelParam.adpcmParam = adpcmInfo.adpcmParam;
                    channelParam.adpcmLoopParam = adpcmInfo.adpcmLoopParam;
                }
    
                channelParam.dataAddress = GetWaveDataAddress(&channelInfo, waveDataOffsetOrigin);
                
                const WaveFile::ChannelInfo& lastChannelInfo {m_pInfoBlockBody->GetChannelInfo(channelCount - 1)};
                channelParam.dataSize = m_pHeader->GetDataBlock()->header.size 
                                        - sizeof(BinaryBlockHeader)
                                        - lastChannelInfo.referToSamples.offset;
            }
        }
        break;
    }
    case WaveType_Dspadpcm:
        m_DspadpcmReader.ReadWaveInfo(info);
        break;
    case WaveType_Invalid:
        break;
    }

    return true;
}

const void* WaveFileReader::GetWaveDataAddress(const WaveFile::ChannelInfo* info, const void* waveDataOffsetOrigin) const {
    return info->GetSamplesAddress(m_pDataBlockBody);
}
} // namespace nn::atk::detail