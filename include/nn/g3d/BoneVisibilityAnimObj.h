#pragma once

#include <nn/g3d/AnimObj.h>
#include <nn/types.h>

namespace nn::g3d {

class BoneVisibilityAnimObj : public ModelAnimObj {
public:
    void ClearResult() override;
    void Calculate() override;
    s32 Bind(const ResModel*) override;
    s32 Bind(const ModelObj*) override;
    void BindFast(const ResModel*) override;
    void ApplyTo(ModelObj*) const override;

private:
    char filler_68[0x18];
};
static_assert(sizeof(BoneVisibilityAnimObj) == 0x80);

}  // namespace nn::g3d
