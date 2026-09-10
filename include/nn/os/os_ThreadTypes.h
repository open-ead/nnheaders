#pragma once

#include <nn/os/detail/os_InternalConditionVariable.h>
#include <nn/os/detail/os_InternalCriticalSection.h>
#include <nn/os/detail/os_ThreadTypes-os.horizon.h>
#include <nn/os/os_ThreadCommon.h>
#include <nn/util.h>
#include <nn/util/util_TypedStorage.h>

namespace nn {

namespace util {
class IntrusiveListNode;
}

namespace os {

struct FiberType;

namespace detail {
class MultiWaitObjectList;
}

struct ThreadType {
    util::TypedStorage<util::IntrusiveListNode, 16, 8> _allThreadsListNode;
    util::TypedStorage<detail::MultiWaitObjectList, 16, 8> _multiWaitObjectList;

    uintptr_t reserved[4];

    uint8_t _state;
    bool _stackIsAliased;
    bool _autoRegistered;
    uint8_t _suspendCount;
    int _basePriority;
    void* _originalStack;
    void* _stack;
    size_t _stackSize;
    void* _argument;
    ThreadFunction _threadFunction;
    FiberType* _currentFiber;
    FiberType* _initialFiber;
    // may be inaccurate: SDK 4.4.0 and 5.4.150 has the field while 7.3.2 and 20.5.6 does not
#if NN_SDK_VER < NN_MAKE_VER(6, 0, 0)
    uint32_t _lockHistory;
#endif
    uintptr_t _tlsValueArray[32];
    char _threadNameBuffer[32];
    const char* _namePointer;
    detail::InternalCriticalSectionStorage _csThread;
    detail::InternalConditionVariableStorage _cvThread;
    detail::InternalThreadHandle _handle;
};
#ifdef SWITCH
#if NN_SDK_VER < NN_MAKE_VER(6, 0, 0)  // see _lockHistory above
static_assert(sizeof(ThreadType) == 0x1C0, "Wrong size");
#else
static_assert(sizeof(ThreadType) == 0x1B8, "Wrong size");
#endif
#endif

}  // namespace os

}  // namespace nn
