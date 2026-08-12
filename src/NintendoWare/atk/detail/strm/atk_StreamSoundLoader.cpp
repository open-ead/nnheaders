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

bool StreamSoundFileLoader::ReadSeekBlockData(u16* yn1, u16* yn2, int blockIndex, int channelCount) {
    const size_t SeekInfoMaxSize = channelCount * 4L;
    size_t readDataSize = SeekInfoMaxSize;
    size_t readOffset = m_SeekBlockOffset + blockIndex * readDataSize + 8;

    m_pStream->Seek(readOffset, fnd::FileStream::SeekOrigin_Begin);
    if (readDataSize <= 64) {
        const int Align = 64;
        u16 bufferBase[128];
        u16* buffer = util::BytePtr(bufferBase).AlignUp(Align).Get<u16>();
        size_t readSize {m_pStream->Read(buffer, readDataSize, nullptr)};

        if (readSize == readDataSize) {
            for (int i {0}; i < channelCount; ++i) {
                yn1[i] = buffer[i * sizeof(u16)];
                yn2[i] = buffer[i * sizeof(u16) + 1];
            }
            return true;
        }
    }

    return false;
}
} // namespace nn::atk::detail