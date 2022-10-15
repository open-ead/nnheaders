/**
 * @file bufferimpl.h
 * @brief Buffer implementation for GFX.
 */

#pragma once

#include <nn/types.h>
#include <nn/gfx/api.h>
#include <nn/gfx/buffer.h>
#include <nn/gfx/detail/deviceimpl.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/detail/nvn.h>
#include <nn/gfx/detail/pool.h>
#include <nn/gfx/detail/stateimpl.h>
#include <nvn/nvn_api.h>
#include <nvn/nvn_types.h>

namespace nn::gfx {

class BufferInfo;

// todo: move these somewhere else?
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
	using Pipeline = PipelineImpl<NvnApi>;
	using Raster = RasterizerStateImpl<NvnApi>;
	
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

		// conditionally sets flag_2 and clears flag_1
		flag = (Flag)(((device->deviceFeatures & Device::Feature_SupportsConservativeRaster) ? (flag | ConservativeRaster) : (flag & 0xFC)) & 0xFE);

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

	void Draw(PrimitiveTopology top, int a, int b) {
		nvnCommandBufferDrawArrays(pnCmdBuf, Nvn::GetDrawPrimitive(top), a, b);
	}

	void Draw(PrimitiveTopology top, int a, int b, int c, int d) {
		nvnCommandBufferDrawArraysInstanced(pnCmdBuf, Nvn::GetDrawPrimitive(top), a, b, c, d);
	}

	void DrawIndexed(PrimitiveTopology top, IndexFormat index, const GpuAddress& addr, int a, int b) {
		nvnCommandBufferDrawElementsBaseVertex(pnCmdBuf, Nvn::GetDrawPrimitive(top), Nvn::GetIndexFormat(index), a, Nvn::GetBufferAddress(addr), b);
	}

	void DrawIndexed(PrimitiveTopology top, IndexFormat index, const GpuAddress& addr, int a, int b, int c, int d) {
		nvnCommandBufferDrawElementsInstanced(pnCmdBuf, Nvn::GetDrawPrimitive(top), Nvn::GetIndexFormat(index), a, Nvn::GetBufferAddress(addr), b, c, d);
	}

	void DispatchIndirect(const GpuAddress& addr) {
		nvnCommandBufferDispatchComputeIndirect(pnCmdBuf, Nvn::GetBufferAddress(addr));
	}

	void DrawIndirect(PrimitiveTopology top, const GpuAddress& addr) {
		nvnCommandBufferDrawArraysIndirect(pnCmdBuf, Nvn::GetDrawPrimitive(top), Nvn::GetBufferAddress(addr));
	}

	void DrawIndexedIndirect(PrimitiveTopology top, IndexFormat index, const GpuAddress& addr1, const GpuAddress& addr2) {
		nvnCommandBufferDrawElementsIndirect(pnCmdBuf, Nvn::GetDrawPrimitive(top), Nvn::GetIndexFormat(index), Nvn::GetBufferAddress(addr1), Nvn::GetBufferAddress(addr2));
	}

	// todo: figure out the PipelineImpl struct
	void SetPipeline(const Pipeline*);

	// todo: rewrite
	void SetRasterizerState(const Raster* rast) {
		nvnCommandBufferBindPolygonState(pnCmdBuf, &rast->nPolygonState);
		nvnCommandBufferSetPolygonOffsetClamp(pnCmdBuf, rast->nFactor, rast->nUnits, rast->nClamp);
		nvnCommandBufferBindMultisampleState(pnCmdBuf, &rast->nMultisampleState);

		// todo: switch to bitflag
		if ((rast->flag & Raster::Flag_Multisample) == Raster::Flag_Multisample) {
			nvnCommandBufferSetSampleMask(pnCmdBuf, rast->nSampleMask);
		}

		nvnCommandBufferSetDepthClamp(pnCmdBuf, (rast->flag & Raster::Flag_Depth) != Raster::Flag_Depth);
		nvnCommandBufferSetRasterizerDiscard(pnCmdBuf, (rast->flag & Raster::Flag_Raster) != Raster::Flag_Raster);
		
		if ((flag & ConservativeRaster) == ConservativeRaster) {
			nvnCommandBufferSetConservativeRasterEnable(pnCmdBuf, rast->flag & Raster::Flag_ConservativeRaster);
		}
	}

	u8 initialized; // 2 is set in Begin (maybe some sort of staging enum?)

	enum Flag {
		Flag_1 = 1,
		ConservativeRaster = 2
	};
	Flag flag;

	Device* pDevice;
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
