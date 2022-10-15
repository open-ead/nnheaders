#pragma once

#include <nn/gfx/api.h>
#include <nn/gfx/detail/fwd.h>
#include <nn/gfx/detail/nvn.h>
#include <nvn/nvn_api.h>
#include <nvn/nvn_types.h>

namespace nn::gfx {

class RasterizerStateInfo {
public:
	void SetDefault() {
		fillMode = FillMode::Fill;
		frontFace = FrontFace::CCW;
		cullMode = CullMode::Back;

		field_3 = 3;
		alphaCoverage &= 0xFFFE;
		flag = (Flag)((flag & 0xFFF0) | Flag_DepthDisable);

		samples = 1;
		factor = 0.0f;
		units = 0;
		clamp = 0.0f;

		conservativeRaster = false;
		sampleMask = -1;
	}

	FillMode fillMode;
	FrontFace frontFace;
	CullMode cullMode;
	u8 field_3;

	// todo: convert to enum class with bitflag class
	enum Flag {
		Flag_RasterDisable = 1,
		Flag_MultisampleEnable = 2,
		Flag_DepthDisable = 4,
		Flag_8 = 8,
	};

	Flag flag;
	bool conservativeRaster;

	float factor;
	int units;
	float clamp;
  	int _14;
	
	char samples;
	char _19;
	u16 alphaCoverage; // enum flag?
	int sampleMask;
};

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
	enum Flag {
		Flag_Multisample = 1,
		Flag_Depth = 2,
		Flag_Raster = 4,
		Flag_ConservativeRaster = 8,
	};

	Flag flag;
	u16 _2E;
};

template <typename Api>
class BlendStateImpl;

template <typename Api>
class DepthStencilStateImpl;

template <typename Api>
class VertexStateImpl;

template <typename Api>
class TessellationStateImpl;


} // namespace detail
} // namespace nn::gfx