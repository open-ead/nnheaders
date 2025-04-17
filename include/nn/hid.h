/**
 * @file hid.h
 * @brief Functions that help process gamepad inputs.
 */

#pragma once

#include <nn/os/os_MutexTypes.h>
#include <nn/types.h>
#include <nn/util.h>
#include <nn/util/MathTypes.h>
#include <nn/util/util_BitFlagSet.h>
#include <nn/xcd.h>

namespace nn {
namespace hid {

// todo: does something like this exist in nn?
typedef util::Color4u8Type Color4u8;

enum class NpadButton {
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    StickL = 4,
    StickR = 5,
    L = 6,
    R = 7,
    ZL = 8,
    ZR = 9,
    Plus = 10,
    Minus = 11,
    Left = 12,
    Up = 13,
    Right = 14,
    Down = 15,
    StickLLeft = 16,
    StickLUp = 17,
    StickLRight = 18,
    StickLDown = 19,
    StickRLeft = 20,
    StickRUp = 21,
    StickRRight = 22,
    StickRDown = 23,
    LeftSL = 24,
    LeftSR = 25,
    RightSL = 26,
    RightSR = 27,
    Palma = 28,
    Verification = 29,
    HandheldLeftB = 30,  // (Left B button on NES controllers in Handheld mode)
    LeftC = 31,          // [12.0.0+] (Left C button in N64 controller)
    UpC = 32,            // [12.0.0+] (Up C button in N64 controller)
    RightC = 33,         // [12.0.0+] (Right C button in N64 controller)
    DownC = 34,          // [12.0.0+] (Down C button in N64 controller)
};

enum class NpadAttribute {
    IsConnected = 0,
    IsWired = 1,
    IsLeftConnected = 2,
    IsLeftWired = 3,
    IsRightConnected = 4,
    IsRightWired = 5,
};

enum class NpadStyleTag {
    NpadStyleFullKey = 0,   // (Pro Controller)
    NpadStyleHandheld = 1,  // (Joy-Con controller in handheld mode)
    NpadStyleJoyDual = 2,   // (Joy-Con controller in dual mode)
    NpadStyleJoyLeft = 3,   // (Joy-Con left controller in single mode)
    NpadStyleJoyRight = 4,  // (Joy-Con right controller in single mode)
#if NN_SDK_VER <= NN_MAKE_VER(3, 5, 1)
    NpadStyleInvalid = 5,
#else
    NpadStyleGc = 5,            // (GameCube controller)
    NpadStylePalma = 6,         // (Poké Ball Plus controller)
    NpadStyleLark = 7,          // (NES/Famicom controller)
    NpadStyleHandheldLark = 8,  // (NES/Famicom controller in handheld mode)
    NpadStyleLucia = 9,         // (SNES controller)
    NpadStyleLagon = 10,        // [12.0.0+] (N64 controller)
    NpadStyleLager = 11,        // [13.0.0+] (Sega Genesis controller)
#endif
    // bits 12-28 Reserved
    NpadStyleSystemExt = 29,  // (generic external controller)
    NpadStyleSystem = 30,     // (generic controller)
    // bit 31 Reserved
};

enum class NpadSystemProperties {
    IsChargingJoyDual = 0,
    IsChargingJoyLeft = 1,
    IsChargingJoyRight = 2,
    IsPoweredJoyDual = 3,
    IsPoweredJoyLeft = 4,
    IsPoweredJoyRight = 5,
    IsUnsuportedButtonPressedOnNpadSystem = 9,
    IsUnsuportedButtonPressedOnNpadSystemExt = 10,
    IsAbxyButtonOriented = 11,
    IsSlSrButtonOriented = 12,
    IsPlusAvailable = 13,                // [4.0.0+]
    IsMinusAvailable = 14,               // [4.0.0+]
    IsDirectionalButtonsAvailable = 15,  // [8.0.0+]
};

enum class NpadSystemButtonProperties { IsUnintendedHomeButtonInputProtectionEnabled };

enum class TouchAttribute { Transferable = 0, IsConnected = 1 };

enum class MouseButton { Left, Right, Middle, Forward, Back };

enum class MouseAttribute { Transferable, IsConnected };

enum class KeyboardKey {
    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,
    D1 = 30,
    D2 = 31,
    D3 = 32,
    D4 = 33,
    D5 = 34,
    D6 = 35,
    D7 = 36,
    D8 = 37,
    D9 = 38,
    D0 = 39,
    Return = 40,
    Escape = 41,
    Backspace = 42,
    Tab = 43,
    Space = 44,
    Minus = 45,
    Plus = 46,
    OpenBracket = 47,
    CloseBracket = 48,
    Pipe = 49,
    Tilde = 50,
    Semicolon = 51,
    Quote = 52,
    Backquote = 53,
    Comma = 54,
    Period = 55,
    Slash = 56,
    CapsLock = 57,
    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,
    PrintScreen = 70,
    ScrollLock = 71,
    Pause = 72,
    Insert = 73,
    Home = 74,
    PageUp = 75,
    Delete = 76,
    End = 77,
    PageDown = 78,
    RightArrow = 79,
    LeftArrow = 80,
    DownArrow = 81,
    UpArrow = 82,
    NumLock = 83,
    NumPadDivide = 84,
    NumPadMultiply = 85,
    NumPadSubtract = 86,
    NumPadAdd = 87,
    NumPadEnter = 88,
    NumPad1 = 89,
    NumPad2 = 90,
    NumPad3 = 91,
    NumPad4 = 92,
    NumPad5 = 93,
    NumPad6 = 94,
    NumPad7 = 95,
    NumPad8 = 96,
    NumPad9 = 97,
    NumPad0 = 98,
    NumPadDot = 99,
    Backslash = 100,
    Application = 101,
    Power = 102,
    NumPadEquals = 103,
    F13 = 104,
    F14 = 105,
    F15 = 106,
    F16 = 107,
    F17 = 108,
    F18 = 109,
    F19 = 110,
    F20 = 111,
    F21 = 112,
    F22 = 113,
    F23 = 114,
    F24 = 115,
    NumPadComma = 133,
    Ro = 135,
    KatakanaHiragana = 136,
    Yen = 137,
    Henkan = 138,
    Muhenkan = 139,
    NumPadCommaPc98 = 140,
    HangulEnglish = 144,
    Hanja = 145,
    Katakana = 146,
    Hiragana = 147,
    ZenkakuHankaku = 148,
    LeftControl = 224,
    LeftShift = 225,
    LeftAlt = 226,
    LeftGui = 227,
    RightControl = 228,
    RightShift = 229,
    RightAlt = 230,
    RightGui = 231,
};

enum class KeyboardModifier {
    Control,
    Shift,
    LeftAlt,
    RightAlt,
    Gui,
    CapsLock,
    ScrollLock,
    NumLock,
    Katakana,
    Hiragana
};

enum class DebugPadButton { A, B, X, Y, L, R, ZL, ZR, Start, Select, Left, Up, Right, Down };

enum class DebugPadAttribute { IsConnected };

enum class BasicXpadButton {};

enum class BasicXpadAttribute {};

enum class GestureType { Idle, Complete, Cancel, Touch, Press, Tap, Pan, Swipe, Pinch, Rotate };

enum class GestureDirection { None, Left, Up, Right, Down };

enum class GestureAttribute { IsNewTouch = 4, IsDoubleTap = 8 };

enum class SixAxisSensorAttribute { IsConnected, IsInterpolated };

enum class NpadJoyHoldType { Vertical, Horizontal };

enum class NpadJoyDeviceType { Left, Right };

enum class NpadHandheldActivationMode { Dual, Single, None };

enum class NpadJoyAssignmentMode { Dual, Single };

enum class DigitizerAttribute {};

enum class DigitizerButton {};

enum class VibrationDeviceType { Unknown, LinearResonantActuator, GcErm, Erm };

enum class VibrationDevicePosition { None, Left, Right };

enum class NpadLarkType { Invalid, H1, H2, NL, NR };

enum class NpadLuciaType { Invalid, J, E, U };

enum class NpadLagerType { Invalid, J, E, U };

typedef nn::util::BitFlagSet<32, NpadAttribute> NpadAttributeSet;
typedef nn::util::BitFlagSet<64, NpadButton> NpadButtonSet;
typedef nn::util::BitFlagSet<32, NpadStyleTag> NpadStyleSet;
typedef nn::util::BitFlagSet<32, MouseButton> MouseButtonSet;
typedef nn::util::BitFlagSet<32, MouseAttribute> MouseAttributeSet;
typedef nn::util::BitFlagSet<32, TouchAttribute> TouchAttributeSet;
typedef nn::util::BitFlagSet<32, DebugPadButton> DebugPadButtonSet;
typedef nn::util::BitFlagSet<32, DebugPadAttribute> DebugPadAttributeSet;
typedef nn::util::BitFlagSet<32, BasicXpadButton> BasicXpadButtonSet;
typedef nn::util::BitFlagSet<32, BasicXpadAttribute> BasicXpadAttributeSet;
typedef nn::util::BitFlagSet<32, GestureAttribute> GestureAttributeSet;
typedef nn::util::BitFlagSet<32, SixAxisSensorAttribute> SixAxisSensorAttributeSet;
typedef nn::util::BitFlagSet<32, DigitizerAttribute> DigitizerAttributeSet;
typedef nn::util::BitFlagSet<32, DigitizerButton> DigitizerButtonSet;

struct AnalogStickState {
    s32 mX;
    s32 mY;
};

struct ControllerSupportArg {
    u8 mMinPlayerCount;
    u8 mMaxPlayerCount;
    u8 mTakeOverConnection;
    bool mLeftJustify;
    bool mPermitJoyconDual;
    bool mSingleMode;
    bool mUseColors;
    Color4u8 mColors[4];
    u8 mUsingControllerNames;
    char mControllerNames[4][0x81];
};

struct ControllerSupportArgV2 {
    u8 mMinPlayerCount;
    u8 mMaxPlayerCount;
    u8 mTakeOverConnection;
    bool mLeftJustify;
    bool mPermitJoyconDual;
    bool mSingleMode;
    bool mUseColors;
    Color4u8 mColors[8];
    u8 mUsingControllerNames;
    char mControllerNames[8][0x81];
};

struct ControllerSupportResultInfo {
    s8 mPlayerCount;
    s32 mSelectedId;
    s32 mResult;
};

struct NpadControllerColor {
    Color4u8 mMain;
    Color4u8 mSub;
};

struct DebugPadState {
    u64 mSamplingNumber;
    DebugPadAttributeSet mAttributes;
    DebugPadButtonSet mButtons;
    AnalogStickState mAnalogStickR;
    AnalogStickState mAnalogStickL;
};

struct TouchState {
    u64 mDeltaTime;
    TouchAttributeSet mAttributes;
    s32 mFingerId;
    s32 mX;
    s32 mY;
    s32 mDiameterX;
    s32 mDiameterY;
    s32 mRotationAngle;
};

template <u64 N>
struct TouchScreenState {
    u64 mSamplingNumber;
    s32 mCount;
    TouchState mTouches[N];
};

struct MouseState {
    u64 mSamplingNumber;
    s32 mX;
    s32 mY;
    s32 mDeltaX;
    s32 mDeltaY;
    s32 mWheelDeltaX;
    s32 mWheelDeltaY;
    MouseButtonSet mButtons;
    MouseAttributeSet mAttributes;
};

struct KeyboardState {
    u64 mSamplingNumber;
    nn::util::BitFlagSet<32, KeyboardModifier> mModifiers;
    nn::util::BitFlagSet<256, KeyboardKey> mKeys;
};

struct BasicXpadState {
    u64 mSamplingNumber;
    BasicXpadAttributeSet mAttributes;
    BasicXpadButtonSet mButtons;
    AnalogStickState mAnalogStickL;
    AnalogStickState mAnalogStickR;
};

struct NpadBaseState {
    u64 mSamplingNumber;
    NpadButtonSet mButtons;
    AnalogStickState mAnalogStickL;
    AnalogStickState mAnalogStickR;
    NpadAttributeSet mAttributes;
};

struct NpadFullKeyState : NpadBaseState {};

struct NpadHandheldState : NpadBaseState {};

struct NpadJoyDualState : NpadBaseState {};

struct NpadJoyLeftState : NpadBaseState {};

struct NpadJoyRightState : NpadBaseState {};

struct NpadPalmaState : NpadBaseState {};

struct DirectionState {
    f32 mMtx[3][3];
};

struct SixAxisSensorState {
    u64 mDeltaTime;
    u64 mSamplingNumber;
    f32 mAcceleration[3];
    f32 mAngularVelocity[3];
    f32 mAngle[3];
    DirectionState mDirection;
    SixAxisSensorAttributeSet mAttributes;
};

struct GesturePoint {
    s32 mX;
    s32 mY;
};

struct GestureState {
    u64 mSamplingNumber;
    u64 mContextNumber;
    GestureType mType;
    GestureDirection mDirection;
    s32 mX;
    s32 mY;
    s32 mDeltaX;
    s32 mDeltaY;
    f32 mVelocityX;
    f32 mVelocityY;
    GestureAttributeSet mAttributes;
    f32 mScale;
    f32 mRotationAngle;
    s32 mPointCount;
    GesturePoint mPoint[4];
};

struct DigitizerState {
    u64 mSamplingNumber;
    u8 padding_8[0x8];
    DigitizerAttributeSet mAttributes;
    DigitizerButtonSet mButtons;
    u8 padding[0x40];
};

struct SixAxisSensorHandle {
    union {
        u32 typeValue;
        struct {
            u8 mNpadStyleIndex;
            u8 mPlayerNumber;
            u8 mDeviceIndex;
        };
    };
};

struct VibrationDeviceHandle {
    union {
        u32 typeValue;
        struct {
            u8 mNpadStyleIndex;
            u8 mPlayerNumber;
            u8 mDeviceIndex;
        };
    };
};

struct VibrationDeviceInfo {
    VibrationDeviceType mDeviceType;
    VibrationDevicePosition mPosition;
};

struct VibrationValue {
    f32 mAmplitudeLow;
    f32 mFrequencyLow;
    f32 mAmplitudeHigh;
    f32 mFrequencyHigh;
};

void InitializeNpad();
void SetSupportedNpadIdType(const u32*, u64);
void SetSupportedNpadStyleSet(NpadStyleSet);
NpadStyleSet GetNpadStyleSet(const u32& port);
s32 ShowControllerSupport(ControllerSupportResultInfo*, const ControllerSupportArg&);

void GetNpadState(NpadFullKeyState*, const u32& port);
void GetNpadState(NpadHandheldState*, const u32& port);
void GetNpadState(NpadJoyDualState*, const u32& port);
void GetNpadState(NpadJoyLeftState*, const u32& port);
void GetNpadState(NpadJoyRightState*, const u32& port);

void GetNpadStates(NpadFullKeyState*, s32, const u32& port);
void GetNpadStates(NpadHandheldState*, s32, const u32& port);
void GetNpadStates(NpadJoyDualState*, s32, const u32& port);
void GetNpadStates(NpadJoyLeftState*, s32, const u32& port);
void GetNpadStates(NpadJoyRightState*, s32, const u32& port);

void InitializeMouse();
void InitializeKeyboard();

void GetMouseState(MouseState*);
void GetKeyboardState(KeyboardState*);

namespace system {

typedef u8 UniquePadSerialNumber[0x10];
typedef u32 BatteryLevel;

enum class UniquePadType : u64 {
    Embedded,
    FullKeyController,
    RightController,
    LeftController,
    DebugPadController
};

enum class UniquePadInterface { Embedded, Rail, Bluetooth, Usb };

enum class HomeButton {};

enum class SleepButton {};

enum class CaptureButton {};

enum class AnalogStickManualCalibrationStage : u64 {
    ReleaseFromRight,
    ReleaseFromBottom,
    ReleaseFromLeft,
    ReleaseFromTop,
    Rotate,
    Update,
    Completed,
    Clear,
    ClearCompleted
};

enum class SixAxisSensorUserCalibrationStage : u64 { Measuring, Update, Completed };

enum class DeviceType {
    FullKey,
    DebugPad,
    HandheldLeft,
    HandheldRight,
    JoyLeft,
    JoyRight,
    Palma,
    LarkHvcLeft,
    LarkHvcRight,
    LarkNesLeft,
    LarkNesRight,
    HandheldLarkHvcLeft,
    HandheldLarkHvcRight,
    HandheldLarkNesLeft,
    HandheldLarkNesRight,
    Lucia,
    Lagon,  // [12.0.0+]
    Lager,  // [13.0.0+]
    System = 31
};

enum class AppletFooterUiType : u8 {
    None,
    HandheldNone,
    HandheldJoyConLeftOnly,
    HandheldJoyConRightOnly,
    HandheldLeftJoyConRight,
    JoyDual,
    JoyDualLeftOnly,
    JoyDualRightOnly,
    JoyLeftHorizontal,
    JoyLeftVertical,
    JoyRightHorizontal,
    JoyRightVertical,
    SwitchProController,
    CompatibleProController,
    CompatibleJoyCon,
    LarkHvc1,
    LarkHvc2,
    LarkNesLeft,
    LarkNesRight,
    Lucia,
    Verification,
    Lagon,  // [13.0.0+]
};

enum class AppletFooterUiAttribute {};

typedef nn::util::BitFlagSet<64, HomeButton> HomeButtonSet;
typedef nn::util::BitFlagSet<64, SleepButton> SleepButtonSet;
typedef nn::util::BitFlagSet<64, CaptureButton> CaptureButtonSet;
typedef nn::util::BitFlagSet<32, AppletFooterUiAttribute> AppletFooterUiAttributeSet;

struct HomeButtonState {
    u64 mSamplingNumber;
    HomeButtonSet mButtons;
};

struct SleepButtonState {
    u64 mSamplingNumber;
    SleepButtonSet mButtons;
};

struct CaptureButtonState {
    u64 mSamplingNumber;
    CaptureButtonSet mButtons;
};

struct NpadSystemState : NpadBaseState {};

struct NpadSystemExtState : NpadBaseState {};

struct InputSourceState {
    u64 mTimestamp;
};

}  // namespace system

namespace tmp {

struct SixAxisSensorCountState {
    u8 padding[0x28];
};

}  // namespace tmp

namespace detail {

template <typename T>

class AtomicStorage {
public:
    u64 mSamplingNumber;
    T mStorage;
};

template <typename T, s32 N, typename Atomic>
struct RingLifo {
    u64 mTimestamp;
    u64 mBufferCount = N + 1;
    u64 mTail = 0;
    u64 mCount = 0;
    Atomic mStorage[N + 1];
};

}  // namespace detail

namespace server {

typedef detail::RingLifo<DigitizerState, 16, detail::AtomicStorage<DigitizerState>> DigitizerLifo;

struct DigitizerSharedMemoryFormat {
    DigitizerLifo mLifo;
    u8 padding[0x1000 - sizeof(DigitizerLifo)];
};

struct NpadGcTriggerState {
    u64 mSamplingNumber;
    u32 mTriggerL;
    u32 mTriggerR;
};

enum class SixAxisSensorProperties {
    IsSixAxisSensorDeviceNewlyAssigned,
    IsFirmwareUpdateAvailableForSixAxisSensor
};

}  // namespace server

namespace detail {

enum class AnalogStickCalibrationFlags {};
enum class SixAxisSensorUserCalibrationFlags {};
enum class ColorAttribute { Ok, ReadError, NoController };

typedef AtomicStorage<TouchScreenState<16>> TouchScreenStateAtomicStorage;
typedef AtomicStorage<server::NpadGcTriggerState> NpadGcTriggerStateAtomicStorage;

typedef nn::util::BitFlagSet<32, AnalogStickCalibrationFlags> AnalogStickCalibrationFlagsSet;
typedef nn::util::BitFlagSet<32, SixAxisSensorUserCalibrationFlags>
    SixAxisSensorUserCalibrationFlagsSet;
typedef nn::util::BitFlagSet<64, NpadSystemProperties> NpadSystemPropertiesSet;
typedef nn::util::BitFlagSet<32, NpadSystemButtonProperties> NpadSystemButtonPropertiesSet;

struct InputDetectorState {
    system::InputSourceState mInputSourceState;
    u64 mSamplingNumber;
};

struct UniquePadConfig {
    system::UniquePadType mType;
    system::UniquePadInterface mInterface;
    system::UniquePadSerialNumber mSerialNumber;
    u32 mControllerNumber;
    bool mIsActive;
    u64 mSamplingNumber;
};

struct AnalogStickCalibrationStateImpl {
    AnalogStickState mState;
    AnalogStickCalibrationFlagsSet mFlags;
    system::AnalogStickManualCalibrationStage mStage;
    u64 mSamplingNumber;
};

struct SixAxisSensorUserCalibrationState {
    SixAxisSensorUserCalibrationFlagsSet mFlags;
    system::SixAxisSensorUserCalibrationStage mStage;
    u64 mSamplingNumber;
};

struct NfcXcdDeviceHandleStateImpl {
    xcd::DeviceHandle mHandle;
    bool mIsAvailable;
    bool mIsActivated;
    u64 mSamplingNumber;
};

typedef RingLifo<MouseState, 16, AtomicStorage<MouseState>> MouseLifo;
typedef RingLifo<DebugPadState, 16, AtomicStorage<DebugPadState>> DebugPadLifo;
typedef RingLifo<KeyboardState, 16, AtomicStorage<KeyboardState>> KeyboardLifo;
typedef RingLifo<BasicXpadState, 16, AtomicStorage<BasicXpadState>> BasicXpadLifo;
typedef RingLifo<NpadFullKeyState, 16, AtomicStorage<NpadFullKeyState>> NpadFullKeyLifo;
typedef RingLifo<NpadJoyDualState, 16, AtomicStorage<NpadJoyDualState>> NpadJoyDualLifo;
typedef RingLifo<NpadJoyLeftState, 16, AtomicStorage<NpadJoyLeftState>> NpadJoyLeftLifo;
typedef RingLifo<TouchScreenState<16ul>, 16, TouchScreenStateAtomicStorage> TouchScreenLifo;
typedef RingLifo<NpadHandheldState, 16, AtomicStorage<NpadHandheldState>> NpadHandheldLifo;
typedef RingLifo<NpadJoyRightState, 16, AtomicStorage<NpadJoyRightState>> NpadJoyRightLifo;
typedef RingLifo<tmp::SixAxisSensorCountState, 32, AtomicStorage<tmp::SixAxisSensorCountState>>
    SixAxisSensorCountStateLifo;
typedef RingLifo<system::HomeButtonState, 16, AtomicStorage<system::HomeButtonState>>
    HomeButtonLifo;
typedef RingLifo<system::NpadSystemState, 16, AtomicStorage<system::NpadSystemState>>
    NpadSystemLifo;
typedef RingLifo<system::SleepButtonState, 16, AtomicStorage<system::SleepButtonState>>
    SleepButtonLifo;
typedef RingLifo<system::CaptureButtonState, 16, AtomicStorage<system::CaptureButtonState>>
    CaptureButtonLifo;
typedef RingLifo<system::NpadSystemExtState, 16, AtomicStorage<system::NpadSystemExtState>>
    NpadSystemExtLifo;
typedef RingLifo<UniquePadConfig, 1, AtomicStorage<UniquePadConfig>> UniquePadConfigLifo;
typedef RingLifo<GestureState, 16, AtomicStorage<GestureState>> GestureLifo;
typedef RingLifo<InputDetectorState, 1, AtomicStorage<InputDetectorState>> InputDetectorLifo;
typedef RingLifo<NfcXcdDeviceHandleStateImpl, 1, AtomicStorage<NfcXcdDeviceHandleStateImpl>>
    NfcXcdDeviceHandleState;
typedef RingLifo<AnalogStickCalibrationStateImpl, 1, AtomicStorage<AnalogStickCalibrationStateImpl>>
    AnalogStickCalibrationStateImplLifo;
typedef RingLifo<SixAxisSensorUserCalibrationState, 1,
                 AtomicStorage<SixAxisSensorUserCalibrationState>>
    SixAxisSensorUserCalibrationStateLifo;
typedef RingLifo<SixAxisSensorState, 16, AtomicStorage<SixAxisSensorState>> NpadSixAxisSensorLifo;
typedef RingLifo<SixAxisSensorState, 32, AtomicStorage<SixAxisSensorState>> SixAxisSensorStateLifo;
typedef RingLifo<SixAxisSensorState, 16, NpadGcTriggerStateAtomicStorage> NpadGcTriggerLifo;

struct DebugPadSharedMemoryFormat {
    DebugPadLifo mLifo;
    u8 padding[0x400 - sizeof(DebugPadLifo)];
};

struct TouchScreenSharedMemoryFormat {
    TouchScreenLifo mLifo;
    u8 padding[0x3000 - sizeof(TouchScreenLifo)];
};

struct MouseSharedMemoryFormat {
    MouseLifo mLifo;
    u8 padding[0x400 - sizeof(MouseLifo)];
};

struct KeyboardSharedMemoryFormat {
    KeyboardLifo mLifo;
    u8 padding[0x400 - sizeof(KeyboardLifo)];
};

struct BasicXpadSharedMemoryEntry {
    BasicXpadLifo mLifo;
    u8 padding[0x400 - sizeof(BasicXpadLifo)];
};

struct BasicXpadSharedMemoryFormat {
    BasicXpadSharedMemoryEntry mEntries[4];
};

struct HomeButtonSharedMemoryFormat {
    HomeButtonLifo mLifo;
    u8 padding[0x200 - sizeof(HomeButtonLifo)];
};

struct SleepButtonSharedMemoryFormat {
    SleepButtonLifo mLifo;
    u8 padding[0x200 - sizeof(SleepButtonLifo)];
};

struct CaptureButtonSharedMemoryFormat {
    CaptureButtonLifo mLifo;
    u8 padding[0x200 - sizeof(CaptureButtonLifo)];
};

struct InputDetectorSharedMemoryEntry {
    InputDetectorLifo mLifo;
    u8 padding[0x80 - sizeof(InputDetectorLifo)];
};

struct InputDetectorSharedMemoryFormat {
    InputDetectorSharedMemoryEntry mEntries[16];
};

struct UniquePadSharedMemoryEntry {
    UniquePadConfigLifo mUniquePadConfigLifo;
    AnalogStickCalibrationStateImplLifo mAnalogStickCalibrationStateImplLifo[2];
    SixAxisSensorUserCalibrationStateLifo mSixAxisSensorUserCalibrationStateLifo;
    os::MutexType mMutex;
    u8 padding[0x400 - 0x1f0];
};

struct UniquePadSharedMemoryFormat {
    UniquePadSharedMemoryEntry mEntries[16];
};

struct NpadFullKeyColorState {
    ColorAttribute mAttribute;
    NpadControllerColor mFullKey;
};

struct NpadJoyColorState {
    ColorAttribute mAttribute;
    NpadControllerColor mLeft;
    NpadControllerColor mRight;
};

struct NpadInternalState {
    NpadStyleSet mStyleSet;
    NpadJoyAssignmentMode mJoyAssignmentMode;
    NpadFullKeyColorState mFullKeyColor;
    NpadJoyColorState mJoyColor;
    NpadFullKeyLifo mFullKeyLifo;
    NpadHandheldLifo mHandheldLifo;
    NpadJoyDualLifo mJoyDualLifo;
    NpadJoyLeftLifo mJoyLeftLifo;
    NpadJoyRightLifo mJoyRightLifo;
    NpadSystemLifo mSystemLifo;
    NpadSystemExtLifo mSystemExtLifo;
    NpadSixAxisSensorLifo mFullKeySixAxisSensorLifo;
    NpadSixAxisSensorLifo mHandheldSixAxisSensorLifo;
    NpadSixAxisSensorLifo mJoyDualLeftSixAxisSensorLifo;
    NpadSixAxisSensorLifo mJoyDualRightSixAxisSensorLifo;
    NpadSixAxisSensorLifo mJoyLeftSixAxisSensorLifo;
    NpadSixAxisSensorLifo mJoyRightSixAxisSensorLifo;
    system::DeviceType mDeviceType;
    NpadSystemPropertiesSet mSystemProperties;
    NpadSystemButtonPropertiesSet mSystemButtonProperties;
    system::BatteryLevel mBatteryLevelJoyDual;
    system::BatteryLevel mBatteryLevelJoyLeft;
    system::BatteryLevel mBatteryLevelJoyRight;
#if NN_SDK_VER < NN_MAKE_VER(9, 0, 0)
    NfcXcdDeviceHandleState mNfcXcdDeviceHandle;
    os::MutexType mMutex;
    u8 padding[0x20];
#else
    system::AppletFooterUiAttributeSet mAppletFooterUiAttributes;
    system::AppletFooterUiType mAppletFooterUiType;
    u8 padding[0x7b];
#endif
    NpadGcTriggerLifo GcTriggerLifo;
    NpadLarkType mLarkTypeLAndMain;
    NpadLarkType mLarkTypeR;
    NpadLuciaType mLuciaType;
    NpadLagerType mLagerType;
    server::SixAxisSensorProperties mSixAxisSensorProperties[6];  // [13.0.0+]
};

struct NpadSharedMemoryEntry {
    NpadInternalState mInternalState;
    u8 padding[0x5000 - sizeof(NpadInternalState)];
};

struct NpadSharedMemoryFormat {
    NpadSharedMemoryEntry mEntries[10];
};

struct GestureSharedMemoryFormat {
    GestureLifo mLifo;
    u8 padding[0x800 - sizeof(GestureLifo)];
};

struct ConsoleSixAxisSensorSharedMemoryFormat {
    u64 mSamplingNumber;
    bool mIsSevenSixAxisSensorAtRest;
    u32 mVerticalizationError;
    f32 mGyroBias[3];
    u8 padding[0x19e4];
};

struct SharedMemoryFormat {
    DebugPadSharedMemoryFormat mDebugPad;
    TouchScreenSharedMemoryFormat mTouchScreen;
    MouseSharedMemoryFormat mMouse;
    KeyboardSharedMemoryFormat mKeyboard;
#if NN_SDK_VER < NN_MAKE_VER(10, 0, 0)
    BasicXpadSharedMemoryFormat mBasicXpad;
#else
    server::DigitizerSharedMemoryFormat mDigitizer;
#endif
    HomeButtonSharedMemoryFormat mHomeButton;
    SleepButtonSharedMemoryFormat mSleepButton;
    CaptureButtonSharedMemoryFormat mCaptureButton;
    InputDetectorSharedMemoryFormat mInputDetector;
#if NN_SDK_VER < NN_MAKE_VER(5, 0, 0)
    UniquePadSharedMemoryFormat mUniquePad;
#else
    u8 padding_5a00[sizeof(UniquePadSharedMemoryFormat)];
#endif
    NpadSharedMemoryFormat mNpad;
    GestureSharedMemoryFormat mGesture;
#if NN_SDK_VER < NN_MAKE_VER(5, 0, 0)
    u8 padding_3c200[sizeof(ConsoleSixAxisSensorSharedMemoryFormat)];
#else
    ConsoleSixAxisSensorSharedMemoryFormat mConsoleSixAxisSensor;
#endif
#if NN_SDK_VER >= NN_MAKE_VER(16, 0, 0)
    MouseSharedMemoryFormat mDebugMouse;
#else
    u8 padding_3dc00[sizeof(MouseSharedMemoryFormat)];
#endif
    u8 padding_3e000[0x2000];
};

static_assert(sizeof(SharedMemoryFormat) == 0x40000);

struct SharedMemoryType {
    u8 padding[0x40];
};

struct SharedMemoryHolder {
    bool mIsOwner;
    bool mIsCreated;
    bool mIsMapped;
    SharedMemoryType mSharedMemory;
    SharedMemoryFormat* mAddress;
};

}  // namespace detail
}  // namespace hid
}  // namespace nn
