#pragma once

#include <nn/types.h>

namespace nn::err {
class ResultBacktrace {
public:
    static void Make(ResultBacktrace* outBackTrace, const Result*, s32);

private:
    // TODO
};
}  // namespace nn::err
