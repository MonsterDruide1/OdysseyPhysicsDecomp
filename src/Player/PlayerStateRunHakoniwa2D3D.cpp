#include "Player/PlayerStateRunHakoniwa2D3D.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Player/PlayerStateRunHakoniwa.h"
#include "PlayerUtil.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/JudgeUtil.h"

namespace {
NERVE_IMPL(PlayerStateRunHakoniwa2D3D, Run3D);
NERVE_IMPL(PlayerStateRunHakoniwa2D3D, GroundSpin3D);
NERVE_IMPL(PlayerStateRunHakoniwa2D3D, Run2D);

NERVE_MAKE(PlayerStateRunHakoniwa2D3D, Run3D);
NERVE_MAKE(PlayerStateRunHakoniwa2D3D, GroundSpin3D);
NERVE_MAKE(PlayerStateRunHakoniwa2D3D, Run2D);
}  // namespace

/* For Gym */
template <typename T> s32 getStateBitMap(u128*, int, T*);
template <>
s32 getStateBitMap<PlayerStateRunHakoniwa2D3D>(u128* state, int startOffset, PlayerStateRunHakoniwa2D3D* player) {
    constexpr int size = 3;
    if(startOffset+size >= 128) CRASH;
    *state <<= size;

    *state |= al::isNerve(player, &Run3D);
    *state |= al::isNerve(player, &GroundSpin3D) << 1;
    *state |= al::isNerve(player, &Run2D) << 2;

    int offset = startOffset+size;
    offset += getStateBitMap(state, offset, player->mStateRunHakoniwa);
    offset += getStateBitMap(state, offset, player->mStateGroundSpin);
    //offset += getStateBitMap(state, offset, player->mStateRun2D);

    if(player->isDead()) {
        u128 mask = ~0;
        mask <<= offset - startOffset;
        *state &= mask;
    }
    return offset - startOffset;
}

PlayerStateRunHakoniwa2D3D::PlayerStateRunHakoniwa2D3D(
    al::LiveActor* player, PlayerConst const* pConst, IUseDimension const* dimension,
    PlayerInput const* input, IUsePlayerCollision const* collider,
    al::WaterSurfaceFinder const* waterSurfaceFinder, PlayerCounterForceRun const* counterForceRun,
    PlayerCounterQuickTurnJump const* counterQuickTurnJump, PlayerTrigger* trigger,
    PlayerAnimator* animator, PlayerEffect* effect, PlayerJointParamCenterDynamics* jointDynamics,
    bool isMoon)
    : al::ActorStateBase("走り[2D3D]", player), mConst(pConst), mInput(input),
      mDimension(dimension), mCollision(collider), mWaterSurfaceFinder(waterSurfaceFinder),
      mCounterForceRun(counterForceRun) {
    mJudgeStartGroundSpin = new PlayerJudgeStartGroundSpin(player, collider, input);
    mJudgeWaterSurfaceRun =
        new PlayerJudgeWaterSurfaceRun(player, mConst, waterSurfaceFinder, counterForceRun);

    initNerve(&Run3D, 3);
    mStateRunHakoniwa = new PlayerStateRunHakoniwa(player, mConst, input, collider, counterForceRun,
                                                   counterQuickTurnJump, trigger, animator, effect,
                                                   jointDynamics, mJudgeWaterSurfaceRun, isMoon);
    mStateGroundSpin = new PlayerStateGroundSpin(player, collider, input, mConst, animator);
    mStateRun2D = new PlayerStateRun2D(player, mConst, input, collider, animator);

    al::initNerveState(this, mStateRunHakoniwa, &Run3D, "走り3D");
    al::initNerveState(this, mStateGroundSpin, &GroundSpin3D, "地上スピン");
    al::initNerveState(this, mStateRun2D, &Run2D, "走り2D");
}

PlayerStateRunHakoniwa2D3D::~PlayerStateRunHakoniwa2D3D() {
    CRASH
}

void PlayerStateRunHakoniwa2D3D::appear() {
    mIsDead = false;

    // something about the judgeWaterSurface

    if (rs::is2D(mDimension))
        al::setNerve(this, &Run2D);
    else if (rs::updateJudgeAndResult(mJudgeStartGroundSpin))
        al::setNerve(this, &GroundSpin3D);
    else
        al::setNerve(this, &Run3D);
}

bool PlayerStateRunHakoniwa2D3D::update() {
    // something about the judgeWaterSurface

    rs::updateJudge(mJudgeWaterSurfaceRun);
    if (al::isNerve(this, &Run2D)) {
        if (al::isNerve(this, &Run2D) && !rs::is2D(mDimension)) {
            if (rs::updateJudgeAndResult(mJudgeStartGroundSpin))
                al::setNerve(this, &GroundSpin3D);
            else
                al::setNerve(this, &Run3D);
        }
    } else if (rs::is2D(mDimension)) {
        al::setNerve(this, &Run2D);
    }

    return al::NerveStateBase::update();
}

bool PlayerStateRunHakoniwa2D3D::isEnableLookAt() const {
    CRASH
}

bool PlayerStateRunHakoniwa2D3D::isGroundSpin() const {
    return al::isNerve(this, &GroundSpin3D);
}

bool PlayerStateRunHakoniwa2D3D::isSpinClockwise() const {
    return mStateGroundSpin->mIsSpinClockwise;
}

bool PlayerStateRunHakoniwa2D3D::isRunDashFast() const {
    return al::isNerve(this, &Run3D) && mStateRunHakoniwa->isRunDashFast();
}

bool PlayerStateRunHakoniwa2D3D::isRunWaterSurface() const {
    return al::isNerve(this, &Run3D) && rs::isJudge(mJudgeWaterSurfaceRun);
}

bool PlayerStateRunHakoniwa2D3D::isBrake2D() const {
    return al::isNerve(this, &Run2D) && mStateRun2D->isBrake();
}

bool PlayerStateRunHakoniwa2D3D::tryTurnJump(IJudge* preInputJumpJudge, sead::Vector3<float>* vec) {
    return al::isNerve(this, &Run3D) && rs::isJudge(preInputJumpJudge) &&
           mStateRunHakoniwa->tryTurnJump(vec);
}

f32 PlayerStateRunHakoniwa2D3D::getTurnTiltRate() const {
    CRASH;
}

f32 PlayerStateRunHakoniwa2D3D::getCenterTiltRate() const {
    CRASH;
}

f32 PlayerStateRunHakoniwa2D3D::getInverseKinematicsRate() const {
    CRASH;
}

f32 PlayerStateRunHakoniwa2D3D::getCapDynamicsRate() const {
    CRASH;
}

void PlayerStateRunHakoniwa2D3D::exeRun3D() {
    if (al::updateNerveState(this)) {
        kill();
        return;
    }

    if (rs::isJudge(mJudgeWaterSurfaceRun)) {
        CRASH
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeStartGroundSpin)) {
        al::setNerve(this, &GroundSpin3D);
        return;
    }
}

void PlayerStateRunHakoniwa2D3D::exeGroundSpin3D() {
    if (al::updateNerveState(this)) {
        if (mInput->isMove())
            al::setNerve(this, &Run3D);
        else
            kill();
    }
}

void PlayerStateRunHakoniwa2D3D::exeRun2D() {
    CRASH
}
