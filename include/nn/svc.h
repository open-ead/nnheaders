#pragma once

#include <nn/types.h>
#include <nn/util.h>

namespace nn::svc {
#define __aarch64__  // TODO: Remove

#ifdef __aarch64__
#define aarch aarch64
#define lp lp64
#define svc_instruction "svc"
#else
#ifdef __arm__
#define aarch aarch32
#define lp ilp32
#define svc_instruction "swi"
#else
#error Unsupported Architecture
#endif
#endif

inline __attribute__((always_inline)) void svc(u8 i) {
    asm(svc_instruction " %[i]" ::[i] "i"(i));
}

struct Handle {
    u32 handle;

    Handle(u32 h) { handle = h; }

    Handle() : Handle(0) {}

    operator u32() const { return handle; }
};

enum class MemoryPermission { Read, Write, Execute, DontCare = 28 };

enum class MemoryType {
    Free,
    Io,
    Static,
    Code,
    CodeData,
    Normal,
    Shared,
    Alias,
    AliasCode,
    AliasCodeData,
    Ipc,
    Stack,
    ThreadLocal,
    Transferred,
    SharedTransferred,
    SharedCode,
    Inaccessible,
    NonSecureIpc,
    NonDeviceIpc,
    Kernel,
    GeneratedCode,
    CodeOut,
    Coverage,
    Insecure
};

enum class MemoryAttribute {
    Locked,
    IpcLocked,
    DeviceShared,
    Uncached,
    PermissionLocked,
    GpuSharable,
    GpuShared
};

struct PageInfo {};  // TODO

namespace lp {
struct MemoryInfo {
    uintptr baseAddress;
    size size;
    MemoryType memoryType;
    MemoryAttribute memoryAttribute;
    MemoryPermission memoryPermission;
    s32 ipcRefCount;
    s32 deviceRefCount;
};
}  // namespace lp

namespace aarch {

#if NN_SDK_VER >= NN_MAKE_VER(1, 0, 0)  // TODO: find when lp namespace was introduced
namespace lp {
#endif

Result SetHeapSize(uintptr* outHeapAddress, size heapSize);
Result SetMemoryPermission(uintptr address, size regionSize, MemoryPermission memoryPermission);
Result SetMemoryAttribute(uintptr address, size size, u32 mask, u32 attr);
Result MapMemory(uintptr dstAddress, uintptr srcAddress, size size);
Result UnmapMemory(uintptr dstAddress, uintptr srcAddress, size size);
Result QueryMemory(svc::lp::MemoryInfo* outMemoryInfo, PageInfo* outPageInfo, uintptr address);
void ExitProcess();
Result CreateThread(Handle* outHandle, uintptr entryPointAddress, uintptr threadContextAddress,
                    uintptr stackAddress, s32 priority, s32 coreId);
Result StartThread(Handle Handle);
void ExitThread();
void SleepThread(s64 ns);
Result GetThreadPriority(s32* outPriority, Handle handle);
Result SetThreadPriority(Handle handle, s32 priority);
Result GetThreadCoreMask(s32* outCoreId, u64* outAffinityMask, Handle handle);
Result SetThreadCoreMask(Handle handle, s32 coreId, u64 affinityMask);
s32 GetCurrentProcessorNumber();
Result SignalEvent(Handle handle);
Result ClearEvent(Handle handle);
Result MapSharedMemory(Handle handle, uintptr address, size size,
                       MemoryPermission memoryPermission);
Result UnmapSharedMemory(Handle handle, uintptr address, size size);
Result CreateTransferMemory(Handle* outHandle, uintptr address, size size,
                            MemoryPermission memoryPermission);
Result CloseHandle(Handle handle);
Result ResetSignal(Handle handle);
Result WaitSynchronization(s32* outHandleIndex, const Handle* handles, u32 handleCount,
                           s64 timeoutNs);

#if NN_SDK_VER >= NN_MAKE_VER(1, 0, 0)
}  // namespace lp
#endif

}  // namespace aarch

}  // namespace nn::svc
