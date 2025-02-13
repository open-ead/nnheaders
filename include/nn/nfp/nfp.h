#pragma once

#include <nn/types.h>

namespace nn::os {
class SystemEventType;
}  // namespace nn::os

namespace nn::nfp {

using DeviceHandle = u64;

enum State : u32;
enum DeviceState : u32;
enum ModelType : u32;
enum MountTarget : u32;

struct TagInfo;
struct CommonInfo;
struct ModelInfo;
struct RegisterInfo;

Result Initialize();
Result Finalize();

Result ListDevices(nn::nfp::DeviceHandle* out, s32* totalOut, s32 count);
Result StartDetection(const nn::nfp::DeviceHandle& handle);
Result StopDetection(const nn::nfp::DeviceHandle& handle);
Result Mount(const nn::nfp::DeviceHandle& handle, nn::nfp::ModelType modelType,
             nn::nfp::MountTarget mountTarget);
Result Unmount(const nn::nfp::DeviceHandle& handle);

Result OpenApplicationArea(const nn::nfp::DeviceHandle& handle, u32 appId);
Result GetApplicationArea(void* buf, u32* outSize, const nn::nfp::DeviceHandle& handle,
                          u64 bufSize);
Result SetApplicationArea(const nn::nfp::DeviceHandle& handle, const void* buf, u64 bufSize);
Result Flush(const nn::nfp::DeviceHandle& handle);
Result Restore(const nn::nfp::DeviceHandle& handle);

Result GetTagInfo(nn::nfp::TagInfo* out, const nn::nfp::DeviceHandle& handle);
Result GetRegisterInfo(nn::nfp::RegisterInfo* out, const nn::nfp::DeviceHandle& handle);
Result GetCommonInfo(nn::nfp::CommonInfo* out, const nn::nfp::DeviceHandle& handle);
Result GetModelInfo(nn::nfp::ModelInfo* out, const nn::nfp::DeviceHandle& handle);

Result AttachActivateEvent(nn::os::SystemEventType* outEvent, const nn::nfp::DeviceHandle& handle);
Result AttachDeactivateEvent(nn::os::SystemEventType* outEvent,
                             const nn::nfp::DeviceHandle& handle);
Result AttachAvailabilityChangeEvent(nn::os::SystemEventType* outEvent);

nn::nfp::State GetState();
nn::nfp::DeviceState GetDeviceState(const nn::nfp::DeviceHandle& handle);
Result GetNpadId(u32* out, const nn::nfp::DeviceHandle& handle);

}  // namespace nn::nfp
