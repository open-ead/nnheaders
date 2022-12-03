#pragma once

#include <nn/gfx/gfx_Enum.h>
#include <nn/util.h>

namespace nn::gfx::util {

enum PrimitiveShapeFormat {
    PrimitiveShapeFormat_Pos = 0x1,
    PrimitiveShapeFormat_Normal = 0x2,
    PrimitiveShapeFormat_Uv = 0x4,
    PrimitiveShapeFormat_Default = 7
};

class PrimitiveShape {
    NN_NO_COPY(PrimitiveShape);

public:
    virtual ~PrimitiveShape();
    void* GetIndexBuffer() const;
    void* GetVertexBuffer() const;
    size_t GetStride() const;
    size_t GetVertexBufferSize() const;
    size_t GetIndexBufferSize() const;
    PrimitiveShapeFormat GetVertexFormat() const;
    PrimitiveTopology GetPrimitiveTopology() const;
    size_t GetIndexBufferAlignment() const;
    IndexFormat GetIndexBufferFormat() const;
    int GetVertexCount() const;
    int GetIndexCount() const;
    void Calculate(void*, size_t, void*, size_t);

protected:
    PrimitiveShape(PrimitiveShapeFormat, PrimitiveTopology);

    virtual void CalculateImpl(void*, size_t, void*, size_t);
    void SetVertexBuffer(void*);
    void SetIndexBuffer(void*);
    void SetVertexBufferSize(size_t);
    void SetIndexBufferSize(size_t);
    void SetVertexCount(int);
    void SetIndexCount(int);

private:
    void* m_pIndexBuffer;
    void* m_pVertexBuffer;
    PrimitiveShapeFormat m_VertexFormat;
    IndexFormat m_IndexBufferFormat;
    PrimitiveTopology m_PrimitiveTopology;
    int m_VertexCount;
    int m_IndexCount;
    size_t m_VertexBufferSize;
    size_t m_IndexBufferSize;
};

class SphereShape : public PrimitiveShape {
    NN_NO_COPY(SphereShape);

public:
    SphereShape(PrimitiveShapeFormat, PrimitiveTopology, int, int);

    virtual ~SphereShape();

protected:
    int CalculateVertexCount();
    int CalculateIndexCount();
    virtual void CalculateImpl(void*, size_t, void*, size_t);

private:
    template <typename T>
    void CalculateIndexBuffer();

    void* CalculateVertexBuffer();

    int m_SliceCount;
    int m_StackCount;
};

//*308*/ CircleShape;
//*382*/ CubeShape;
//*434*/ CubeShape::CubeVertex;
//*443*/ CubeShape::CubeIndex;
//*470*/ QuadShape;
//*522*/ QuadShape::QuadVertex;
//*530*/ QuadShape::QuadIndex;
//*557*/ HemiSphereShape;
//*631*/ PipeShape;
//*705*/ CylinderShape;
//*779*/ ConeShape;
}  // namespace nn::gfx::util