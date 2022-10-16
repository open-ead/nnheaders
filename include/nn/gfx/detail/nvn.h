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

enum class BlendFunction : u8 {
	Add,
	Sub,
	ReverseSub,
	Min,
	Max
};

enum class BlendFactor : u8 {
	Zero,
	One,
	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,
	SrcAlpha,
	OneMinusSrcAlpha,
	DstAlpha,
	OneMinusDstAlpha,
	ConstantColor,
	OneMinusConstantColor,
	ConstantAlpha,
	OneMinusConstantAlpha,
	SrcAlphaSaturate,
	Src1Color,
	OneMinusSrc1Color,
	Src1Alpha,
	OneMinusSrc1Alpha
};

enum class LogicOperation : u8 {
	Clear,
	And,
	AndReverse,
	Copy,
	AndInverted,
	Noop,
	Xor,
	Or,
	Nor,
	Equiv,
	Invert,
	OrReverse,
	CopyInverted,
	OrInverted,
	Nand,
	Set
};

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

	static NVNblendEquation GetBlendEquation(BlendFunction a) {
		static const NVNblendEquation eqs[] = {
			NVN_BLEND_EQUATION_ADD,
			NVN_BLEND_EQUATION_SUB,
			NVN_BLEND_EQUATION_REVERSE_SUB,
			NVN_BLEND_EQUATION_MIN,
			NVN_BLEND_EQUATION_MAX
		};

		return eqs[static_cast<u8>(a)];
	}

	static NVNblendFunc GetBlendFunction(BlendFactor a) {
		static const NVNblendFunc funcs[] = {
			NVN_BLEND_FUNC_ZERO,
			NVN_BLEND_FUNC_ONE,
			NVN_BLEND_FUNC_SRC_COLOR,
			NVN_BLEND_FUNC_ONE_MINUS_SRC_COLOR,
			NVN_BLEND_FUNC_DST_COLOR,
			NVN_BLEND_FUNC_ONE_MINUS_DST_COLOR,
			NVN_BLEND_FUNC_SRC_ALPHA,
			NVN_BLEND_FUNC_ONE_MINUS_SRC_ALPHA,
			NVN_BLEND_FUNC_DST_ALPHA,
			NVN_BLEND_FUNC_ONE_MINUS_DST_ALPHA,
			NVN_BLEND_FUNC_CONSTANT_COLOR,
			NVN_BLEND_FUNC_ONE_MINUS_CONSTANT_COLOR,
			NVN_BLEND_FUNC_CONSTANT_ALPHA,
			NVN_BLEND_FUNC_ONE_MINUS_CONSTANT_ALPHA,
			NVN_BLEND_FUNC_SRC_ALPHA_SATURATE,
			NVN_BLEND_FUNC_SRC1_COLOR,
			NVN_BLEND_FUNC_ONE_MINUS_SRC1_COLOR,
			NVN_BLEND_FUNC_SRC1_ALPHA,
			NVN_BLEND_FUNC_ONE_MINUS_SRC1_ALPHA,
		};

		return funcs[static_cast<u8>(a)];
	}

	static NVNlogicOp GetLogicOperation(LogicOperation a) {
		static const NVNlogicOp ops[] = {
			NVN_LOGIC_OP_CLEAR,
			NVN_LOGIC_OP_AND,
			NVN_LOGIC_OP_AND_REVERSE,
			NVN_LOGIC_OP_COPY,
			NVN_LOGIC_OP_AND_INVERTED,
			NVN_LOGIC_OP_NOOP,
			NVN_LOGIC_OP_XOR,
			NVN_LOGIC_OP_OR,
			NVN_LOGIC_OP_NOR,
			NVN_LOGIC_OP_EQUIV,
			NVN_LOGIC_OP_INVERT,
			NVN_LOGIC_OP_OR_REVERSE,
			NVN_LOGIC_OP_COPY_INVERTED,
			NVN_LOGIC_OP_OR_INVERTED,
			NVN_LOGIC_OP_NAND,
			NVN_LOGIC_OP_SET
		};

		return ops[static_cast<u8>(a)];
	}
	
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