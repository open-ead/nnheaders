#pragma once

#include <nn/types.h>

namespace nn {
namespace g3d {
struct DDLDeclarations {
    u64 _0;
    u32 _8;
    u32 DDLDeclarations_xC;
    u64 _10;
    u64 _18;
};

class BindFuncTable { //FIXME is that really its structure?
private:
    void* a1;
    void* a2;
};

};  // namespace g3d
};  // namespace nn