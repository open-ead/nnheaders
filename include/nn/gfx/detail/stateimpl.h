#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/detail/nvn.h>
#include <nn/gfx/detail/stateinfo.h>
#include <nvn/nvn_api.h>
#include <nvn/nvn_types.h>

namespace nn::gfx {
namespace detail {

template <>
class RasterizerStateImpl<NvnApi> {
public:
    using Device = DeviceImpl<NvnApi>;
	using Info = RasterizerStateInfo;

	// todo: memset?
	RasterizerStateImpl() = default;
	
	~RasterizerStateImpl() {

	}

	void Initialize(Device* device, const Info& info) {
		NVNpolygonState* pnPolygonState = &nPolygonState;
		NVNmultisampleState* pnMultisampleState = &nMultisampleState;

		nvnPolygonStateSetDefaults(pnPolygonState);
		nvnPolygonStateSetCullFace(pnPolygonState, Nvn::GetCullMode(info.cullMode));
		nvnPolygonStateSetFrontFace(pnPolygonState, Nvn::GetFrontFace(info.frontFace));
		nvnPolygonStateSetPolygonMode(pnPolygonState, Nvn::GetFillMode(info.fillMode));

		nUnits = info.units;
		nClamp = info.clamp;
		nFactor = info.factor;

		bool polyEnables = (info.factor == 0.0f && info.clamp == 0.0f && info.units == 0);
		nvnPolygonStateSetPolygonOffsetEnables(pnPolygonState, (polyEnables) ? (NVN_POLYGON_OFFSET_ENABLE_POINT | NVN_POLYGON_OFFSET_ENABLE_LINE | NVN_POLYGON_OFFSET_ENABLE_FILL) : NVN_POLYGON_OFFSET_ENABLE_NONE);

		nSampleMask = info.sampleMask;
		nvnMultisampleStateSetDefaults(pnMultisampleState);
		nvnMultisampleStateSetMultisampleEnable(pnMultisampleState, info.flag & Info::Flag_MultisampleEnable);
		nvnMultisampleStateSetSamples(pnMultisampleState, info.samples);
		nvnMultisampleStateSetAlphaToCoverageEnable(pnMultisampleState, info.alphaCoverage & 1);

		flag = (Flag)((info.flag & Info::Flag_MultisampleEnable) ? (flag | Flag_Multisample) : (flag & 0xFE));
		flag = (Flag)((info.flag & Info::Flag_DepthDisable) ? (flag & 0xFD) : (flag | Flag_Depth));
		flag = (Flag)((info.flag & Info::Flag_RasterDisable) ? (flag & 0xFB) : (flag | Flag_Raster));
		flag = (Flag)((info.conservativeRaster) ? (flag | Flag_ConservativeRaster) : (flag & 0xF7));

		initialized = 1;
	}

	void Finalize(Device*) {
		initialized = false;
	}

	NVNpolygonState nPolygonState;
	NVNmultisampleState nMultisampleState;
	int nSampleMask;
	float nUnits;
	float nClamp;
	float nFactor;
	bool initialized;

	// todo: switch to enum class with a bitflag class
	enum Flag : u8 {
		Flag_Multisample = 1,
		Flag_Depth = 2,
		Flag_Raster = 4,
		Flag_ConservativeRaster = 8,
	};

	Flag flag;
	u16 _2E;
};

template <>
class BlendStateImpl<NvnApi> {
public:
	using Device = DeviceImpl<NvnApi>;

    enum State : u8 {
        State_NotInitialized = 0,
        State_Initialized = 1,
    };

	static size_t GetRequiredMemorySize(const BlendStateInfo& info) {
		return 8 * info.count;
	}
	
	// todo: memset
	BlendStateImpl() = default;
	
	~BlendStateImpl() {

	}

	void SetMemory(void* p, size_t s) {
		memory = p;
		memorySize = s;
	}

	void* GetMemory() {
		return memory;
	}

	void* GetMemory() const { 
		return memory;
	}
	
	void Initialize(Device* device, const BlendStateInfo& info) {
		auto pnColorState = reinterpret_cast<NVNcolorState*>(nColorState);
		auto pnChannelMaskState = reinterpret_cast<NVNchannelMaskState*>(nChannelMaskState);
		auto pnBlendState = reinterpret_cast<NVNblendState*>(memory);
		auto pTargetInfo = info.targetInfoArray;

		blendColor[0] = info.blendColor[0];
		blendColor[1] = info.blendColor[1];
		blendColor[2] = info.blendColor[2];
		blendColor[3] = info.blendColor[3];

		nvnColorStateSetDefaults(pnColorState);
		nvnChannelMaskStateSetDefaults(pnChannelMaskState);

		count = info.count;

		for (int i = 0; i < info.count; ++i) {
			nvnColorStateSetBlendEnable(pnColorState, i, pTargetInfo[i].flag & BlendTargetStateInfo::Flag_EnableBlend);
			nvnBlendStateSetDefaults(&pnBlendState[i]);
			nvnBlendStateSetBlendTarget(&pnBlendState[i], i);

			int infoOffset = (info.flag & BlendStateInfo::Flag_4) ? i : 0;
			
			nvnBlendStateSetBlendFunc(&pnBlendState[i],
				Nvn::GetBlendFunction(pTargetInfo[infoOffset].field_1),
				Nvn::GetBlendFunction(pTargetInfo[infoOffset].field_2),
				Nvn::GetBlendFunction(pTargetInfo[infoOffset].field_4),
				Nvn::GetBlendFunction(pTargetInfo[infoOffset].field_5));

			
			nvnBlendStateSetBlendEquation(&pnBlendState[i], 
				Nvn::GetBlendEquation(pTargetInfo[infoOffset].field_3),
				Nvn::GetBlendEquation(pTargetInfo[infoOffset].field_6));

			nvnChannelMaskStateSetChannelMask(pnChannelMaskState, i,
				pTargetInfo[infoOffset].channelMask & BlendTargetStateInfo::ChannelMask_1,
				pTargetInfo[infoOffset].channelMask & BlendTargetStateInfo::ChannelMask_2,
				pTargetInfo[infoOffset].channelMask & BlendTargetStateInfo::ChannelMask_4,
				pTargetInfo[infoOffset].channelMask & BlendTargetStateInfo::ChannelMask_8);
		}

		if (info.flag & BlendStateInfo::Flag_8) {
			nvnColorStateSetLogicOp(pnColorState, Nvn::GetLogicOperation(info.logicOperation));
		}

		state = State_Initialized;
	}

	void Finalize(Device* device) {
		state = State_NotInitialized;
	}

	void* memory;
	int nColorState; // ???
	int nChannelMaskState; // ???
	float blendColor[4];
	int memorySize;
	State state;
	u8 count;
	char _26[2];
};

template <>
class DepthStencilStateImpl<NvnApi> {
	
};

template <>
class VertexStateImpl<NvnApi> {
	
};

template <>
class TessellationStateImpl<NvnApi> {
	
};


} // namespace detail
} // namespace nn::gfx