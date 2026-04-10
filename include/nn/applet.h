#pragma once

#include <nn/types.h>

namespace nn::applet {
enum class ExitReason { Normal = 0, Canceled = 1, Abnormal = 2, Unexpected = 10 };

// TODO: Fill members of LibraryAppletHandle. This contains a service ctx
struct LibraryAppletHandle;
}  // namespace nn::applet
