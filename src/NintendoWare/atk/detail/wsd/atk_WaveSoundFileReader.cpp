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

}  // namespace nn::atk::detail