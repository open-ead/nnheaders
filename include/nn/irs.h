#pragma once

#include <nn/irs/MomentStatistic.h>
#include <nn/irs/Rect.h>
#include <nn/types.h>
#include <nn/util.h>

namespace nn::irsensor {
struct ClusteringProcessorConfig;
struct ClusteringProcessorState;

#if NN_SDK_VER <= NN_MAKE_VER(4, 0, 0)
struct DpdProcessorConfig;
struct DpdProcessorPointingState;
struct DpdProcessorState;
#endif

struct HandAnalysisConfig;
struct ImageTransferProcessorConfig;
struct ImageTransferProcessorState;
struct MomentProcessorConfig;
struct MomentProcessorState;
struct TeraPluginProcessorConfig;
struct TeraPluginProcessorState;

enum class IrCameraStatus : u32 {
    Available,
    Unsupported,
    Unconnected,
};

struct IrCameraHandle {
    u8 playerNumber;
    u8 deviceType;
    u8 reserved[2];
};

IrCameraHandle GetIrCameraHandle(const u32&);
void Initialize(const IrCameraHandle& handle);
void Finalize(const IrCameraHandle& handle);
IrCameraStatus GetIrCameraStatus(const IrCameraHandle& handle);
void StopImageProcessor(const IrCameraHandle& handle);

void GetMomentProcessorDefaultConfig(MomentProcessorConfig* outConfig);
void RunMomentProcessor(const IrCameraHandle& handle, const MomentProcessorConfig& config);
Result GetMomentProcessorState(MomentProcessorState* outState, const IrCameraHandle& handle);
Result GetMomentProcessorStates(MomentProcessorState* outStates, s32* outCount, s32,
                                const IrCameraHandle& handle);
MomentStatistic CalculateMomentRegionStatistic(const MomentProcessorState* state, const Rect&, s32,
                                               s32, s32, s32);

void GetClusteringProcessorDefaultConfig(ClusteringProcessorConfig* outConfig);
void RunClusteringProcessor(const IrCameraHandle& handle, const ClusteringProcessorConfig& config);
Result GetClusteringProcessorState(ClusteringProcessorState* outState,
                                   const IrCameraHandle& handle);
Result GetClusteringProcessorStates(ClusteringProcessorState* outStates, s32* outCount, s32,
                                    const IrCameraHandle& handle);

void GetImageTransferProcessorDefaultConfig(ImageTransferProcessorConfig* outConfig);
void RunImageTransferProcessor(const IrCameraHandle& handle,
                               const ImageTransferProcessorConfig& config, void*, u64);
Result GetImageTransferProcessorState(ImageTransferProcessorState* outState, void*, u64,
                                      const IrCameraHandle& handle);

Result RunHandAnalysis(const IrCameraHandle& handle, const HandAnalysisConfig& config);

Result RunTeraPluginProcessor(const IrCameraHandle& handle,
                              const TeraPluginProcessorConfig& config);

#if NN_SDK_VER <= NN_MAKE_VER(4, 0, 0)
void RunDpdProcessor(const IrCameraHandle& handle);
void GetDpdProcessorDefaultConfig(DpdProcessorConfig* outConfig);
void RunDpdProcessor(const IrCameraHandle& handle, const DpdProcessorConfig& config);
Result GetDpdProcessorStates(DpdProcessorPointingState* outStates, s32* outCount, s32,
                             const IrCameraHandle& handle);
Result GetDpdProcessorStates(DpdProcessorState* outStates, s32* outCount, s32,
                             const IrCameraHandle& handle);
#endif

Result GetTeraPluginProcessorStates(TeraPluginProcessorState* outStates, s32* outCount, s32, long,
                                    u32, s32, const IrCameraHandle& handle);

Rect MakeRect(s32 x, s32 y, s32 width, s32 height);
}  // namespace nn::irsensor
