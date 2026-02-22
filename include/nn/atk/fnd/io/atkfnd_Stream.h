#pragma once

namespace nn::atk::detail::fnd {
class Stream {
public:
    enum SeekOrigin {
        SeekOrigin_Begin,
        SeekOrigin_End,
        SeekOrigin_Current,
    };

    virtual ~Stream();
};
static_assert(sizeof(Stream) == 0x8);
} // namespace nn::atk::detail::fnd