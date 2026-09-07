#include <nn/atk/atk_WaveSoundFileReader.h>

namespace nn::atk::detail {

namespace {

const u32 SignatureInfoBlockWsd{0x4f464e49};  // INFO

const u32 SupportedFileVersionWsd{0x10000};
const u32 CurrentFileVersionWsd{0x10100};
const u32 FilterSupportedVersionWsd{0x10100};

bool IsValidFileHeaderWsd(const void* waveSoundFile) {
    const BinaryFileHeader* header{util::ConstBytePtr(waveSoundFile).Get<BinaryFileHeader>()};

    bool isSupportedVersion{header->signature == WaveSoundFileReader::SignatureFile &&
                            header->byteOrder == BinaryFileHeader::ValidByteOrderMark &&
                            header->version >= SupportedFileVersionWsd &&
                            header->version <= CurrentFileVersionWsd};

    return isSupportedVersion;
}

}  // anonymous namespace

WaveSoundFileReader::WaveSoundFileReader(const void* waveSoundFile) {
    if (IsValidFileHeaderWsd(waveSoundFile)) {
        m_pHeader = reinterpret_cast<const WaveSoundFile::FileHeader*>(waveSoundFile);

        const WaveSoundFile::InfoBlock* infoBlock{m_pHeader->GetInfoBlock()};

        if (infoBlock != nullptr && infoBlock->header.kind == SignatureInfoBlockWsd)
            m_pInfoBlockBody = &infoBlock->body;
    }
}

u32 WaveSoundFileReader::GetWaveSoundCount() const {
    return m_pInfoBlockBody->GetWaveSoundCount();
}

u32 WaveSoundFileReader::GetNoteInfoCount(u32 index) const {
    const WaveSoundFile::WaveSoundData& wsdData{m_pInfoBlockBody->GetWaveSoundData(index)};

    return wsdData.GetNoteCount();
}

u32 WaveSoundFileReader::GetTrackInfoCount(u32 index) const {
    const WaveSoundFile::WaveSoundData& wsdData{m_pInfoBlockBody->GetWaveSoundData(index)};

    return wsdData.GetTrackCount();
}

bool WaveSoundFileReader::ReadWaveSoundInfo(WaveSoundInfo* dst, u32 index) const {
    const WaveSoundFile::WaveSoundInfo& src{m_pInfoBlockBody->GetWaveSoundData(index).GetWaveSoundInfo()};

    dst->pitch = src.GetPitch();
    dst->pan = src.GetPan();
    dst->surroundPan = src.GetSurroundPan();
    src.GetSendValue(&dst->mainSend, dst->fxSend, AuxBus_Count);
    dst->adshr = src.GetAdshrCurve();

    if (IsFilterSupportedVersion()) {
        dst->lpfFreq = src.GetLpfFreq();
        dst->biquadType = src.GetBiquadType();
        dst->biquadValue = src.GetBiquadValue();
    }
    else {
        dst->lpfFreq = 64; // WsdDefaultLpfFreq
        dst->biquadType = 0; // WsdDefaultBiquadType
        dst->biquadValue = 0; // WsdDefaultBiquadValue
    }

    return true;
}

bool WaveSoundFileReader::IsFilterSupportedVersion() const {
    return m_pHeader->header.version >= FilterSupportedVersionWsd;
}

}  // namespace nn::atk::detail