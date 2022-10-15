/**
 * @file bufferimpl.h
 * @brief Buffer implementation for GFX.
 */

#pragma once

#include <nn/types.h>
#include <nn/gfx/api.h>
#include <nn/gfx/buffer.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/detail/deviceimpl.h>
#include <nn/gfx/detail/pool.h>
#include <nvn/nvn_api.h>
#include <nvn/nvn_types.h>

namespace nn::gfx {

class GpuAddress;
class BufferInfo;

// todo: move this somewhere else?
struct OutOfMemoryEventArg {
	size_t size;
};

namespace detail {

template <>
class BufferImpl<NvnApi> {
public:
    using Device = DeviceImpl<NvnApi>;
    using MemoryPool = MemoryPoolImpl<NvnApi>;

    BufferImpl();
    ~BufferImpl();

    void Initialize(Device*, const BufferInfo&, MemoryPool*, s64, u64);
    void Finalize(Device*);
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(s64, u64) const;
    void InvalidateMappedRange(s64, u64) const;
    void GetGpuAddress(GpuAddress*) const;

    void* mBuff;  // _0
};

template <>
class CommandBufferImpl<NvnApi> {
public:
    using Device = DeviceImpl<NvnApi>;
    using MemoryPool = MemoryPoolImpl<NvnApi>;
	
	using OutOfMemCallback = void (*)(TCommandBuffer<NvnApi>*, const OutOfMemoryEventArg&); 

	static size_t GetCommandMemoryAlignment(Device* device) {
		int align;
		nvnDeviceGetInteger(device->pnDevice, NVN_DEVICE_INFO_COMMAND_BUFFER_COMMAND_ALIGNMENT, &align);
		return align;
	}

	static size_t GetControlMemoryAlignment(Device* device) {
		int align;
		nvnDeviceGetInteger(device->pnDevice, NVN_DEVICE_INFO_COMMAND_BUFFER_CONTROL_ALIGNMENT, &align);
		return align;
	}

    CommandBufferImpl() = default; // todo: figure out memset
    
	~CommandBufferImpl() {

	}

	void Initialize(Device* device, const CommandBufferInfo& unused) {
		pDevice = device;
		pnCmdBuf = &nCmdBuf;

		nvnCommandBufferInitialize(pnCmdBuf, device->pnDevice);
		
		outOfCmdMemoryCallback = 0;
		outOfCtrlMemoryCallback = 0;
		field_E0 = 0;

		// todo: figure out what to do with this
		const auto callback = [](NVNcommandBuffer* buffer, NVNcommandBufferMemoryEvent event, size_t size, void* userData) {
			auto pThis = static_cast<CommandBufferImpl*>(userData);
			auto cmdBuf = reinterpret_cast<TCommandBuffer<NvnApi>*>(pThis); 
			OutOfMemoryEventArg arg {size};
			
			switch (event) {
				case NVN_COMMAND_BUFFER_MEMORY_EVENT_OUT_OF_CONTROL_MEMORY:
					pThis->outOfCtrlMemoryCallback(cmdBuf, arg);
					break;

				case NVN_COMMAND_BUFFER_MEMORY_EVENT_OUT_OF_COMMAND_MEMORY:
					pThis->outOfCmdMemoryCallback(cmdBuf, arg);
					break;

				default:
					//nn::detail::UnexpectedDefaultImpl("", "", 0, userData);
					pThis->Finalize();
					break;
			}
		};

		nvnCommandBufferSetMemoryCallback(pnCmdBuf, callback);
		nvnCommandBufferSetMemoryCallbackData(pnCmdBuf, this);

		cmdHandle = 0;

		// enum flag shennanigans?
		field_1 = ((device->deviceFeatures & 8) ? (field_1 | 2) : (field_1 & 0xFC)) & 0xFE;

		initialized = 1;
	}

	void Finalize() {
		if (cmdHandle != 0) {
			nvnDeviceFinalizeCommandHandle(pDevice->pnDevice, cmdHandle);
			cmdHandle = 0;
		}

		nvnCommandBufferFinalize(pnCmdBuf);
		initialized = 0;
	}

	void AddCommandMemory(MemoryPool* pool, ptrdiff_t ptr, size_t size) {
		nvnCommandBufferAddCommandMemory(pnCmdBuf, pool->pnPool, ptr, size);
	}

	void AddControlMemory(void* ptr, size_t size) {
		nvnCommandBufferAddControlMemory(pnCmdBuf, ptr, size);
	}

	void SetOutOfCommandMemoryEventCallback(OutOfMemCallback callback) {
		outOfCmdMemoryCallback = callback;
	}

	void SetOutOfControlMemoryEventCallback(OutOfMemCallback callback) {
		outOfCtrlMemoryCallback = callback;
	}

    void Reset() {

	}

    void Begin() {
		if (cmdHandle != 0) {
			nvnDeviceFinalizeCommandHandle(pDevice->pnDevice, cmdHandle);
			cmdHandle = 0;
		}

		nvnCommandBufferBeginRecording(pnCmdBuf);
		initialized = 2;
	}

    void End() {
		cmdHandle = nvnCommandBufferEndRecording(pnCmdBuf);
		initialized = 1;
	}

    void Dispatch(int a, int b, int c) {
		nvnCommandBufferDispatchCompute(pnCmdBuf, a, b, c);
	}

	u8 initialized; // 2 is set in Begin (maybe some sort of staging enum?)
	u8 field_1; // 2 bit enum?
	DeviceImpl<NvnApi>* pDevice;
	char _10[8];
	NVNcommandBuffer nCmdBuf;
	NVNcommandBuffer* pnCmdBuf;
	NVNcommandHandle cmdHandle;
	char _C8[8];
	OutOfMemCallback outOfCmdMemoryCallback;
	OutOfMemCallback outOfCtrlMemoryCallback;
	void* field_E0;
};

}  // namespace detail

}  // namespace nn::gfx
