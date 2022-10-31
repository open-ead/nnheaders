#include "gfx_NvnHelper.h"

#include "nvn/nvn_api.h"

namespace nn::gfx::detail {

NVNblendEquation Nvn::GetBlendEquation(BlendFunction a) {
    const NVNblendEquation eqs[] = {NVN_BLEND_EQUATION_ADD, NVN_BLEND_EQUATION_SUB,
                                           NVN_BLEND_EQUATION_REVERSE_SUB, NVN_BLEND_EQUATION_MIN,
                                           NVN_BLEND_EQUATION_MAX};

    return eqs[static_cast<u8>(a)];
}

NVNblendFunc Nvn::GetBlendFunction(BlendFactor a) {
    const NVNblendFunc funcs[] = {
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

NVNlogicOp Nvn::GetLogicOperation(LogicOperation a) {
    const NVNlogicOp ops[] = {NVN_LOGIC_OP_CLEAR,         NVN_LOGIC_OP_AND,
                                     NVN_LOGIC_OP_AND_REVERSE,   NVN_LOGIC_OP_COPY,
                                     NVN_LOGIC_OP_AND_INVERTED,  NVN_LOGIC_OP_NOOP,
                                     NVN_LOGIC_OP_XOR,           NVN_LOGIC_OP_OR,
                                     NVN_LOGIC_OP_NOR,           NVN_LOGIC_OP_EQUIV,
                                     NVN_LOGIC_OP_INVERT,        NVN_LOGIC_OP_OR_REVERSE,
                                     NVN_LOGIC_OP_COPY_INVERTED, NVN_LOGIC_OP_OR_INVERTED,
                                     NVN_LOGIC_OP_NAND,          NVN_LOGIC_OP_SET};

    return ops[static_cast<u8>(a)];
}

NVNfrontFace Nvn::GetFrontFace(FrontFace a) {
    const NVNfrontFace faces[] = {NVN_FRONT_FACE_CCW, NVN_FRONT_FACE_CW};

    return faces[static_cast<u8>(a)];
}

NVNpolygonMode Nvn::GetFillMode(FillMode a) {
    const NVNpolygonMode faces[] = {NVN_POLYGON_MODE_POINT, NVN_POLYGON_MODE_LINE,
                                           NVN_POLYGON_MODE_FILL};

    return faces[static_cast<u8>(a)];
}

NVNdrawPrimitive Nvn::GetDrawPrimitive(PrimitiveTopology a) {
    const NVNdrawPrimitive prims[] = {
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

NVNindexType Nvn::GetIndexFormat(IndexFormat a) {
    const NVNindexType formats[] = {
        NVN_INDEX_TYPE_UNSIGNED_BYTE, NVN_INDEX_TYPE_UNSIGNED_SHORT, NVN_INDEX_TYPE_UNSIGNED_INT};

    return formats[static_cast<int>(a)];
}

NVNface Nvn::GetCullMode(CullMode a) {
    const NVNface culls[] = {NVN_FACE_NONE, NVN_FACE_FRONT, NVN_FACE_BACK};

    return culls[static_cast<u8>(a)];
}

NVNbufferAddress Nvn::GetBufferAddress(GpuAddress addr) {
    return addr.ToData()->value;
}


util::BitPack32 Nvn::GetDeviceFeature(const NVNdevice* device) {
	util::BitPack32 feature;
	int supported;

	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_MIN_MAX_FILTERING, &supported);
	feature.SetBit(NvnDeviceFeature_SupportMinMaxFiltering, supported);

	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_STENCIL8_FORMAT, &supported);
	feature.SetBit(NvnDeviceFeature_SupportStencil8Format, supported);

	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_ASTC_FORMATS, &supported);
	feature.SetBit(NvnDeviceFeature_SupportAstcFormat, supported);

	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_CONSERVATIVE_RASTER, &supported);
	feature.SetBit(NvnDeviceFeature_SupportConservativeRaster, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_ZERO_FROM_UNMAPPED_VIRTUAL_POOL_PAGES, &supported);
	feature.SetBit(NvnDeviceFeature_SupportZeroFromUnmappedVirtualPoolPage, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_PASSTHROUGH_GEOMETRY_SHADERS, &supported);
	feature.SetBit(NvnDeviceFeature_SupportPassthroughGeometryShader, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_VIEWPORT_SWIZZLE, &supported);
	feature.SetBit(NvnDeviceFeature_SupportViewportSwizzle, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_SPARSE_TILED_PACKAGED_TEXTURES, &supported);
	feature.SetBit(NvnDeviceFeature_SupportSparseTiledPackagedTexture, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_ADVANCED_BLEND_MODES, &supported);
	feature.SetBit(NvnDeviceFeature_AdvancedBlendModes, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_DRAW_TEXTURE, &supported);
	feature.SetBit(NvnDeviceFeature_DrawTexture, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_TARGET_INDEPENDENT_RASTERIZATION, &supported);
	feature.SetBit(NvnDeviceFeature_TargetIndependentRasterization, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_FRAGMENT_COVERAGE_TO_COLOR, &supported);
	feature.SetBit(NvnDeviceFeature_FragmentCoverageToColor, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_POST_DEPTH_COVERAGE, &supported);
	feature.SetBit(NvnDeviceFeature_PostDepthCoverage, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_IMAGES_USING_TEXTURE_HANDLES, &supported);
	feature.SetBit(NvnDeviceFeature_ImagesUsingTextureHandles, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_SAMPLE_LOCATIONS, &supported);
	feature.SetBit(NvnDeviceFeature_SampleLocations, supported);
	
	nvnDeviceGetInteger(device, NVN_DEVICE_INFO_SUPPORTS_FRAGMENT_SHADER_INTERLOCK, &supported);
	feature.SetBit(NvnDeviceFeature_SupportFragmentShaderInterlock, supported);

	return feature;
}

}  // namespace nn::gfx::detail