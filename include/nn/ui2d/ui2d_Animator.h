#pragma once

#include <nn/ui2d/ui2d_Animation.h>
#include <nn/util/util_BitPack.h>

namespace nn {
namespace ui2d {

class Animator : public AnimTransformBasic {
public:
    NN_RUNTIME_TYPEINFO(AnimTransformBasic);

    enum PlayType { PlayType_OneShot, PlayType_Loop, PlayType_RoundTrip };

    Animator();
    virtual ~Animator();
    virtual void Play(PlayType, float);
    virtual void PlayAuto(float);
    virtual void PlayFromCurrent(PlayType, float);
    virtual void StopAt(float);
    virtual void StopAtCurrentFrame();
    virtual void StopAtStartFrame();
    virtual void StopAtEndFrame();
    virtual void UpdateFrame(float);

    float GetSpeed() const { return m_Speed; }

    bool IsPlaying() const;
    bool IsMinFrame() const;
    bool IsStartFrame() const;
    bool IsMaxFrame() const;
    bool IsEndFrame() const;
    bool IsFrameMidway() const;

    PlayType GetPlayType() const { return m_PlayType; }

    bool IsTurnedMaxTrigger() const;
    bool IsTurnedMinTrigger() const;
    bool IsEndTrigger() const;

    // newer
    // virtual void Unbind();

    enum FlagMask {
        FlagMask_PlayStatusEnd = 1,
        FlagMask_PlayStatusTurnedMax = 2,
        FlagMask_PlayStatusTurnedMin = 4
    };

protected:
    void ClearPlayStatusFlag();

private:
    float m_Speed;
    PlayType m_PlayType;
    util::BitPack32 m_Flag;
};

class PaneAnimator : public Animator {
public:
    NN_RUNTIME_TYPEINFO(Animator);

    PaneAnimator() = default;
    void Setup(Pane*, bool);
    virtual void Unbind();

    Pane* GetPane() const { return m_pPane; }

private:
    Pane* m_pPane = nullptr;
};

class GroupAnimator : public Animator {
public:
    NN_RUNTIME_TYPEINFO(Animator);

    GroupAnimator() = default;
    void Setup(Group*, bool);
    void Setup(const AnimResource&, GroupContainer*, int, bool);
    virtual void Unbind();

    Group* GetGroup() const { return m_pGroup; }

private:
    Group* m_pGroup = nullptr;
};

class GroupArrayAnimator : public Animator {
public:
    NN_RUNTIME_TYPEINFO(Animator);

    GroupArrayAnimator() = default;
    void Setup(const AnimResource&, GroupContainer*, Group**, bool);
    virtual void Unbind();

    Group** GetGroups() const { return m_pGroupArray; }
    int GetGroupCount() const { return m_GroupCount; }

private:
    Group** m_pGroupArray = nullptr;
    int32_t m_GroupCount = 0;
};

}  // namespace ui2d
}  // namespace nn
