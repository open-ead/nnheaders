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

using DefaultApi = ApiVariation<ApiType<4>, ApiVersion<8>>;

}  // namespace nn::gfx
