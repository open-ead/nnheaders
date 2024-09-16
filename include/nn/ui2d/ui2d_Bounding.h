#pragma once

#include <nn/ui2d/ui2d_Pane.h>

namespace nn {
namespace ui2d {

struct ResBounding;

class Bounding : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane);

    Bounding();
    Bounding(const Size&);
    Bounding(const ResBounding*, const ResBounding*, const BuildArgSet&);
    Bounding(const Bounding&);
    virtual ~Bounding();

    bool CompareCopiedInstanceTest(const Bounding&) const;

protected:
    virtual void DrawSelf(DrawInfo&, gfx::CommandBuffer&);

private:
    const Bounding& operator=(const Bounding&) = delete;
};

}  // namespace ui2d
}  // namespace nn
