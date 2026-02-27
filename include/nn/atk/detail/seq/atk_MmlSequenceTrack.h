#pragma once

#include <nn/atk/detail/seq/atk_SequenceTrack.h>

namespace nn::atk::detail::driver {
class MmlParser;
class MmlSequenceTrack : SequenceTrack {
public:
    MmlSequenceTrack();
    ~MmlSequenceTrack() override;

    ParseResult Parse(bool doNoteOn) override;

private:
    MmlParser* m_pParser;
};
static_assert(sizeof(MmlSequenceTrack) == 0x1f0);
} // namespace nn::atk::detail::driver