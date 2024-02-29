#pragma once

#include <nn/account.h>
#include <nn/fs/fs_types.h>

namespace nn::fs {

Result EnsureSaveData(const nn::account::Uid&);
Result MountSaveData(const char*, UserId);
Result MountSaveDataForDebug(const char*);
Result CommitSaveData(const char* path);

}  // namespace nn::fs
