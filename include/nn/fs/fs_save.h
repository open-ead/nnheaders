#pragma once

#include <nn/account.h>
#include <nn/fs/fs_types.h>

namespace nn {
typedef u64 ApplicationId;
};

namespace nn::fs {

Result EnsureSaveData(const nn::account::Uid&);
Result MountSaveData(const char*, UserId);
Result MountSaveData(const char*, const nn::account::Uid&);
Result MountSaveData(const char*, ApplicationId, const nn::account::Uid&);
Result MountSaveDataForDebug(const char*);
Result CommitSaveData(const char* path);

}  // namespace nn::fs
