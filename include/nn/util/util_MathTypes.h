#pragma once

#include <nn/types.h>
#include <nn/util/detail/util_MathTypes.neon.h>

namespace nn::util {

typedef uint32_t AngleIndex;

struct Unorm8x4 {
    uint8_t v[4];
};

struct Float2 {
    union {
        float v[2];
        struct {
            float x;
            float y;
        };
    };
};

struct Float3 {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
};

struct Float4 {
    union {
        float v[4];
        struct {
            float x;
            float y;
            float z;
            float w;
        };
    };
};

struct FloatRowMajor4x3 {
    union {
        float m[4][3];
        struct {
            float m00;
            float m01;
            float m02;
            float m10;
            float m11;
            float m12;
            float m20;
            float m21;
            float m22;
            float m30;
            float m31;
            float m32;
        };
    };
};

struct FloatColumnMajor4x3 {
    union {
        float m[3][4];
        struct {
            float m00;
            float m10;
            float m20;
            float m30;
            float m01;
            float m11;
            float m21;
            float m31;
            float m02;
            float m12;
            float m22;
            float m32;
        };
    };
};

struct FloatRowMajor4x4 {
    union {
        float m[4][4];
        struct {
            float m00;
            float m01;
            float m02;
            float m03;
            float m10;
            float m11;
            float m12;
            float m13;
            float m20;
            float m21;
            float m22;
            float m23;
            float m30;
            float m31;
            float m32;
            float m33;
        };
    };
};

struct FloatColumnMajor4x4 {
    union {
        float m[4][4];
        struct {
            float m00;
            float m10;
            float m20;
            float m30;
            float m01;
            float m11;
            float m21;
            float m31;
            float m02;
            float m12;
            float m22;
            float m32;
            float m03;
            float m13;
            float m23;
            float m33;
        };
    };
};

typedef Unorm8x4 Color4u8Type;
typedef FloatRowMajor4x3 Float4x3;
typedef FloatRowMajor4x4 Float4x4;
typedef FloatColumnMajor4x3 FloatT4x3;
typedef FloatColumnMajor4x4 FloatT4x4;

// dwarf says a using statement for neon is used?
using namespace neon;

typedef MatrixRowMajor4x3fType Matrix4x3fType;
typedef MatrixRowMajor4x4fType Matrix4x4fType;
typedef MatrixColumnMajor4x3fType MatrixT4x3fType;
typedef MatrixColumnMajor4x4fType MatrixT4x4fType;

}  // namespace nn::util
