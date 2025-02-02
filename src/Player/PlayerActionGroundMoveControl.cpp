#include "Player/PlayerActionGroundMoveControl.h"
#include "Library/Collision/CollisionUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionFunction.h"
#include "Player/PlayerActionTurnControl.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "Util/Hack.h"
#include "Util/JudgeUtil.h"
#include "math/seadVectorFwd.h"

PlayerActionGroundMoveControl::PlayerActionGroundMoveControl(al::LiveActor* player,
                                                             PlayerConst const* pConst,
                                                             PlayerInput const* input,
                                                             IUsePlayerCollision const* collision)
    : mActor(player), mConst(pConst), mInput(input), mCollision(collision),
      mTurnControl(new PlayerActionTurnControl(player)) {}

void PlayerActionGroundMoveControl::initDash(IJudge*, float, int) {
    CRASH
}

void PlayerActionGroundMoveControl::setupDash(float, int) {
    CRASH
}

void PlayerActionGroundMoveControl::setup(float a2, float a3, int a4, int a5, int a6, float a7,
                                          float a8, int a9) {
    someFlag = 1;
    mMaxSpeed = a2;
    mMinSpeed = a3;
    mAccelFrame = a4;
    mStickOnBrakeFrame = a5;
    mNormalBrakeFrame = a6;
    mGravity = a7;
    _80 = 0;
    mBrakeOnSpeed = a8;
    mBrakeOnCounterCorner = a9;
}

void PlayerActionGroundMoveControl::appear() {
    sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalExceptJustLandOrGravityDir(&a3, mActor, mCollision);
    rs::calcGroundNormalOrGravityDir(&mGroundNormal, mActor, mCollision);
    someVec = mGroundNormal;
    sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
    al::alongVectorNormalH(&a1, al::getVelocity(mActor), a3, mGroundNormal);
    sead::Vector3f v12 = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&v12, mActor);
    al::verticalizeVec(&v12, mGroundNormal, v12);
    al::tryNormalizeOrZero(&v12);
    al::setVelocity(mActor, a1 - (mGravity * mGroundNormal));
    someVec = mGroundNormal;
    _7C = 0.0;
    calcInitBrakeOnCounter();
    mTurnControl->reset();
    _C0 = 0;
}

void PlayerActionGroundMoveControl::reset(sead::Vector3f const& a2) {
    mGroundNormal = a2;
    someVec = a2;
    _7C = 0.0;
    calcInitBrakeOnCounter();
    mTurnControl->reset();
    _C0 = 0;
}

void PlayerActionGroundMoveControl::calcInitBrakeOnCounter() {
    float v2;           // s8
    sead::Vector3f v3;  // [xsp+0h] [xbp-40h] BYREF
    sead::Vector3f a1;  // [xsp+10h] [xbp-30h] BYREF

    this->_60 = 0;
    updateNormalAndSnap(&a1);
    v2 = a1.length();
    if ((rs::isOnGroundSkateCode(this->mActor, this->mCollision) && !al::isNearZero(v2, 0.001)) ||
        (v2 >= this->mBrakeOnSpeed && al::tryNormalizeOrZero(&a1) &&
         (al::calcFrontDir(&v3, this->mActor), al::verticalizeVec(&v3, this->mGroundNormal, v3),
          al::tryNormalizeOrZero(&v3)) &&
         !PlayerActionFunction::isOppositeDir(v3, a1)))
        this->_60 = this->mBrakeOnCounterCorner;
}

f32 PlayerActionGroundMoveControl::update() {
    if (rs::isOnGroundSkateCode(mActor, mCollision))
        return updateSkateMove();
    else
        return updateNormalMove();
}

f32 PlayerActionGroundMoveControl::updateSkateMove() {
    CRASH
}

void sub_7100418AEC(sead::Vector3f* x0_0, const sead::Vector3f& a2, const IUsePlayerCollision* a3,
                    const sead::Vector3f& a4) {
    if (!rs::isCollidedWallFace(a3))
        return;

    const sead::Vector3f& CollidedWallNormal = rs::getCollidedWallNormal(a3);
    sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
    al::verticalizeVec(&a1, CollidedWallNormal, *x0_0);
    sead::Vector3f v15 = {0.0f, 0.0f, 0.0f};
    f32 v11 = a4.dot(a1);
    al::verticalizeVec(&v15, CollidedWallNormal, a2);
    f32 v12 = v11 + (a4.dot(a2) * 0.9f);
    if (al::isNearZeroOrGreater(v12, 0.001f))
        *x0_0 -= a4 * v12;
}

f32 PlayerActionGroundMoveControl::updateNormalMove() {
    float v3;
    float v9;
    float v13;
    float v14;
    int v15;
    int v16;
    float v18;
    bool v24;
    bool v25;
    bool v36;
    const sead::Vector3f* v37;
    bool v43;
    bool v44;
    bool va68;
    bool v48;
    float A0;
    float v50;
    float maxSpeed;
    bool v52;
    bool v53;
    float v54;
    float v60;
    bool v66;
    bool v67;
    float v69;
    float C0;
    float v71;
    float v83;
    float minSpeed;
    float v85;
    float v118;
    sead::Vector3f a2;
    sead::Vector3f va50;
    sead::Vector3f v124;
    sead::Vector3f a1;
    sead::Vector3f v126;
    sead::Vector3f a3;
    sead::Vector3f v128;

    if (mJudgeStartDash)
        rs::updateJudge(mJudgeStartDash);

    this->_64 = 0;
    this->_80 = 0;
    this->_BC = 0;
    v128 = {0.0f, 0.0f, 0.0f};
    updateNormalAndSnap(&v128);
    v3 = v128.length();
    if (v3 < 0.1f)
        v128 = {0.0f, 0.0f, 0.0f};

    a3 = {0.0f, 0.0f, 0.0f};
    al::tryNormalizeOrZero(&a3, v128);
    v9 = sead::Mathf::clamp(
        1.0f - al::calcRate01(
                   v3, this->mConst->getDashBorderSpeed() - this->mConst->getDashBlendRange(),
                   this->mConst->getDashBorderSpeed() + this->mConst->getDashBlendRange()),
        0.0f, 1.0f);
    v13 = al::lerpValue(this->mConst->getRoundLimitDegreeMin(), this->mConst->getRoundLimitDegree(),
                        v9);
    v14 = this->mConst->getRoundFastDegree();
    v15 = this->mConst->getRoundAccelFrameFast();
    v16 = this->mConst->getRoundBrakeFrame();
    if (this->_9C) {
        v18 = this->mConst->getRoundLimitDegreeForce();
        v13 = this->mConst->getRoundLimitDegreeForceFast();
        v14 = this->mConst->getRoundFastDegreeForce();
        v15 = this->mConst->getRoundAccelFrameForceFast();
        v16 = this->mConst->getRoundBrakeFrameForce();
    } else {
        v18 = v13;
    }

    if (this->someFlags[2])
        v13 = 0.0;

    if (this->someFlags[2])
        v18 = 0.0;

    mTurnControl->setup(this->mConst->getRoundMinDegree(), v14, v18, v13,
                        this->mConst->getRoundAccelFrame(), v15, v16);
    this->mTurnControl->_88 = this->someFlags[1] || this->_9C;
    v126 = {0.0f, 0.0f, 0.0f};
    if (_20)
        rs::calcHackerMoveVec(&v126, *_20, this->mGroundNormal);
    else
        mInput->calcMoveInput(&v126, this->mGroundNormal);

    this->_84 = v126;
    a1 = {0.0f, 0.0f, 0.0f};
    v124 = {0.0f, 0.0f, 0.0f};
    v24 = al::tryNormalizeOrZero(&a1, v126);
    va50 = {0.0f, 0.0f, 0.0f};
    v25 = this->someFlags[0];
    mTurnControl->update(v126, this->mGroundNormal);

    sead::Vector3f xyz;
    if (v25 || !v24) {
        xyz = this->mTurnControl->_5c;
        v124 = xyz;
        va50 = xyz;
    } else {
        xyz = a1;
        v124 = a1;
        va50 = this->mTurnControl->_50;
    }

    if (this->_9C) {
        if (v24) {
            v126 = a1;
            v24 = 1;
        } else {
            v24 = 1;
            v126 = xyz;
            a1 = xyz;
        }
    }

    v118 = v3;
    sead::Vector3f v33v;
    if (rs::isCollidedWall(this->mCollision))
        v33v = rs::getCollidedWallNormal(this->mCollision);
    else
        v33v = sead::Vector3f::zero;

    v36 = al::isNearZero(va50, 0.001f);
    v43 = 0;
    v44 = !v24;
    if (!v36 && !v44)
        v43 = PlayerActionFunction::isOppositeDir(a1, va50) && !this->_9C;

    va68 = this->mTurnControl->_68;
    v48 = this->_D5 && !this->_20 && mInput->isHoldSquat();
    A0 = v118;
    v50 = va50.dot(a3);
    maxSpeed = 0.0;
    v52 = v33v.dot(a3) < -0.17365f && !v48;
    updateHillAffect(*v37, v124, v24);
    v53 = va68 | v44;
    if (v50 >= 0.0 && !(va68 | v44)) {
        v54 = v126.length();

        v60 = calcMaxSpeed(v118);

        maxSpeed = (float)(v54 * v60) + (float)((float)(1.0f - v54) * this->mMinSpeed);
        if (v52 && maxSpeed > this->mMaxSpeed)
            maxSpeed = this->mMaxSpeed;

        A0 = PlayerActionFunction::accel(v118, maxSpeed,
                                         PlayerActionGroundMoveControl::calcAccelRate(v118));
    }

    if (A0 <= this->mBrakeOnSpeed) {
        if (_60 < mBrakeOnCounterCorner)
            this->_60 = 0;
    } else {
        this->_60 = sead::Mathf::clampMax(this->_60 + 1, this->mBrakeOnCounterCorner);
    }

    f32 mNormalBrakeFrame = this->mNormalBrakeFrame;
    if (v52)
        goto LABEL_76;

    if (v50 < 0.0 && v43)
        goto LABEL_75;

    if (!(v44 || (v50 >= 0.0)))
        goto LABEL_76;

    if (v43 || v44)
        if (!(_60 >= mBrakeOnCounterCorner || v48))
        LABEL_76:
            mNormalBrakeFrame = 1;

        else
        LABEL_75:;
            else mNormalBrakeFrame = this->mStickOnBrakeFrame;

    if (A0 > maxSpeed) {
        A0 = sead::Mathf::max(PlayerActionFunction::brake(A0, mNormalBrakeFrame, this->mMaxSpeed),
                              maxSpeed);
    }

    if (this->_9C && A0 < this->_A0)
        A0 = this->_A0;

    if (v53 && !v36 && !v48 && this->_60 >= this->mBrakeOnCounterCorner) {
        updatePoseUpFront(this->mGroundNormal, va50, A0);
        al::setVelocity(mActor, (A0 * va50) - (this->mGravity * this->mGroundNormal));
        this->_64 = 1;
        _90 = va50;
        return A0;
    }

    if (this->someFlags[0] && v43 && !v36 && (v50 < 0.0 || A0 <= this->mMinSpeed)) {
        updatePoseUpFront(this->mGroundNormal, va50, A0);
        al::setVelocity(this->mActor, (A0 * a3) - (this->mGravity * this->mGroundNormal));
        this->_BC = 1;
        _90 = va50;
        return A0;
    }

    v66 = al::isNearZero(A0, 0.001);
    v67 = !v24;
    if (!v66)
        v67 = 0;

    this->_80 = v67;
    if (this->_BD) {
        if (al::isNormalize(a3, 0.001f)) {
            v69 = sead::Mathf::abs(mTurnControl->calcTurnPowerRate(this->mGroundNormal));

            C0 = this->_C0;
            v71 = sead::Mathf::clamp(v69 - C0, 0.0f, 1.0f);

            this->_C0 = al::lerpValue(C0, v69, 0.15f);
            a2 = {0.0f, 0.0f, 0.0f};
            al::verticalizeVec(&a2, v124, a3);
            if (al::tryNormalizeOrZero(&a2)) {
                v83 = al::lerpValue(0.3f, 0.9f, v71);
                minSpeed = A0 * v124.dot(a3);
                if (minSpeed < this->mMinSpeed)
                    minSpeed = this->mMinSpeed;

                v85 = a3.dot(a2);
                sead::Vector3f v121 = (v124 * minSpeed) + (v83 * (A0 * (a2 * v85)));
                f32 v121_len = v121.length();
                if (v121_len > 0.0f) {
                    f32 factor = A0 / v121_len;
                    v121 *= factor;
                }

                sub_7100418AEC(&v121, this->mGroundNormal * -this->mGravity, mCollision,
                               -al::getGravity(this->mActor));
                al::setVelocity(this->mActor, v121 - (this->mGravity * this->mGroundNormal));
                goto LABEL_110;
            }
        } else {
            this->_C0 = 0.0;
        }
    }

    a2 = A0 * v124;
    sub_7100418AEC(&a2, this->mGroundNormal * -this->mGravity, this->mCollision,
                   -al::getGravity(this->mActor));
    al::setVelocity(this->mActor, a2 - (this->mGravity * this->mGroundNormal));

LABEL_110:
    a2 = this->mGroundNormal;
    if (this->_BB)
        this->mTurnControl->calcTilt(&a2, this->mGroundNormal,
                                     this->mConst->getTiltPoseDegreeMax());

    updatePoseUpFront(a2, v124, A0);
    this->_90 = v124;

    return A0;
}

void PlayerActionGroundMoveControl::updateNormalAndSnap(sead::Vector3f* a2) {
    sead::Vector3f* p_mGravityDir;
    sead::Vector3f v19;
    sead::Vector3f a2a;
    sead::Vector3f a3;

    p_mGravityDir = &this->mGroundNormal;
    a3 = this->mGroundNormal;
    if (!this->_C4 || rs::isCollidedGround(this->mCollision)) {
        rs::calcGroundNormalOrGravityDir(p_mGravityDir, this->mActor, this->mCollision);
        this->someVec = this->mGroundNormal;
        goto LABEL_5;
    }

    this->mGroundNormal = -al::getGravity(mActor);
    a2a = {0.0f, 0.0f, 0.0f};
    v19 = {0.0f, 0.0f, 0.0f};

    if (alCollisionUtil::getHitPosAndNormalOnArrow(mActor, &a2a, &v19, al::getTrans(this->mActor),
                                                   al::getGravity(this->mActor) * 100.0f, 0LL,
                                                   0LL)) {
        if (al::isFloorPolygon(v19, al::getGravity(this->mActor))) {
            if (someVec.dot(v19) <= 0.087156) {
                sead::Vector3f* VelocityPtr = al::getVelocityPtr(this->mActor);
                al::limitLength(VelocityPtr, *VelocityPtr, al::calcSpeedH(this->mActor));
            } else {
                al::turnVecToVecRate(p_mGravityDir, a3, v19, 0.5);
                this->someVec = this->mGroundNormal;
            }
        }
    }

LABEL_5:
    al::alongVectorNormalH(a2, al::getVelocity(this->mActor), a3, *p_mGravityDir);
}

f32 PlayerActionGroundMoveControl::calcTurnTiltRate() const {
    if (rs::isOnGroundSkateCode(this->mActor, this->mCollision))
        return 0.0;
    return mTurnControl->calcTurnPowerRate(this->mGroundNormal);
}

void PlayerActionGroundMoveControl::calcMoveInput(sead::Vector3f*, sead::Vector3f const&) {
    CRASH
}

bool PlayerActionGroundMoveControl::isActiveSquatBrake() const {
    CRASH
}

void PlayerActionGroundMoveControl::updateHillAffect(sead::Vector3f const& a2,
                                                     sead::Vector3f const& a3, bool a4) {
    float v9;
    float v10;
    float v13;
    float v15;
    float v16;
    float v17;
    bool v18;
    float v20;
    int v23;
    float v25;
    float v26;
    bool v27;
    float v30;
    sead::Vector3f v34;

    auto* a1 = this;

    if (!a1->_78)
        return;

    v9 = sead::Mathf::clamp((al::getGravity(a1->mActor)).dot(-a1->mGroundNormal), -1.0, 1.0);

    v10 = sead::Mathf::acos(v9);
    v34 = {0.0f, 0.0f, 0.0f};
    al::verticalizeVec(&v34, a1->mGroundNormal, al::getGravity(a1->mActor));
    if (al::tryNormalizeOrZero(&v34)) {
        v13 = sead::Mathf::rad2deg(v10);
        v15 = sead::Mathf::clamp(v13 / a1->mConst->getStandAngleMin(), 0.0, 1.0);

        v16 = 0.0;
        if (a4 && (v16 = v34.dot(a3), v16 > 0.0))
            v17 = (v16 * al::easeOut(v15)) + 0.0;
        else
            v17 = 0.0 - al::easeIn(v15);

        v20 = sead::Mathf::clampMin(v13 * v16, 0.0);
        v18 = v17 > 0.0;
        if (v17 > 0.0 && _7C < 0.0)
            _7C = 0.0;
    } else {
        v17 = 0.0;
        v18 = 0;
        v20 = 0.0;
    }

    v23 = mConst->getHillAccelAddFrame();
    if (_7C > v17) {
        v25 = sead::Mathf::clamp(v17 + 1.0, 0.0, 1.0);

        v26 = v25 * (mConst->getHillAccelSubFrame() - 1);
        if ((int)v26 == v26)
            v27 = 0;
        else
            v27 = v26 >= 0.0;

        v23 = (int)v26 + v27 + 1;
        if (v18) {
            v30 = al::calcRate01(v20, mConst->getHillAccelSubAngleMin(),
                                 mConst->getHillAccelSubAngleMax());
            v17 = al::lerpValue(v17, _7C, al::easeOut(al::easeOut(v30)));
        }
    }

    _7C = al::converge(_7C, v17, 1.0 / (float)v23);
}

f32 PlayerActionGroundMoveControl::calcMaxSpeed(float a2) const {
    float v6;
    bool v7;
    char isJudge;

    if (this->_78)
        v6 = sead::Mathf::abs(this->_7C) * this->mConst->getHillAddSpeed();
    else
        v6 = 0.0;

    if (this->mJudgeStartDash &&
        (v7 = al::isNearZeroOrLess((float)(this->mMaxSpeed * 0.99) - a2, 0.001),
         isJudge = rs::isJudge(this->mJudgeStartDash), v7) &&
        (isJudge & 1) != 0) {
        return v6 + this->mDashMaxSpeed2D;
    } else {
        return PlayerActionFunction::accel(v6 + this->mMaxSpeed, this->somethingAboutSpeed2,
                                           this->somethingAboutSpeed);
    }
}

f32 PlayerActionGroundMoveControl::calcAccelRate(float a2) const {
    IJudge* l_mJudgeStartDash;
    bool v6;
    bool isJudge;
    float v11;
    float v12;
    float v13;

    l_mJudgeStartDash = mJudgeStartDash;
    if ((l_mJudgeStartDash &&
         !((mMaxSpeed - a2 <= 0.0) ?
               (IJudge*)(v6 = 0) :
               (v6 = !al::isNearZero(mMaxSpeed - a2, 0.001f), l_mJudgeStartDash = mJudgeStartDash),
           (isJudge = rs::isJudge(l_mJudgeStartDash), v6) || !isJudge)))
        return (mDashMaxSpeed2D - mMaxSpeed) / mDashAccelFrame2D;

    if (!_D4 || !(mMaxSpeed > a2))
        return mMaxSpeed / mAccelFrame;

    v11 = (mAccelFrame * mAccelFrame);
    v12 = (mMaxSpeed / mAccelFrame) * mConst->getRunAccelAverageScale();
    v13 = ((mMaxSpeed - (v12 * mAccelFrame)) * 2) / v11;
    return v12 + (v13 * (((sqrtf((v12 * v12) + ((v13 + v13) * a2)) - v12) / v13) + 1.0f));
}

void PlayerActionGroundMoveControl::updatePoseUpFront(sead::Vector3f const& a2,
                                                      sead::Vector3f const& a3, float a4) {
    float v7;
    sead::Vector3f v11;

    v11 = a3;
    if (al::isParallelDirection(a2, v11, 0.01)) {
        al::calcFrontDir(&v11, this->mActor);
        if (al::isParallelDirection(a2, v11, 0.01))
            al::calcUpDir(&v11, this->mActor);
    }

    if (this->_A4)
        v7 = al::calcRate01(a4, this->_A8, this->_AC);
    else
        v7 = 1.0;

    rs::slerpUpFront(this->mActor, a2, v11, this->mConst->getSlerpQuatRate(),
                     v7 * this->mConst->getHillPoseDegreeMax());
}
