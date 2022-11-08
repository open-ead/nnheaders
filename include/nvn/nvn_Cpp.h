#pragma once

#include <stddef.h>
#include <stdint.h>

// todo: implement?
#define NVN_ENUM(CLASS)                                                                            \
private:                                                                                           \
    Enum m_value;                                                                                  \
                                                                                                   \
public:                                                                                            \
    CLASS();                                                                                       \
    CLASS(const CLASS&);                                                                           \
    CLASS(Enum);                                                                                   \
    operator Enum() const;                                                                         \
    bool operator==(const CLASS&) const;                                                           \
    bool operator==(const Enum&) const;                                                            \
    bool operator!=(const CLASS&) const;                                                           \
    bool operator!=(const Enum&) const;

#define NVN_BITS(CLASS)                                                                            \
private:                                                                                           \
    int m_value;                                                                                   \
                                                                                                   \
public:                                                                                            \
    CLASS();                                                                                       \
    CLASS(const CLASS&);                                                                           \
    CLASS(int);                                                                                    \
    CLASS(Enum);                                                                                   \
    operator int() const;                                                                          \
    bool operator==(const CLASS&) const;                                                           \
    bool operator==(const Enum&) const;                                                            \
    bool operator!=(const CLASS&) const;                                                           \
    bool operator!=(const Enum&) const;                                                            \
    CLASS operator|(const CLASS&) const;                                                           \
    CLASS operator|(const Enum&) const;                                                            \
    CLASS operator&(const CLASS&) const;                                                           \
    CLASS operator&(const Enum&) const;                                                            \
    CLASS operator^(const CLASS&) const;                                                           \
    CLASS operator^(const Enum&) const;                                                            \
    CLASS& operator|=(const CLASS&);                                                               \
    CLASS& operator|=(const Enum&);                                                                \
    CLASS& operator&=(const CLASS&);                                                               \
    CLASS& operator&=(const Enum&);                                                                \
    CLASS& operator^=(const CLASS&);                                                               \
    CLASS& operator^=(const Enum&);

typedef uint8_t NVNboolean;
typedef void* NVNnativeWindow;
typedef void* NVNsubroutineLinkageMapPtr;
typedef int NVNstorageClass;
typedef int NVNdebugDomainId;

namespace nvn {

/*?*/ union QueueErrorInfo;
/*?*/ struct DrawTextureRegion;
/*?*/ struct ShaderDataExt;
/*?*/ struct MappingRequest;
/*?*/ struct TextureSparseTileLayout;
/*?*/ struct Rectangle;

/*330*/ typedef uint64_t BufferAddress;
/*333*/ typedef uint64_t CommandHandle;
/*337*/ typedef uint64_t TextureHandle;
/*341*/ typedef uint64_t ImageHandle;
/*349*/ typedef uint64_t TextureAddress;
/*361*/ struct SeparateSamplerHandle { uint64_t value; };
/*375*/ struct SeparateTextureHandle { uint64_t value; };

/*388*/ struct DeviceInfo {
    /*391*/ enum Enum {
        API_MAJOR_VERSION,
        API_MINOR_VERSION,
        UNIFORM_BUFFER_BINDINGS_PER_STAGE,
        MAX_UNIFORM_BUFFER_SIZE,
        UNIFORM_BUFFER_ALIGNMENT,
        COLOR_BUFFER_BINDINGS,
        VERTEX_BUFFER_BINDINGS,
        TRANSFORM_FEEDBACK_BUFFER_BINDINGS,
        SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE,
        TEXTURE_BINDINGS_PER_STAGE,
        COUNTER_ALIGNMENT,
        TRANSFORM_FEEDBACK_BUFFER_ALIGNMENT,
        TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT,
        INDIRECT_DRAW_ALIGNMENT,
        VERTEX_ATTRIBUTES,
        TEXTURE_DESCRIPTOR_SIZE,
        SAMPLER_DESCRIPTOR_SIZE,
        RESERVED_TEXTURE_DESCRIPTORS,
        RESERVED_SAMPLER_DESCRIPTORS,
        COMMAND_BUFFER_COMMAND_ALIGNMENT,
        COMMAND_BUFFER_CONTROL_ALIGNMENT,
        COMMAND_BUFFER_MIN_COMMAND_SIZE,
        COMMAND_BUFFER_MIN_CONTROL_SIZE,
        SHADER_SCRATCH_MEMORY_SCALE_FACTOR_MINIMUM,
        SHADER_SCRATCH_MEMORY_SCALE_FACTOR_RECOMMENDED,
        SHADER_SCRATCH_MEMORY_ALIGNMENT,
        SHADER_SCRATCH_MEMORY_GRANULARITY,
        MAX_TEXTURE_ANISOTROPY,
        MAX_COMPUTE_WORK_GROUP_SIZE_X,
        MAX_COMPUTE_WORK_GROUP_SIZE_Y,
        MAX_COMPUTE_WORK_GROUP_SIZE_Z,
        MAX_COMPUTE_WORK_GROUP_SIZE_THREADS,
        MAX_COMPUTE_DISPATCH_WORK_GROUPS_X,
        MAX_COMPUTE_DISPATCH_WORK_GROUPS_Y,
        MAX_COMPUTE_DISPATCH_WORK_GROUPS_Z,
        IMAGE_BINDINGS_PER_STAGE,
        MAX_TEXTURE_POOL_SIZE,
        MAX_SAMPLER_POOL_SIZE,
        MAX_VIEWPORTS,
        MEMPOOL_TEXTURE_OBJECT_PAGE_ALIGNMENT,
        SUPPORTS_MIN_MAX_FILTERING,
        SUPPORTS_STENCIL8_FORMAT,
        SUPPORTS_ASTC_FORMATS,
        L2_SIZE,
        MAX_TEXTURE_LEVELS,
        MAX_TEXTURE_LAYERS,
        GLSLC_MAX_SUPPORTED_GPU_CODE_MAJOR_VERSION,
        GLSLC_MIN_SUPPORTED_GPU_CODE_MAJOR_VERSION,
        GLSLC_MAX_SUPPORTED_GPU_CODE_MINOR_VERSION,
        GLSLC_MIN_SUPPORTED_GPU_CODE_MINOR_VERSION,
        SUPPORTS_CONSERVATIVE_RASTER,
        SUBPIXEL_BITS,
        MAX_SUBPIXEL_BIAS_BITS,
        INDIRECT_DISPATCH_ALIGNMENT,
        ZCULL_SAVE_RESTORE_ALIGNMENT,
        SHADER_SCRATCH_MEMORY_COMPUTE_SCALE_FACTOR_MINIMUM,
        LINEAR_TEXTURE_STRIDE_ALIGNMENT,
        LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT,
        MEMORY_POOL_PAGE_SIZE,
        SUPPORTS_ZERO_FROM_UNMAPPED_VIRTUAL_POOL_PAGES,
        UNIFORM_BUFFER_UPDATE_ALIGNMENT,
        MAX_TEXTURE_SIZE,
        MAX_BUFFER_TEXTURE_SIZE,
        MAX_3D_TEXTURE_SIZE,
        MAX_CUBE_MAP_TEXTURE_SIZE,
        MAX_RECTANGLE_TEXTURE_SIZE,
        SUPPORTS_PASSTHROUGH_GEOMETRY_SHADERS,
        SUPPORTS_VIEWPORT_SWIZZLE,
        SUPPORTS_SPARSE_TILED_PACKAGED_TEXTURES,
        SUPPORTS_ADVANCED_BLEND_MODES,
        MAX_PRESENT_INTERVAL,
        SUPPORTS_DRAW_TEXTURE,
        SUPPORTS_TARGET_INDEPENDENT_RASTERIZATION,
        SUPPORTS_FRAGMENT_COVERAGE_TO_COLOR,
        SUPPORTS_POST_DEPTH_COVERAGE,
        SUPPORTS_IMAGES_USING_TEXTURE_HANDLES,
        SUPPORTS_SAMPLE_LOCATIONS,
        MAX_SAMPLE_LOCATION_TABLE_ENTRIES,
        SHADER_CODE_MEMORY_POOL_PADDING_SIZE,
        MAX_PATCH_SIZE,
        QUEUE_COMMAND_MEMORY_GRANULARITY,
        QUEUE_COMMAND_MEMORY_MIN_SIZE,
        QUEUE_COMMAND_MEMORY_DEFAULT_SIZE,
        QUEUE_COMPUTE_MEMORY_GRANULARITY,
        QUEUE_COMPUTE_MEMORY_MIN_SIZE,
        QUEUE_COMPUTE_MEMORY_DEFAULT_SIZE,
        QUEUE_COMMAND_MEMORY_MIN_FLUSH_THRESHOLD,
        SUPPORTS_FRAGMENT_SHADER_INTERLOCK,
        MAX_TEXTURES_PER_WINDOW,
        MIN_TEXTURES_PER_WINDOW,
        SUPPORTS_DEBUG_LAYER,
        QUEUE_CONTROL_MEMORY_MIN_SIZE,
        QUEUE_CONTROL_MEMORY_DEFAULT_SIZE,
        QUEUE_CONTROL_MEMORY_GRANULARITY,
        SEPARATE_TEXTURE_BINDINGS_PER_STAGE,
        SEPARATE_SAMPLER_BINDINGS_PER_STAGE,
        DEBUG_GROUPS_MAX_DOMAIN_ID,
        EVENTS_SUPPORT_REDUCTION_OPERATIONS,
        NVN_DEVICE_INFO_LARGE = -1
    };

    NVN_ENUM(DeviceInfo);
};

/*981*/ struct TextureTarget {
    /*984*/ enum Enum {
        TARGET_1D,
        TARGET_2D,
        TARGET_3D,
        TARGET_1D_ARRAY,
        TARGET_2D_ARRAY,
        TARGET_2D_MULTISAMPLE,
        TARGET_2D_MULTISAMPLE_ARRAY,
        TARGET_RECTANGLE,
        TARGET_CUBEMAP,
        TARGET_CUBEMAP_ARRAY,
        TARGET_BUFFER,
        NVN_TEXTURE_TARGET_LARGE = -1
    };

    NVN_ENUM(TextureTarget);
};

/*1030*/ struct TextureSwizzle {
    /*1033*/ enum Enum { ZERO, ONE, R, G, B, A, NVN_TEXTURE_SWIZZLE_LARGE = -1 };

    NVN_ENUM(TextureSwizzle);
};

/*1066*/ struct TextureDepthStencilMode {
    /*1069*/ enum Enum { DEPTH, STENCIL, NVN_TEXTURE_DEPTH_STENCIL_MODE_LARGE = -1 };

    NVN_ENUM(TextureDepthStencilMode);
};

/*1091*/ struct Format {
    /*1094*/ enum Enum {
        NONE,
        R8,
        R8SN,
        R8UI,
        R8I,
        R16F,
        R16,
        R16SN,
        R16UI,
        R16I,
        R32F,
        R32UI,
        R32I,
        RG8,
        RG8SN,
        RG8UI,
        RG8I,
        RG16F,
        RG16,
        RG16SN,
        RG16UI,
        RG16I,
        RG32F,
        RG32UI,
        RG32I,
        RGB8,
        RGB8SN,
        RGB8UI,
        RGB8I,
        RGB16F,
        RGB16,
        RGB16SN,
        RGB16UI,
        RGB16I,
        RGB32F,
        RGB32UI,
        RGB32I,
        RGBA8,
        RGBA8SN,
        RGBA8UI,
        RGBA8I,
        RGBA16F,
        RGBA16,
        RGBA16SN,
        RGBA16UI,
        RGBA16I,
        RGBA32F,
        RGBA32UI,
        RGBA32I,
        STENCIL8,
        DEPTH16,
        DEPTH24,
        DEPTH32F,
        DEPTH24_STENCIL8,
        DEPTH32F_STENCIL8,
        RGBX8_SRGB,
        RGBA8_SRGB,
        RGBA4,
        RGB5,
        RGB5A1,
        RGB565,
        RGB10A2,
        RGB10A2UI,
        R11G11B10F,
        RGB9E5F,
        RGB_DXT1,
        RGBA_DXT1,
        RGBA_DXT3,
        RGBA_DXT5,
        RGB_DXT1_SRGB,
        RGBA_DXT1_SRGB,
        RGBA_DXT3_SRGB,
        RGBA_DXT5_SRGB,
        RGTC1_UNORM,
        RGTC1_SNORM,
        RGTC2_UNORM,
        RGTC2_SNORM,
        BPTC_UNORM,
        BPTC_UNORM_SRGB,
        BPTC_SFLOAT,
        BPTC_UFLOAT,
        R8_UI2F,
        R8_I2F,
        R16_UI2F,
        R16_I2F,
        R32_UI2F,
        R32_I2F,
        RG8_UI2F,
        RG8_I2F,
        RG16_UI2F,
        RG16_I2F,
        RG32_UI2F,
        RG32_I2F,
        RGB8_UI2F,
        RGB8_I2F,
        RGB16_UI2F,
        RGB16_I2F,
        RGB32_UI2F,
        RGB32_I2F,
        RGBA8_UI2F,
        RGBA8_I2F,
        RGBA16_UI2F,
        RGBA16_I2F,
        RGBA32_UI2F,
        RGBA32_I2F,
        RGB10A2SN,
        RGB10A2I,
        RGB10A2_UI2F,
        RGB10A2_I2F,
        RGBX8,
        RGBX8SN,
        RGBX8UI,
        RGBX8I,
        RGBX16F,
        RGBX16,
        RGBX16SN,
        RGBX16UI,
        RGBX16I,
        RGBX32F,
        RGBX32UI,
        RGBX32I,
        RGBA_ASTC_4x4,
        RGBA_ASTC_5x4,
        RGBA_ASTC_5x5,
        RGBA_ASTC_6x5,
        RGBA_ASTC_6x6,
        RGBA_ASTC_8x5,
        RGBA_ASTC_8x6,
        RGBA_ASTC_8x8,
        RGBA_ASTC_10x5,
        RGBA_ASTC_10x6,
        RGBA_ASTC_10x8,
        RGBA_ASTC_10x10,
        RGBA_ASTC_12x10,
        RGBA_ASTC_12x12,
        RGBA_ASTC_4x4_SRGB,
        RGBA_ASTC_5x4_SRGB,
        RGBA_ASTC_5x5_SRGB,
        RGBA_ASTC_6x5_SRGB,
        RGBA_ASTC_6x6_SRGB,
        RGBA_ASTC_8x5_SRGB,
        RGBA_ASTC_8x6_SRGB,
        RGBA_ASTC_8x8_SRGB,
        RGBA_ASTC_10x5_SRGB,
        RGBA_ASTC_10x6_SRGB,
        RGBA_ASTC_10x8_SRGB,
        RGBA_ASTC_10x10_SRGB,
        RGBA_ASTC_12x10_SRGB,
        RGBA_ASTC_12x12_SRGB,
        BGR565,
        BGR5,
        BGR5A1,
        A1BGR5,
        BGRX8,
        BGRA8,
        BGRX8_SRGB,
        BGRA8_SRGB,
        NVN_FORMAT_LARGE = -1
    };

    NVN_ENUM(Format);
};

/*1650*/ struct BlendFunc {
    /*1653*/ enum Enum {
        ZERO = 1,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA_SATURATE,
        SRC1_COLOR = 16,
        ONE_MINUS_SRC1_COLOR,
        SRC1_ALPHA,
        ONE_MINUS_SRC1_ALPHA,
        CONSTANT_COLOR = 97,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA,
        NVN_BLEND_FUNC_LARGE = -1
    };

    NVN_ENUM(BlendFunc);
};

/*1742*/ struct BlendAdvancedMode {
    /*1745*/ enum Enum {
        BLEND_NONE,
        BLEND_ZERO = 5,
        BLEND_SRC,
        BLEND_DST,
        BLEND_SRC_OVER,
        BLEND_DST_OVER,
        BLEND_SRC_IN,
        BLEND_DST_IN,
        BLEND_SRC_OUT,
        BLEND_DST_OUT,
        BLEND_SRC_ATOP,
        BLEND_DST_ATOP,
        BLEND_XOR,
        BLEND_PLUS,
        BLEND_PLUS_CLAMPED,
        BLEND_PLUS_CLAMPED_ALPHA,
        BLEND_PLUS_DARKER,
        BLEND_MULTIPLY,
        BLEND_SCREEN,
        BLEND_OVERLAY,
        BLEND_DARKEN,
        BLEND_LIGHTEN,
        BLEND_COLORDODGE,
        BLEND_COLORBURN,
        BLEND_HARDLIGHT,
        BLEND_SOFTLIGHT,
        BLEND_DIFFERENCE,
        BLEND_MINUS,
        BLEND_MINUS_CLAMPED,
        BLEND_EXCLUSION,
        BLEND_CONTRAST,
        BLEND_INVERT,
        BLEND_INVERT_RGB,
        BLEND_INVERT_OVG,
        BLEND_LINEARDODGE,
        BLEND_LINEARBURN,
        BLEND_VIVIDLIGHT,
        BLEND_LINEARLIGHT,
        BLEND_PINLIGHT,
        BLEND_HARDMIX,
        BLEND_RED,
        BLEND_GREEN,
        BLEND_BLUE,
        BLEND_HSL_HUE,
        BLEND_HSL_SATURATION,
        BLEND_HSL_COLOR,
        BLEND_HSL_LUMINOSITY,
        NVN_BLEND_ADVANCED_MODE_LARGE = -1
    };

    NVN_ENUM(BlendAdvancedMode);
};

/*1900*/ struct BlendAdvancedOverlap {
    /*1903*/ enum Enum { UNCORRELATED, DISJOINT, CONJOINT, NVN_BLEND_ADVANCED_OVERLAP_LARGE = -1 };

    NVN_ENUM(BlendAdvancedOverlap);
};

/*1944*/ struct BlendEquation {
    /*1947*/ enum Enum { ADD = 1, SUB, REVERSE_SUB, MIN, MAX, NVN_BLEND_EQUATION_LARGE = -1 };

    NVN_ENUM(BlendEquation);
};

/*1978*/ struct LogicOp {
    /*1981*/ enum Enum {
        CLEAR,
        AND,
        AND_REVERSE,
        COPY,
        AND_INVERTED,
        NOOP,
        XOR,
        OR,
        NOR,
        EQUIV,
        INVERT,
        OR_REVERSE,
        COPY_INVERTED,
        OR_INVERTED,
        NAND,
        SET,
        NVN_LOGIC_OP_LARGE = -1
    };

    NVN_ENUM(LogicOp);
};

/*2038*/ struct AlphaFunc {
    /*2041*/ enum Enum {
        NEVER = 1,
        LESS,
        EQUAL,
        LEQUAL,
        GREATER,
        NOTEQUAL,
        GEQUAL,
        ALWAYS,
        NVN_ALPHA_FUNC_LARGE = -1
    };

    NVN_ENUM(AlphaFunc);
};

/*2079*/ struct DrawPrimitive {
    /*2082*/ enum Enum {
        POINTS,
        LINES,
        LINE_LOOP,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        QUADS,
        QUAD_STRIP,
        POLYGON,
        LINES_ADJACENCY,
        LINE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY,
        TRIANGLE_STRIP_ADJACENCY,
        PATCHES,
        NVN_DRAW_PRIMITIVE_LARGE = -1
    };

    NVN_ENUM(DrawPrimitive);
};

/*2201*/ struct IndexType {
    /*2204*/ enum Enum { UNSIGNED_BYTE, UNSIGNED_SHORT, UNSIGNED_INT, NVN_INDEX_TYPE_LARGE = -1 };

    NVN_ENUM(IndexType);
};

/*2222*/ struct DepthFunc {
    /*2225*/ enum Enum {
        NEVER = 1,
        LESS,
        EQUAL,
        LEQUAL,
        GREATER,
        NOTEQUAL,
        GEQUAL,
        ALWAYS,
        NVN_DEPTH_FUNC_LARGE = -1
    };

    NVN_ENUM(DepthFunc);
};

/*2267*/ struct MagFilter {
    /*2270*/ enum Enum { NEAREST, LINEAR, NVN_MAG_FILTER_LARGE = -1 };

    NVN_ENUM(MagFilter);
};

/*2293*/ struct MinFilter {
    /*2296*/ enum Enum {
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR,
        NVN_MIN_FILTER_LARGE = -1
    };

    NVN_ENUM(MinFilter);
};

/*2336*/ struct WrapMode {
    /*2339*/ enum Enum {
        CLAMP,
        REPEAT,
        MIRROR_CLAMP,
        MIRROR_CLAMP_TO_EDGE,
        MIRROR_CLAMP_TO_BORDER,
        CLAMP_TO_BORDER,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        NVN_WRAP_MODE_LARGE = -1
    };

    NVN_ENUM(WrapMode);
};

/*2394*/ struct CompareMode {
    /*2397*/ enum Enum { NONE, COMPARE_R_TO_TEXTURE, NVN_COMPARE_MODE_LARGE = -1 };

    NVN_ENUM(CompareMode);
};

/*2414*/ struct CompareFunc {
    /*2417*/ enum Enum {
        NEVER = 1,
        LESS,
        EQUAL,
        LEQUAL,
        GREATER,
        NOTEQUAL,
        GEQUAL,
        ALWAYS,
        NVN_COMPARE_FUNC_LARGE = -1
    };

    NVN_ENUM(CompareFunc);
};

/*2455*/ struct SamplerReduction {
    /*2458*/ enum Enum { AVERAGE, MIN, MAX, NVN_SAMPLER_REDUCTION_LARGE = -1 };

    NVN_ENUM(SamplerReduction);
};

/*2485*/ struct Face {
    /*2488*/ enum Enum { NONE, FRONT, BACK, FRONT_AND_BACK, NVN_FACE_LARGE = -1 };

    NVN_ENUM(Face);
};

/*2513*/ struct StencilFunc {
    /*2516*/ enum Enum {
        NEVER = 1,
        LESS,
        EQUAL,
        LEQUAL,
        GREATER,
        NOTEQUAL,
        GEQUAL,
        ALWAYS,
        NVN_STENCIL_FUNC_LARGE = -1
    };

    NVN_ENUM(StencilFunc);
};

/*2559*/ struct StencilOp {
    /*2562*/ enum Enum {
        KEEP = 1,
        ZERO,
        REPLACE,
        INCR,
        DECR,
        INVERT,
        INCR_WRAP,
        DECR_WRAP,
        NVN_STENCIL_OP_LARGE = -1
    };

    NVN_ENUM(StencilOp);
};

/*2603*/ struct FrontFace {
    /*2606*/ enum Enum { CW, CCW, NVN_FRONT_FACE_LARGE = -1 };

    NVN_ENUM(FrontFace);
};

/*2621*/ struct PolygonMode {
    /*2624*/ enum Enum { POINT, LINE, FILL, NVN_POLYGON_MODE_LARGE = -1 };

    NVN_ENUM(PolygonMode);
};

/*2649*/ struct PolygonOffsetEnable {
    /*2652*/ enum Enum {
        NONE = 0,
        POINT = 1,
        LINE = 2,
        FILL = 4,
        NVN_POLYGON_OFFSET_ENABLE_LARGE = -1
    };

    NVN_BITS(PolygonOffsetEnable);
};

/*2677*/ struct SyncCondition {
    /*2680*/ enum Enum {
        ALL_GPU_COMMANDS_COMPLETE,
        GRAPHICS_WORLD_SPACE_COMPLETE,
        NVN_SYNC_CONDITION_LARGE = -1
    };

    NVN_ENUM(SyncCondition);
};

/*2696*/ struct SyncWaitResult {
    /*2699*/ enum Enum {
        ALREADY_SIGNALED,
        CONDITION_SATISFIED,
        TIMEOUT_EXPIRED,
        FAILED,
        NVN_SYNC_WAIT_RESULT_LARGE = -1
    };

    NVN_ENUM(SyncWaitResult);
};

/*2726*/ struct QueueAcquireTextureResult {
    /*2729*/ enum Enum { SUCCESS, NATIVE_ERROR, NVN_QUEUE_ACQUIRE_TEXTURE_RESULT_LARGE = -1 };

    NVN_ENUM(QueueAcquireTextureResult);
};

/*2744*/ struct WindowAcquireTextureResult {
    /*2747*/ enum Enum { SUCCESS, NATIVE_ERROR, NVN_WINDOW_ACQUIRE_TEXTURE_RESULT_LARGE = -1 };

    NVN_ENUM(WindowAcquireTextureResult);
};

/*2762*/ struct ShaderStage {
    /*2765*/ enum Enum {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        TESS_CONTROL,
        TESS_EVALUATION,
        COMPUTE,
        NVN_SHADER_STAGE_LARGE = -1
    };

    NVN_ENUM(ShaderStage);
};

/*2792*/ struct TiledCacheAction {
    /*2795*/ enum Enum {
        ENABLE = 1,
        DISABLE,
        FLUSH,
        FLUSH_NO_TILING,
        ENABLE_RENDER_TARGET_BINNING,
        DISABLE_RENDER_TARGET_BINNING,
        NVN_TILED_CACHE_ACTION_LARGE = -1
    };

    NVN_ENUM(TiledCacheAction);
};

/*2836*/ struct ShaderStageBits {
    /*2839*/ enum Enum {
        VERTEX = 1,
        FRAGMENT = 2,
        GEOMETRY = 4,
        TESS_CONTROL = 8,
        TESS_EVALUATION = 16,
        COMPUTE = 32,
        ALL_GRAPHICS_BITS = 31,
        NVN_SHADER_STAGE_BITS_LARGE = -1
    };

    NVN_BITS(ShaderStageBits);
};

/*2873*/ struct SyncFlagBits {
    /*2876*/ enum Enum { FLUSH_FOR_CPU = 1, NVN_SYNC_FLAG_BITS_LARGE = -1 };

    NVN_BITS(SyncFlagBits);
};

/*2889*/ struct DeviceFlagBits {
    /*2892*/ enum Enum {
        DEBUG_ENABLE = 1,
        DEBUG_SKIP_CALLS_ON_ERROR = 2,
        DEBUG_DRAW_VALIDATION = 4,
        DEFERRED_FINALIZE = 8,
        DEBUG_DRAW_VALIDATION_HEAVY = 16,
        DEBUG_ENABLE_LEVEL_0 = 32,
        DEBUG_ENABLE_LEVEL_1 = 64,
        DEBUG_ENABLE_LEVEL_2 = 1,
        DEBUG_ENABLE_LEVEL_3 = 4,
        DEBUG_ENABLE_LEVEL_4 = 16,
        DEFERRED_FIRMWARE_MEMORY_RECLAIM = 128,
        ENABLE_SEPARATE_SAMPLER_TEXTURE_SUPPORT = 256,
        NVN_DEVICE_FLAG_BITS_LARGE = -1
    };

    NVN_BITS(DeviceFlagBits);
};

/*3030*/ struct ClearColorMask {
    /*3033*/ enum Enum { R = 1, G = 2, B = 4, A = 8, RGBA = 15, NVN_CLEAR_COLOR_MASK_LARGE = -1 };

    NVN_BITS(ClearColorMask);
};

/*3061*/ struct DebugCallbackSource {
    /*3064*/ enum Enum { API, NVN_DEBUG_CALLBACK_SOURCE_LARGE = -1 };

    NVN_ENUM(DebugCallbackSource);
};

/*3076*/ struct DebugCallbackType {
    /*3079*/ enum Enum { API_ERROR, API_WARNING, NVN_DEBUG_CALLBACK_TYPE_LARGE = -1 };

    NVN_ENUM(DebugCallbackType);
};

/*3095*/ struct DebugCallbackSeverity {
    /*3098*/ enum Enum { HIGH, MEDIUM, LOW, NOTIFICATION, NVN_DEBUG_CALLBACK_SEVERITY_LARGE = -1 };

    NVN_ENUM(DebugCallbackSeverity);
};

/*3123*/ struct MemoryPoolFlags {
    /*3126*/ enum Enum {
        CPU_NO_ACCESS = 1,
        CPU_UNCACHED = 2,
        CPU_CACHED = 4,
        GPU_NO_ACCESS = 8,
        GPU_UNCACHED = 16,
        GPU_CACHED = 32,
        SHADER_CODE = 64,
        COMPRESSIBLE = 128,
        PHYSICAL = 256,
        VIRTUAL = 512,
        NVN_MEMORY_POOL_FLAGS_LARGE = -1
    };

    NVN_BITS(MemoryPoolFlags);
};

/*3314*/ struct CounterType {
    /*3317*/ enum Enum {
        TIMESTAMP,
        SAMPLES_PASSED,
        INPUT_VERTICES,
        INPUT_PRIMITIVES,
        VERTEX_SHADER_INVOCATIONS,
        TESS_CONTROL_SHADER_INVOCATIONS,
        TESS_EVALUATION_SHADER_INVOCATIONS,
        GEOMETRY_SHADER_INVOCATIONS,
        FRAGMENT_SHADER_INVOCATIONS,
        TESS_EVALUATION_SHADER_PRIMITIVES,
        GEOMETRY_SHADER_PRIMITIVES,
        CLIPPER_INPUT_PRIMITIVES,
        CLIPPER_OUTPUT_PRIMITIVES,
        PRIMITIVES_GENERATED,
        TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,
        ZCULL_STATS,
        TIMESTAMP_TOP,
        NVN_COUNTER_TYPE_LARGE = -1
    };

    NVN_ENUM(CounterType);
};

/*3462*/ struct CommandBufferMemoryEvent {
    /*3465*/ enum Enum {
        OUT_OF_COMMAND_MEMORY,
        OUT_OF_CONTROL_MEMORY,
        NVN_COMMAND_BUFFER_MEMORY_EVENT_LARGE = -1
    };

    NVN_ENUM(CommandBufferMemoryEvent);
};

/*3487*/ struct ConditionalRenderMode {
    /*3490*/ enum Enum {
        RENDER_IF_EQUAL,
        RENDER_IF_NOT_EQUAL,
        NVN_CONDITIONAL_RENDER_MODE_LARGE = -1
    };

    NVN_ENUM(ConditionalRenderMode);
};

/*3507*/ struct WindowOriginMode {
    /*3510*/ enum Enum { LOWER_LEFT, UPPER_LEFT, NVN_WINDOW_ORIGIN_MODE_LARGE = -1 };

    NVN_ENUM(WindowOriginMode);
};

/*3526*/ struct DepthMode {
    /*3529*/ enum Enum { NEAR_IS_MINUS_W, NEAR_IS_ZERO, NVN_DEPTH_MODE_LARGE = -1 };

    NVN_ENUM(DepthMode);
};

/*3549*/ struct TextureFlags {
    /*3552*/ enum Enum {
        DISPLAY = 1,
        VIDEO_DECODE = 2,
        IMAGE = 4,
        COMPRESSIBLE = 8,
        LINEAR = 16,
        SPARSE = 32,
        LINEAR_RENDER_TARGET = 64,
        ADAPTIVE_ZCULL = 128,
        DEPTH16_PREFER_FAST_CLEAR = 256,
        MINIMAL_LAYOUT = 512,
        ZCULL_SUPPORT_STENCIL = 1024,
        NVN_TEXTURE_FLAGS_LARGE = -1
    };

    NVN_BITS(TextureFlags);
};

/*3741*/ struct BarrierBits {
    /*3744*/ enum Enum {
        ORDER_PRIMITIVES = 1,
        ORDER_FRAGMENTS = 2,
        ORDER_FRAGMENTS_TILED = 4,
        ORDER_INDIRECT_DATA = 8,
        INVALIDATE_TEXTURE = 16,
        INVALIDATE_SHADER = 32,
        INVALIDATE_TEXTURE_DESCRIPTOR = 64,
        INVALIDATE_ZCULL = 128,
        INVALIDATE_L2_CACHE = 256,
        NVN_BARRIER_BITS_LARGE = -1
    };

    NVN_BITS(BarrierBits);
};

/*3894*/ struct CopyFlags {
    /*3897*/ enum Enum {
        NONE = 0,
        LINEAR_FILTER = 1,
        ENGINE_2D = 2,
        MIRROR_X = 4,
        MIRROR_Y = 8,
        MIRROR_Z = 16,
        NVN_COPY_FLAGS_LARGE = -1
    };

    NVN_BITS(CopyFlags);
};

/*3958*/ struct ViewportSwizzle {
    /*3961*/ enum Enum {
        POSITIVE_X,
        NEGATIVE_X,
        POSITIVE_Y,
        NEGATIVE_Y,
        POSITIVE_Z,
        NEGATIVE_Z,
        POSITIVE_W,
        NEGATIVE_W,
        NVN_VIEWPORT_SWIZZLE_LARGE = -1
    };

    NVN_ENUM(ViewportSwizzle);
};

/*4007*/ struct CoverageModulationMode {
    /*4010*/ enum Enum { NONE, RGB, ALPHA, RGBA, NVN_COVERAGE_MODULATION_MODE_LARGE = -1 };

    NVN_ENUM(CoverageModulationMode);
};

/*4033*/ struct QueueFlags {
    /*4036*/ enum Enum {
        NO_FRAGMENT_INTERLOCK = 1,
        NO_ZCULL = 2,
        NONE = 0,
        NVN_QUEUE_FLAGS_LARGE = -1
    };

    NVN_BITS(QueueFlags);
};

/*4073*/ struct DebugObjectType {
    /*4076*/ enum Enum {
        WINDOW,
        QUEUE,
        COMMAND_BUFFER,
        MEMORY_POOL,
        SYNC = 7,
        PROGRAM = 9,
        TEXTURE_POOL,
        SAMPLER_POOL,
        NVN_DEBUG_OBJECT_TYPE_LARGE = -1
    };

    NVN_ENUM(DebugObjectType);
};

/*4110*/ struct EventSignalMode {
    /*4113*/ enum Enum { WRITE, ADD, NVN_EVENT_SIGNAL_MODE_LARGE = -1 };

    NVN_ENUM(EventSignalMode);
};

/*4139*/ struct EventWaitMode {
    /*4142*/ enum Enum { EQUAL, GEQUAL_WRAP, NVN_EVENT_WAIT_MODE_LARGE = -1 };

    NVN_ENUM(EventWaitMode);
};

/*4167*/ struct EventSignalLocation {
    /*4170*/ enum Enum { TOP, VERTEX_PIPE, BOTTOM, NVN_EVENT_SIGNAL_LOCATION_LARGE = -1 };

    NVN_ENUM(EventSignalLocation);
};

/*4191*/ struct EventSignalFlags {
    /*4194*/ enum Enum { NO_WAIT_MEMORY = 1, NVN_EVENT_SIGNAL_FLAGS_LARGE = -1 };

    NVN_BITS(EventSignalFlags);
};

/*4216*/ struct FormatClass {
    /*4219*/ enum Enum { UNORM8, UNORM10, UNORM16, FP11, FP16, SRGB8, NVN_FORMAT_CLASS_LARGE = -1 };

    NVN_ENUM(FormatClass);
};

/*4269*/ struct QueueGetErrorResult {
    /*4272*/ enum Enum {
        GPU_NO_ERROR,
        GPU_ERROR_UNKNOWN,
        GPU_ERROR_MMU_FAULT,
        GPU_ERROR_PBDMA_EXCEPTION,
        GPU_ERROR_ENGINE_EXCEPTION,
        GPU_ERROR_TIMEOUT,
        NVN_QUEUE_GET_ERROR_RESULT_LARGE = -1
    };

    NVN_ENUM(QueueGetErrorResult);
};

/*4328*/ struct QueuePriority {
    /*4331*/ enum Enum { LOW = 10, NORMAL = 100, HIGH = 1000, NVN_QUEUE_PRIORITY_LARGE = -1 };

    NVN_ENUM(QueuePriority);
};

/*4364*/ struct RawStorageClass {
    /*4367*/ enum Enum {
        NONE,
        LINEAR,
        COLOR_AND_ZF32,
        S8_AND_Z16,
        Z24_AND_Z24S8,
        ZF32S8,
        NVN_RAW_STORAGE_CLASS_LARGE = -1
    };

    NVN_ENUM(RawStorageClass);
};

/*4400*/ struct SampleShadingMode {
    /*4403*/ enum Enum {
        DEFAULT_FRAGMENT,
        FORCE_SAMPLE_SHADING_ON,
        FORCE_SAMPLE_SHADING_OFF,
        NVN_SAMPLE_SHADING_MODE_LARGE = -1
    };

    NVN_ENUM(SampleShadingMode);
};

/*4432*/ struct ShaderData {
    BufferAddress data;
    const void* control;
};

/*4412*/ struct CounterData {
    uint64_t counter;
    uint64_t timestamp;
};

/*4519*/ struct CopyRegion {
    int xoffset;
    int yoffset;
    int zoffset;
    int width;
    int height;
    int depth;
};

/*4601*/ struct PackagedTextureLayout { char layout[8]; };

/*4613*/ struct BufferRange {
    BufferAddress address;
    uint64_t size;
};

/*4686*/ typedef void (*GenericFuncPtrFunc)(void);
/*4720*/ typedef void (*DebugCallbackFunc)(DebugCallbackSource::Enum, DebugCallbackType::Enum, int,
                                           DebugCallbackSeverity::Enum, const char*, void*);
/*4748*/ typedef void (*CommandBufferMemoryCallbackFunc)(CommandBuffer*,
                                                         CommandBufferMemoryEvent::Enum, size_t,
                                                         void*);
/*4761*/ typedef void (*WalkDebugDatabaseCallbackFunc)(void*, void*);

/*4770*/ class DeviceBuilder {
protected:
    char reserved[64];

public:
    void SetDefaults();
    void SetFlags(struct DeviceFlagBits);
    struct DeviceFlagBits GetFlags() const;
};

/*4791*/ class Device {
protected:
    char reserved[12288];

    Device(const Device&);

public:
    Device();

protected:
    const Device& operator=(const Device&);

public:
    NVNboolean Initialize(const DeviceBuilder*);
    void Finalize();
    void SetDebugLabel(const char*);
    GenericFuncPtrFunc GetProcAddress(const char*) const;
    void GetInteger(DeviceInfo, int*) const;
    uint64_t GetCurrentTimestampInNanoseconds() const;
    void SetIntermediateShaderCache(int);
    TextureHandle GetTextureHandle(int, int) const;
    TextureHandle GetTexelFetchHandle(int) const;
    ImageHandle GetImageHandle(int) const;
    void InstallDebugCallback(const DebugCallbackFunc, void*, NVNboolean);
    NVNdebugDomainId GenerateDebugDomainId(const char*) const;
    void SetWindowOriginMode(WindowOriginMode);
    void SetDepthMode(DepthMode);
    NVNboolean RegisterFastClearColor(const float*, Format);
    NVNboolean RegisterFastClearColori(const int*, Format);
    NVNboolean RegisterFastClearColorui(const uint32_t*, Format);
    NVNboolean RegisterFastClearDepth(float);
    WindowOriginMode GetWindowOriginMode() const;
    DepthMode GetDepthMode() const;
    uint64_t GetTimestampInNanoseconds(const CounterData*) const;
    void ApplyDeferredFinalizes(int);
    void FinalizeCommandHandle(CommandHandle);
    void WalkDebugDatabase(DebugObjectType, WalkDebugDatabaseCallbackFunc, void*) const;
    SeparateTextureHandle GetSeparateTextureHandle(int) const;
    SeparateTextureHandle getSeparateTextureHandle(int) const;
    SeparateSamplerHandle GetSeparateSamplerHandle(int) const;
    SeparateSamplerHandle getSeparateSamplerHandle(int) const;
    NVNboolean IsExternalDebuggerAttached() const;
};

/*5137*/ class QueueBuilder {
protected:
    char reserved[64];

public:
    QueueBuilder& SetDevice(Device*);
    QueueBuilder& SetDefaults();
    QueueBuilder& SetFlags(QueueFlags);
    QueueBuilder& SetCommandMemorySize(size_t);
    QueueBuilder& SetComputeMemorySize(size_t);
    QueueBuilder& SetControlMemorySize(size_t);
    size_t GetQueueMemorySize() const;
    QueueBuilder& SetQueueMemory(void*, size_t);
    QueueBuilder& SetCommandFlushThreshold(size_t);
    QueueBuilder& SetQueuePriority(const QueuePriority);
    QueuePriority GetQueuePriority() const;
    const Device* GetDevice() const;
    int GetFlags() const;
    NVNboolean GetCommandMemorySize(int*) const;
    NVNboolean GetComputeMemorySize(int*) const;
    NVNboolean GetControlMemorySize(int*) const;
    NVNboolean GetCommandFlushThreshold(int*) const;
    int GetMemorySize() const;
    void* GetMemory() const;
};

/*5367*/ class Queue {
protected:
    char reserved[8192];

    Queue(const Queue&);

public:
    Queue();

protected:
    const Queue& operator=(const Queue&);

public:
    QueueGetErrorResult GetError(QueueErrorInfo*);
    size_t GetTotalCommandMemoryUsed();
    size_t GetTotalControlMemoryUsed();
    size_t GetTotalComputeMemoryUsed();
    void ResetMemoryUsageCounts();
    NVNboolean Initialize(const QueueBuilder*);
    void Finalize();
    void SetDebugLabel(const char*);
    void SubmitCommands(int, const nvn::CommandHandle*);
    void Flush();
    void Finish();
    void PresentTexture(Window*, int);
    QueueAcquireTextureResult AcquireTexture(Window*, int*);
    void FenceSync(Sync*, SyncCondition, SyncFlagBits);
    NVNboolean WaitSync(const Sync*);
};

/*5570*/ class CommandBuffer {
protected:
    char reserved[160];

    CommandBuffer(const CommandBuffer&);

public:
    CommandBuffer();

protected:
    const CommandBuffer& operator=(const CommandBuffer&);

public:
    NVNboolean Initialize(Device*);
    void Finalize();
    void SetDebugLabel(const char*);
    void SetMemoryCallback(CommandBufferMemoryCallbackFunc);
    void SetMemoryCallbackData(void*);
    void AddCommandMemory(const MemoryPool*, ptrdiff_t, size_t);
    void AddControlMemory(void*, size_t);
    size_t GetCommandMemorySize() const;
    size_t GetCommandMemoryUsed() const;
    size_t GetCommandMemoryFree() const;
    size_t GetControlMemorySize() const;
    size_t GetControlMemoryUsed() const;
    size_t GetControlMemoryFree() const;
    void BeginRecording();
    CommandHandle EndRecording();
    void CallCommands(int, const CommandHandle*);
    void CopyCommands(int, const CommandHandle*);
    void BindBlendState(const BlendState*);
    void BindChannelMaskState(const ChannelMaskState*);
    void BindColorState(const ColorState*);
    void BindMultisampleState(const MultisampleState*);
    void BindPolygonState(const PolygonState*);
    void BindDepthStencilState(const DepthStencilState*);
    void BindVertexAttribState(int, const VertexAttribState*);
    void BindVertexStreamState(int, const VertexStreamState*);
    void BindProgram(const Program*, ShaderStageBits);
    void BindVertexBuffer(int, BufferAddress, size_t);
    void BindVertexBuffers(int, int, const BufferRange*);
    void BindUniformBuffer(ShaderStage, int, BufferAddress, size_t);
    void BindUniformBuffers(ShaderStage, int, int, const BufferRange*);
    void BindTransformFeedbackBuffer(int, BufferAddress, size_t);
    void BindTransformFeedbackBuffers(int, int, const BufferRange*);
    void BindStorageBuffer(ShaderStage, int, BufferAddress, size_t);
    void BindStorageBuffers(ShaderStage, int, int, const BufferRange*);
    void BindTexture(ShaderStage, int, TextureHandle);
    void BindTextures(ShaderStage, int, int, const TextureHandle*);
    void BindImage(ShaderStage, int, ImageHandle);
    void BindImages(ShaderStage, int, int, const ImageHandle*);
    void SetPatchSize(int);
    void SetInnerTessellationLevels(const float*);
    void SetOuterTessellationLevels(const float*);
    void SetPrimitiveRestart(NVNboolean, int);
    void BeginTransformFeedback(BufferAddress);
    void EndTransformFeedback(BufferAddress);
    void PauseTransformFeedback(BufferAddress);
    void ResumeTransformFeedback(BufferAddress);
    void DrawTransformFeedback(DrawPrimitive, BufferAddress);
    void DrawArrays(DrawPrimitive, int, int);
    void DrawElements(DrawPrimitive, IndexType, int, BufferAddress);
    void DrawElementsBaseVertex(DrawPrimitive, IndexType, int, BufferAddress, int);
    void DrawArraysInstanced(DrawPrimitive, int, int, int, int);
    void DrawElementsInstanced(DrawPrimitive, IndexType, int, BufferAddress, int, int, int);
    void DrawArraysIndirect(DrawPrimitive, BufferAddress);
    void DrawElementsIndirect(DrawPrimitive, IndexType, BufferAddress, BufferAddress);
    void MultiDrawArraysIndirectCount(DrawPrimitive, BufferAddress, BufferAddress, int, ptrdiff_t);
    void MultiDrawElementsIndirectCount(DrawPrimitive, IndexType, BufferAddress, BufferAddress,
                                        BufferAddress, int, ptrdiff_t);
    void ClearColor(int, const float*, ClearColorMask);
    void ClearColori(int, const int*, ClearColorMask);
    void ClearColorui(int, const uint32_t*, ClearColorMask);
    void ClearDepthStencil(float, NVNboolean, int, int);
    void DispatchCompute(int, int, int);
    void DispatchComputeIndirect(BufferAddress);
    void SetViewport(int, int, int, int);
    void SetViewports(int, int, const float*);
    void SetViewportSwizzles(int, int, const ViewportSwizzle*);
    void SetScissor(int, int, int, int);
    void SetScissors(int, int, const int*);
    void SetDepthRange(float, float);
    void SetDepthBounds(NVNboolean, float, float);
    void SetDepthRanges(int, int, const float*);
    void SetTiledCacheAction(TiledCacheAction);
    void SetTiledCacheTileSize(int, int);
    void BindSeparateTexture(ShaderStage, int, SeparateTextureHandle);
    void bindSeparateTexture(ShaderStage, int, SeparateTextureHandle);
    void BindSeparateSampler(ShaderStage, int, SeparateSamplerHandle);
    void bindSeparateSampler(ShaderStage, int, SeparateSamplerHandle);
    void BindSeparateTextures(ShaderStage, int, int, const SeparateTextureHandle*);
    void BindSeparateSamplers(ShaderStage, int, int, const SeparateSamplerHandle*);
    void SetStencilValueMask(Face, int);
    void SetStencilMask(Face, int);
    void SetStencilRef(Face, int);
    void SetBlendColor(const float*);
    void SetPointSize(float);
    void SetLineWidth(float);
    void SetPolygonOffsetClamp(float, float, float);
    void SetAlphaRef(float);
    void SetSampleMask(int);
    void SetRasterizerDiscard(NVNboolean);
    void SetDepthClamp(NVNboolean);
    void SetConservativeRasterEnable(NVNboolean);
    void SetConservativeRasterDilate(float);
    void SetSubpixelPrecisionBias(int, int);
    void CopyBufferToTexture(BufferAddress, const Texture*, const TextureView*, const CopyRegion*,
                             CopyFlags);
    void CopyTextureToBuffer(const Texture*, const TextureView*, const CopyRegion*, BufferAddress,
                             CopyFlags);
    void CopyTextureToTexture(const Texture*, const TextureView*, const CopyRegion*, const Texture*,
                              const TextureView*, const CopyRegion*, CopyFlags);
    void CopyBufferToBuffer(BufferAddress, BufferAddress, size_t, CopyFlags);
    void ClearBuffer(BufferAddress, size_t, uint32_t);
    void ClearTexture(const Texture*, const TextureView*, const CopyRegion*, const float*,
                      ClearColorMask);
    void ClearTexturei(const Texture*, const TextureView*, const CopyRegion*, const int*,
                       ClearColorMask);
    void ClearTextureui(const Texture*, const TextureView*, const CopyRegion*, const uint32_t*,
                        ClearColorMask);
    void UpdateUniformBuffer(BufferAddress, size_t, ptrdiff_t, size_t, const void*);
    void ReportCounter(CounterType, BufferAddress);
    void ResetCounter(CounterType);
    void ReportValue(uint32_t, BufferAddress);
    void SetRenderEnable(NVNboolean);
    void SetRenderEnableConditional(ConditionalRenderMode, BufferAddress);
    void SetRenderTargets(int, const Texture* const*, const TextureView* const*, const Texture*,
                          const TextureView*);
    void DiscardColor(int);
    void DiscardDepthStencil();
    void Downsample(const Texture*, const Texture*);
    void TiledDownsample(const Texture*, const Texture*);
    void DownsampleTextureView(const Texture*, const TextureView*, const Texture*,
                               const TextureView*);
    void TiledDownsampleTextureView(const Texture*, const TextureView*, const Texture*,
                                    const TextureView*);
    void Barrier(BarrierBits);
    void WaitSync(const Sync*);
    void FenceSync(Sync*, SyncCondition, SyncFlagBits);
    void SetTexturePool(const TexturePool*);
    void SetSamplerPool(const SamplerPool*);
    void SetShaderScratchMemory(const MemoryPool*, ptrdiff_t, size_t);
    void SaveZCullData(BufferAddress, size_t);
    void RestoreZCullData(BufferAddress, size_t);
    void SetCopyRowStride(ptrdiff_t);
    void SetCopyImageStride(ptrdiff_t);
    ptrdiff_t GetCopyRowStride() const;
    ptrdiff_t GetCopyImageStride() const;
    void DrawTexture(TextureHandle, const DrawTextureRegion*, const DrawTextureRegion*);
    void SetProgramSubroutines(Program*, ShaderStage, const int, const int, const int*);
    void BindCoverageModulationTable(const float*);
    void ResolveDepthBuffer();
    void SetColorReductionEnable(NVNboolean);
    void SetColorReductionThresholds(FormatClass, int, int);
    void PushDebugGroupStatic(uint32_t, const char*);
    void PushDebugGroupDynamic(uint32_t, const char*);
    void PushDebugGroup(const char*);
    void PopDebugGroup();
    void PopDebugGroupId(uint32_t);
    void InsertDebugMarkerStatic(uint32_t, const char*);
    void InsertDebugMarkerDynamic(uint32_t, const char*);
    void InsertDebugMarker(const char*);
    CommandBufferMemoryCallbackFunc GetMemoryCallback() const;
    void* GetMemoryCallbackData() const;
    NVNboolean IsRecording() const;
    void WaitEvent(const Event*, EventWaitMode, uint32_t);
    void SignalEvent(const Event*, EventSignalMode, EventSignalLocation, EventSignalFlags,
                     uint32_t);
    void SetStencilCullCriteria(StencilFunc, int, int);
};

/*8170*/ class BlendState {
protected:
    char reserved[8];

public:
    BlendState& SetDefaults();
    BlendState& SetBlendTarget(int);
    BlendState& SetBlendFunc(BlendFunc, BlendFunc, BlendFunc, BlendFunc);
    BlendState& SetBlendEquation(BlendEquation, BlendEquation);
    BlendState& SetAdvancedMode(BlendAdvancedMode);
    BlendState& SetAdvancedOverlap(BlendAdvancedOverlap);
    BlendState& SetAdvancedPremultipliedSrc(NVNboolean);
    BlendState& SetAdvancedNormalizedDst(NVNboolean);
    int GetBlendTarget() const;
    void GetBlendFunc(BlendFunc*, BlendFunc*, BlendFunc*, BlendFunc*) const;
    void GetBlendEquation(BlendEquation*, BlendEquation*) const;
    BlendAdvancedMode GetAdvancedMode() const;
    BlendAdvancedOverlap GetAdvancedOverlap() const;
    NVNboolean GetAdvancedPremultipliedSrc() const;
    NVNboolean GetAdvancedNormalizedDst() const;
};

/*8350*/ class ChannelMaskState {
protected:
    char reserved[4];

public:
    ChannelMaskState& SetDefaults();
    ChannelMaskState& SetChannelMask(int, NVNboolean, NVNboolean, NVNboolean, NVNboolean);
    void GetChannelMask(int, NVNboolean*, NVNboolean*, NVNboolean*, NVNboolean*) const;
};

/*8417*/ class ColorState {
protected:
    char reserved[4];

public:
    ColorState& SetDefaults();
    ColorState& SetBlendEnable(int, NVNboolean);
    ColorState& SetLogicOp(LogicOp);
    ColorState& SetAlphaTest(AlphaFunc);
    NVNboolean GetBlendEnable(int) const;
    LogicOp GetLogicOp() const;
    AlphaFunc GetAlphaTest() const;
};

/*8487*/ class DepthStencilState {
protected:
    char reserved[8];

public:
    DepthStencilState& SetDefaults();
    DepthStencilState& SetDepthTestEnable(NVNboolean);
    DepthStencilState& SetDepthWriteEnable(NVNboolean);
    DepthStencilState& SetDepthFunc(DepthFunc);
    DepthStencilState& SetStencilTestEnable(NVNboolean);
    DepthStencilState& SetStencilFunc(Face, StencilFunc);
    DepthStencilState& SetStencilOp(Face, StencilOp, StencilOp, StencilOp);
    NVNboolean GetDepthTestEnable() const;
    NVNboolean GetDepthWriteEnable() const;
    DepthFunc GetDepthFunc() const;
    NVNboolean GetStencilTestEnable() const;
    StencilFunc GetStencilFunc(Face) const;
    void GetStencilOp(Face, StencilOp*, StencilOp*, StencilOp*) const;
};

/*8620*/ class MultisampleState {
protected:
    char reserved[24];

public:
    MultisampleState& SetDefaults();
    MultisampleState& SetMultisampleEnable(NVNboolean);
    MultisampleState& SetSamples(int);
    MultisampleState& SetAlphaToCoverageEnable(NVNboolean);
    MultisampleState& SetAlphaToCoverageDither(NVNboolean);
    NVNboolean GetMultisampleEnable() const;
    int GetSamples() const;
    NVNboolean GetAlphaToCoverageEnable() const;
    NVNboolean GetAlphaToCoverageDither() const;
    MultisampleState& SetRasterSamples(int);
    int GetRasterSamples();
    MultisampleState& SetCoverageModulationMode(CoverageModulationMode);
    CoverageModulationMode GetCoverageModulationMode() const;
    MultisampleState& SetCoverageToColorEnable(NVNboolean);
    NVNboolean GetCoverageToColorEnable() const;
    MultisampleState& SetCoverageToColorOutput(int);
    int GetCoverageToColorOutput() const;
    MultisampleState& SetSampleLocationsEnable(NVNboolean);
    NVNboolean GetSampleLocationsEnable() const;
    void GetSampleLocationsGrid(int*, int*);
    MultisampleState& SetSampleLocationsGridEnable(NVNboolean);
    NVNboolean GetSampleLocationsGridEnable() const;
    MultisampleState& SetSampleLocations(int, int, const float*);
};

/*8894*/ class PolygonState {
protected:
    char reserved[4];

public:
    PolygonState& SetDefaults();
    PolygonState& SetCullFace(Face);
    PolygonState& SetFrontFace(FrontFace);
    PolygonState& SetPolygonMode(PolygonMode);
    PolygonState& SetPolygonOffsetEnables(PolygonOffsetEnable);
    Face GetCullFace() const;
    FrontFace GetFrontFace() const;
    PolygonMode GetPolygonMode() const;
    PolygonOffsetEnable GetPolygonOffsetEnables() const;
};

/*8960*/ class VertexAttribState {
protected:
    char reserved[4];

public:
    VertexAttribState& SetDefaults();
    VertexAttribState& SetFormat(Format, ptrdiff_t);
    VertexAttribState& SetStreamIndex(int);
    void GetFormat(Format*, ptrdiff_t*) const;
    int GetStreamIndex() const;
};

/*9015*/ class VertexStreamState {
protected:
    char reserved[8];

public:
    VertexStreamState& SetDefaults();
    VertexStreamState& SetStride(ptrdiff_t);
    VertexStreamState& SetDivisor(int);
    ptrdiff_t GetStride() const;
    int GetDivisor() const;
};

/*9060*/ class Program {
protected:
    char reserved[192];

    Program(const Program&);

public:
    Program();

protected:
    const Program& operator=(const Program&);

public:
    NVNboolean Initialize(Device*);
    void Finalize();
    void SetDebugLabel(const char*);
    NVNboolean SetShaders(int, const ShaderData*);
    NVNboolean SetShadersExt(int, const ShaderDataExt*);
    void SetSampleShading(SampleShadingMode);
    NVNboolean SetSubroutineLinkage(int, const NVNsubroutineLinkageMapPtr*);
};

/*9132*/ class MemoryPoolBuilder {
protected:
    char reserved[64];

public:
    MemoryPoolBuilder& SetDevice(Device*);
    MemoryPoolBuilder& SetDefaults();
    MemoryPoolBuilder& SetStorage(void*, size_t);
    MemoryPoolBuilder& SetFlags(MemoryPoolFlags);
    const Device* GetDevice() const;
    void* GetMemory() const;
    size_t GetSize() const;
    MemoryPoolFlags GetFlags() const;
};

/*9222*/ class MemoryPool {
protected:
    char reserved[256];

    MemoryPool(const MemoryPool&);

public:
    MemoryPool();

protected:
    const MemoryPool& operator=(const MemoryPool&);

public:
    NVNboolean Initialize(const MemoryPoolBuilder*);
    void SetDebugLabel(const char*);
    void Finalize();
    void* Map() const;
    void FlushMappedRange(ptrdiff_t, size_t) const;
    void InvalidateMappedRange(ptrdiff_t, size_t) const;
    nvn::BufferAddress GetBufferAddress() const;
    NVNboolean MapVirtual(int, const MappingRequest*);
    size_t GetSize() const;
    MemoryPoolFlags GetFlags() const;
};

/*9338*/ class TexturePool {
protected:
    char reserved[32];

    TexturePool(const TexturePool&);

public:
    TexturePool();

protected:
    const TexturePool& operator=(const TexturePool&);

public:
    NVNboolean Initialize(const MemoryPool*, ptrdiff_t, int);
    void SetDebugLabel(const char*);
    void Finalize();
    void RegisterTexture(int, const Texture*, const TextureView*) const;
    void RegisterImage(int, const Texture*, const TextureView*) const;
    const MemoryPool* GetMemoryPool() const;
    ptrdiff_t GetMemoryOffset() const;
    int GetSize() const;
};

/*9459*/ class SamplerPool {
protected:
    char reserved[32];

    SamplerPool(const SamplerPool&);

public:
    SamplerPool();

protected:
    const SamplerPool& operator=(const SamplerPool&);

public:
    NVNboolean Initialize(const MemoryPool*, ptrdiff_t, int);
    void SetDebugLabel(const char*);
    void Finalize();
    void RegisterSampler(int, const Sampler*) const;
    void RegisterSamplerBuilder(int, const SamplerBuilder*) const;
    const MemoryPool* GetMemoryPool() const;
    ptrdiff_t GetMemoryOffset() const;
    int GetSize() const;
};

/*9572*/ class BufferBuilder {
protected:
    char reserved[64];

public:
    BufferBuilder& SetDevice(Device*);
    BufferBuilder& SetDefaults();
    BufferBuilder& SetStorage(MemoryPool*, ptrdiff_t, size_t);
    const Device* GetDevice() const;
    MemoryPool* GetMemoryPool() const;
    ptrdiff_t GetMemoryOffset() const;
    size_t GetSize() const;
};

/*9627*/ class Buffer {
protected:
    char reserved[48];

    Buffer(const Buffer&);

public:
    Buffer();

protected:
    const Buffer& operator=(const Buffer&);

public:
    NVNboolean Initialize(const BufferBuilder*);
    void SetDebugLabel(const char*);
    void Finalize();
    void* Map() const;
    nvn::BufferAddress GetAddress() const;
    void FlushMappedRange(ptrdiff_t, size_t) const;
    void InvalidateMappedRange(ptrdiff_t, size_t) const;
    MemoryPool* GetMemoryPool() const;
    ptrdiff_t GetMemoryOffset() const;
    size_t GetSize() const;
    uint64_t GetDebugID() const;
};

/*9715*/ class Texture {
protected:
    char reserved[192];

    Texture(const Texture&);

public:
    Texture();

protected:
    const Texture& operator=(const Texture&);

public:
    NVNboolean Initialize(const TextureBuilder*);
    size_t GetZCullStorageSize() const;
    void Finalize();
    void SetDebugLabel(const char*);
    NVNstorageClass GetStorageClass() const;
    ptrdiff_t GetViewOffset(const TextureView*) const;
    TextureFlags GetFlags() const;
    TextureTarget GetTarget() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    int GetLevels() const;
    Format GetFormat() const;
    int GetSamples() const;
    void GetSwizzle(TextureSwizzle*, TextureSwizzle*, TextureSwizzle*, TextureSwizzle*) const;
    TextureDepthStencilMode GetDepthStencilMode() const;
    ptrdiff_t GetStride() const;
    nvn::TextureAddress GetTextureAddress() const;
    void GetSparseTileLayout(TextureSparseTileLayout*) const;
    void WriteTexels(const TextureView*, const CopyRegion*, const void*) const;
    void WriteTexelsStrided(const TextureView*, const CopyRegion*, const void*, ptrdiff_t,
                            ptrdiff_t) const;
    void ReadTexels(const TextureView*, const CopyRegion*, void*) const;
    void ReadTexelsStrided(const TextureView*, const CopyRegion*, void*, ptrdiff_t,
                           ptrdiff_t) const;
    void FlushTexels(const TextureView*, const CopyRegion*) const;
    void InvalidateTexels(const TextureView*, const CopyRegion*) const;
    MemoryPool* GetMemoryPool() const;
    ptrdiff_t GetMemoryOffset() const;
    int GetStorageSize() const;
    NVNboolean Compare(const Texture*) const;
    uint64_t GetDebugID() const;
    RawStorageClass GetRawStorageClass() const;
};

/*10081*/ class TextureBuilder {
protected:
    char reserved[128];

public:
    TextureBuilder& SetDevice(Device*);
    TextureBuilder& SetDefaults();
    TextureBuilder& SetFlags(TextureFlags);
    TextureBuilder& SetTarget(TextureTarget);
    TextureBuilder& SetWidth(int);
    TextureBuilder& SetHeight(int);
    TextureBuilder& SetDepth(int);
    TextureBuilder& SetSize1D(int);
    TextureBuilder& SetSize2D(int, int);
    TextureBuilder& SetSize3D(int, int, int);
    TextureBuilder& SetLevels(int);
    TextureBuilder& SetFormat(Format);
    TextureBuilder& SetSamples(int);
    TextureBuilder& SetSwizzle(TextureSwizzle, TextureSwizzle, TextureSwizzle, TextureSwizzle);
    TextureBuilder& SetDepthStencilMode(TextureDepthStencilMode);
    size_t GetStorageSize() const;
    size_t GetStorageAlignment() const;
    TextureBuilder& SetStorage(MemoryPool*, ptrdiff_t);
    TextureBuilder& SetPackagedTextureData(const void*);
    TextureBuilder& SetPackagedTextureLayout(const PackagedTextureLayout*);
    TextureBuilder& SetStride(ptrdiff_t);
    TextureBuilder& SetGLTextureName(uint32_t);
    NVNstorageClass GetStorageClass() const;
    const Device* GetDevice() const;
    TextureFlags GetFlags() const;
    TextureTarget GetTarget() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    int GetLevels() const;
    Format GetFormat() const;
    int GetSamples() const;
    void GetSwizzle(TextureSwizzle*, TextureSwizzle*, TextureSwizzle*, TextureSwizzle*) const;
    TextureDepthStencilMode GetDepthStencilMode() const;
    const void* GetPackagedTextureData() const;
    NVNboolean GetPackagedTextureLayout(PackagedTextureLayout*) const;
    ptrdiff_t GetStride() const;
    void GetSparseTileLayout(TextureSparseTileLayout*) const;
    uint32_t GetGLTextureName() const;
    size_t GetZCullStorageSize() const;
    MemoryPool* GetMemoryPool() const;
    ptrdiff_t GetMemoryOffset() const;
    RawStorageClass GetRawStorageClass() const;
};

/*10500*/ class TextureView {
protected:
    char reserved[40];

public:
    TextureView& SetDefaults();
    TextureView& SetLevels(int, int);
    TextureView& SetLayers(int, int);
    TextureView& SetFormat(Format);
    TextureView& SetSwizzle(TextureSwizzle, TextureSwizzle, TextureSwizzle, TextureSwizzle);
    TextureView& SetDepthStencilMode(TextureDepthStencilMode);
    TextureView& SetTarget(TextureTarget);
    NVNboolean GetLevels(int*, int*) const;
    NVNboolean GetLayers(int*, int*) const;
    NVNboolean GetFormat(Format*) const;
    NVNboolean GetSwizzle(TextureSwizzle*, TextureSwizzle*, TextureSwizzle*, TextureSwizzle*) const;
    NVNboolean GetDepthStencilMode(TextureDepthStencilMode*) const;
    NVNboolean GetTarget(TextureTarget*) const;
    NVNboolean Compare(const TextureView*) const;
};

/*10717*/ class SamplerBuilder {
protected:
    char reserved[96];

public:
    SamplerBuilder& SetDevice(Device*);
    SamplerBuilder& SetDefaults();
    SamplerBuilder& SetMinMagFilter(MinFilter, MagFilter);
    SamplerBuilder& SetWrapMode(WrapMode, WrapMode, WrapMode);
    SamplerBuilder& SetLodClamp(float, float);
    SamplerBuilder& SetLodBias(float);
    SamplerBuilder& SetCompare(CompareMode, CompareFunc);
    SamplerBuilder& SetBorderColor(const float*);
    SamplerBuilder& SetBorderColori(const int*);
    SamplerBuilder& SetBorderColorui(const uint32_t*);
    SamplerBuilder& SetMaxAnisotropy(float);
    SamplerBuilder& SetReductionFilter(SamplerReduction);
    SamplerBuilder& SetLodSnap(float);
    const Device* GetDevice() const;
    void GetMinMagFilter(MinFilter*, MagFilter*) const;
    void GetWrapMode(WrapMode*, WrapMode*, WrapMode*) const;
    void GetLodClamp(float*, float*) const;
    float GetLodBias() const;
    void GetCompare(CompareMode*, CompareFunc*) const;
    void GetBorderColor(float*) const;
    void GetBorderColori(int*) const;
    void GetBorderColorui(uint32_t*) const;
    float GetMaxAnisotropy() const;
    SamplerReduction GetReductionFilter() const;
    float GetLodSnap() const;
};

/*10954*/ class Sampler {
protected:
    char reserved[96];

    Sampler(const Sampler&);

public:
    Sampler();

protected:
    const Sampler& operator=(const Sampler&);

public:
    NVNboolean Initialize(const SamplerBuilder*);
    void Finalize();
    void SetDebugLabel(const char*);
    void GetMinMagFilter(MinFilter*, MagFilter*) const;
    void GetWrapMode(WrapMode*, WrapMode*, WrapMode*) const;
    void GetLodClamp(float*, float*) const;
    float GetLodBias() const;
    void GetCompare(CompareMode*, CompareFunc*) const;
    void GetBorderColor(float*) const;
    void GetBorderColori(int*) const;
    void GetBorderColorui(uint32_t*) const;
    float GetMaxAnisotropy() const;
    SamplerReduction GetReductionFilter() const;
    NVNboolean Compare(const Sampler*) const;
    uint64_t GetDebugID() const;
};

/*11062*/ class Sync {
protected:
    char reserved[64];

    Sync(const Sync&);

public:
    Sync();

protected:
    const Sync& operator=(const Sync&);

public:
    NVNboolean Initialize(Device*);
    void Finalize();
    void SetDebugLabel(const char*);
    SyncWaitResult Wait(uint64_t) const;
};

/*11102*/ class Window {
protected:
    char reserved[384];

    Window(const Window&);

public:
    Window();

protected:
    const Window& operator=(const Window&);

public:
    NVNboolean Initialize(const WindowBuilder*);
    void Finalize();
    void SetDebugLabel(const char*);
    WindowAcquireTextureResult AcquireTexture(Sync*, int*);
    NVNnativeWindow GetNativeWindow() const;
    int GetPresentInterval() const;
    void SetPresentInterval(int);
    void SetCrop(int, int, int, int);
    void GetCrop(Rectangle*) const;
};

/*11243*/ class WindowBuilder {
protected:
    char reserved[64];

public:
    WindowBuilder& SetDevice(Device*);
    WindowBuilder& SetDefaults();
    WindowBuilder& SetNativeWindow(NVNnativeWindow);
    WindowBuilder& SetTextures(int, Texture* const*);
    WindowBuilder& SetPresentInterval(int);
    const Device* GetDevice() const;
    int GetNumTextures() const;
    const Texture* GetTexture(int) const;
    NVNnativeWindow GetNativeWindow() const;
    int GetPresentInterval() const;
};

/*11374*/ class EventBuilder {
protected:
    char reserved[32];

public:
    EventBuilder& SetDefaults();
    EventBuilder& SetStorage(const MemoryPool*, int64_t);
    const MemoryPool* GetStorage(int64_t*) const;
};

/*11421*/ class Event {
protected:
    char reserved[64];

    Event(const Event&);

public:
    Event();

protected:
    const Event& operator=(const Event&);

public:
    NVNboolean Initialize(const EventBuilder*);
    void Finalize();
    uint32_t GetValue() const;
    void Signal(EventSignalMode, uint32_t);
};

/*11454*/ typedef void (*DeviceBuilderSetDefaultsFunc)(DeviceBuilder*);
/*11455*/ typedef void (*DeviceBuilderSetFlagsFunc)(DeviceBuilder*, int);
/*11456*/ typedef int (*DeviceBuilderGetFlagsFunc)(const DeviceBuilder*);
/*11457*/ typedef NVNboolean (*DeviceInitializeFunc)(Device*, const DeviceBuilder*);
/*11458*/ typedef void (*DeviceFinalizeFunc)(Device*);
/*11459*/ typedef void (*DeviceSetDebugLabelFunc)(Device*, const char*);
/*11460*/ typedef GenericFuncPtrFunc (*DeviceGetProcAddressFunc)(const Device*, const char*);
/*11461*/ typedef void (*DeviceGetIntegerFunc)(const Device*, DeviceInfo::Enum, int*);
/*11462*/ typedef uint64_t (*DeviceGetCurrentTimestampInNanosecondsFunc)(const Device*);
/*11463*/ typedef void (*DeviceSetIntermediateShaderCacheFunc)(Device*, int);
/*11464*/ typedef TextureHandle (*DeviceGetTextureHandleFunc)(const Device*, int, int);
/*11465*/ typedef TextureHandle (*DeviceGetTexelFetchHandleFunc)(const Device*, int);
/*11466*/ typedef ImageHandle (*DeviceGetImageHandleFunc)(const Device*, int);
/*11467*/ typedef void (*DeviceInstallDebugCallbackFunc)(Device*, const DebugCallbackFunc, void*,
                                                         NVNboolean);
/*11468*/ typedef NVNdebugDomainId (*DeviceGenerateDebugDomainIdFunc)(const Device*, const char*);
/*11469*/ typedef void (*DeviceSetWindowOriginModeFunc)(Device*, WindowOriginMode::Enum);
/*11470*/ typedef void (*DeviceSetDepthModeFunc)(Device*, DepthMode::Enum);
/*11471*/ typedef NVNboolean (*DeviceRegisterFastClearColorFunc)(Device*, const float*,
                                                                 Format::Enum);
/*11472*/ typedef NVNboolean (*DeviceRegisterFastClearColoriFunc)(Device*, const int*,
                                                                  Format::Enum);
/*11473*/ typedef NVNboolean (*DeviceRegisterFastClearColoruiFunc)(Device*, const uint32_t*,
                                                                   Format::Enum);
/*11474*/ typedef NVNboolean (*DeviceRegisterFastClearDepthFunc)(Device*, float);
/*11475*/ typedef WindowOriginMode::Enum (*DeviceGetWindowOriginModeFunc)(const Device*);
/*11476*/ typedef DepthMode::Enum (*DeviceGetDepthModeFunc)(const Device*);
/*11477*/ typedef uint64_t (*DeviceGetTimestampInNanosecondsFunc)(const Device*,
                                                                  const CounterData*);
/*11478*/ typedef void (*DeviceApplyDeferredFinalizesFunc)(Device*, int);
/*11479*/ typedef void (*DeviceFinalizeCommandHandleFunc)(Device*, CommandHandle);
/*11480*/ typedef void (*DeviceWalkDebugDatabaseFunc)(const Device*, DebugObjectType::Enum,
                                                      WalkDebugDatabaseCallbackFunc, void*);
/*11481*/ typedef SeparateTextureHandle (*DeviceGetSeparateTextureHandleFunc)(const Device*, int);
/*11482*/ typedef SeparateSamplerHandle (*DeviceGetSeparateSamplerHandleFunc)(const Device*, int);
/*11483*/ typedef NVNboolean (*DeviceIsExternalDebuggerAttachedFunc)(const Device*);
/*11484*/ typedef QueueGetErrorResult::Enum (*QueueGetErrorFunc)(Queue*, QueueErrorInfo*);
/*11485*/ typedef size_t (*QueueGetTotalCommandMemoryUsedFunc)(Queue*);
/*11486*/ typedef size_t (*QueueGetTotalControlMemoryUsedFunc)(Queue*);
/*11487*/ typedef size_t (*QueueGetTotalComputeMemoryUsedFunc)(Queue*);
/*11488*/ typedef void (*QueueResetMemoryUsageCountsFunc)(Queue*);
/*11489*/ typedef void (*QueueBuilderSetDeviceFunc)(QueueBuilder*, Device*);
/*11490*/ typedef void (*QueueBuilderSetDefaultsFunc)(QueueBuilder*);
/*11491*/ typedef void (*QueueBuilderSetFlagsFunc)(QueueBuilder*, int);
/*11492*/ typedef void (*QueueBuilderSetCommandMemorySizeFunc)(QueueBuilder*, size_t);
/*11493*/ typedef void (*QueueBuilderSetComputeMemorySizeFunc)(QueueBuilder*, size_t);
/*11494*/ typedef void (*QueueBuilderSetControlMemorySizeFunc)(QueueBuilder*, size_t);
/*11495*/ typedef size_t (*QueueBuilderGetQueueMemorySizeFunc)(const QueueBuilder*);
/*11496*/ typedef void (*QueueBuilderSetQueueMemoryFunc)(QueueBuilder*, void*, size_t);
/*11497*/ typedef void (*QueueBuilderSetCommandFlushThresholdFunc)(QueueBuilder*, size_t);
/*11498*/ typedef const Device* (*QueueBuilderGetDeviceFunc)(const QueueBuilder*);
/*11499*/ typedef int (*QueueBuilderGetFlagsFunc)(const QueueBuilder*);
/*11500*/ typedef NVNboolean (*QueueBuilderGetCommandMemorySizeFunc)(const QueueBuilder*, int*);
/*11501*/ typedef NVNboolean (*QueueBuilderGetComputeMemorySizeFunc)(const QueueBuilder*, int*);
/*11502*/ typedef NVNboolean (*QueueBuilderGetControlMemorySizeFunc)(const QueueBuilder*, int*);
/*11503*/ typedef NVNboolean (*QueueBuilderGetCommandFlushThresholdFunc)(const QueueBuilder*, int*);
/*11504*/ typedef int (*QueueBuilderGetMemorySizeFunc)(const QueueBuilder*);
/*11505*/ typedef void* (*QueueBuilderGetMemoryFunc)(const QueueBuilder*);
/*11506*/ typedef NVNboolean (*QueueInitializeFunc)(Queue*, const QueueBuilder*);
/*11507*/ typedef void (*QueueFinalizeFunc)(Queue*);
/*11508*/ typedef void (*QueueSetDebugLabelFunc)(Queue*, const char*);
/*11509*/ typedef void (*QueueSubmitCommandsFunc)(Queue*, int, const CommandHandle*);
/*11510*/ typedef void (*QueueFlushFunc)(Queue*);
/*11511*/ typedef void (*QueueFinishFunc)(Queue*);
/*11512*/ typedef void (*QueuePresentTextureFunc)(Queue*, Window*, int);
/*11513*/ typedef QueueAcquireTextureResult::Enum (*QueueAcquireTextureFunc)(Queue*, Window*, int*);
/*11514*/ typedef void (*WindowBuilderSetDeviceFunc)(WindowBuilder*, Device*);
/*11515*/ typedef void (*WindowBuilderSetDefaultsFunc)(WindowBuilder*);
/*11516*/ typedef void (*WindowBuilderSetNativeWindowFunc)(WindowBuilder*, NVNnativeWindow);
/*11517*/ typedef void (*WindowBuilderSetTexturesFunc)(WindowBuilder*, int, Texture* const*);
/*11518*/ typedef void (*WindowBuilderSetPresentIntervalFunc)(WindowBuilder*, int);
/*11519*/ typedef const Device* (*WindowBuilderGetDeviceFunc)(const WindowBuilder*);
/*11520*/ typedef int (*WindowBuilderGetNumTexturesFunc)(const WindowBuilder*);
/*11521*/ typedef const Texture* (*WindowBuilderGetTextureFunc)(const WindowBuilder*, int);
/*11522*/ typedef NVNnativeWindow (*WindowBuilderGetNativeWindowFunc)(const WindowBuilder*);
/*11523*/ typedef int (*WindowBuilderGetPresentIntervalFunc)(const WindowBuilder*);
/*11524*/ typedef NVNboolean (*WindowInitializeFunc)(Window*, const WindowBuilder*);
/*11525*/ typedef void (*WindowFinalizeFunc)(Window*);
/*11526*/ typedef void (*WindowSetDebugLabelFunc)(Window*, const char*);
/*11527*/ typedef WindowAcquireTextureResult::Enum (*WindowAcquireTextureFunc)(Window*, Sync*,
                                                                               int*);
/*11528*/ typedef NVNnativeWindow (*WindowGetNativeWindowFunc)(const Window*);
/*11529*/ typedef int (*WindowGetPresentIntervalFunc)(const Window*);
/*11530*/ typedef void (*WindowSetPresentIntervalFunc)(Window*, int);
/*11531*/ typedef void (*WindowSetCropFunc)(Window*, int, int, int, int);
/*11532*/ typedef void (*WindowGetCropFunc)(const Window*, Rectangle*);
/*11533*/ typedef NVNboolean (*ProgramInitializeFunc)(Program*, Device*);
/*11534*/ typedef void (*ProgramFinalizeFunc)(Program*);
/*11535*/ typedef void (*ProgramSetDebugLabelFunc)(Program*, const char*);
/*11536*/ typedef NVNboolean (*ProgramSetShadersFunc)(Program*, int, const ShaderData*);
/*11537*/ typedef void (*MemoryPoolBuilderSetDeviceFunc)(MemoryPoolBuilder*, Device*);
/*11538*/ typedef void (*MemoryPoolBuilderSetDefaultsFunc)(MemoryPoolBuilder*);
/*11539*/ typedef void (*MemoryPoolBuilderSetStorageFunc)(MemoryPoolBuilder*, void*, size_t);
/*11540*/ typedef void (*MemoryPoolBuilderSetFlagsFunc)(MemoryPoolBuilder*, int);
/*11541*/ typedef const Device* (*MemoryPoolBuilderGetDeviceFunc)(const MemoryPoolBuilder*);
/*11542*/ typedef void* (*MemoryPoolBuilderGetMemoryFunc)(const MemoryPoolBuilder*);
/*11543*/ typedef size_t (*MemoryPoolBuilderGetSizeFunc)(const MemoryPoolBuilder*);
/*11544*/ typedef int (*MemoryPoolBuilderGetFlagsFunc)(const MemoryPoolBuilder*);
/*11545*/ typedef NVNboolean (*MemoryPoolInitializeFunc)(MemoryPool*, const MemoryPoolBuilder*);
/*11546*/ typedef void (*MemoryPoolSetDebugLabelFunc)(MemoryPool*, const char*);
/*11547*/ typedef void (*MemoryPoolFinalizeFunc)(MemoryPool*);
/*11548*/ typedef void* (*MemoryPoolMapFunc)(const MemoryPool*);
/*11549*/ typedef void (*MemoryPoolFlushMappedRangeFunc)(const MemoryPool*, ptrdiff_t, size_t);
/*11550*/ typedef void (*MemoryPoolInvalidateMappedRangeFunc)(const MemoryPool*, ptrdiff_t, size_t);
/*11551*/ typedef BufferAddress (*MemoryPoolGetBufferAddressFunc)(const MemoryPool*);
/*11552*/ typedef NVNboolean (*MemoryPoolMapVirtualFunc)(MemoryPool*, int, const MappingRequest*);
/*11553*/ typedef size_t (*MemoryPoolGetSizeFunc)(const MemoryPool*);
/*11554*/ typedef int (*MemoryPoolGetFlagsFunc)(const MemoryPool*);
/*11555*/ typedef NVNboolean (*TexturePoolInitializeFunc)(TexturePool*, const MemoryPool*,
                                                          ptrdiff_t, int);
/*11556*/ typedef void (*TexturePoolSetDebugLabelFunc)(TexturePool*, const char*);
/*11557*/ typedef void (*TexturePoolFinalizeFunc)(TexturePool*);
/*11558*/ typedef void (*TexturePoolRegisterTextureFunc)(const TexturePool*, int, const Texture*,
                                                         const TextureView*);
/*11559*/ typedef void (*TexturePoolRegisterImageFunc)(const TexturePool*, int, const Texture*,
                                                       const TextureView*);
/*11560*/ typedef const MemoryPool* (*TexturePoolGetMemoryPoolFunc)(const TexturePool*);
/*11561*/ typedef ptrdiff_t (*TexturePoolGetMemoryOffsetFunc)(const TexturePool*);
/*11562*/ typedef int (*TexturePoolGetSizeFunc)(const TexturePool*);
/*11563*/ typedef NVNboolean (*SamplerPoolInitializeFunc)(SamplerPool*, const MemoryPool*,
                                                          ptrdiff_t, int);
/*11564*/ typedef void (*SamplerPoolSetDebugLabelFunc)(SamplerPool*, const char*);
/*11565*/ typedef void (*SamplerPoolFinalizeFunc)(SamplerPool*);
/*11566*/ typedef void (*SamplerPoolRegisterSamplerFunc)(const SamplerPool*, int, const Sampler*);
/*11567*/ typedef void (*SamplerPoolRegisterSamplerBuilderFunc)(const SamplerPool*, int,
                                                                const SamplerBuilder*);
/*11568*/ typedef const MemoryPool* (*SamplerPoolGetMemoryPoolFunc)(const SamplerPool*);
/*11569*/ typedef ptrdiff_t (*SamplerPoolGetMemoryOffsetFunc)(const SamplerPool*);
/*11570*/ typedef int (*SamplerPoolGetSizeFunc)(const SamplerPool*);
/*11571*/ typedef void (*BufferBuilderSetDeviceFunc)(BufferBuilder*, Device*);
/*11572*/ typedef void (*BufferBuilderSetDefaultsFunc)(BufferBuilder*);
/*11573*/ typedef void (*BufferBuilderSetStorageFunc)(BufferBuilder*, MemoryPool*, ptrdiff_t,
                                                      size_t);
/*11574*/ typedef const Device* (*BufferBuilderGetDeviceFunc)(const BufferBuilder*);
/*11575*/ typedef MemoryPool* (*BufferBuilderGetMemoryPoolFunc)(const BufferBuilder*);
/*11576*/ typedef ptrdiff_t (*BufferBuilderGetMemoryOffsetFunc)(const BufferBuilder*);
/*11577*/ typedef size_t (*BufferBuilderGetSizeFunc)(const BufferBuilder*);
/*11578*/ typedef NVNboolean (*BufferInitializeFunc)(Buffer*, const BufferBuilder*);
/*11579*/ typedef void (*BufferSetDebugLabelFunc)(Buffer*, const char*);
/*11580*/ typedef void (*BufferFinalizeFunc)(Buffer*);
/*11581*/ typedef void* (*BufferMapFunc)(const Buffer*);
/*11582*/ typedef BufferAddress (*BufferGetAddressFunc)(const Buffer*);
/*11583*/ typedef void (*BufferFlushMappedRangeFunc)(const Buffer*, ptrdiff_t, size_t);
/*11584*/ typedef void (*BufferInvalidateMappedRangeFunc)(const Buffer*, ptrdiff_t, size_t);
/*11585*/ typedef MemoryPool* (*BufferGetMemoryPoolFunc)(const Buffer*);
/*11586*/ typedef ptrdiff_t (*BufferGetMemoryOffsetFunc)(const Buffer*);
/*11587*/ typedef size_t (*BufferGetSizeFunc)(const Buffer*);
/*11588*/ typedef uint64_t (*BufferGetDebugIDFunc)(const Buffer*);
/*11589*/ typedef void (*TextureBuilderSetDeviceFunc)(TextureBuilder*, Device*);
/*11590*/ typedef void (*TextureBuilderSetDefaultsFunc)(TextureBuilder*);
/*11591*/ typedef void (*TextureBuilderSetFlagsFunc)(TextureBuilder*, int);
/*11592*/ typedef void (*TextureBuilderSetTargetFunc)(TextureBuilder*, TextureTarget::Enum);
/*11593*/ typedef void (*TextureBuilderSetWidthFunc)(TextureBuilder*, int);
/*11594*/ typedef void (*TextureBuilderSetHeightFunc)(TextureBuilder*, int);
/*11595*/ typedef void (*TextureBuilderSetDepthFunc)(TextureBuilder*, int);
/*11596*/ typedef void (*TextureBuilderSetSize1DFunc)(TextureBuilder*, int);
/*11597*/ typedef void (*TextureBuilderSetSize2DFunc)(TextureBuilder*, int, int);
/*11598*/ typedef void (*TextureBuilderSetSize3DFunc)(TextureBuilder*, int, int, int);
/*11599*/ typedef void (*TextureBuilderSetLevelsFunc)(TextureBuilder*, int);
/*11600*/ typedef void (*TextureBuilderSetFormatFunc)(TextureBuilder*, Format::Enum);
/*11601*/ typedef void (*TextureBuilderSetSamplesFunc)(TextureBuilder*, int);
/*11602*/ typedef void (*TextureBuilderSetSwizzleFunc)(TextureBuilder*, TextureSwizzle::Enum,
                                                       TextureSwizzle::Enum, TextureSwizzle::Enum,
                                                       TextureSwizzle::Enum);
/*11603*/ typedef void (*TextureBuilderSetDepthStencilModeFunc)(TextureBuilder*,
                                                                TextureDepthStencilMode::Enum);
/*11604*/ typedef size_t (*TextureBuilderGetStorageSizeFunc)(const TextureBuilder*);
/*11605*/ typedef size_t (*TextureBuilderGetStorageAlignmentFunc)(const TextureBuilder*);
/*11606*/ typedef void (*TextureBuilderSetStorageFunc)(TextureBuilder*, MemoryPool*, ptrdiff_t);
/*11607*/ typedef void (*TextureBuilderSetPackagedTextureDataFunc)(TextureBuilder*, const void*);
/*11608*/ typedef void (*TextureBuilderSetPackagedTextureLayoutFunc)(TextureBuilder*,
                                                                     const PackagedTextureLayout*);
/*11609*/ typedef void (*TextureBuilderSetStrideFunc)(TextureBuilder*, ptrdiff_t);
/*11610*/ typedef void (*TextureBuilderSetGLTextureNameFunc)(TextureBuilder*, uint32_t);
/*11611*/ typedef NVNstorageClass (*TextureBuilderGetStorageClassFunc)(const TextureBuilder*);
/*11612*/ typedef const Device* (*TextureBuilderGetDeviceFunc)(const TextureBuilder*);
/*11613*/ typedef int (*TextureBuilderGetFlagsFunc)(const TextureBuilder*);
/*11614*/ typedef TextureTarget::Enum (*TextureBuilderGetTargetFunc)(const TextureBuilder*);
/*11615*/ typedef int (*TextureBuilderGetWidthFunc)(const TextureBuilder*);
/*11616*/ typedef int (*TextureBuilderGetHeightFunc)(const TextureBuilder*);
/*11617*/ typedef int (*TextureBuilderGetDepthFunc)(const TextureBuilder*);
/*11618*/ typedef int (*TextureBuilderGetLevelsFunc)(const TextureBuilder*);
/*11619*/ typedef Format::Enum (*TextureBuilderGetFormatFunc)(const TextureBuilder*);
/*11620*/ typedef int (*TextureBuilderGetSamplesFunc)(const TextureBuilder*);
/*11621*/ typedef void (*TextureBuilderGetSwizzleFunc)(const TextureBuilder*, TextureSwizzle*,
                                                       TextureSwizzle*, TextureSwizzle*,
                                                       TextureSwizzle*);
/*11622*/ typedef TextureDepthStencilMode::Enum (*TextureBuilderGetDepthStencilModeFunc)(
    const TextureBuilder*);
/*11623*/ typedef const void* (*TextureBuilderGetPackagedTextureDataFunc)(const TextureBuilder*);
/*11624*/ typedef NVNboolean (*TextureBuilderGetPackagedTextureLayoutFunc)(const TextureBuilder*,
                                                                           PackagedTextureLayout*);
/*11625*/ typedef ptrdiff_t (*TextureBuilderGetStrideFunc)(const TextureBuilder*);
/*11626*/ typedef void (*TextureBuilderGetSparseTileLayoutFunc)(const TextureBuilder*,
                                                                TextureSparseTileLayout*);
/*11627*/ typedef uint32_t (*TextureBuilderGetGLTextureNameFunc)(const TextureBuilder*);
/*11628*/ typedef size_t (*TextureBuilderGetZCullStorageSizeFunc)(const TextureBuilder*);
/*11629*/ typedef MemoryPool* (*TextureBuilderGetMemoryPoolFunc)(const TextureBuilder*);
/*11630*/ typedef ptrdiff_t (*TextureBuilderGetMemoryOffsetFunc)(const TextureBuilder*);
/*11631*/ typedef void (*TextureViewSetDefaultsFunc)(TextureView*);
/*11632*/ typedef void (*TextureViewSetLevelsFunc)(TextureView*, int, int);
/*11633*/ typedef void (*TextureViewSetLayersFunc)(TextureView*, int, int);
/*11634*/ typedef void (*TextureViewSetFormatFunc)(TextureView*, Format::Enum);
/*11635*/ typedef void (*TextureViewSetSwizzleFunc)(TextureView*, TextureSwizzle::Enum,
                                                    TextureSwizzle::Enum, TextureSwizzle::Enum,
                                                    TextureSwizzle::Enum);
/*11636*/ typedef void (*TextureViewSetDepthStencilModeFunc)(TextureView*,
                                                             TextureDepthStencilMode::Enum);
/*11637*/ typedef void (*TextureViewSetTargetFunc)(TextureView*, TextureTarget::Enum);
/*11638*/ typedef NVNboolean (*TextureViewGetLevelsFunc)(const TextureView*, int*, int*);
/*11639*/ typedef NVNboolean (*TextureViewGetLayersFunc)(const TextureView*, int*, int*);
/*11640*/ typedef NVNboolean (*TextureViewGetFormatFunc)(const TextureView*, Format*);
/*11641*/ typedef NVNboolean (*TextureViewGetSwizzleFunc)(const TextureView*, TextureSwizzle*,
                                                          TextureSwizzle*, TextureSwizzle*,
                                                          TextureSwizzle*);
/*11642*/ typedef NVNboolean (*TextureViewGetDepthStencilModeFunc)(const TextureView*,
                                                                   TextureDepthStencilMode*);
/*11643*/ typedef NVNboolean (*TextureViewGetTargetFunc)(const TextureView*, TextureTarget*);
/*11644*/ typedef NVNboolean (*TextureViewCompareFunc)(const TextureView*, const TextureView*);
/*11645*/ typedef NVNboolean (*TextureInitializeFunc)(Texture*, const TextureBuilder*);
/*11646*/ typedef size_t (*TextureGetZCullStorageSizeFunc)(const Texture*);
/*11647*/ typedef void (*TextureFinalizeFunc)(Texture*);
/*11648*/ typedef void (*TextureSetDebugLabelFunc)(Texture*, const char*);
/*11649*/ typedef NVNstorageClass (*TextureGetStorageClassFunc)(const Texture*);
/*11650*/ typedef ptrdiff_t (*TextureGetViewOffsetFunc)(const Texture*, const TextureView*);
/*11651*/ typedef int (*TextureGetFlagsFunc)(const Texture*);
/*11652*/ typedef TextureTarget::Enum (*TextureGetTargetFunc)(const Texture*);
/*11653*/ typedef int (*TextureGetWidthFunc)(const Texture*);
/*11654*/ typedef int (*TextureGetHeightFunc)(const Texture*);
/*11655*/ typedef int (*TextureGetDepthFunc)(const Texture*);
/*11656*/ typedef int (*TextureGetLevelsFunc)(const Texture*);
/*11657*/ typedef Format::Enum (*TextureGetFormatFunc)(const Texture*);
/*11658*/ typedef int (*TextureGetSamplesFunc)(const Texture*);
/*11659*/ typedef void (*TextureGetSwizzleFunc)(const Texture*, TextureSwizzle*, TextureSwizzle*,
                                                TextureSwizzle*, TextureSwizzle*);
/*11660*/ typedef TextureDepthStencilMode::Enum (*TextureGetDepthStencilModeFunc)(const Texture*);
/*11661*/ typedef ptrdiff_t (*TextureGetStrideFunc)(const Texture*);
/*11662*/ typedef TextureAddress (*TextureGetTextureAddressFunc)(const Texture*);
/*11663*/ typedef void (*TextureGetSparseTileLayoutFunc)(const Texture*, TextureSparseTileLayout*);
/*11664*/ typedef void (*TextureWriteTexelsFunc)(const Texture*, const TextureView*,
                                                 const CopyRegion*, const void*);
/*11665*/ typedef void (*TextureWriteTexelsStridedFunc)(const Texture*, const TextureView*,
                                                        const CopyRegion*, const void*, ptrdiff_t,
                                                        ptrdiff_t);
/*11666*/ typedef void (*TextureReadTexelsFunc)(const Texture*, const TextureView*,
                                                const CopyRegion*, void*);
/*11667*/ typedef void (*TextureReadTexelsStridedFunc)(const Texture*, const TextureView*,
                                                       const CopyRegion*, void*, ptrdiff_t,
                                                       ptrdiff_t);
/*11668*/ typedef void (*TextureFlushTexelsFunc)(const Texture*, const TextureView*,
                                                 const CopyRegion*);
/*11669*/ typedef void (*TextureInvalidateTexelsFunc)(const Texture*, const TextureView*,
                                                      const CopyRegion*);
/*11670*/ typedef MemoryPool* (*TextureGetMemoryPoolFunc)(const Texture*);
/*11671*/ typedef ptrdiff_t (*TextureGetMemoryOffsetFunc)(const Texture*);
/*11672*/ typedef int (*TextureGetStorageSizeFunc)(const Texture*);
/*11673*/ typedef NVNboolean (*TextureCompareFunc)(const Texture*, const Texture*);
/*11674*/ typedef uint64_t (*TextureGetDebugIDFunc)(const Texture*);
/*11675*/ typedef void (*SamplerBuilderSetDeviceFunc)(SamplerBuilder*, Device*);
/*11676*/ typedef void (*SamplerBuilderSetDefaultsFunc)(SamplerBuilder*);
/*11677*/ typedef void (*SamplerBuilderSetMinMagFilterFunc)(SamplerBuilder*, MinFilter::Enum,
                                                            MagFilter::Enum);
/*11678*/ typedef void (*SamplerBuilderSetWrapModeFunc)(SamplerBuilder*, WrapMode::Enum,
                                                        WrapMode::Enum, WrapMode::Enum);
/*11679*/ typedef void (*SamplerBuilderSetLodClampFunc)(SamplerBuilder*, float, float);
/*11680*/ typedef void (*SamplerBuilderSetLodBiasFunc)(SamplerBuilder*, float);
/*11681*/ typedef void (*SamplerBuilderSetCompareFunc)(SamplerBuilder*, CompareMode::Enum,
                                                       CompareFunc::Enum);
/*11682*/ typedef void (*SamplerBuilderSetBorderColorFunc)(SamplerBuilder*, const float*);
/*11683*/ typedef void (*SamplerBuilderSetBorderColoriFunc)(SamplerBuilder*, const int*);
/*11684*/ typedef void (*SamplerBuilderSetBorderColoruiFunc)(SamplerBuilder*, const uint32_t*);
/*11685*/ typedef void (*SamplerBuilderSetMaxAnisotropyFunc)(SamplerBuilder*, float);
/*11686*/ typedef void (*SamplerBuilderSetReductionFilterFunc)(SamplerBuilder*,
                                                               SamplerReduction::Enum);
/*11687*/ typedef void (*SamplerBuilderSetLodSnapFunc)(SamplerBuilder*, float);
/*11688*/ typedef const Device* (*SamplerBuilderGetDeviceFunc)(const SamplerBuilder*);
/*11689*/ typedef void (*SamplerBuilderGetMinMagFilterFunc)(const SamplerBuilder*, MinFilter*,
                                                            MagFilter*);
/*11690*/ typedef void (*SamplerBuilderGetWrapModeFunc)(const SamplerBuilder*, WrapMode*, WrapMode*,
                                                        WrapMode*);
/*11691*/ typedef void (*SamplerBuilderGetLodClampFunc)(const SamplerBuilder*, float*, float*);
/*11692*/ typedef float (*SamplerBuilderGetLodBiasFunc)(const SamplerBuilder*);
/*11693*/ typedef void (*SamplerBuilderGetCompareFunc)(const SamplerBuilder*, CompareMode*,
                                                       CompareFunc*);
/*11694*/ typedef void (*SamplerBuilderGetBorderColorFunc)(const SamplerBuilder*, float*);
/*11695*/ typedef void (*SamplerBuilderGetBorderColoriFunc)(const SamplerBuilder*, int*);
/*11696*/ typedef void (*SamplerBuilderGetBorderColoruiFunc)(const SamplerBuilder*, uint32_t*);
/*11697*/ typedef float (*SamplerBuilderGetMaxAnisotropyFunc)(const SamplerBuilder*);
/*11698*/ typedef SamplerReduction::Enum (*SamplerBuilderGetReductionFilterFunc)(
    const SamplerBuilder*);
/*11699*/ typedef float (*SamplerBuilderGetLodSnapFunc)(const SamplerBuilder*);
/*11700*/ typedef NVNboolean (*SamplerInitializeFunc)(Sampler*, const SamplerBuilder*);
/*11701*/ typedef void (*SamplerFinalizeFunc)(Sampler*);
/*11702*/ typedef void (*SamplerSetDebugLabelFunc)(Sampler*, const char*);
/*11703*/ typedef void (*SamplerGetMinMagFilterFunc)(const Sampler*, MinFilter*, MagFilter*);
/*11704*/ typedef void (*SamplerGetWrapModeFunc)(const Sampler*, WrapMode*, WrapMode*, WrapMode*);
/*11705*/ typedef void (*SamplerGetLodClampFunc)(const Sampler*, float*, float*);
/*11706*/ typedef float (*SamplerGetLodBiasFunc)(const Sampler*);
/*11707*/ typedef void (*SamplerGetCompareFunc)(const Sampler*, CompareMode*, CompareFunc*);
/*11708*/ typedef void (*SamplerGetBorderColorFunc)(const Sampler*, float*);
/*11709*/ typedef void (*SamplerGetBorderColoriFunc)(const Sampler*, int*);
/*11710*/ typedef void (*SamplerGetBorderColoruiFunc)(const Sampler*, uint32_t*);
/*11711*/ typedef float (*SamplerGetMaxAnisotropyFunc)(const Sampler*);
/*11712*/ typedef SamplerReduction::Enum (*SamplerGetReductionFilterFunc)(const Sampler*);
/*11713*/ typedef NVNboolean (*SamplerCompareFunc)(const Sampler*, const Sampler*);
/*11714*/ typedef uint64_t (*SamplerGetDebugIDFunc)(const Sampler*);
/*11715*/ typedef void (*BlendStateSetDefaultsFunc)(BlendState*);
/*11716*/ typedef void (*BlendStateSetBlendTargetFunc)(BlendState*, int);
/*11717*/ typedef void (*BlendStateSetBlendFuncFunc)(BlendState*, BlendFunc::Enum, BlendFunc::Enum,
                                                     BlendFunc::Enum, BlendFunc::Enum);
/*11718*/ typedef void (*BlendStateSetBlendEquationFunc)(BlendState*, BlendEquation::Enum,
                                                         BlendEquation::Enum);
/*11719*/ typedef void (*BlendStateSetAdvancedModeFunc)(BlendState*, BlendAdvancedMode::Enum);
/*11720*/ typedef void (*BlendStateSetAdvancedOverlapFunc)(BlendState*, BlendAdvancedOverlap::Enum);
/*11721*/ typedef void (*BlendStateSetAdvancedPremultipliedSrcFunc)(BlendState*, NVNboolean);
/*11722*/ typedef void (*BlendStateSetAdvancedNormalizedDstFunc)(BlendState*, NVNboolean);
/*11723*/ typedef int (*BlendStateGetBlendTargetFunc)(const BlendState*);
/*11724*/ typedef void (*BlendStateGetBlendFuncFunc)(const BlendState*, BlendFunc*, BlendFunc*,
                                                     BlendFunc*, BlendFunc*);
/*11725*/ typedef void (*BlendStateGetBlendEquationFunc)(const BlendState*, BlendEquation*,
                                                         BlendEquation*);
/*11726*/ typedef BlendAdvancedMode::Enum (*BlendStateGetAdvancedModeFunc)(const BlendState*);
/*11727*/ typedef BlendAdvancedOverlap::Enum (*BlendStateGetAdvancedOverlapFunc)(const BlendState*);
/*11728*/ typedef NVNboolean (*BlendStateGetAdvancedPremultipliedSrcFunc)(const BlendState*);
/*11729*/ typedef NVNboolean (*BlendStateGetAdvancedNormalizedDstFunc)(const BlendState*);
/*11730*/ typedef void (*ColorStateSetDefaultsFunc)(ColorState*);
/*11731*/ typedef void (*ColorStateSetBlendEnableFunc)(ColorState*, int, NVNboolean);
/*11732*/ typedef void (*ColorStateSetLogicOpFunc)(ColorState*, LogicOp::Enum);
/*11733*/ typedef void (*ColorStateSetAlphaTestFunc)(ColorState*, AlphaFunc::Enum);
/*11734*/ typedef NVNboolean (*ColorStateGetBlendEnableFunc)(const ColorState*, int);
/*11735*/ typedef LogicOp::Enum (*ColorStateGetLogicOpFunc)(const ColorState*);
/*11736*/ typedef AlphaFunc::Enum (*ColorStateGetAlphaTestFunc)(const ColorState*);
/*11737*/ typedef void (*ChannelMaskStateSetDefaultsFunc)(ChannelMaskState*);
/*11738*/ typedef void (*ChannelMaskStateSetChannelMaskFunc)(ChannelMaskState*, int, NVNboolean,
                                                             NVNboolean, NVNboolean, NVNboolean);
/*11739*/ typedef void (*ChannelMaskStateGetChannelMaskFunc)(const ChannelMaskState*, int,
                                                             NVNboolean*, NVNboolean*, NVNboolean*,
                                                             NVNboolean*);
/*11740*/ typedef void (*MultisampleStateSetDefaultsFunc)(MultisampleState*);
/*11741*/ typedef void (*MultisampleStateSetMultisampleEnableFunc)(MultisampleState*, NVNboolean);
/*11742*/ typedef void (*MultisampleStateSetSamplesFunc)(MultisampleState*, int);
/*11743*/ typedef void (*MultisampleStateSetAlphaToCoverageEnableFunc)(MultisampleState*,
                                                                       NVNboolean);
/*11744*/ typedef void (*MultisampleStateSetAlphaToCoverageDitherFunc)(MultisampleState*,
                                                                       NVNboolean);
/*11745*/ typedef NVNboolean (*MultisampleStateGetMultisampleEnableFunc)(const MultisampleState*);
/*11746*/ typedef int (*MultisampleStateGetSamplesFunc)(const MultisampleState*);
/*11747*/ typedef NVNboolean (*MultisampleStateGetAlphaToCoverageEnableFunc)(
    const MultisampleState*);
/*11748*/ typedef NVNboolean (*MultisampleStateGetAlphaToCoverageDitherFunc)(
    const MultisampleState*);
/*11749*/ typedef void (*MultisampleStateSetRasterSamplesFunc)(MultisampleState*, int);
/*11750*/ typedef int (*MultisampleStateGetRasterSamplesFunc)(MultisampleState*);
/*11751*/ typedef void (*MultisampleStateSetCoverageModulationModeFunc)(
    MultisampleState*, CoverageModulationMode::Enum);
/*11752*/ typedef CoverageModulationMode::Enum (*MultisampleStateGetCoverageModulationModeFunc)(
    const MultisampleState*);
/*11753*/ typedef void (*MultisampleStateSetCoverageToColorEnableFunc)(MultisampleState*,
                                                                       NVNboolean);
/*11754*/ typedef NVNboolean (*MultisampleStateGetCoverageToColorEnableFunc)(
    const MultisampleState*);
/*11755*/ typedef void (*MultisampleStateSetCoverageToColorOutputFunc)(MultisampleState*, int);
/*11756*/ typedef int (*MultisampleStateGetCoverageToColorOutputFunc)(const MultisampleState*);
/*11757*/ typedef void (*MultisampleStateSetSampleLocationsEnableFunc)(MultisampleState*,
                                                                       NVNboolean);
/*11758*/ typedef NVNboolean (*MultisampleStateGetSampleLocationsEnableFunc)(
    const MultisampleState*);
/*11759*/ typedef void (*MultisampleStateGetSampleLocationsGridFunc)(MultisampleState*, int*, int*);
/*11760*/ typedef void (*MultisampleStateSetSampleLocationsGridEnableFunc)(MultisampleState*,
                                                                           NVNboolean);
/*11761*/ typedef NVNboolean (*MultisampleStateGetSampleLocationsGridEnableFunc)(
    const MultisampleState*);
/*11762*/ typedef void (*MultisampleStateSetSampleLocationsFunc)(MultisampleState*, int, int,
                                                                 const float*);
/*11763*/ typedef void (*PolygonStateSetDefaultsFunc)(PolygonState*);
/*11764*/ typedef void (*PolygonStateSetCullFaceFunc)(PolygonState*, Face::Enum);
/*11765*/ typedef void (*PolygonStateSetFrontFaceFunc)(PolygonState*, FrontFace::Enum);
/*11766*/ typedef void (*PolygonStateSetPolygonModeFunc)(PolygonState*, PolygonMode::Enum);
/*11767*/ typedef void (*PolygonStateSetPolygonOffsetEnablesFunc)(PolygonState*, int);
/*11768*/ typedef Face::Enum (*PolygonStateGetCullFaceFunc)(const PolygonState*);
/*11769*/ typedef FrontFace::Enum (*PolygonStateGetFrontFaceFunc)(const PolygonState*);
/*11770*/ typedef PolygonMode::Enum (*PolygonStateGetPolygonModeFunc)(const PolygonState*);
/*11771*/ typedef int (*PolygonStateGetPolygonOffsetEnablesFunc)(const PolygonState*);
/*11772*/ typedef void (*DepthStencilStateSetDefaultsFunc)(DepthStencilState*);
/*11773*/ typedef void (*DepthStencilStateSetDepthTestEnableFunc)(DepthStencilState*, NVNboolean);
/*11774*/ typedef void (*DepthStencilStateSetDepthWriteEnableFunc)(DepthStencilState*, NVNboolean);
/*11775*/ typedef void (*DepthStencilStateSetDepthFuncFunc)(DepthStencilState*, DepthFunc::Enum);
/*11776*/ typedef void (*DepthStencilStateSetStencilTestEnableFunc)(DepthStencilState*, NVNboolean);
/*11777*/ typedef void (*DepthStencilStateSetStencilFuncFunc)(DepthStencilState*, Face::Enum,
                                                              StencilFunc::Enum);
/*11778*/ typedef void (*DepthStencilStateSetStencilOpFunc)(DepthStencilState*, Face::Enum,
                                                            StencilOp::Enum, StencilOp::Enum,
                                                            StencilOp::Enum);
/*11779*/ typedef NVNboolean (*DepthStencilStateGetDepthTestEnableFunc)(const DepthStencilState*);
/*11780*/ typedef NVNboolean (*DepthStencilStateGetDepthWriteEnableFunc)(const DepthStencilState*);
/*11781*/ typedef DepthFunc::Enum (*DepthStencilStateGetDepthFuncFunc)(const DepthStencilState*);
/*11782*/ typedef NVNboolean (*DepthStencilStateGetStencilTestEnableFunc)(const DepthStencilState*);
/*11783*/ typedef StencilFunc::Enum (*DepthStencilStateGetStencilFuncFunc)(const DepthStencilState*,
                                                                           Face::Enum);
/*11784*/ typedef void (*DepthStencilStateGetStencilOpFunc)(const DepthStencilState*, Face::Enum,
                                                            StencilOp*, StencilOp*, StencilOp*);
/*11785*/ typedef void (*VertexAttribStateSetDefaultsFunc)(VertexAttribState*);
/*11786*/ typedef void (*VertexAttribStateSetFormatFunc)(VertexAttribState*, Format::Enum,
                                                         ptrdiff_t);
/*11787*/ typedef void (*VertexAttribStateSetStreamIndexFunc)(VertexAttribState*, int);
/*11788*/ typedef void (*VertexAttribStateGetFormatFunc)(const VertexAttribState*, Format*,
                                                         ptrdiff_t*);
/*11789*/ typedef int (*VertexAttribStateGetStreamIndexFunc)(const VertexAttribState*);
/*11790*/ typedef void (*VertexStreamStateSetDefaultsFunc)(VertexStreamState*);
/*11791*/ typedef void (*VertexStreamStateSetStrideFunc)(VertexStreamState*, ptrdiff_t);
/*11792*/ typedef void (*VertexStreamStateSetDivisorFunc)(VertexStreamState*, int);
/*11793*/ typedef ptrdiff_t (*VertexStreamStateGetStrideFunc)(const VertexStreamState*);
/*11794*/ typedef int (*VertexStreamStateGetDivisorFunc)(const VertexStreamState*);
/*11795*/ typedef NVNboolean (*CommandBufferInitializeFunc)(CommandBuffer*, Device*);
/*11796*/ typedef void (*CommandBufferFinalizeFunc)(CommandBuffer*);
/*11797*/ typedef void (*CommandBufferSetDebugLabelFunc)(CommandBuffer*, const char*);
/*11798*/ typedef void (*CommandBufferSetMemoryCallbackFunc)(CommandBuffer*,
                                                             CommandBufferMemoryCallbackFunc);
/*11799*/ typedef void (*CommandBufferSetMemoryCallbackDataFunc)(CommandBuffer*, void*);
/*11800*/ typedef void (*CommandBufferAddCommandMemoryFunc)(CommandBuffer*, const MemoryPool*,
                                                            ptrdiff_t, size_t);
/*11801*/ typedef void (*CommandBufferAddControlMemoryFunc)(CommandBuffer*, void*, size_t);
/*11802*/ typedef size_t (*CommandBufferGetCommandMemorySizeFunc)(const CommandBuffer*);
/*11803*/ typedef size_t (*CommandBufferGetCommandMemoryUsedFunc)(const CommandBuffer*);
/*11804*/ typedef size_t (*CommandBufferGetCommandMemoryFreeFunc)(const CommandBuffer*);
/*11805*/ typedef size_t (*CommandBufferGetControlMemorySizeFunc)(const CommandBuffer*);
/*11806*/ typedef size_t (*CommandBufferGetControlMemoryUsedFunc)(const CommandBuffer*);
/*11807*/ typedef size_t (*CommandBufferGetControlMemoryFreeFunc)(const CommandBuffer*);
/*11808*/ typedef void (*CommandBufferBeginRecordingFunc)(CommandBuffer*);
/*11809*/ typedef CommandHandle (*CommandBufferEndRecordingFunc)(CommandBuffer*);
/*11810*/ typedef void (*CommandBufferCallCommandsFunc)(CommandBuffer*, int, const CommandHandle*);
/*11811*/ typedef void (*CommandBufferCopyCommandsFunc)(CommandBuffer*, int, const CommandHandle*);
/*11812*/ typedef void (*CommandBufferBindBlendStateFunc)(CommandBuffer*, const BlendState*);
/*11813*/ typedef void (*CommandBufferBindChannelMaskStateFunc)(CommandBuffer*,
                                                                const ChannelMaskState*);
/*11814*/ typedef void (*CommandBufferBindColorStateFunc)(CommandBuffer*, const ColorState*);
/*11815*/ typedef void (*CommandBufferBindMultisampleStateFunc)(CommandBuffer*,
                                                                const MultisampleState*);
/*11816*/ typedef void (*CommandBufferBindPolygonStateFunc)(CommandBuffer*, const PolygonState*);
/*11817*/ typedef void (*CommandBufferBindDepthStencilStateFunc)(CommandBuffer*,
                                                                 const DepthStencilState*);
/*11818*/ typedef void (*CommandBufferBindVertexAttribStateFunc)(CommandBuffer*, int,
                                                                 const VertexAttribState*);
/*11819*/ typedef void (*CommandBufferBindVertexStreamStateFunc)(CommandBuffer*, int,
                                                                 const VertexStreamState*);
/*11820*/ typedef void (*CommandBufferBindProgramFunc)(CommandBuffer*, const Program*, int);
/*11821*/ typedef void (*CommandBufferBindVertexBufferFunc)(CommandBuffer*, int, BufferAddress,
                                                            size_t);
/*11822*/ typedef void (*CommandBufferBindVertexBuffersFunc)(CommandBuffer*, int, int,
                                                             const BufferRange*);
/*11823*/ typedef void (*CommandBufferBindUniformBufferFunc)(CommandBuffer*, ShaderStage::Enum, int,
                                                             BufferAddress, size_t);
/*11824*/ typedef void (*CommandBufferBindUniformBuffersFunc)(CommandBuffer*, ShaderStage::Enum,
                                                              int, int, const BufferRange*);
/*11825*/ typedef void (*CommandBufferBindTransformFeedbackBufferFunc)(CommandBuffer*, int,
                                                                       BufferAddress, size_t);
/*11826*/ typedef void (*CommandBufferBindTransformFeedbackBuffersFunc)(CommandBuffer*, int, int,
                                                                        const BufferRange*);
/*11827*/ typedef void (*CommandBufferBindStorageBufferFunc)(CommandBuffer*, ShaderStage::Enum, int,
                                                             BufferAddress, size_t);
/*11828*/ typedef void (*CommandBufferBindStorageBuffersFunc)(CommandBuffer*, ShaderStage::Enum,
                                                              int, int, const BufferRange*);
/*11829*/ typedef void (*CommandBufferBindTextureFunc)(CommandBuffer*, ShaderStage::Enum, int,
                                                       TextureHandle);
/*11830*/ typedef void (*CommandBufferBindTexturesFunc)(CommandBuffer*, ShaderStage::Enum, int, int,
                                                        const TextureHandle*);
/*11831*/ typedef void (*CommandBufferBindImageFunc)(CommandBuffer*, ShaderStage::Enum, int,
                                                     ImageHandle);
/*11832*/ typedef void (*CommandBufferBindImagesFunc)(CommandBuffer*, ShaderStage::Enum, int, int,
                                                      const ImageHandle*);
/*11833*/ typedef void (*CommandBufferSetPatchSizeFunc)(CommandBuffer*, int);
/*11834*/ typedef void (*CommandBufferSetInnerTessellationLevelsFunc)(CommandBuffer*, const float*);
/*11835*/ typedef void (*CommandBufferSetOuterTessellationLevelsFunc)(CommandBuffer*, const float*);
/*11836*/ typedef void (*CommandBufferSetPrimitiveRestartFunc)(CommandBuffer*, NVNboolean, int);
/*11837*/ typedef void (*CommandBufferBeginTransformFeedbackFunc)(CommandBuffer*, BufferAddress);
/*11838*/ typedef void (*CommandBufferEndTransformFeedbackFunc)(CommandBuffer*, BufferAddress);
/*11839*/ typedef void (*CommandBufferPauseTransformFeedbackFunc)(CommandBuffer*, BufferAddress);
/*11840*/ typedef void (*CommandBufferResumeTransformFeedbackFunc)(CommandBuffer*, BufferAddress);
/*11841*/ typedef void (*CommandBufferDrawTransformFeedbackFunc)(CommandBuffer*,
                                                                 DrawPrimitive::Enum,
                                                                 BufferAddress);
/*11842*/ typedef void (*CommandBufferDrawArraysFunc)(CommandBuffer*, DrawPrimitive::Enum, int,
                                                      int);
/*11843*/ typedef void (*CommandBufferDrawElementsFunc)(CommandBuffer*, DrawPrimitive::Enum,
                                                        IndexType::Enum, int, BufferAddress);
/*11844*/ typedef void (*CommandBufferDrawElementsBaseVertexFunc)(CommandBuffer*,
                                                                  DrawPrimitive::Enum,
                                                                  IndexType::Enum, int,
                                                                  BufferAddress, int);
/*11845*/ typedef void (*CommandBufferDrawArraysInstancedFunc)(CommandBuffer*, DrawPrimitive::Enum,
                                                               int, int, int, int);
/*11846*/ typedef void (*CommandBufferDrawElementsInstancedFunc)(CommandBuffer*,
                                                                 DrawPrimitive::Enum,
                                                                 IndexType::Enum, int,
                                                                 BufferAddress, int, int, int);
/*11847*/ typedef void (*CommandBufferDrawArraysIndirectFunc)(CommandBuffer*, DrawPrimitive::Enum,
                                                              BufferAddress);
/*11848*/ typedef void (*CommandBufferDrawElementsIndirectFunc)(CommandBuffer*, DrawPrimitive::Enum,
                                                                IndexType::Enum, BufferAddress,
                                                                BufferAddress);
/*11849*/ typedef void (*CommandBufferMultiDrawArraysIndirectCountFunc)(
    CommandBuffer*, DrawPrimitive::Enum, BufferAddress, BufferAddress, int, ptrdiff_t);
/*11850*/ typedef void (*CommandBufferMultiDrawElementsIndirectCountFunc)(
    CommandBuffer*, DrawPrimitive::Enum, IndexType::Enum, BufferAddress, BufferAddress,
    BufferAddress, int, ptrdiff_t);
/*11851*/ typedef void (*CommandBufferClearColorFunc)(CommandBuffer*, int, const float*, int);
/*11852*/ typedef void (*CommandBufferClearColoriFunc)(CommandBuffer*, int, const int*, int);
/*11853*/ typedef void (*CommandBufferClearColoruiFunc)(CommandBuffer*, int, const uint32_t*, int);
/*11854*/ typedef void (*CommandBufferClearDepthStencilFunc)(CommandBuffer*, float, NVNboolean, int,
                                                             int);
/*11855*/ typedef void (*CommandBufferDispatchComputeFunc)(CommandBuffer*, int, int, int);
/*11856*/ typedef void (*CommandBufferDispatchComputeIndirectFunc)(CommandBuffer*, BufferAddress);
/*11857*/ typedef void (*CommandBufferSetViewportFunc)(CommandBuffer*, int, int, int, int);
/*11858*/ typedef void (*CommandBufferSetViewportsFunc)(CommandBuffer*, int, int, const float*);
/*11859*/ typedef void (*CommandBufferSetViewportSwizzlesFunc)(CommandBuffer*, int, int,
                                                               const ViewportSwizzle*);
/*11860*/ typedef void (*CommandBufferSetScissorFunc)(CommandBuffer*, int, int, int, int);
/*11861*/ typedef void (*CommandBufferSetScissorsFunc)(CommandBuffer*, int, int, const int*);
/*11862*/ typedef void (*CommandBufferSetDepthRangeFunc)(CommandBuffer*, float, float);
/*11863*/ typedef void (*CommandBufferSetDepthBoundsFunc)(CommandBuffer*, NVNboolean, float, float);
/*11864*/ typedef void (*CommandBufferSetDepthRangesFunc)(CommandBuffer*, int, int, const float*);
/*11865*/ typedef void (*CommandBufferSetTiledCacheActionFunc)(CommandBuffer*,
                                                               TiledCacheAction::Enum);
/*11866*/ typedef void (*CommandBufferSetTiledCacheTileSizeFunc)(CommandBuffer*, int, int);
/*11867*/ typedef void (*CommandBufferBindSeparateTextureFunc)(CommandBuffer*, ShaderStage::Enum,
                                                               int, SeparateTextureHandle);
/*11868*/ typedef void (*CommandBufferBindSeparateSamplerFunc)(CommandBuffer*, ShaderStage::Enum,
                                                               int, SeparateSamplerHandle);
/*11869*/ typedef void (*CommandBufferBindSeparateTexturesFunc)(CommandBuffer*, ShaderStage::Enum,
                                                                int, int,
                                                                const SeparateTextureHandle*);
/*11870*/ typedef void (*CommandBufferBindSeparateSamplersFunc)(CommandBuffer*, ShaderStage::Enum,
                                                                int, int,
                                                                const SeparateSamplerHandle*);
/*11871*/ typedef void (*CommandBufferSetStencilValueMaskFunc)(CommandBuffer*, Face::Enum, int);
/*11872*/ typedef void (*CommandBufferSetStencilMaskFunc)(CommandBuffer*, Face::Enum, int);
/*11873*/ typedef void (*CommandBufferSetStencilRefFunc)(CommandBuffer*, Face::Enum, int);
/*11874*/ typedef void (*CommandBufferSetBlendColorFunc)(CommandBuffer*, const float*);
/*11875*/ typedef void (*CommandBufferSetPointSizeFunc)(CommandBuffer*, float);
/*11876*/ typedef void (*CommandBufferSetLineWidthFunc)(CommandBuffer*, float);
/*11877*/ typedef void (*CommandBufferSetPolygonOffsetClampFunc)(CommandBuffer*, float, float,
                                                                 float);
/*11878*/ typedef void (*CommandBufferSetAlphaRefFunc)(CommandBuffer*, float);
/*11879*/ typedef void (*CommandBufferSetSampleMaskFunc)(CommandBuffer*, int);
/*11880*/ typedef void (*CommandBufferSetRasterizerDiscardFunc)(CommandBuffer*, NVNboolean);
/*11881*/ typedef void (*CommandBufferSetDepthClampFunc)(CommandBuffer*, NVNboolean);
/*11882*/ typedef void (*CommandBufferSetConservativeRasterEnableFunc)(CommandBuffer*, NVNboolean);
/*11883*/ typedef void (*CommandBufferSetConservativeRasterDilateFunc)(CommandBuffer*, float);
/*11884*/ typedef void (*CommandBufferSetSubpixelPrecisionBiasFunc)(CommandBuffer*, int, int);
/*11885*/ typedef void (*CommandBufferCopyBufferToTextureFunc)(CommandBuffer*, BufferAddress,
                                                               const Texture*, const TextureView*,
                                                               const CopyRegion*, int);
/*11886*/ typedef void (*CommandBufferCopyTextureToBufferFunc)(CommandBuffer*, const Texture*,
                                                               const TextureView*,
                                                               const CopyRegion*, BufferAddress,
                                                               int);
/*11887*/ typedef void (*CommandBufferCopyTextureToTextureFunc)(CommandBuffer*, const Texture*,
                                                                const TextureView*,
                                                                const CopyRegion*, const Texture*,
                                                                const TextureView*,
                                                                const CopyRegion*, int);
/*11888*/ typedef void (*CommandBufferCopyBufferToBufferFunc)(CommandBuffer*, BufferAddress,
                                                              BufferAddress, size_t, int);
/*11889*/ typedef void (*CommandBufferClearBufferFunc)(CommandBuffer*, BufferAddress, size_t,
                                                       uint32_t);
/*11890*/ typedef void (*CommandBufferClearTextureFunc)(CommandBuffer*, const Texture*,
                                                        const TextureView*, const CopyRegion*,
                                                        const float*, int);
/*11891*/ typedef void (*CommandBufferClearTextureiFunc)(CommandBuffer*, const Texture*,
                                                         const TextureView*, const CopyRegion*,
                                                         const int*, int);
/*11892*/ typedef void (*CommandBufferClearTextureuiFunc)(CommandBuffer*, const Texture*,
                                                          const TextureView*, const CopyRegion*,
                                                          const uint32_t*, int);
/*11893*/ typedef void (*CommandBufferUpdateUniformBufferFunc)(CommandBuffer*, BufferAddress,
                                                               size_t, ptrdiff_t, size_t,
                                                               const void*);
/*11894*/ typedef void (*CommandBufferReportCounterFunc)(CommandBuffer*, CounterType::Enum,
                                                         BufferAddress);
/*11895*/ typedef void (*CommandBufferResetCounterFunc)(CommandBuffer*, CounterType::Enum);
/*11896*/ typedef void (*CommandBufferReportValueFunc)(CommandBuffer*, uint32_t, BufferAddress);
/*11897*/ typedef void (*CommandBufferSetRenderEnableFunc)(CommandBuffer*, NVNboolean);
/*11898*/ typedef void (*CommandBufferSetRenderEnableConditionalFunc)(CommandBuffer*,
                                                                      ConditionalRenderMode::Enum,
                                                                      BufferAddress);
/*11899*/ typedef void (*CommandBufferSetRenderTargetsFunc)(CommandBuffer*, int,
                                                            const Texture* const*,
                                                            const TextureView* const*,
                                                            const Texture*, const TextureView*);
/*11900*/ typedef void (*CommandBufferDiscardColorFunc)(CommandBuffer*, int);
/*11901*/ typedef void (*CommandBufferDiscardDepthStencilFunc)(CommandBuffer*);
/*11902*/ typedef void (*CommandBufferDownsampleFunc)(CommandBuffer*, const Texture*,
                                                      const Texture*);
/*11903*/ typedef void (*CommandBufferTiledDownsampleFunc)(CommandBuffer*, const Texture*,
                                                           const Texture*);
/*11904*/ typedef void (*CommandBufferDownsampleTextureViewFunc)(CommandBuffer*, const Texture*,
                                                                 const TextureView*, const Texture*,
                                                                 const TextureView*);
/*11905*/ typedef void (*CommandBufferTiledDownsampleTextureViewFunc)(
    CommandBuffer*, const Texture*, const TextureView*, const Texture*, const TextureView*);
/*11906*/ typedef void (*CommandBufferBarrierFunc)(CommandBuffer*, int);
/*11907*/ typedef void (*CommandBufferWaitSyncFunc)(CommandBuffer*, const Sync*);
/*11908*/ typedef void (*CommandBufferFenceSyncFunc)(CommandBuffer*, Sync*, SyncCondition::Enum,
                                                     int);
/*11909*/ typedef void (*CommandBufferSetTexturePoolFunc)(CommandBuffer*, const TexturePool*);
/*11910*/ typedef void (*CommandBufferSetSamplerPoolFunc)(CommandBuffer*, const SamplerPool*);
/*11911*/ typedef void (*CommandBufferSetShaderScratchMemoryFunc)(CommandBuffer*, const MemoryPool*,
                                                                  ptrdiff_t, size_t);
/*11912*/ typedef void (*CommandBufferSaveZCullDataFunc)(CommandBuffer*, BufferAddress, size_t);
/*11913*/ typedef void (*CommandBufferRestoreZCullDataFunc)(CommandBuffer*, BufferAddress, size_t);
/*11914*/ typedef void (*CommandBufferSetCopyRowStrideFunc)(CommandBuffer*, ptrdiff_t);
/*11915*/ typedef void (*CommandBufferSetCopyImageStrideFunc)(CommandBuffer*, ptrdiff_t);
/*11916*/ typedef ptrdiff_t (*CommandBufferGetCopyRowStrideFunc)(const CommandBuffer*);
/*11917*/ typedef ptrdiff_t (*CommandBufferGetCopyImageStrideFunc)(const CommandBuffer*);
/*11918*/ typedef void (*CommandBufferDrawTextureFunc)(CommandBuffer*, TextureHandle,
                                                       const DrawTextureRegion*,
                                                       const DrawTextureRegion*);
/*11919*/ typedef NVNboolean (*ProgramSetSubroutineLinkageFunc)(Program*, int,
                                                                const NVNsubroutineLinkageMapPtr*);
/*11920*/ typedef void (*CommandBufferSetProgramSubroutinesFunc)(CommandBuffer*, Program*,
                                                                 ShaderStage::Enum, const int,
                                                                 const int, const int*);
/*11921*/ typedef void (*CommandBufferBindCoverageModulationTableFunc)(CommandBuffer*,
                                                                       const float*);
/*11922*/ typedef void (*CommandBufferResolveDepthBufferFunc)(CommandBuffer*);
/*11923*/ typedef void (*CommandBufferSetColorReductionEnableFunc)(CommandBuffer*, NVNboolean);
/*11924*/ typedef void (*CommandBufferSetColorReductionThresholdsFunc)(CommandBuffer*,
                                                                       FormatClass::Enum, int, int);
/*11925*/ typedef void (*CommandBufferPushDebugGroupStaticFunc)(CommandBuffer*, uint32_t,
                                                                const char*);
/*11926*/ typedef void (*CommandBufferPushDebugGroupDynamicFunc)(CommandBuffer*, uint32_t,
                                                                 const char*);
/*11927*/ typedef void (*CommandBufferPushDebugGroupFunc)(CommandBuffer*, const char*);
/*11928*/ typedef void (*CommandBufferPopDebugGroupFunc)(CommandBuffer*);
/*11929*/ typedef void (*CommandBufferPopDebugGroupIdFunc)(CommandBuffer*, uint32_t);
/*11930*/ typedef void (*CommandBufferInsertDebugMarkerStaticFunc)(CommandBuffer*, uint32_t,
                                                                   const char*);
/*11931*/ typedef void (*CommandBufferInsertDebugMarkerDynamicFunc)(CommandBuffer*, uint32_t,
                                                                    const char*);
/*11932*/ typedef void (*CommandBufferInsertDebugMarkerFunc)(CommandBuffer*, const char*);
/*11933*/ typedef CommandBufferMemoryCallbackFunc (*CommandBufferGetMemoryCallbackFunc)(
    const CommandBuffer*);
/*11934*/ typedef void* (*CommandBufferGetMemoryCallbackDataFunc)(const CommandBuffer*);
/*11935*/ typedef NVNboolean (*CommandBufferIsRecordingFunc)(const CommandBuffer*);
/*11936*/ typedef NVNboolean (*SyncInitializeFunc)(Sync*, Device*);
/*11937*/ typedef void (*SyncFinalizeFunc)(Sync*);
/*11938*/ typedef void (*SyncSetDebugLabelFunc)(Sync*, const char*);
/*11939*/ typedef void (*QueueFenceSyncFunc)(Queue*, Sync*, SyncCondition::Enum, int);
/*11940*/ typedef SyncWaitResult::Enum (*SyncWaitFunc)(const Sync*, uint64_t);
/*11941*/ typedef NVNboolean (*QueueWaitSyncFunc)(Queue*, const Sync*);
/*11942*/ typedef void (*EventBuilderSetDefaultsFunc)(EventBuilder*);
/*11943*/ typedef void (*EventBuilderSetStorageFunc)(EventBuilder*, const MemoryPool*, int64_t);
/*11944*/ typedef const MemoryPool* (*EventBuilderGetStorageFunc)(const EventBuilder*, int64_t*);
/*11945*/ typedef NVNboolean (*EventInitializeFunc)(Event*, const EventBuilder*);
/*11946*/ typedef void (*EventFinalizeFunc)(Event*);
/*11947*/ typedef uint32_t (*EventGetValueFunc)(const Event*);
/*11948*/ typedef void (*EventSignalFunc)(Event*, EventSignalMode::Enum, uint32_t);
/*11949*/ typedef void (*CommandBufferWaitEventFunc)(CommandBuffer*, const Event*,
                                                     EventWaitMode::Enum, uint32_t);
/*11950*/ typedef void (*CommandBufferSignalEventFunc)(CommandBuffer*, const Event*,
                                                       EventSignalMode::Enum,
                                                       EventSignalLocation::Enum, int, uint32_t);
/*11951*/ typedef void (*CommandBufferSetStencilCullCriteriaFunc)(CommandBuffer*, StencilFunc::Enum,
                                                                  int, int);
}  // namespace nvn