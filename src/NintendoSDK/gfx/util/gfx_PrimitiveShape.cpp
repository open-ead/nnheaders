#include <nn/gfx/util/gfx_PrimitiveShape.h>

namespace nn::gfx::util {

PrimitiveShape::PrimitiveShape(PrimitiveShapeFormat vertexFormat,
                               PrimitiveTopology primitiveTopology) {
    m_pIndexBuffer = nullptr;
    m_pVertexBuffer = nullptr;
    m_VertexFormat = vertexFormat;
    m_IndexBufferFormat = IndexFormat_Uint32;
    m_PrimitiveTopology = primitiveTopology;
    m_VertexCount = 0;
    m_IndexCount = 0;
    m_VertexBufferSize = 0;
    m_IndexBufferSize = 0;
}

/*
void PrimitiveShape::~PrimitiveShape(); void*
PrimitiveShape::GetIndexBuffer() const; void* PrimitiveShape::GetVertexBuffer() const;
*/

size_t PrimitiveShape::GetStride() const {
    size_t stride = 0;
    if (m_VertexFormat & PrimitiveShapeFormat_Pos) {
        stride += 12;
    }
    if (m_VertexFormat & PrimitiveShapeFormat_Normal) {
        stride += 12;
    }
    if (m_VertexFormat & PrimitiveShapeFormat_Uv) {
        stride += 8;
    }

    return stride;
}

/*
size_t PrimitiveShape::GetVertexBufferSize()
const; size_t PrimitiveShape::GetIndexBufferSize() const;
*/

PrimitiveTopology PrimitiveShape::GetPrimitiveTopology() const {
    return m_PrimitiveTopology;
}

/*
 PrimitiveShapeFormat
PrimitiveShape::GetVertexFormat() const; IndexFormat PrimitiveShape::GetIndexBufferFormat()
const; */

int PrimitiveShape::GetVertexCount() const {
    return m_VertexCount;
}

int PrimitiveShape::GetIndexCount() const {
    return m_IndexCount;
}

/* void PrimitiveShape::Calculate(void*, unsigned long, void*, unsigned long); void
PrimitiveShape::SetVertexBuffer(void*); void PrimitiveShape::SetIndexBuffer(void*);
*/

void PrimitiveShape::SetVertexBufferSize(size_t vertexBufferSize) {
    m_VertexBufferSize = vertexBufferSize;
}

void PrimitiveShape::SetIndexBufferSize(size_t indexBufferSize) {
    m_IndexBufferSize = indexBufferSize;
}

void PrimitiveShape::SetVertexCount(int vertexCount) {
    m_VertexCount = vertexCount;
}

void PrimitiveShape::SetIndexCount(int indexCount) {
    if (indexCount <= 0xFF) {
        m_IndexBufferFormat = IndexFormat_Uint8;
        m_IndexCount = indexCount;
    } else if (indexCount <= 0xFFFF) {
        m_IndexBufferFormat = IndexFormat_Uint16;
        m_IndexCount = indexCount;
    } else {
        m_IndexBufferFormat = IndexFormat_Uint32;
        m_IndexCount = indexCount;
    }
}

SphereShape::SphereShape(PrimitiveShapeFormat vertexFormat, PrimitiveTopology primitiveTopology,
                         int sliceCount, int stackCount)
    : PrimitiveShape(vertexFormat, primitiveTopology) {
    m_SliceCount = sliceCount;
    m_StackCount = stackCount;

    SetVertexCount(CalculateVertexCount());
    SetIndexCount(CalculateIndexCount());

    const size_t stride = GetStride();
    SetVertexBufferSize(GetVertexCount() * stride);
    SetIndexBufferSize(GetIndexCount() * 4);
}

// void SphereShape::~SphereShape();

int SphereShape::CalculateVertexCount() {
    int numVertices = (m_StackCount + 1) * (m_SliceCount + 1);
    return numVertices;
}

int SphereShape::CalculateIndexCount() {
    int numIndices = 0;
    PrimitiveTopology primitiveTopology = GetPrimitiveTopology();

    if (primitiveTopology == PrimitiveTopology_LineList) {
        numIndices = (2 * m_StackCount - 1) * (2 * m_SliceCount);
    } else if (primitiveTopology == PrimitiveTopology_TriangleList) {
        int numFaces = (m_StackCount - 2) * (2 * m_SliceCount) + (2 * m_SliceCount);
        numIndices = numFaces * 3;
    }

    return numIndices;
}

// void* SphereShape::CalculateVertexBuffer();

template <typename T>
void SphereShape::CalculateIndexBuffer() {}

/*
void SphereShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
void CircleShape::CircleShape(PrimitiveShapeFormat, PrimitiveTopology, int);
void CircleShape::~CircleShape();
int CircleShape::CalculateVertexCount();
int CircleShape::CalculateIndexCount();
void* CircleShape::CalculateVertexBuffer();
void CircleShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
void CubeShape::CubeShape(PrimitiveShapeFormat, PrimitiveTopology);
void CubeShape::~CubeShape();
int CubeShape::CalculateVertexCount();
int CubeShape::CalculateIndexCount();
void* CubeShape::CalculateVertexBuffer();
void CubeShape::CalculateIndexBuffer<unsigned char>();
void CubeShape::CalculateIndexBuffer<unsigned int>();
void CubeShape::CalculateIndexBuffer<unsigned short>();
void CubeShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
void QuadShape::QuadShape(PrimitiveShapeFormat, PrimitiveTopology);
void QuadShape::~QuadShape();
int QuadShape::CalculateVertexCount();
int QuadShape::CalculateIndexCount();
void* QuadShape::CalculateVertexBuffer();
void QuadShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
void HemiSphereShape::HemiSphereShape(PrimitiveShapeFormat, PrimitiveTopology, int);
void HemiSphereShape::~HemiSphereShape();
int HemiSphereShape::CalculateVertexCount();
int HemiSphereShape::CalculateIndexCount();
void* HemiSphereShape::CalculateVertexBuffer();
void HemiSphereShape::CalculateIndexBuffer<unsigned char>();
void HemiSphereShape::CalculateIndexBuffer<unsigned int>();
void HemiSphereShape::CalculateIndexBuffer<unsigned short>();
void HemiSphereShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
void PipeShape::PipeShape(PrimitiveShapeFormat, PrimitiveTopology, int);
void PipeShape::~PipeShape();
int PipeShape::CalculateVertexCount();
int PipeShape::CalculateIndexCount();
void* PipeShape::CalculateVertexBuffer();
void PipeShape::CalculateIndexBuffer<unsigned char>();
void PipeShape::CalculateIndexBuffer<unsigned int>();
void PipeShape::CalculateIndexBuffer<unsigned short>();
void PipeShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
void CylinderShape::CylinderShape(PrimitiveShapeFormat, PrimitiveTopology, int);
void CylinderShape::~CylinderShape();
int CylinderShape::CalculateVertexCount();
int CylinderShape::CalculateIndexCount();
void* CylinderShape::CalculateVertexBuffer();
void CylinderShape::CalculateIndexBuffer<unsigned char>();
void CylinderShape::CalculateIndexBuffer<unsigned int>();
void CylinderShape::CalculateIndexBuffer<unsigned short>();
void CylinderShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
void ConeShape::ConeShape(PrimitiveShapeFormat, PrimitiveTopology, int);
void ConeShape::~ConeShape();
int ConeShape::CalculateVertexCount();
int ConeShape::CalculateIndexCount();
void* ConeShape::CalculateVertexBuffer();
void ConeShape::CalculateIndexBuffer<unsigned char>();
void ConeShape::CalculateIndexBuffer<unsigned int>();
void ConeShape::CalculateIndexBuffer<unsigned short>();
void ConeShape::CalculateImpl(void*, unsigned long, void*, unsigned long);
*/

}  // namespace nn::gfx::util