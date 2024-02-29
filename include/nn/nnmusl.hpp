#pragma once

#include <nn/types.h>

extern "C" int _nnmusl_errno_from_result(nn::Result);
extern "C" int __nnmusl_BroadcastConditionVariable(void*);
