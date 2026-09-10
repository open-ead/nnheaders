#pragma once

#include <nn/types.h>

namespace nn::err {
class ErrorMessageDatabaseVersion {
public:
private:
    s32 mVersion;
};

static_assert(sizeof(ErrorMessageDatabaseVersion) == 4);
}  // namespace nn::err
