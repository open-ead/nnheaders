#pragma once

#include <nn/util/detail/util_MathTypes.neon.h>
#include <nn/util/detail/util_MatrixApiDetailImpl.neon.h>

namespace nn {
namespace util {
namespace neon {

void MatrixIdentity(MatrixColumnMajor4x3fType* pOutValue) {
    pOutValue->_m = {{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
    }};
}

void MatrixMultiply(MatrixColumnMajor4x3fType* pOutValue, const MatrixColumnMajor4x3fType& matrix1,
                    const MatrixColumnMajor4x3fType& matrix2) {
    pOutValue->_m = detail::MatrixColumnMajor4x3fMultiply(matrix1._m, matrix2._m);
}

}  // namespace neon
}  // namespace util
}  // namespace nn
