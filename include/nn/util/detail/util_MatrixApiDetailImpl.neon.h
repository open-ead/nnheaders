#pragma once

namespace nn {
namespace util {
namespace neon {
namespace detail {

// todo: complicated neon code
float32x4x3_t MatrixColumnMajor4x3fMultiply(float32x4x3_t matrix1, float32x4x3_t matrix2) {
    float32x2_t vLow, vHigh;
    float32x4_t vTmp;
    float32x4x3_t vResult;
    float32x4_t vZero;

    NN_UNUSED(matrix1);
    NN_UNUSED(matrix2);
    NN_UNUSED(vLow);
    NN_UNUSED(vHigh);
    NN_UNUSED(vTmp);
    NN_UNUSED(vZero);

    return vResult;
}

}  // namespace detail
}  // namespace neon
}  // namespace util
}  // namespace nn
