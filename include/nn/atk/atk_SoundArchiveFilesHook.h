#pragma once

#include <nn/atk/fnd/io/atkfnd_FileStream.h>
#include <nn/types.h>

namespace nn::atk::detail {

class SoundArchiveFilesHook {
public:
    constexpr static const char ItemTypeWaveSound[] = "wsd";
    constexpr static const char ItemTypeStreamSound[] = "stm";
    constexpr static const char ItemTypeSequenceSound[] = "seq";

    constexpr static const char FileTypeStreamBinary[] = "bxstm";
    constexpr static const char FileTypeWaveSoundBinary[] = "bxwsd";
    constexpr static const char FileTypeSequenceBinary[] = "bxseq";
    constexpr static const char FileTypeBankBinary[] = "bxbnk";
    constexpr static const char FileTypeWaveArchiveBinary[] = "bxwar";
    constexpr static const char FileTypeStreamPrefetchBinary[] = "bxstp";

    bool GetIsEnable() const;
    void SetIsEnable(bool value);

    bool IsTargetItem(const char* itemLabel);

    void Lock();
    void Unlock();

    fnd::FileStream* OpenFile(void* buffer, size_t bufferLength, void* cacheBuffer,
                              size_t cacheBufferLength, const char* itemLabel,
                              const char* fileType) {
        if (!m_IsEnable)
            return nullptr;

        return OpenFileImpl(buffer, bufferLength, cacheBuffer, cacheBufferLength, itemLabel,
                            fileType);
    }

    const void* GetFileAddress(const char* itemLabel, const char* itemType, const char* fileType,
                               u32 fileIndex);

protected:
    virtual void Impl0();
    virtual void Impl1();
    virtual void Impl2();
    virtual void Impl3();
    virtual void Impl4();
    virtual fnd::FileStream* OpenFileImpl(void* buffer, size_t bufferLength, void* cacheBuffer,
                                          size_t cacheBufferLength, const char* itemLabel,
                                          const char* fileType);

private:
    bool m_IsEnable;
};

}  // namespace nn::atk::detail
