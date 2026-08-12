#include <nn/atk/atk_StreamSoundFileLoader.h>

namespace nn::atk::detail {
bool StreamSoundFileLoader::LoadFileHeader(StreamSoundFileReader* reader, void* buffer, u64 size) {
    const size_t HeaderSize = 0x60;
    const size_t AlignSize = 0x40;
    char buffer2[HeaderSize + AlignSize];

    m_pStream->Seek(0, fnd::FileStream::SeekOrigin_Begin);
    size_t readSize {m_pStream->Read(util::BytePtr(buffer2).AlignUp(AlignSize).Get(), HeaderSize, nullptr)};
    auto* header {util::BytePtr(buffer2).AlignUp(AlignSize).Get<StreamSoundFile::FileHeader>()};

    if (readSize == HeaderSize && StreamSoundFileReader::IsValidFileHeader(header)) {
        u32 loadSize {header->GetInfoBlockOffset() + header->GetInfoBlockSize()};
        if (loadSize <= size) {
            m_pStream->Seek(0, fnd::FileStream::SeekOrigin_Begin);
            readSize = m_pStream->Read(buffer, loadSize, nullptr);
            if (readSize == (int)loadSize) {
                reader->Initialize(buffer);
                m_SeekBlockOffset = reader->GetSeekBlockOffset();
                m_RegionDataOffset = reader->GetRegionDataOffset();
                m_RegionInfoBytes = reader->GetRegionInfoBytes();
                return true;
            }
        }
    }

    return false;
}
} // namespace nn::atk::detail