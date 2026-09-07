#include <nn/atk/atk_BankFileReader.h>

namespace nn::atk::detail {

namespace {
const u32 SignatureInfoBlockBank{0x4f464e49};  // INFO

const u32 SupportedFileVersionBank{0x10000};
const u32 CurrentFileVersionBank{0x10000};

bool IsValidFileHeaderBank(const void* bankFile) {
    const BinaryFileHeader* header{util::ConstBytePtr(bankFile).Get<BinaryFileHeader>()};

    bool isSupportedVersion{header->signature == BankFileReader::SignatureFile &&
                            header->byteOrder == BinaryFileHeader::ValidByteOrderMark &&
                            header->version >= SupportedFileVersionBank &&
                            header->version <= CurrentFileVersionBank};

    return isSupportedVersion;
}
}  // anonymous namespace

BankFileReader::BankFileReader() = default;

BankFileReader::BankFileReader(const void* bankFile) {
    Initialize(bankFile);
}

void BankFileReader::Initialize(const void* bankFile) {
    if (bankFile == nullptr || !IsValidFileHeaderBank(bankFile))
        return;

    m_pHeader = reinterpret_cast<const BankFile::FileHeader*>(bankFile);

    const BankFile::InfoBlock* infoBlock{m_pHeader->GetInfoBlock()};

    if (infoBlock->header.kind == SignatureInfoBlockBank) {
        m_pInfoBlockBody = &infoBlock->body;
        m_IsInitialized = true;
    }
}

void BankFileReader::Finalize() {
    if (m_IsInitialized) {
        m_IsInitialized = false;
        m_pHeader = nullptr;
        m_pInfoBlockBody = nullptr;
    }
}

bool BankFileReader::ReadVelocityRegionInfo(VelocityRegionInfo* info, int programNo, int key,
                                            int velocity) const {
    if (!m_IsInitialized || programNo < 0 || programNo >= GetInstrumentCount())
        return false;

    const BankFile::Instrument* instrument{m_pInfoBlockBody->GetInstrument(programNo)};
    if (instrument == nullptr)
        return false;

    const BankFile::KeyRegion* keyRegion{instrument->GetKeyRegion(key)};
    if (keyRegion == nullptr)
        return false;

    const BankFile::VelocityRegion* velocityRegion{keyRegion->GetVelocityRegion(velocity)};
    if (velocityRegion == nullptr)
        return false;

    const Util::WaveId* pWaveId{m_pInfoBlockBody->GetWaveId(velocityRegion->waveIdTableIndex)};
    if (pWaveId == nullptr || pWaveId->waveIndex == 0xffffffff)
        return false;

    info->waveArchiveId = pWaveId->waveArchiveId;
    info->waveIndex = pWaveId->waveIndex;

    const BankFile::RegionParameter* regionParameter{velocityRegion->GetRegionParameter()};
    if (regionParameter == nullptr) {
        info->originalKey = velocityRegion->GetOriginalKey();
        info->volume = velocityRegion->GetVolume();
        info->pan = velocityRegion->GetPan();
        info->pitch = velocityRegion->GetPitch();
        info->isIgnoreNoteOff = velocityRegion->IsIgnoreNoteOff();
        info->keyGroup = velocityRegion->GetKeyGroup();
        info->interpolationType = velocityRegion->GetInterpolationType();
        info->adshrCurve = velocityRegion->GetAdshrCurve();
    } else {
        info->originalKey = regionParameter->originalKey;
        info->volume = regionParameter->volume;
        info->pan = regionParameter->pan;
        info->pitch = regionParameter->pitch;
        info->isIgnoreNoteOff = regionParameter->isIgnoreNoteOff;
        info->keyGroup = regionParameter->keyGroup;
        info->interpolationType = regionParameter->interpolationType;
        info->adshrCurve = regionParameter->adshrCurve;
    }

    return true;
}

const Util::WaveIdTable* BankFileReader::GetWaveIdTable() const {
    if (!m_IsInitialized)
        return nullptr;

    return &m_pInfoBlockBody->GetWaveIdTable();
}

}  // namespace nn::atk::detail