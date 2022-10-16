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
	enum Flag : u8 {
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

// todo: figure out fields
class BlendTargetStateInfo {
public:
	void SetDefault() {
		field_1 = BlendFactor::One;
		field_4 = BlendFactor::One;
		flag = (Flag)(flag & 0xFE);
		field_2 = BlendFactor::Zero;
		field_3 = BlendFunction::Add;
		field_5 = BlendFactor::Zero;
		field_6 = BlendFunction::Add;
		channelMask = (ChannelMask)(ChannelMask_1 | ChannelMask_2 | ChannelMask_4 | ChannelMask_8);
	}

	enum Flag : u8 {
		Flag_EnableBlend = 1
	};

	Flag flag;
	BlendFactor field_1;
	BlendFactor field_2;
	BlendFunction field_3;
	BlendFactor field_4;
	BlendFactor field_5;
	BlendFunction field_6;

	// rgba?
	enum ChannelMask : u8 {
		ChannelMask_1 = 1,
		ChannelMask_2 = 2,
		ChannelMask_4 = 4,
		ChannelMask_8 = 8
	};

	ChannelMask channelMask;
	char _8[24];
};

class BlendStateInfo {
public:
	void SetDefault() {
		logicOperation = LogicOperation::Noop;
		blendColor[0] = 0.0f;
		blendColor[1] = 0.0f;
		blendColor[2] = 0.0f;
		blendColor[3] = 1.0f;

		flag = (Flag)(flag & 0xFFF0);
		count = 0;
		targetInfoArray = nullptr;
	}

	enum Flag : u8 {
		Flag_1 = 1,
		Flag_2 = 2,
		Flag_4 = 4,
		Flag_8 = 8,
	};

	u8 count;
	LogicOperation logicOperation;
	Flag flag;
	float blendColor[4]; // SetDefault = { 0.0f, 0.0f, 0.0f, 1.0f }
	BlendTargetStateInfo* targetInfoArray;
};

}