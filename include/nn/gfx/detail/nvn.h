#pragma once

#include <nn/types.h>
#include <nvn/nvn_types.h>

namespace nn::gfx {

struct ImageFormatProperty;
struct TextureInfo;
struct SwapChainInfo;

struct GpuAddress {
	u64 addr;
	u64 field_8; //BufferImpl::GetGpuAddress
};

enum class ImageFormat;
enum class AttributeFormat;
enum class ImageDimension;
enum class ComparisonFunction;
enum class StencilOperation;
enum class ComparisonFunction;
enum class BlendFunction;
enum class BlendFactor;

enum class FrontFace : u8 {
	CCW,
	CW,
};

enum class FillMode : u8 {
	Point,
	Line,
	Fill
};

enum class PrimitiveTopology {
	Points,
	Lines,
	LineStrip,
	Triangles,
	TringleStrip,
	LinesAdj,
	LineStripAdj,
	TrianglesAdj,
	TriangleStripAdj,
	Patches
};

enum class FilterMode;
enum class TextureAddressMode;
enum class ShaderStage;
enum class ComparsionFunction;

enum class IndexFormat {
	Byte,
	Short,
	Int
};

enum class CullMode : u8 {
	None,
	Front,
	Back
};

enum class QueryTarget;

namespace detail {

// todo: fix return types
class Nvn {
public:
	static void GetImageFormat(ImageFormat);
	static void GetAttributeFormat(AttributeFormat);
	static void GetImageTarget(ImageDimension);
	static void GetDepthFunction(ComparisonFunction);
	static void GetStencilOperation(StencilOperation);
	static void GetStencilFunction(ComparisonFunction);
	static void GetBlendEquation(BlendFunction);
	static void GetBlendFunction(BlendFactor);
	
	static NVNfrontFace GetFrontFace(FrontFace a) {
		static const NVNfrontFace faces[] = {
			NVN_FRONT_FACE_CCW,
			NVN_FRONT_FACE_CW
		};

		return faces[static_cast<u8>(a)];
	}

	static NVNpolygonMode GetFillMode(FillMode a) {
		static const NVNpolygonMode faces[] = {
			NVN_POLYGON_MODE_POINT,
			NVN_POLYGON_MODE_LINE,
			NVN_POLYGON_MODE_FILL 
		};

		return faces[static_cast<u8>(a)];
	}

	static NVNdrawPrimitive GetDrawPrimitive(PrimitiveTopology a) {
		static const NVNdrawPrimitive prims[] = {
			NVN_DRAW_PRIMITIVE_POINTS,
			NVN_DRAW_PRIMITIVE_LINES,
			NVN_DRAW_PRIMITIVE_LINE_STRIP,
			NVN_DRAW_PRIMITIVE_TRIANGLES,
			NVN_DRAW_PRIMITIVE_TRIANGLE_STRIP,
			NVN_DRAW_PRIMITIVE_LINES_ADJACENCY,
			NVN_DRAW_PRIMITIVE_LINE_STRIP_ADJACENCY,
			NVN_DRAW_PRIMITIVE_TRIANGLES_ADJACENCY,
			NVN_DRAW_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY,
			NVN_DRAW_PRIMITIVE_PATCHES,
		};

		return prims[static_cast<int>(a)];
	}

	static void GetMinFilter(FilterMode);
	static void GetMagFilter(FilterMode);
	static void GetWrapMode(TextureAddressMode);
	static void GetShaderStage(ShaderStage);
	static void GetShaderStageBits(int);
	static void GetRComparisonFunction(ComparisonFunction);

	static NVNindexType GetIndexFormat(IndexFormat a) {
		static const NVNindexType formats[] = {
			NVN_INDEX_TYPE_UNSIGNED_BYTE,
			NVN_INDEX_TYPE_UNSIGNED_SHORT,
			NVN_INDEX_TYPE_UNSIGNED_INT
		};

		return formats[static_cast<int>(a)];
	}

	static NVNface GetCullMode(CullMode a) {
		static const NVNface culls[] = {
			NVN_FACE_NONE,
			NVN_FACE_FRONT,
			NVN_FACE_BACK
		};

		return culls[static_cast<u8>(a)];
	}

	static void GetMemoryPoolFlags(int);

	static NVNbufferAddress GetBufferAddress(GpuAddress addr) {
		return addr.addr;
	}

	static void SetupScanBufferTextureInfo(TextureInfo*, const SwapChainInfo&);
	static void GetCounterType(QueryTarget);
	static void GetDeviceFeature(const NVNdevice*);
	static void GetImageFormatProperty(ImageFormatProperty*, NVNformat);
	static void GetGfxImageFormat(NVNformat);
	static void DebugCallback(NVNdebugCallbackSource, NVNdebugCallbackType, int, NVNdebugCallbackSeverity, const char*, void*);
	static void GetFirstScanBufferIndex();
	static void SetPackagedTextureDataImpl(NVNtextureBuilder*, const TextureInfo&);
	static s64 ToTimeSpan(s64);
};

}
}