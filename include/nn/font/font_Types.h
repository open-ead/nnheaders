#pragma once

#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_Types.h>

namespace nn {
namespace font {

typedef bool (*RegisterTextureViewSlot)(gfx::DescriptorSlot*, const gfx::TextureView&, void*);
typedef bool (*RegisterSamplerSlot)(gfx::DescriptorSlot*, const gfx::Sampler&, void*);
typedef void (*UnregisterTextureViewSlot)(gfx::DescriptorSlot*, const gfx::TextureView&, void*);
typedef void (*UnregisterSamplerSlot)(gfx::DescriptorSlot*, const gfx::Sampler&, void*);

class Rectangle {
public:
    float left;
    float top;
    float right;
    float bottom;

    Rectangle();
    Rectangle(float, float, float, float);
    Rectangle(const Rectangle&);
    ~Rectangle();
    float GetWidth() const;
    float GetHeight() const;
    float GetX() const;
    float GetY() const;
    void SetWidth(float);
    void SetHeight(float);
    void MoveTo(float, float);
    void Move(float, float);
    void SetEdge(float, float, float, float);
    void SetOriginAndSize(float, float, float, float);
    void SetCenterAndSize(float, float, float, float);
    void Normalize();
};

namespace detail {

enum TextColor { TextColor_Start, TextColor_End, TextColor_MaxTextColor };

}
}  // namespace font
}  // namespace nn
