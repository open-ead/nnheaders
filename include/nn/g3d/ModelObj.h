#pragma once

#include <nn/types.h>

#include <nn/g3d/MaterialObj.h>

namespace nn::g3d {

class MaterialObj;
class ResModel;
class ShapeObj;
class SkeletonObj;

struct Bounds {
    f32 x;
    f32 y;
    f32 z;
    f32 unk;
    f32 radius;
};

// TODO
class ModelObj {
public:
    MaterialObj* FindMaterial(const char* materialName);

    SkeletonObj* GetSkeleton() const { return m_Skeleton; }

    s32 GetNumShapes() const { return m_NumShapes; }

    u8 GetViewDependentModelFlags() const { return m_ViewDependentModelFlags; }

    MaterialObj* GetMaterial(s32 index) const { return &m_Materials[index]; }

    const Bounds& GetBounds() const { return *m_Bounds; }

    s32 get_8c() const { return _8c; }

private:
    struct InitializeArgument;

    bool Initialize(const InitializeArgument& arg, void* buffer, size_t bufferSize);

    const ResModel* m_ResModel;
    void* _8;
    void* _10;
    u8 _18;
    u8 m_ViewDependentModelFlags;
    u16 _1a;
    void* _20;
    void* _28;
    u16 m_NumShapes;
    u16 m_NumMaterials;
    SkeletonObj* m_Skeleton;
    ShapeObj* m_Shapes;
    MaterialObj* m_Materials;
    Bounds* m_Bounds;
    void* m_UserData;
    void* _60;
    void* _68;
    void* _70;
    void* _78;
    void* _80;
    bool _88;
    int _8c;
};

}  // namespace nn::g3d
