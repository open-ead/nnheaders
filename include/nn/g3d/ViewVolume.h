#pragma once

#include "nn/types.h"

namespace nn::util::neon {
class MatrixRowMajor4x3fType;
}

namespace nn::g3d {
class Aabb;
class Sphere;

class ViewVolume {
public:
    void SetPerspective(f32, f32, f32, f32, const nn::util::neon::MatrixRowMajor4x3fType&);
    void SetFrustum(f32, f32, f32, f32, f32, f32, const nn::util::neon::MatrixRowMajor4x3fType&);
    void SetOrtho(f32, f32, f32, f32, f32, f32, const nn::util::neon::MatrixRowMajor4x3fType&);
    void TestIntersection(const nn::g3d::Sphere&);
    void TestIntersectionEx(const nn::g3d::Sphere&);
    void TestIntersection(const nn::g3d::Aabb&);
    void TestIntersectionEx(const nn::g3d::Aabb&);

private:
    void* filler[29];
};
}  // namespace nn::g3d
