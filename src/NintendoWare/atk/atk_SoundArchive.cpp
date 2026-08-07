#include <nn/atk/atk_SoundArchive.h>

namespace nn::atk {
SoundArchive::SoundArchive() {
    m_ExtFileRoot[0] = '/';
    m_ExtFileRoot[1] = '\0';
};
} // namespace nn::atk