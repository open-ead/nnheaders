#pragma once

#include <nn/types.h>

namespace nn::applet {
enum class ExitReason { Normal = 0, Canceled = 1, Abnormal = 2, Unexpected = 10 };

// https://switchbrew.org/wiki/Applet_Manager_services#AppletId
enum class AppletId {
    None = 0x00,
    Application = 0x01,
    OverlayApplet = 0x02,
    SystemAppletMenu = 0x03,
    SystemApplication = 0x04,
    LibraryAppletAuth = 0x0A,
    LibraryAppletCabinet = 0x0B,
    LibraryAppletController = 0x0C,
    LibraryAppletDataErase = 0x0D,
    LibraryAppletError = 0x0E,
    LibraryAppletNetConnect = 0x0F,
    LibraryAppletPlayerSelect = 0x10,
    LibraryAppletSwkbd = 0x11,
    LibraryAppletMiiEdit = 0x12,
    LibraryAppletWeb = 0x13,
    LibraryAppletShop = 0x14,
    LibraryAppletPhotoViewer = 0x15,
    LibraryAppletSet = 0x16,
    LibraryAppletOfflineWeb = 0x17,
    LibraryAppletLoginShare = 0x18,
    LibraryAppletWifiWebAuth = 0x19,
    LibraryAppletMyPage = 0x1A,
    LibraryAppletGift = 0x1B,
    LibraryAppletUserMigration = 0x1C,
    // NOTE: more entries exist in later versions

};

// https://switchbrew.org/wiki/Applet_Manager_services#LibraryAppletMode
enum class LibraryAppletMode {
    AllForeground = 0,
    PartialForeground = 1,
    NoUi = 2,
    PartialForegroundWithIndirectDisplay = 3,
    AllForegroundInitiallyHidden = 4,
};

struct LibraryAppletHandle {
    char _filler0[0x18];
    AppletId id;
    AppletMode mode;
    char _filler20[0x78];
};
static_assert(sizeof(LibraryAppletHandle) == 0x98);

}  // namespace nn::applet
