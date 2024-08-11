#pragma once

#include <math/seadVector.h>

class PlayerAnimator;
class PlayerEffect;
class PlayerConst;
class IJudge;

class PlayerAnimControlRun {
public:
    PlayerAnimControlRun(PlayerAnimator*, PlayerConst const*, IJudge const*, PlayerEffect*, bool);
    void reset(float, bool);
    bool isAnimDashFast() const;
    void update(float, sead::Vector3f const&);

private:
    PlayerAnimator* mAnimator;
    PlayerEffect* mEffect;
    const PlayerConst* mConst;
    const IJudge* mJudgeWaterSurfaceRun;
    bool _20 = false;
    int _24 = 0;
    int _28 = 0;
    float _2c = 0.0;
    bool _30;
    const char* mMoveAnimName;

    f32 customDashBlendWeight = 0.0f;
};
