#include <nn/util.h>

namespace nn::g3d::detail {

NN_MIDDLEWARE(g_MiddlewareInfo, "Nintendo", "NintendoWare_G3d" NN_SDK_BUILD_STR);

inline void UseMiddleWare() {
    nn::util::ReferSymbol(g_MiddlewareInfo);
}

}  // namespace nn::g3d::detail