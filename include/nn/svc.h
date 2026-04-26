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

enum class BreakReason {};  // TODO

enum class InfoType {
    CoreMask,
    PriorityMask,
    AliasRegionAddress,
    AliasRegionSize,
    HeapRegionAddress,
    HeapRegionSize,
    TotalMemorySize,
    UsedMemorySize,
    DebuggerAttached,
    ResourceLimit,
    IdleTickCount,
    RandomEntropy,
    AslrRegionAddress,
    AslrRegionSize,
    StackRegionAddress,
    StackRegionSize,
    SystemResourceSizeTotal,
    SystemResourceSizeUsed,
    ProgramId,
    InitialProcessIdRange_LowerBound,
    InitialProcessIdRange_UpperBound,
    UserExceptionContextAddress,
    TotalNonSystemMemorySize,
    UsedNonSystemMemorySize,
    IsApplication,
    FreeThreadCount,
    ThreadTickCount,
    IsSvcPermitted,
    IoRegionHint,
    AliasRegionExtraSize,
    RemoteRegionAddress,
    RemoteRegionSize,
    RemoteMemoryUsage,
    RemoteMemoryUsagePeak,
    ProcessPageSize,
    TransferMemoryHint
};

enum class LimitableResource {
    PhysicalMemoryMax,
    ThreadCountMax,
    EventCountMax,
    TransferMemoryCountMax,
    SessionCountMax
};

enum class ThreadActivity { None, Runnable };

enum class ProcessActivity { None, Runnable };

struct ThreadContext {
    s8 padding[0xe8];
};  // TODO

enum class DumpInfoType {};  // TODO

struct InterruptEvent {};  // TODO

enum class InterruptEventType {};  // TODO

enum class DeviceName {
    AFI,
    AVPC,
    DC,
    DCB,
    HC,
    HDA,
    ISP2,
    MSENCNVENC,
    NV,
    NV2,
    PPCS,
    SATA,
    VI,
    VIC,
    XUSB_HOST,
    XUSB_DEV,
    TSEC,
    PPCS1,
    DC1,
    SDMMC1A,
    SDMMC2A,
    SDMMC3A,
    SDMMC4A,
    ISP2B,
    GPU,
    GPUB,
    PPCS2,
    NVDEC,
    APE,
    SE,
    NVJPG,
    HC1,
    SE1,
    AXIAP,
    ETR,
    TSECB,
    TSEC1,
    TSECB1,
    NVDEC1
};

enum class HardwareBreakPointRegisterName {};  // TODO

enum class DebugThreadParam {
    DynamicPriority,
    SchedulingStatus,
    PreferredCpuCore,
    CurrentCpuCore,
    AffinityMask
};

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

struct LastThreadContext {
    u64 _0;
    u64 _8;
    u64 _10;
    u64 _18;
};  // TODO

struct PhysicalMemoryInfo {
    u64 _0;
    u64 _8;
    u64 _10;
};  // TODO

struct DebugEventInfo {
    s8 padding[0x40];
};  // TODO

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
Result CancelSynchronization(Handle handle);
Result ArbitrateLock(Handle handle, uintptr address, u32 tag);
Result ArbitrateUnlock(uintptr address);
Result WaitProcessWideKeyAtomic(uintptr keyAddress, uintptr tagAddress, u32 tag, s64 timeoutNs);
Result SignalProcessWideKey(uintptr address, s32 value);
s64 GetSystemTick();
Result ConnectToNamedPort(Handle* outHandle, const char* portName);
Result SendSyncRequestLight(Handle handle);
Result SendSyncRequest(Handle handle);
Result SendSyncRequestWithUserBuffer(uintptr address, size size, Handle handle);
Result SendAsyncRequestWithUserBuffer(Handle* outHandle, uintptr address, size size, Handle handle);
Result GetProcessId(u64* outProcessId, Handle handle);
Result GetThreadId(u64* outThreadId, Handle handle);
Result Break(BreakReason reason, u64, u64);
Result OutputDebugString(const char* message, size size);
void ReturnFromException(Result result);
Result GetInfo(u64* outInfo, InfoType infotype, Handle handle, u64 infoSubType);
void FlushEntireDataCache();
Result FlushDataCache(uintptr address, size size);
Result MapPhysicalMemory(uintptr address, size size);
Result UnmapPhysicalMemory(uintptr address, size size);
Result GetLastThreadInfo(svc::lp::LastThreadContext* outContext, uintptr* outTlsAddress,
                         u32* outFlags);
Result GetResourceLimitLimitValue(s64* outLimit, Handle handle, LimitableResource resource);
Result GetResourceLimitCurrentValue(s64* outCurrentValue, Handle handle,
                                    LimitableResource resource);
Result SetThreadActivity(Handle handle, ThreadActivity activity);
Result GetThreadContext3(ThreadContext* outThreadContext, Handle handle);
Result DumpInfo(DumpInfoType dumpInfoType, u64);
Result ReadWriteRegister(u32* outValue, uintptr registerAddress, u32 rwMask, u32 value);
Result SetProcessActivity(Handle handle, ProcessActivity activity);
Result CreateSharedMemory(Handle* outHandle, size size, MemoryPermission localMemoryPermission,
                          MemoryPermission remoteMemoryPermission);
Result MapTransferMemory(Handle handle, uintptr address, size size,
                         MemoryPermission memoryPermission);
Result UnmapTransferMemory(Handle handle, uintptr address, size size);
Result CreateInterruptEvent(Handle* outHandle, InterruptEvent interruptEvent,
                            InterruptEventType eventType);
Result QueryPhysicalAddress(svc::lp::PhysicalMemoryInfo* outPhysicalMemoryInfo,
                            uintptr virtualAddress);
Result QueryIoMapping(uintptr* outVirtualAddress, uintptr ioAddress, size size);
Result CreateDeviceAddressSpace(Handle* outHandle, uintptr startAddress, uintptr endAddress);
Result AttachDeviceAddressSpace(DeviceName deviceName, Handle handle);
Result DetachDeviceAddressSpace(DeviceName deviceName, Handle handle);
Result MapDeviceAddressSpaceByForce(Handle deviceAddressSpaceHandle, Handle processHandle,
                                    uintptr processAddress, size size, uintptr deviceAddress,
                                    MemoryPermission memoryPermission);
Result MapDeviceAddressSpaceAligned(Handle deviceAddressSpaceHandle, Handle processHandle,
                                    uintptr processAddress, size size, uintptr deviceAddress,
                                    MemoryPermission memoryPermission);
Result MapDeviceAddressSpace(size* outMappedSize, Handle deviceAddressSpaceHandle,
                             Handle processHandle, uintptr processAddress, size size,
                             uintptr deviceAddress, MemoryPermission memoryPermission);
Result UnmapDeviceAddressSpace(Handle deviceAddressSpaceHandle, Handle processHandle,
                               uintptr processAddress, size size, uintptr deviceAddress,
                               MemoryPermission memoryPermission);
Result InvalidateProcessDataCache(Handle handle, uintptr address, size size);
Result StoreProcessDataCache(Handle handle, uintptr address, size size);
Result FlushProcessDataCache(Handle handle, uintptr address, size size);
Result DebugActiveProcess(Handle handle, u64 processId);
Result BreakDebugProcess(Handle handle);
Result TerminateDebugProcess(Handle handle);
Result GetDebugEvent(svc::lp::DebugEventInfo* outDebugEventInfo, Handle handle);
Result ContinueDebugEvent(Handle handle, u32 flags, const u64* threadIds, u32 threadIdCount);
Result GetProcessList(u32* outProcessIdCount, u64* outProcessIds, u32 maxProcessIdCount);
Result GetThreadList(u32* outThreadIdCount, u64* outThreadIds, u32 maxThreadIdCount, Handle handle);
Result GetDebugThreadContext(ThreadContext* outThreadContext, Handle handle, u64 threadId,
                             u32 flags);
Result SetDebugThreadContext(Handle handle, u64 threadId, const ThreadContext& threadContext,
                             u32 flags);
Result QueryDebugProcessMemory(svc::lp::MemoryInfo* outMemoryInfo, PageInfo* outPageInfo,
                               Handle handle, uintptr address);
Result ReadDebugProcessMemory(uintptr bufferAddress, Handle handle, uintptr srcAddress, size size);
Result WriteDebugProcessMemory(Handle handle, uintptr bufferAddress, uintptr dstAddress, size size);
Result SetHardwareBreakPoint(HardwareBreakPointRegisterName registerName, u64 flags, u64 value);
Result GetDebugThreadParam(u64*, u32*, Handle handle, u64 threadId, DebugThreadParam param);
Result CreateSession(Handle* outServerHandle, Handle* outClientHandle, bool isLight, u64);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);
Result ArbitrateUnlock(uintptr address);

#if NN_SDK_VER >= NN_MAKE_VER(1, 0, 0)
}  // namespace lp
#endif

}  // namespace aarch

}  // namespace nn::svc
