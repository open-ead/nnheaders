/**
 * @file hid.h
 * @brief Functions that help process gamepad inputs.
 */

#pragma once

#include <nn/types.h>
#include <nn/util.h>

namespace nn {
namespace hid {
struct NpadHandheldState;
struct NpadStyleTag;

struct ControllerSupportArg {
    u8 mMinPlayerCount;
    u8 mMaxPlayerCount;
    u8 mTakeOverConnection;
    bool mLeftJustify;
    bool mPermitJoyconDual;
    bool mSingleMode;
    bool mUseColors;
    sead::Color4u8 mColors[4];
    u8 mUsingControllerNames;
    char mControllerNames[4][0x81];
};

struct ControllerSupportResultInfo {
    int mPlayerCount;
    int mSelectedId;
};

struct NpadBaseState {
    long mSamplingNumber;
    long mButtons;
    long mAnalogStickL;
    long mAnalogStickR;
    int mAttributes;
    int RESERVED;
};

struct NpadHandheldState : NpadBaseState {};
struct NpadJoyDualState : NpadBaseState {};
struct NpadFullKeyState : NpadBaseState {};
struct NpadJoyLeftState : NpadBaseState {};
struct NpadJoyRightState : NpadBaseState {};

void InitializeNpad();
void SetSupportedNpadIdType(u32 const*, u64);
void SetSupportedNpadStyleSet(nn::util::BitFlagSet<32, nn::hid::NpadStyleTag>);
void GetNpadStyleSet(u32 const&);
static int ShowControllerSupport(nn::hid::ControllerSupportResultInfo*,
                                 ControllerSupportArg const&);

void GetNpadStates(NpadHandheldState*, int, const u32&);
void GetNpadStates(NpadJoyDualState*, int, const u32&);
void GetNpadStates(NpadFullKeyState*, int, const u32&);
void GetNpadStates(NpadJoyLeftState*, int, const u32&);
void GetNpadStates(NpadJoyRightState*, int, const u32&);

}  // namespace hid
}  // namespace nn
