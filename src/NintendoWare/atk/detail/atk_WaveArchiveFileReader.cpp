#include <nn/atk/atk_WaveArchiveFileReader.h>

namespace nn::atk::detail {

namespace {

const u32 SupportedFileVersionWar{0x10000};
const u32 CurrentFileVersionWar{0x10000};

bool IsValidFileHeaderWar(const void* waveArchiveData) {
    const BinaryFileHeader& header{*util::ConstBytePtr(waveArchiveData).Get<BinaryFileHeader>()};

    bool isSupportedVersion{header.signature == WaveArchiveFileReader::SignatureFile &&
                            header.byteOrder == BinaryFileHeader::ValidByteOrderMark &&
                            header.version >= SupportedFileVersionWar &&
                            header.version <= CurrentFileVersionWar};

    return isSupportedVersion;
}

}  // anonymous namespace

WaveArchiveFileReader::WaveArchiveFileReader() = default;

WaveArchiveFileReader::WaveArchiveFileReader(const void* pWaveArchiveFile, bool isIndividual) {
    Initialize(pWaveArchiveFile, isIndividual);
}

void WaveArchiveFileReader::Initialize(const void* pWaveArchiveFile, bool isIndividual) {
    if (pWaveArchiveFile == nullptr || !IsValidFileHeaderWar(pWaveArchiveFile))
        return;

    m_pHeader = reinterpret_cast<const WaveArchiveFile::FileHeader*>(pWaveArchiveFile);

    m_pInfoBlockBody = &m_pHeader->GetInfoBlock()->body;
    m_IsInitialized = true;

    m_pLoadTable = nullptr;

    if (!isIndividual || !HasIndividualLoadTable())
        return;

    m_pLoadTable = util::BytePtr(const_cast<void*>(pWaveArchiveFile))
                       .Advance(m_pHeader->GetFileBlockOffset() +
                                sizeof(WaveArchiveFileReader::SignatureWarcTable))
                       .Get<IndividualLoadTable>();
}

void WaveArchiveFileReader::Finalize() {
    if (m_IsInitialized) {
        m_pHeader = nullptr;
        m_pInfoBlockBody = nullptr;
        m_pLoadTable = nullptr;
        m_IsInitialized = false;
    }
}

void WaveArchiveFileReader::InitializeFileTable() {
    for (u32 i{0}; i < GetWaveFileCount(); ++i)
        m_pLoadTable->waveFile[i] = nullptr;
}

u32 WaveArchiveFileReader::GetWaveFileCount() const {
    if (!m_IsInitialized)
        return 0;

    return m_pInfoBlockBody->GetWaveFileCount();
}

u32 WaveArchiveFileReader::GetWaveFileSize(u32 waveIndex) const {
    if (!m_IsInitialized)
        return 0;

    return m_pInfoBlockBody->GetSize(waveIndex);
}

u32 WaveArchiveFileReader::GetWaveFileOffsetFromFileHead(u32 waveIndex) const {
    u32 result{0};

    if (m_IsInitialized)
        result = m_pHeader->GetFileBlockOffset() + offsetof(WaveArchiveFile::FileBlock, body) +
                 m_pInfoBlockBody->GetOffsetFromFileBlockBody(waveIndex);

    return result;
}

const void* WaveArchiveFileReader::GetWaveFile(u32 waveIndex) const {
    if (!m_IsInitialized)
        return nullptr;

    if (waveIndex >= m_pInfoBlockBody->GetWaveFileCount())
        return nullptr;

    if (m_pLoadTable != nullptr)
        return GetWaveFileForIndividual(waveIndex);

    return GetWaveFileForWhole(waveIndex);
}

const void* WaveArchiveFileReader::SetWaveFile(u32 waveIndex, const void* pWaveFile) {
    if (!m_IsInitialized)
        return nullptr;

    if (m_pLoadTable == nullptr)
        return nullptr;

    if (waveIndex >= m_pInfoBlockBody->GetWaveFileCount())
        return nullptr;

    const void* preAddress{GetWaveFileForIndividual(waveIndex)};
    m_pLoadTable->waveFile[waveIndex] = pWaveFile;

    return preAddress;
}

bool WaveArchiveFileReader::HasIndividualLoadTable() const {
    if (!m_IsInitialized)
        return false;

    const u32* signature{util::ConstBytePtr(m_pHeader, m_pHeader->GetFileBlockOffset()).Get<u32>()};

    return *signature == SignatureWarcTable;
}

}  // namespace nn::atk::detail