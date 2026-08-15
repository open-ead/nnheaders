#include <nn/atk/atk_StreamSoundPrefetchFileReader.h>

namespace nn::atk::detail {
StreamSoundPrefetchFileReader::StreamSoundPrefetchFileReader() = default;

StreamSoundPrefetchFileReader::~StreamSoundPrefetchFileReader() {
    Finalize();
}
} // namespace nn::atk::detail