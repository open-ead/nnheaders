/**
 * @file Parts.h
 * @brief Layout parts.
 */

#pragma once

#include <nn/ui2d/ui2d_Pane.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace ui2d {

struct BuildArgSet;
struct ResParts;
class Layout;
class Material;

class Parts : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane);

    Parts();
    Parts(const ResParts*, const ResParts*, const BuildArgSet&);
    Parts(const Parts&);

    virtual ~Parts();

    Layout* GetLayout() const { return m_pLayout; }
    void SetLayout(Layout* pLayout) { m_pLayout = pLayout; }

    virtual Pane* FindPaneByNameRecursive(const char*);
    virtual const Pane* FindPaneByNameRecursive(const char*) const;
    virtual Material* FindMaterialByNameRecursive(const char*);
    virtual const Material* FindMaterialByNameRecursive(const char*) const;
    bool CompareCopiedInstanceTest(const Parts&) const;

    util::IntrusiveListNode m_Link;

private:
    const Parts& operator=(const Parts&);

    Layout* m_pLayout;
};

}  // namespace ui2d
}  // namespace nn
