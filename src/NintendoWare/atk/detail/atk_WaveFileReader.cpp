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
} // namespace nn::atk::detail