#include <nn/ui2d/ui2d_Bounding.h>

#include <nn/ui2d/ui2d_Resources.h>

namespace nn {
namespace ui2d {

Bounding::Bounding(const ResBounding* pBaseBlock, const ResBounding* pOverrideBlock,
                   const BuildArgSet& buildArgSet)
    : Pane(pBaseBlock, buildArgSet) {
    NN_UNUSED(pOverrideBlock);
}

Bounding::Bounding(const Bounding& bounding) : Pane(bounding) {}

Bounding::~Bounding() {}

void Bounding::DrawSelf(DrawInfo& drawInfo, gfx::CommandBuffer& commandBuffer) {
    NN_UNUSED(drawInfo);
    NN_UNUSED(commandBuffer);
}

bool Bounding::CompareCopiedInstanceTest(const Bounding& target) const {
    NN_UNUSED(target);
    return true;
}

}  // namespace ui2d
}  // namespace nn
