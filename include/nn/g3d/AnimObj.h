#pragma once

#include <nn/types.h>

namespace nn::g3d {
class AnimFrameCache;
class ModelObj;
class ResModel;

class AnimBindTable {
public:
    void Initialize(u32*, s32);
    void ClearAll(s32);
    void BindAll(u16);

private:
    u32* _0;
    s16 _8;
    s16 _a;
    s16 _c;
    s16 _e;
};
static_assert(sizeof(AnimBindTable) == 0x10);

class AnimFrameCtrl {
public:
    using PlayFunc = f32 (*)(f32, f32, f32, void*);

    void Initialize(f32, f32, PlayFunc);
    static f32 PlayOneTime(f32, f32, f32, void*);
    static f32 PlayLoop(f32, f32, f32, void*);

    void update() { mFrame = mPlayFunc(mFrame + mFrameRate, _4, mFrameMax, _18); }

    f32 getFrame() const { return mFrame; }

    void setFrame(f32 frame) { mFrame = mPlayFunc(frame, _4, mFrameMax, _18); }

    f32 getFrameMax() const { return mFrameMax; }

    f32 getFrameRate() const { return mFrameRate; }

    void setFrameRate(f32 rate) { mFrameRate = rate; }

    bool isEnd() const { return mFrameMax <= mFrame; }

    bool isOneTime() const { return mPlayFunc == PlayOneTime; }

private:
    f32 mFrame;
    f32 _4;
    f32 mFrameMax;
    f32 mFrameRate;
    PlayFunc mPlayFunc;
    void* _18;
};
static_assert(sizeof(AnimFrameCtrl) == 0x20);

class AnimContext {
public:
    void Initialize(AnimFrameCache*, s32);

private:
    AnimFrameCache* mAnimFrameCache;
    s32 _8;
    s32 _c;
    s32 _10;
};
static_assert(sizeof(AnimContext) == 0x18);

class AnimObj {
public:
    enum class BindFlag;

    virtual ~AnimObj();

    virtual void ClearResult();
    virtual void Calculate();
    virtual s32 Bind(const ResModel*);
    virtual s32 Bind(const ModelObj*);
    virtual void BindFast(const ResModel*);
    virtual void ApplyTo(ModelObj*) const;

    void ResetFrameCtrl(s32, bool);

    AnimFrameCtrl* getFrameCtrlPtr() const { return mFrameCtrlPtr; }

private:
    AnimFrameCtrl* mFrameCtrlPtr;
    AnimFrameCtrl mFrameCtrl;
    AnimContext mAnimContext;
    void* _48;
    void* _50;
};
static_assert(sizeof(AnimObj) == 0x58);

class ModelAnimObj : public AnimObj {
public:
    void SetBindFlagImpl(s32, AnimObj::BindFlag);
    const AnimObj::BindFlag& GetBindFlagImpl(s32) const;

private:
    AnimBindTable mBindTable;
};
static_assert(sizeof(ModelAnimObj) == 0x68);

}  // namespace nn::g3d
