#pragma once

#include <nn/types.h>
#include <type_traits>

namespace nn::util {

template <class T, size_t Size, size_t Alignment>
struct TypedStorage {
    typename std::aligned_storage<Size, Alignment>::type storage;
};

}  // namespace nn::util