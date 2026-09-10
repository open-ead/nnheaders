#pragma once

#include <nn/types.h>

namespace nn::err::detail {
// What does Flv stand for?
enum class MessageKind : u8 { DialogueMessage, DialogueButton, FlvMessage, FlvButton };
}  // namespace nn::err::detail
