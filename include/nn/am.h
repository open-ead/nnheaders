#pragma once

namespace nn::am {
// based on https://switchbrew.org/wiki/Applet_Manager_services#AppletMessage
enum AppletMessage {
    AppletMessage_None,
    AppletMessage_ChangeIntoForeground,
    AppletMessage_ChangeIntoBackground,
    AppletMessage_ExitRequested = 4,
    AppletMessage_ApplicationExited = 6,
    AppletMessage_7,
    AppletMessage_FocusStateChanged = 15,
    AppletMessage_Resume,
    AppletMessage_DetectShortPressingHomeButton = 20,
    AppletMessage_DetectLongPressingHomeButton,
    AppletMessage_DetectShortPressingPowerButton,
    AppletMessage_DetectMiddlePressingPowerButton,
    AppletMessage_DetectLongPressingPowerButton,
    AppletMessage_RequestToPrepareSleep,
    AppletMessage_FinishedSleepSequence,
    AppletMessage_SleepRequiredByHighTemperature,
    AppletMessage_SleepRequiredByLowBattery,
    AppletMessage_AutoPowerDown,
    AppletMessage_OperationModeChanged,
    AppletMessage_PerformanceModeChanged,
    AppletMessage_DetectReceivingCecSystemStandby,
    AppletMessage_SdCardRemoved,
    AppletMessage_34,
    AppletMessage_LaunchApplicationRequested = 50,
    AppletMessage_RequestToDisplay,
    AppletMessage_ShowApplicationLogo = 55,
    AppletMessage_HideApplicationLogo,
    AppletMessage_ForceHideApplicationLogo,
    AppletMessage_58,
    AppletMessage_59,
    AppletMessage_FloatingApplicationDetected,
    // 7.0.0+
    AppletMessage_PerformanceConfigurationChanged,
    AppletMessage_70 = 70,
    AppletMessage_71,
    AppletMessage_72,
    AppletMessage_73,
    AppletMessage_75 = 75,
    AppletMessage_80 = 80,
    AppletMessage_81,
    AppletMessage_82,
    AppletMessage_83,
    AppletMessage_85 = 85,
    AppletMessage_86,
    AppletMessage_DetectShortPressingCaptureButton = 90,
    AppletMessage_AlbumScreenShotTaken = 92,
    AppletMessage_AlbumRecordingSaved,
    AppletMessage_94,
    AppletMessage_100 = 100,
    AppletMessage_101,
    // 14.0.0+
    AppletMessage_DetectShortPressingCaptureButtonForApplet = 110,
    // 14.0.0+
    AppletMessage_DetectLongPressingCaptureButtonForApplet,
    AppletMessage_1000 = 1000
};
}  // namespace nn::am
