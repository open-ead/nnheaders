#pragma once

#include <arm_neon.h>

namespace nn {
namespace util {
namespace neon {

struct alignas(16) Vector3fType {
    float32x4_t _v;
};

struct alignas(16) Vector4fType {
    float32x4_t _v;
};

struct alignas(16) MatrixRowMajor4x3fType {
    float32x4x4_t _m;
};

struct alignas(16) MatrixRowMajor4x4fType {
    float32x4x4_t _m;
};

struct alignas(16) MatrixColumnMajor4x3fType {
    float32x4x3_t _m;
};

struct alignas(16) MatrixColumnMajor4x4fType {
    float32x4x4_t _m;
};

}  // namespace neon
}  // namespace util
}  // namespace nn
