/**
 * @file api.h
 * @brief GFX API version and typing.
 */

#pragma once

namespace nn::gfx {

template <typename Type, typename Version>
class ApiVariation {};

template <int T>
class ApiType {};

template <int T>
class ApiVersion {};

// todo: seems to be named ApiVariationNvn8
using NvnApi = ApiVariation<ApiType<4>, ApiVersion<8>>;
using DefaultApi = NvnApi;

}  // namespace nn::gfx
