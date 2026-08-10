#include <memory>
#include <nn/atk/fnd/basis/atkfnd_HeapBase.h>

namespace nn::atk::detail::fnd {
namespace {
    HeapBase::HeapList* sRootList;
    u32 sFillVals[HeapBase::FillType_Max] { 0xC3C3C3C3, 0xF3F3F3F3, 0xD3D3D3D3 };
}
} // namespace nn::atk::detail::fnd