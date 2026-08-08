#pragma once

#include <nn/types.h>

namespace nn::atk::detail {
struct Command {
    Command* next;
    u32 id;
    u32 tag;
    std::uintptr_t memory_next;
};
static_assert(sizeof(Command) == 0x18);
} // namespace nn::atk::detail 