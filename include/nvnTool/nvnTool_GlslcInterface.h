#pragma once

#include <nvn/nvn.h>

/*
GLSLCpiqUniformKind;
GLSLCpiqTypeEnum;
*/

typedef enum GLSLCdebugInfoLevel_enum {
    GLSLC_DEBUG_LEVEL_NONE,
    GLSLC_DEBUG_LEVEL_G0,
    GLSLC_DEBUG_LEVEL_G1,
    GLSLC_DEBUG_LEVEL_G2
} GLSLCdebugInfoLevelEnum;

typedef enum GLSLClanguageTypeEnum_enum {
    GLSLC_LANGUAGE_GLSL,
    GLSLC_LANGUAGE_GLES,
    GLSLC_LANGUAGE_SPIRV
} GLSLClanguageTypeEnum;

typedef enum SpillControlEnum_enum { DEFAULT_SPILL, NO_SPILL } SpillControlEnum;

typedef enum GLSLCoptLevel_enum { GLSLC_OPTLEVEL_DEFAULT, GLSLC_OPTLEVEL_NONE } GLSLCoptLevelEnum;

typedef enum GLSLCunrollControl_enum {
    GLSLC_LOOP_UNROLL_DEFAULT,
    GLSLC_LOOP_UNROLL_NONE,
    GLSLC_LOOP_UNROLL_ALL
} GLSLCunrollControlEnum;

typedef enum GLSLCwarnUninitControl_enum {
    GLSLC_WARN_UNINIT_DEFAULT,
    GLSLC_WARN_UNINIT_NONE,
    GLSLC_WARN_UNINIT_ALL
} GLSLCwarnUninitControlEnum;

typedef enum GLSLCsectionTypeEnum_enum {
    GLSLC_SECTION_TYPE_GPU_CODE,
    GLSLC_SECTION_TYPE_ASM_DUMP,
    GLSLC_SECTION_TYPE_PERF_STATS,
    GLSLC_SECTION_TYPE_REFLECTION,
    GLSLC_SECTION_TYPE_DEBUG_INFO,
    GLSLC_NUM_SECTION_TYPES,
    GLSLC_SECTION_TYPE_ENUM_LARGE = -1
} GLSLCsectionTypeEnum;

/*
GLSLCpiqName_rec;
typedef GLSLCpiqName_rec GLSLCpiqName;
GLSLCuniformBlockInfo_rec;
GLSLCuniform_rec;
GLSLCProgramInputInfo_rec;
GLSLCProgramOutput_rec;
GLSLCssboInfo_rec;
GLSLCxfbInfo_rec;
typedef GLSLCxfbInfo_rec GLSLCxfbInfo;
GLSLCincludeInfo_rec;
typedef GLSLCincludeInfo_rec GLSLCincludeInfo;
typedef GLSLCspecializationBatch_rec GLSLCspecializationBatch;
*/

typedef struct GLSLCoptionFlags_rec {
    uint32_t glslSeparable : 1;
    uint32_t outputAssembly : 1;
    uint32_t outputGpuBinaries : 1;
    uint32_t outputPerfStats : 1;
    uint32_t outputShaderReflection : 1;
    GLSLClanguageTypeEnum language : 4;
    GLSLCdebugInfoLevelEnum outputDebugInfo : 4;
    SpillControlEnum spillControl : 4;
    uint32_t outputThinGpuBinaries : 1;
    uint32_t tessellationAndPassthroughGS : 1;
    uint32_t prioritizeConsecutiveTextureInstructions : 1;
    uint32_t enableFastMathMask : 6;
    GLSLCoptLevelEnum optLevel : 3;
    GLSLCunrollControlEnum unrollControl : 3;
    uint32_t errorOnScratchMemUsage : 1;
    uint32_t enableCBFOptimization : 1;
    uint32_t enableWarpCulling : 1;
    uint32_t enableMultithreadCompilation : 1;
    GLSLCwarnUninitControlEnum warnUninitControl : 3;
} GLSLCoptionFlags;

/*
GLSLCoptions_rec;
typedef GLSLCoptions_rec GLSLCoptions;
GLSLCcompilationStatus_rec;
typedef GLSLCcompilationStatus_rec GLSLCcompilationStatus;
typedef GLSLCspirvSpecializationInfo_rec GLSLCspirvSpecializationInfo;
GLSLCinput_rec;
typedef GLSLCinput_rec GLSLCinput;
*/

typedef struct GLSLCsectionHeaderCommon_rec {
    uint32_t size;
    uint32_t dataOffset;
    GLSLCsectionTypeEnum type;
    uint8_t reserved[32];
} GLSLCsectionHeaderCommon;

/*
GLSLCshaderInfoCompute_rec;
typedef GLSLCshaderInfoCompute_rec GLSLCshaderInfoCompute;
GLSLCshaderInfoUnion_rec;
typedef GLSLCshaderInfoUnion_rec GLSLCshaderInfoUnion;
GLSLCperStageShaderInfo_rec;
typedef GLSLCperStageShaderInfo_rec GLSLCperStageShaderInfo;
*/

typedef struct GLSLCprogramReflectionHeader_rec {
    GLSLCsectionHeaderCommon common;
    uint32_t numUniformBlocks;
    uint32_t uniformBlockOffset;
    uint32_t numUniforms;
    uint32_t uniformOffset;
    uint32_t numProgramInputs;
    uint32_t programInputsOffset;
    uint32_t numProgramOutputs;
    uint32_t programOutputsOffset;
    uint32_t numSsbo;
    uint32_t ssboOffset;
    uint32_t numBufferVariables;
    uint32_t bufferVariableOffset;
    uint32_t numXfbVaryings;
    uint32_t xfbVaryingsOffset;
    uint32_t stringPoolSize;
    uint32_t stringPoolOffset;
    uint32_t shaderInfoOffset;
    uint32_t numSubroutines;
    uint32_t subroutineOffset;
    uint32_t numSubroutineUniforms;
    uint32_t subroutineUniformOffset;
    uint32_t subroutineCompatibleIndexPoolSize;
    uint32_t subroutineCompatibleIndexPoolOffset;
    uint8_t reserved[4];
} GLSLCprogramReflectionHeader;

typedef struct GLSLCgpuCodeHeader_rec {
    GLSLCsectionHeaderCommon common;
    NVNshaderStage stage;
    uint32_t controlOffset;
    uint32_t dataOffset;
    uint32_t dataSize;
    uint32_t controlSize;
    uint32_t scratchMemBytesPerWarp;
    uint32_t scratchMemBytesRecommended;
    uint32_t asmDumpSectionIdx;
    uint32_t perfStatsSectionNdx;
    uint32_t subroutineLinkageMapOffset;
    uint32_t subroutineLinkageMapSize;
    uint8_t reserved[56];
} GLSLCgpuCodeHeader;

typedef struct GLSLCasmDumpHeader_rec {
    GLSLCsectionHeaderCommon common;
    NVNshaderStage stage;
    uint8_t reserved[60];
} GLSLCasmDumpHeader;

typedef struct GLSLCgenericHeader_rec {
    GLSLCsectionHeaderCommon common;
    uint8_t reserved[32];
} GLSLCgenericHeader;

typedef struct GLSLCperfStatsHeader_rec {
    GLSLCsectionHeaderCommon common;
    uint8_t reserved[32];
} GLSLCperfStatsHeader;

typedef struct GLSLCdebugDataHash_rec {
    uint32_t buildId[4];
    uint32_t debugHashLo32;
    uint32_t debugHashHi32;
} GLSLCdebugDataHash;

typedef struct GLSLCdebugInfoHeader_rec {
    GLSLCsectionHeaderCommon common;
    GLSLCdebugDataHash debugDataHash;
    uint8_t reserved[40];
} GLSLCdebugInfoHeader;

typedef union GLSLCsectionHeaderUnion_rec {
    GLSLCgenericHeader genericHeader;
    GLSLCgpuCodeHeader gpuCodeHeader;
    GLSLCasmDumpHeader asmDumpHeader;
    GLSLCperfStatsHeader perfStatsHeader;
    GLSLCprogramReflectionHeader programReflectionHeader;
    GLSLCdebugInfoHeader debugInfoHeader;
} GLSLCsectionHeaderUnion;

typedef struct GLSLCversion_rec {
    uint32_t apiMajor;
    uint32_t apiMinor;
    uint32_t gpuCodeVersionMajor;
    uint32_t gpuCodeVersionMinor;
    uint32_t package;
    uint8_t reserved[32];
} GLSLCversion;

typedef struct GLSLCoutput_rec {
    uint32_t magic;
    uint32_t reservedBits;
    GLSLCoptionFlags optionFlags;
    GLSLCversion versionInfo;
    uint32_t size;
    uint32_t dataOffset;
    uint32_t numSections;
    uint8_t reserved[64];
    GLSLCsectionHeaderUnion headers[1];
} GLSLCoutput;

/*
GLSLCresults_rec;
typedef GLSLCresults_rec GLSLCresults;
GLSLCinitializationStatus_enum;
typedef GLSLCinitializationStatus_enum GLSLCinitializationStatus;
GLSLCcompileObject;
typedef GLSLCcompileObject GLSLCcompileObject;
typedef void* (*GLSLCallocateFunction)(size_t, size_t, void*);
typedef void (*GLSLCfreeFunction)(void*, void*);
typedef void* (*GLSLCreallocateFunction)(void*, size_t, void*);
*/