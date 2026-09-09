#pragma once

#include <nn/types.h>

namespace nn::ssl::Debug {
struct Output;            // TODO
struct Input;             // TODO
enum class IoctlCommand;  // TODO

struct HeapTrackPoint {
public:
    HeapTrackPoint();

private:
    char _0[0x10];
    bool _10;
};

Result Ioctl(Output*, Input*, IoctlCommand);
}  // namespace nn::ssl::Debug
