#include <nn/atk/atk_StreamSoundFileLoader.h>

namespace nn::atk::detail {
bool StreamSoundFileLoader::LoadFileHeader(StreamSoundFileReader* reader, void* buffer, u64 size) {
    const size_t HeaderSize {0x60};
    const size_t AlignSize {64};
    char buffer2[HeaderSize + AlignSize];

    m_pStream->Seek(0, fnd::FileStream::SeekOrigin_Begin);
    size_t readSize {m_pStream->Read(util::BytePtr(buffer2).AlignUp(AlignSize).Get(), HeaderSize, nullptr)};
    auto* header {util::BytePtr(buffer2).AlignUp(AlignSize).Get<StreamSoundFile::FileHeader>()};

    if (readSize == HeaderSize && reader->IsValidFileHeader(header)) {
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
    const size_t SeekInfoMaxSize {channelCount * sizeof(u16) * 2};
    size_t readDataSize {SeekInfoMaxSize};
    size_t readOffset {m_SeekBlockOffset + sizeof(BinaryBlockHeader) + blockIndex * readDataSize};

    m_pStream->Seek(readOffset, fnd::FileStream::SeekOrigin_Begin);
    if (readDataSize <= 64) {
        const int Align {64};
        u16 bufferBase[128];
        u16* buffer {util::BytePtr(bufferBase).AlignUp(Align).Get<u16>()};
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

bool StreamSoundFileLoader::ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo, u32 regionIndex) const {
    if (m_RegionDataOffset != 0 && m_RegionInfoBytes != 0) {
        position_t offset {m_RegionDataOffset + static_cast<position_t>(m_RegionInfoBytes) * regionIndex};
        m_pStream->Seek(offset, fnd::FileStream::SeekOrigin_Begin);
    
        const int Align {64};
        const size_t ReadSize {sizeof(StreamSoundFile::RegionInfo)};
        u8 bufferBase[ReadSize + Align];
        u8* buffer {util::BytePtr(bufferBase).AlignUp(Align).Get<u8>()};
        
        size_t readSize {m_pStream->Read(buffer, ReadSize, nullptr)};
        if (readSize == ReadSize) {
            *pInfo = *reinterpret_cast<StreamSoundFile::RegionInfo*>(buffer);
            return true;
        }
        return false;
    }

    return false;
}
} // namespace nn::atk::detail