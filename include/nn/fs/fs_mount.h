#pragma once

#include <nn/fs/fs_types.h>

namespace nn::fs {

/*
    Mount SD card. Must have explicit permission.
    mount: drive to mount to.
*/
bool MountSdCardForDebug(const char* mount);

Result MountSdCard(const char* mountPoint);
bool IsSdCardInserted();
Result FormatSdCard();
Result FormatSdCardDryRun();
bool IsExFatSupported();

Result MountHost(const char* mount, const char* path);
Result MountHostRoot();
Result UnmountHostRoot();

Result Unmount(const char* mountPoint);

}  // namespace nn::fs
