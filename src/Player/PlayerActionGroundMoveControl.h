#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class IUsePlayerHack;
class IJudge;
class PlayerActionTurnControl;

class PlayerActionGroundMoveControl {
public:
    PlayerActionGroundMoveControl(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                                  const IUsePlayerCollision*);

    void initDash(IJudge*, f32, s32);
    void setupDash(f32, s32);
    void setup(f32, f32, s32, s32, s32, f32, f32, s32);
    void appear();
    void reset(const sead::Vector3f&);
    void calcInitBrakeOnCounter();
    f32 update();
    f32 updateSkateMove();
    f32 updateNormalMove();
    void updateNormalAndSnap(sead::Vector3f*);
    f32 calcTurnTiltRate() const;
    void calcMoveInput(sead::Vector3f*, const sead::Vector3f&);
    bool isActiveSquatBrake() const;
    void updateHillAffect(const sead::Vector3f&, const sead::Vector3f&, bool);
    f32 calcMaxSpeed(f32) const;
    f32 calcAccelRate(f32) const;
    void updatePoseUpFront(const sead::Vector3f&, const sead::Vector3f&, f32);

    const sead::Vector3f& getGroundNormal() const { return mGravityDir; }

    void set_c4(bool c4) { _C4 = c4; }

public:
    al::LiveActor *mActor;
    const PlayerConst *mConst;
    const PlayerInput *mInput;
    const IUsePlayerCollision *mCollision;
    const IUsePlayerHack **_20 = 0;
    bool someFlag = 0;
    sead::Vector3f mGroundNormal = {0.0f, 0.0f, 0.0f};
    float mMaxSpeed = 0;
    float mMinSpeed = 0;
    float somethingAboutSpeed = 0;
    float somethingAboutSpeed2 = 0;
    int mAccelFrame = 0;
    int mStickOnBrakeFrame = 0;
    int mNormalBrakeFrame = 0;
    float mGravity = 0;
    float mBrakeOnSpeed = 0;
    int mBrakeOnCounterCorner = 0;
    int _60 = 0;
    bool _64 = 0;
    IJudge *mJudgeStartDash = 0;
    float mDashMaxSpeed2D = 0;
    int mDashAccelFrame2D = 0;
    bool _78 = 0;
    float _7C = 0;
    bool _80 = 0;
    sead::Vector3f _84 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _90 = {0.0f, 0.0f, 0.0f};
    bool _9C = 0;
    float _A0 = 0;
    bool _A4 = 0;
    float _A8 = 0;
    float _AC = 0;
    PlayerActionTurnControl *mTurnControl = 0;
    bool someFlags[3] = {false, false, false};
    bool _BB = 0;
    bool _BC = 0;
    bool _BD = 0;
    float _C0 = 0;
    bool _C4 = 0;
    sead::Vector3f someVec = {0.0f, 0.0f, 0.0f};
    bool _D4 = 0;
    bool _D5 = 0;
};

static_assert(sizeof(PlayerActionGroundMoveControl) == 0xD8);
