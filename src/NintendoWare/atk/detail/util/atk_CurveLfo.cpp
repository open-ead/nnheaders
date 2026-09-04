#include <nn/atk/atk_CurveLfo.h>

#include <cstring>

#include <nn/util/util_Arithmetic.h>

#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {

namespace {

float CurveSine(float arg) {
    return util::SinTable(static_cast<util::AngleIndex>(arg * UINT_MAX));
}

float CurveSquare(float arg) {
    return arg < 0.5f ? 0.0f : 1.0f;
}

float CurveSaw(float arg) {
    return arg;
}

float CurveTriangle(float arg) {
    static const float gradients[5]{4.0, -4.0, -4.0, 4.0, 4.0};
    static const float intercepts[5]{0.0, 2.0, 2.0, -4.0, 0.0};

    u32 tmp{static_cast<u32>(arg * 4)};

    return gradients[tmp] * arg + intercepts[tmp];
}

float CurveRandom(float arg) {
    u16 rand{Util::CalcRandom()};
    float ret{rand / 65535.0f};

    return ret;
}

CurveLfo::CurveFunc g_CurveFuncTable[128];

}  // anonymous namespace

void CurveLfo::InitializeCurveTable() {
    std::memset(static_cast<void*>(g_CurveFuncTable), 0, sizeof(g_CurveFuncTable));
    g_CurveFuncTable[0] = CurveSine;
    g_CurveFuncTable[1] = CurveTriangle;
    g_CurveFuncTable[2] = CurveRandom;
    g_CurveFuncTable[3] = CurveSaw;
    g_CurveFuncTable[4] = CurveSquare;
}
}  // namespace nn::atk::detail