#pragma once

#include <nn/fs/fs_types.h>

namespace nn::fs {

Result MountBcatSaveData(const char*, ApplicationId);
Result CreateBcatSaveData(ApplicationId, s64);

}  // namespace nn::fs
