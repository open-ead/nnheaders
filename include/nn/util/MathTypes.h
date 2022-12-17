#pragma once

namespace nn::util {

typedef uint32_t AngleIndex;

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

struct FloatColumnMajor4x3 {
    float m[3][4];
};

}  // namespace nn::util
