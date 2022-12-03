#include <nvnTool/nvnTool_GlslcInterface.h>

namespace nn::gfx::detail {
typedef bool (*GlslcCompilePreSpecializedType)(GLSLCcompileObject*);
typedef const GLSLCoutput* const* (*GlslcCompileSpecializedType)(GLSLCcompileObject*,
                                                                 const GLSLCspecializationBatch*);
typedef uint8_t (*GlslcInitializeType)(GLSLCcompileObject*);
typedef void (*GlslcFinalizeType)(GLSLCcompileObject*);
typedef uint8_t (*GlslcCompileType)(GLSLCcompileObject*);
typedef GLSLCversion (*GlslcGetVersionType)(void);
typedef void (*GlslcSetAllocatorType)(GLSLCallocateFunction, GLSLCfreeFunction,
                                      GLSLCreallocateFunction, void*);
typedef GLSLCoptions (*GlslcGetDefaultOptionsType)(void);
}  // namespace nn::gfx::detail