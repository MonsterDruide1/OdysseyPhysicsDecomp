#include "Player/PlayerActorHakoniwa.h"
#include "Library/Area/AreaObjDirector.h"
#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorKeeper.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/ComboCounter.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/Thread/FunctorV0M.h"
#include "Library/stuff.h"
#include "Player/HackCap.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerCollider.h"
#include "Player/PlayerColliderHakoniwa.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerInputFunction.h"
#include "Player/PlayerJudgePreInputJump.h"
#include "Player/PlayerJumpMessageRequest.h"
#include "Player/PlayerPainPartsKeeper.h"
#include "Player/PlayerStateFallHakoniwa.h"
#include "Player/PlayerStateJump.h"
#include "Player/PlayerStateRunHakoniwa2D3D.h"
#include "Player/PlayerTrigger.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "System/GameDataFunction.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/JudgeUtil.h"
#include "math/seadQuat.h"
#include "playerUtil.h"

namespace {

NERVE_IMPL(PlayerActorHakoniwa, Wait);
NERVE_IMPL(PlayerActorHakoniwa, Squat);
NERVE_IMPL(PlayerActorHakoniwa, Run);
NERVE_IMPL(PlayerActorHakoniwa, Slope);
NERVE_IMPL(PlayerActorHakoniwa, Rolling);
NERVE_IMPL(PlayerActorHakoniwa, SpinCap);
NERVE_IMPL(PlayerActorHakoniwa, Jump);
NERVE_IMPL(PlayerActorHakoniwa, CapCatchPop);
NERVE_IMPL(PlayerActorHakoniwa, WallAir);
NERVE_IMPL(PlayerActorHakoniwa, WallCatch);
NERVE_IMPL(PlayerActorHakoniwa, GrabCeil);
NERVE_IMPL(PlayerActorHakoniwa, PoleClimb);
NERVE_IMPL(PlayerActorHakoniwa, HipDrop);
NERVE_IMPL(PlayerActorHakoniwa, HeadSliding);
NERVE_IMPL(PlayerActorHakoniwa, LongJump);
NERVE_IMPL(PlayerActorHakoniwa, SandSink);
NERVE_IMPL(PlayerActorHakoniwa, SandGeyser);
NERVE_IMPL(PlayerActorHakoniwa, Rise);
NERVE_IMPL(PlayerActorHakoniwa, Swim);
NERVE_IMPL(PlayerActorHakoniwa, Damage);
NERVE_IMPL(PlayerActorHakoniwa, DamageSwim);
NERVE_IMPL(PlayerActorHakoniwa, DamageFire);
NERVE_IMPL(PlayerActorHakoniwa, Press);
NERVE_IMPL(PlayerActorHakoniwa, Hack);
NERVE_IMPL(PlayerActorHakoniwa, EndHack);
NERVE_IMPL(PlayerActorHakoniwa, Bind);
NERVE_IMPL(PlayerActorHakoniwa, Camera);
NERVE_IMPL(PlayerActorHakoniwa, Abyss);
NERVE_IMPL(PlayerActorHakoniwa, Demo);
NERVE_IMPL(PlayerActorHakoniwa, Dead);
NERVE_IMPL(PlayerActorHakoniwa, Fall);

NERVE_MAKE(PlayerActorHakoniwa, Wait);
NERVE_MAKE(PlayerActorHakoniwa, Squat);
NERVE_MAKE(PlayerActorHakoniwa, Run);
NERVE_MAKE(PlayerActorHakoniwa, Slope);
NERVE_MAKE(PlayerActorHakoniwa, Rolling);
NERVE_MAKE(PlayerActorHakoniwa, SpinCap);
NERVE_MAKE(PlayerActorHakoniwa, Jump);
NERVE_MAKE(PlayerActorHakoniwa, CapCatchPop);
NERVE_MAKE(PlayerActorHakoniwa, WallAir);
NERVE_MAKE(PlayerActorHakoniwa, WallCatch);
NERVE_MAKE(PlayerActorHakoniwa, GrabCeil);
NERVE_MAKE(PlayerActorHakoniwa, PoleClimb);
NERVE_MAKE(PlayerActorHakoniwa, HipDrop);
NERVE_MAKE(PlayerActorHakoniwa, HeadSliding);
NERVE_MAKE(PlayerActorHakoniwa, LongJump);
NERVE_MAKE(PlayerActorHakoniwa, SandSink);
NERVE_MAKE(PlayerActorHakoniwa, SandGeyser);
NERVE_MAKE(PlayerActorHakoniwa, Rise);
NERVE_MAKE(PlayerActorHakoniwa, Swim);
NERVE_MAKE(PlayerActorHakoniwa, Damage);
NERVE_MAKE(PlayerActorHakoniwa, DamageSwim);
NERVE_MAKE(PlayerActorHakoniwa, DamageFire);
NERVE_MAKE(PlayerActorHakoniwa, Press);
NERVE_MAKE(PlayerActorHakoniwa, Hack);
NERVE_MAKE(PlayerActorHakoniwa, EndHack);
NERVE_MAKE(PlayerActorHakoniwa, Bind);
NERVE_MAKE(PlayerActorHakoniwa, Camera);
NERVE_MAKE(PlayerActorHakoniwa, Abyss);
NERVE_MAKE(PlayerActorHakoniwa, Demo);
NERVE_MAKE(PlayerActorHakoniwa, Dead);
NERVE_MAKE(PlayerActorHakoniwa, Fall);

}  // namespace

/* For Gym */
template <typename T> s32 getStateBitMap(u128*, int, T*);
template <>
s32 getStateBitMap<PlayerActorHakoniwa>(u128* state, int startOffset, PlayerActorHakoniwa* player) {
    constexpr int size = 31;
    if(startOffset+size >= 128) CRASH;
    *state <<= size;

    *state |= al::isNerve(player, &Wait);
    *state |= al::isNerve(player, &Squat) << 1;
    *state |= al::isNerve(player, &Run) << 2;
    *state |= al::isNerve(player, &Slope) << 3;
    *state |= al::isNerve(player, &Rolling) << 4;
    *state |= al::isNerve(player, &SpinCap) << 5;
    *state |= al::isNerve(player, &Jump) << 6;
    *state |= al::isNerve(player, &CapCatchPop) << 7;
    *state |= al::isNerve(player, &WallAir) << 8;
    *state |= al::isNerve(player, &WallCatch) << 9;
    *state |= al::isNerve(player, &GrabCeil) << 10;
    *state |= al::isNerve(player, &PoleClimb) << 11;
    *state |= al::isNerve(player, &HipDrop) << 12;
    *state |= al::isNerve(player, &HeadSliding) << 13;
    *state |= al::isNerve(player, &LongJump) << 14;
    *state |= al::isNerve(player, &SandSink) << 15;
    *state |= al::isNerve(player, &SandGeyser) << 16;
    *state |= al::isNerve(player, &Rise) << 17;
    *state |= al::isNerve(player, &Swim) << 18;
    *state |= al::isNerve(player, &Damage) << 19;
    *state |= al::isNerve(player, &DamageSwim) << 20;
    *state |= al::isNerve(player, &DamageFire) << 21;
    *state |= al::isNerve(player, &Press) << 22;
    *state |= al::isNerve(player, &Hack) << 23;
    *state |= al::isNerve(player, &EndHack) << 24;
    *state |= al::isNerve(player, &Bind) << 25;
    *state |= al::isNerve(player, &Camera) << 26;
    *state |= al::isNerve(player, &Abyss) << 27;
    *state |= al::isNerve(player, &Demo) << 28;
    *state |= al::isNerve(player, &Dead) << 29;
    *state |= al::isNerve(player, &Fall) << 30;

    int offset = startOffset+size;
    offset += getStateBitMap(state, offset, player->mStateWait);
    offset += getStateBitMap(state, offset, player->mStateSquat);
    offset += getStateBitMap(state, offset, player->mStateRunHakoniwa2D3D);
    offset += getStateBitMap(state, offset, player->mStateSlope);
    offset += getStateBitMap(state, offset, player->mStateRolling);
    offset += getStateBitMap(state, offset, player->mStateSpinCap);
    offset += getStateBitMap(state, offset, player->mStateJump);
    //offset += getStateBitMap(state, offset, player->mStateCapCatchPop);
    offset += getStateBitMap(state, offset, player->mStateWallAir);
    //offset += getStateBitMap(state, offset, player->mStateWallCatch);
    //offset += getStateBitMap(state, offset, player->mStateGrabCeil);
    //offset += getStateBitMap(state, offset, player->mStatePoleClimb);
    offset += getStateBitMap(state, offset, player->mStateHipDrop);
    offset += getStateBitMap(state, offset, player->mStateHeadSliding);
    offset += getStateBitMap(state, offset, player->mStateLongJump);
    offset += getStateBitMap(state, offset, player->mStateFallHakoniwa);
    //offset += getStateBitMap(state, offset, player->mStateSandSink);
    //offset += getStateBitMap(state, offset, player->mActorStateSandGeyser);
    //offset += getStateBitMap(state, offset, player->mStateRise);
    //offset += getStateBitMap(state, offset, player->mStateSwim);
    offset += getStateBitMap(state, offset, player->mStateDamageLife);
    //offset += getStateBitMap(state, offset, player->mStateDamageSwim);
    //offset += getStateBitMap(state, offset, player->mStateDamageFire);
    //offset += getStateBitMap(state, offset, player->mStatePress);
    //offset += getStateBitMap(state, offset, player->mStateHack);
    //offset += getStateBitMap(state, offset, player->mStateEndHack);
    //offset += getStateBitMap(state, offset, player->mStateBind);
    //offset += getStateBitMap(state, offset, player->mStateCameraSubjective);
    //offset += getStateBitMap(state, offset, player->mStateAbyss);

    if(al::isDead(player)) {
        u128 mask = ~0;
        mask <<= offset - startOffset;
        *state &= mask;
    }
    return offset - startOffset;
}


PlayerActorHakoniwa::PlayerActorHakoniwa(const char* name)
    : PlayerActorBase(name), mComboCounter(new al::ComboCounter()) {}

void PlayerActorHakoniwa::initAfterPlacement() {
    mDimensionKeeper->update();
    if (!rs::isIn2DArea(this))
        mDimensionKeeper->invalidate();

    mJointControlKeeper->resetPartsDynamics();
    mModelChangerHakoniwa->initStartModel();

    al::addVelocityToGravity(this, 0.01f);
    updateCollider();
    al::setVelocityZero(this);

    if (mHackKeeper->executeForceHackStageStart(mBodyHitSensor, this)) {
        mStateHack->prepareStageStartHack();
        mModelChangerHakoniwa->syncHost(1);
        al::setNerve(this, &Hack);
        return;
    }

    if (al::isNerve(this, &Fall)) {
        if (rs::isCollidedGround(mColliderHakoniwa) ||
            (mColliderHakoniwa->isAboveGround() &&
            mColliderHakoniwa->getGroundHeight() < 10.0f)) {
            mTrigger->set(PlayerTrigger::ECollisionTrigger_val8);
            mStateWait->initSceneStartAnim();
            mModelChangerHakoniwa->resetPosition();
            al::setNerve(this, &Wait);
            return;
        }

        mTrigger->set(PlayerTrigger::ECollisionTrigger_val7);
        mInput->convergedInUpdateToDisableInput = 1;
        mInput->mIsDisableInput = 1;
        mInput->usedInUpdate = 0;
    }
}

void PlayerActorHakoniwa::movement() {
    if (mHackCap->isNoPutOnHide())
        CRASH

    mInput->update();
    rs::updateJudge(mJudgePreInputJump);
    rs::updateJudge(mJudgePreInputCapThrow);
    rs::updateJudge(mJudgePreInputHackAction);
    rs::updateJudge(mHackCapJudgePreInputSeparateThrow);
    rs::updateJudge(mHackCapJudgePreInputSeparateJump);
    rs::updateJudge(mHackCapJudgePreInputHoveringJump);

    if (rs::isPlayerDamageStopDemo(this)) {
        CRASH  // very end of the function
            return;
    }

    mExternalVelocity->update();
    int a1;
    float a2;
    if (PlayerEquipmentFunction::tryGetEquipmentForceDashInfo(&a1, &a2, mEquipmentUser))
        CRASH

    checkDamageFromCollision();
    mTrigger->clearPreMovementTrigger();
    executePreMovementNerveChange();

    if (!al::isNerve(this, &Hack) || mHackKeeper->_5D)
        mAnimator->updateAnimFrame();

    mTrigger->clearReceiveSensorTrigger();

    if (!al::isNerve(this, &SandGeyser))
        mActorStateSandGeyser->mGeyserSensor = nullptr;

    mTrigger->clearActionTrigger();
    mTrigger->clearAttackSensorTrigger();
    mEyeSensorHitHolder->clear();
    mHitPush->clearHitFlag();

    PlayerActorBase::movement();

    mModelChangerHakoniwa->syncHost(!al::isNerve(this, &Hack) ||
                                          mStateHack->isEnableModelSyncShowHide());

    // TODO animation model alpha updates
    mPainPartsKeeper->mModelAlphaMask = mAnimator->getModelAlpha();

    if (!al::isNerve(this, &Demo)) {
        // TODO wet/stain effects
    }

    mTrigger->clearMaterialChangeTrigger();

    // TODO ground material, set mTrigger->mMaterialChangeTrigger

    /*if(al::isNerve(this, &Bind)) {
        CRASH
    } else if(al::isNerve(this, &Demo)) {
        CRASH
    } else if(al::isPlayingEntranceCamera(this, 0) || mAnimator->unk2) {
        CRASH
    } else if(al::isNerve(this, &Wait)) {

    }*/

    // TODO more stuff about setting the JointCtrl with LookAt

    // TODO more stuff about controlling the eyes of Mario and Cappy, and footprints

    mRecoverySafetyPoint->updateRecoveryBubble();

    // TODO more things about storing the position for bubbles

    mPushReceiver->clear();
    mCapActionHistory->update();
    mCounterAfterUpperPunch->update(mTrigger);
    mCounterForceRun->update();

    if (al::isNerve(this, &Demo) || al::isNerve(this, &Hack)) {
        mCounterIceWater->clearIceWaterCount();
    } else {
        mCounterIceWater->updateCount(
            al::isNerve(this, &Swim) || al::isNerve(this, &DamageSwim), mDamageKeeper->_10);
    }

    mCounterQuickTurnJump->update();

    if (al::isNerve(this, &Demo) || rs::isPlayer2D(this)) {
        mRippleGenerator->reset();
    } else {
        // TODO generate ripples
    }

    mWallActionHistory->update(mColliderHakoniwa);

    if (!al::isNerve(this, &Demo))
        mPainPartsKeeper->update();

    rs::updateJudge(mJudgeDeadWipeStart);

    bool runOrWait = al::isNerve(this, &Run) || al::isNerve(this, &Wait);
    if (!al::isNerve(this, &Jump))
        mContinuousJump->update(runOrWait);
    if (!al::isNerve(this, &LongJump) && !mStateRolling->isRollingJump())
        mContinuousLongJump->update();

    mStateWait->tryClearIgnoreSwitchOnAreaAnim();

    if (PlayerFunction::isPlayerDeadStatus(this) || rs::isKidsMode(this) ||
        al::isNerve(this, &Hack) || al::isNerve(this, &Demo) || al::isNerve(this, &Camera) ||
        al::isPlayingEntranceCamera(this, 0)) {
        // TODO something about gauge/air
    } else {
        // TOOD other thing about gauge/air/oxygen
    }

    // TODO layout stuff: position gauge next to player head

    mJudgeWallCatchInputDir->updateWallCatchEnviroment();

    // TODO camera stuff for 2d sections

    // TODO invisibility effect with and without Hack

    mSeCtrl->update();
    {
        // sub_7100420438:
        // TODO more stuff about 2D-2P sections
        mSeparateCapFlag->_0 = 0;
        mSeparateCapFlag->_1 = 0;
        mHackCap->updateSeparateMode(mSeparateCapFlag);
        sead::Vector3f sepCapLocalOffset = {0.0f, 0.0f, 0.0f};
        if (mSeparateCapFlag->someCheck())
            mColliderHakoniwa->calcSeparateCapLocalOffset(&sepCapLocalOffset);
        sead::Vector3f sepOffset = {0.0f, 0.0f, 0.0f};
        al::lerpVec(&sepOffset, mSeparateCapFlag->_4, sepCapLocalOffset,
                    mConst->getSeparateOffsetLerpRate());
        mSeparateCapFlag->_4 = sepOffset;
        // TODO call to sub_710042983C, dealing with 2d-animation stuff
    }

    // TODO more stuff about moving CapMan's eyes (skipped for specific ROMs)
}

void PlayerActorHakoniwa::attackSensor(al::HitSensor*, al::HitSensor*) {
    CRASH
}

bool PlayerActorHakoniwa::receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) {
    CRASH
}

void PlayerActorHakoniwa::control() {
    al::AreaObjGroup* hackerCheckKeepOnAreaGroup =
        al::tryFindAreaObjGroup(this, "HackerCheckKeepOnArea");
    if (hackerCheckKeepOnAreaGroup)
        CRASH

    al::AreaObj* hackerCheckArea = al::tryFindAreaObj(this, "HackerCheckArea", al::getTrans(this));
    if (hackerCheckArea)
        CRASH

    if (al::isNerve(this, &Hack)) {
        CRASH
    } else {
        mDamageKeeper->update(mModelChangerHakoniwa,
                                    rs::isJudge(mJudgeRecoveryLifeFast),
                                    !al::isNerve(this, &Demo));
        if (!rs::isJudge(mInfo->mJudgeSafetyPointRecovery)) {
            mDimensionKeeper->update();
            // possibly jumping out of 2d area
            al::setGravity(this, {0.0f, -1.0f, 0.0f});
        }

        updateCarry();
        bool isOnGround = rs::isOnGroundRunAngle(this, mColliderHakoniwa, mConst);
        // animation control

        // nerve changer for switching after 2d->3d dropout
    }

    bool isInWater = false;  // rs::isPlayerInWater(this) || mStateSwim->isReduceOxygen() ||
                             // mStateDamageSwim->isReduceOxygen();
    // BGM changer for going into/out of water
    mIsInWater = isInWater;
}

void PlayerActorHakoniwa::updateCollider() {
    al::updatePoseTrans(this, al::getTrans(this));
    syncSensorAndCollision();
    if (al::isNerve(this, &Demo) || al::isNerve(this, &Dead))
        CRASH
    if (al::isNerve(this, &Hack))
        CRASH
    if (PlayerFunction::isPlayerDeadStatus(this) && !al::isNerve(this, &Abyss))
        CRASH

    sead::Vector3f velocity = {0.0f, 0.0f, 0.0f};
    if (PlayerFunction::isPlayerDeadStatus(this) || al::isNerve(this, &Hack) ||
        al::isNerve(this, &Demo) || al::isNerve(this, &Camera) || al::isNerve(this, &PoleClimb)) {
        velocity = al::getVelocity(this);
    } else {
        sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
        if (al::isNerve(this, &WallAir)) {
            mStateWallAir->calcSnapMoveCutDir(&a3);
            rs::calcSnapVelocitySnapMoveAreaWithCutDir(&velocity, this, mColliderHakoniwa,
                                                       al::getVelocity(this), 5.0f, a3);
        } else {
            rs::calcSnapVelocitySnapMoveArea(&velocity, this, mColliderHakoniwa,
                                             al::getVelocity(this), 5.0f);
        }
    }

    mPushReceiver->calcPushedVelocityWithCollide(
        &velocity, velocity + mExternalVelocity->someVec2, mColliderHakoniwa,
        mConst->getCollisionRadius());
    mWorldEndBorderKeeper->update(al::getTrans(this), velocity,
                                  al::isNerve(this, &Swim) ||
                                      !rs::isCollidedGround(mColliderHakoniwa));
    velocity += mWorldEndBorderKeeper->_54;

    // if Damage, SpinCap, GrabCeil or DamageFire and snapping
    if (false)
        CRASH

    if (al::isNoCollide(this) || mPuppet->isNoCollide()) {
        rs::resetCollision(mColliderHakoniwa);
        *al::getTransPtr(this) += velocity;
    } else {
        //*al::getTransPtr(this) += mColliderHakoniwa->updateCollider(velocity);
        sead::Vector3f result = mColliderHakoniwa->updateCollider(velocity);
        const sead::Vector3f& a2 = velocity;
        const PlayerCollider* a1 = mColliderHakoniwa->mPlayerCollider;
        dbg_printf("Collide: (%.02f, %.02f, %.02f)+(%.02f, %.02f, %.02f) => (%.02f, %.02f, %.02f) ; "
               "isRecovery=%s\n",
               a1->mTrans.x, a1->mTrans.y, a1->mTrans.z, a2.x, a2.y, a2.z, result.x, result.y,
               result.z, "false");
        *al::getTransPtr(this) += result;
    }

    sead::Vector3f gravity = al::getGravity(this);
    al::updatePoseTrans(this, al::getTrans(this));

    mWaterSurfaceFinder->update(al::getTrans(this), -al::getGravity(this), 200.0f);
    mEffect->updateWaterSurfaceMtx(mWaterSurfaceFinder);
    mColliderHakoniwa->updateHeightCheck(al::getTrans(this), -al::getGravity(this), true);
    mColliderHakoniwa->updateCeilingCheck(
        al::isNerve(this, &WallCatch) && mStateWallCatch->isWallCatchForm() ?
            mStateWallCatch->getCeilingCheckPos() :
            al::getTrans(this),
        -gravity, 0.0f, mCarryKeeper->isCarryUp() ? 150.0f : 0.0f);

    // updating WaterSurfaceShadow

    mColliderHakoniwa->updateFallDistanceCheck(al::getTrans(this), velocity, gravity,
                                                     mConst->getFallSpeedMax());

    if (rs::isCollidedGround(mColliderHakoniwa))
        mComboCounter->reset();

    if (mCarryKeeper->updateCollideLockUp(mColliderHakoniwa, mPushReceiver))
        mCarryKeeper->startCancelAndRelease();
}

void PlayerActorHakoniwa::initPlayer(const al::ActorInitInfo& actorInfo,
                                     const PlayerInitInfo& playerInfo) {
    al::initActorWithArchiveName(this, actorInfo, "PlayerActorHakoniwa", nullptr);
    rs::initPlayerActorInfo(this, playerInfo);
    al::onCollide(this);

    bool isMoon = false;
    al::tryGetArg(&isMoon, actorInfo, "IsMoon");
    al::tryGetArg(&mIsLongShadow, actorInfo, "IsLongShadow");

    bool invalidateCap = false;
    al::tryGetArg(&invalidateCap, actorInfo, "IsInvalidateCap");
    if (invalidateCap)
        GameDataFunction::disableCapByPlacement(this);

    al::registerExecutorFunctor(
        "プレイヤー後処理", actorInfo.executeDirector,
        al::FunctorV0M<PlayerActorHakoniwa*, void (PlayerActorHakoniwa::*)(void)>(
            this, &PlayerActorHakoniwa::initAfterPlacement));
    mBodyHitSensor = al::getHitSensor(this, "Body");

    mDimensionKeeper = new ActorDimensionKeeper(this);
    mTrigger = new PlayerTrigger();

    al::AudioKeeper* audioKeeper = getAudioKeeper();
    PlayerModelHolder* modelHolder = new PlayerModelHolder(3);
    LiveActor* modelActor = new al::LiveActor("マリオモデル");
    const char* initPlayerModelName = rs::getInitPlayerModelName(playerInfo);
    PlayerCostumeInfo* costumeInfo = PlayerFunction::initMarioModelActor(
        modelActor, actorInfo, initPlayerModelName, rs::getInitCapTypeName(playerInfo), audioKeeper,
        rs::isClosetScenePlayer(playerInfo));
    modelHolder->registerModel(modelActor, "Normal");

    // NON_MATCHING: allocated/created differently
    al::StringTmp<128> initPlayerModelNameWithoutMario = "";
    al::tryReplaceString(&initPlayerModelNameWithoutMario, initPlayerModelName, "Mario", "");
    modelHolder->_10.format("%s", initPlayerModelNameWithoutMario.cstr());

    al::StringTmp<64> modelName2d = &sead::SafeString::cNullChar;
    if (costumeInfo->isEnableCostume2D())
        modelName2d.format("%s2D", initPlayerModelName);
    else
        modelName2d.format("Mario2D");

    LiveActor* modelActor2d = new al::LiveActor("ドットマリオモデル");
    PlayerFunction::initMarioModelActor2D(modelActor2d, actorInfo, modelName2d.cstr(),
                                          PlayerFunction::isInvisibleCap(costumeInfo));
    modelHolder->registerModel(modelActor2d, "Normal2D");

    LiveActor* mario2DMini = new al::LiveActor("ドット死亡モデル");
    al::initChildActorWithArchiveNameNoPlacementInfo(mario2DMini, actorInfo, "Mario2DMini",
                                                     nullptr);
    mario2DMini->makeActorDead();
    modelHolder->registerModel(mario2DMini, "Mario2DMini");

    modelHolder->changeModel("Normal");
    mModelHolder = modelHolder;

    mPainPartsKeeper = new PlayerPainPartsKeeper(this, costumeInfo);

    if (rs::isNeedCreateNoseNeedle(playerInfo))
        mPainPartsKeeper->createNoseNeedle(mModelHolder, actorInfo);

    mConst = PlayerFunction::createMarioConst(isMoon ? "Moon" : "");
    mJointControlKeeper = new PlayerJointControlKeeper(
        //mModelHolder->findModelActor("Normal")
        this, mConst, costumeInfo->isNeedShrinkNose(),
        costumeInfo->isEnableBigEar(), costumeInfo->isFollowJoeStrap());

    PlayerColliderHakoniwa* collider = new PlayerColliderHakoniwa(this, mConst);
    collider->init();
    mColliderHakoniwa = collider;

    mInput = new PlayerInput(this, collider, this);
    mModelChangerHakoniwa = new PlayerModelChangerHakoniwa(
        this, mModelHolder, mPainPartsKeeper, costumeInfo, this);

    mAnimator = new PlayerAnimator(mModelHolder,
                                         PlayerFunction::createPlayerDitherAnimator(this, 200.0f));
    if (costumeInfo->isNeedFullFaceAnim())
        mAnimator->mIsNeedFullFaceAnim = true;

    mEffect = new PlayerEffect(this, mModelHolder, mJointControlKeeper->mtx);
    mContinuousJump = new PlayerContinuousJump(mConst);
    mContinuousLongJump = new PlayerContinuousLongJump(mConst);
    mDamageKeeper = new PlayerDamageKeeper(this, mEffect);
    mDemoActionFlag = new PlayerDemoActionFlag();
    mCapActionHistory =
        new PlayerCapActionHistory(this, mConst, mTrigger, mColliderHakoniwa);
    mAreaChecker = new PlayerAreaChecker(this, mModelHolder);
    mWaterSurfaceFinder = new al::WaterSurfaceFinder(this);
    mWorldEndBorderKeeper = new WorldEndBorderKeeper(this);
    mWallActionHistory = new PlayerWallActionHistory();
    mCounterAfterUpperPunch = new PlayerCounterAfterUpperPunch();
    mCounterForceRun = new PlayerCounterForceRun();
    mCounterQuickTurnJump = new PlayerCounterQuickTurnJump(mConst, mTrigger);
    mJumpMessageRequest = new PlayerJumpMessageRequest();
    mSandSinkAffect = new PlayerSandSinkAffect(this, mConst, mInput,
                                                     mColliderHakoniwa, mEffect);
    mEyeSensorHitHolder = new PlayerEyeSensorHitHolder(128);
    mEyeSensorHitHolder->createTargetMarkerBuffer(128);
    mPushReceiver = new PlayerPushReceiver(this);
    mHitPush = new PlayerHitPush(this, mConst);
    mExternalVelocity =
        new PlayerExternalVelocity(this, mColliderHakoniwa, al::getTransPtr(this));

    al::LiveActor* currentActor = mModelHolder->mCurrentModel->actor;
    al::FootPrintHolder* footPrintHolder =
        new al::FootPrintHolder(currentActor ? currentActor : nullptr, "FootPrint", mBodyHitSensor,
                                (al::FootPrintServer*)al::getSceneObj(currentActor, 15));
    footPrintHolder->size[9] = nullptr;
    footPrintHolder->size[10] = nullptr;
    mFootPrintHolder = footPrintHolder;

    mSeparateCapFlag = new PlayerSeparateCapFlag();

    mWetControl = new PlayerWetControl(this, mModelHolder->findModelActor("Normal"),
                                             mAreaChecker);
    mWetControl->mEffect = mEffect;

    IUsePlayerCollision* playerCollision = getPlayerCollision();
    mHackCapJudgePreInputSeparateThrow =
        new HackCapJudgePreInputSeparateThrow(mInput, mSeparateCapFlag);
    mHackCapJudgePreInputSeparateJump =
        new HackCapJudgePreInputSeparateJump(mInput, mSeparateCapFlag);

    const char* capName = rs::getInitCapTypeName(playerInfo);
    mHackCap =
        new HackCap(this, capName, mInput, mAreaChecker, mWallActionHistory,
                    mCapActionHistory, mEyeSensorHitHolder, mSeparateCapFlag,
                    mColliderHakoniwa, mColliderHakoniwa, mWetControl,
                    mJointControlKeeper, mHackCapJudgePreInputSeparateThrow,
                    mHackCapJudgePreInputSeparateJump);
    mHackCap->init(actorInfo);
    al::registerExecutorFunctor(
        "帽子装着位置更新", actorInfo.executeDirector,
        al::FunctorV0M<PlayerActorHakoniwa*, void (PlayerActorHakoniwa::*)(void)>(
            this, &PlayerActorHakoniwa::executeAfterCapTarget));
    PlayerCounterAfterCapCatch* counterAfterCapCatch =
        mCapActionHistory->mCounterAfterCapCatch;

    mRippleGenerator = new PlayerRippleGenerator(this, mHackCap, mModelHolder);
    mRecoverySafetyPoint = new PlayerRecoverySafetyPoint(
        this, mHackCap, actorInfo, this, mColliderHakoniwa->mFilter2DOnly, mBodyHitSensor);
    mPuppet = new PlayerPuppet(
        this, mHackCap, mAnimator, mColliderHakoniwa, mDimensionKeeper,
        mModelChangerHakoniwa, mWorldEndBorderKeeper, mCounterForceRun,
        mDamageKeeper, mEffect, mInput, mConst);
    mBindKeeper = new PlayerBindKeeper(mBodyHitSensor, mPuppet);
    mCarryKeeper = new PlayerCarryKeeper(
        this, mBodyHitSensor, mAnimator, mModelChangerHakoniwa, mColliderHakoniwa,
        (PlayerJointParamHandLegAngle*)mJointControlKeeper->gap2[6]);
    mEquipmentUser =
        new PlayerEquipmentUser(mInput, mModelHolder, mModelChangerHakoniwa);
    mSeCtrl = new PlayerSeCtrl(
        this, mAnimator, mHackCap, mModelChangerHakoniwa,
        //                 mModelHolder->currentModel->mLiveActor, mExternalVelocity);
        nullptr, mExternalVelocity);

    mHackKeeper = new PlayerHackKeeper(this, mHackCap, mRecoverySafetyPoint,
                                             mInput, getViewMtx(), mDamageKeeper,
                                             mModelChangerHakoniwa, mColliderHakoniwa);
    mHackKeeper->createHackModel(actorInfo);

    mFormSensorCollisionArranger = new PlayerFormSensorCollisionArranger(
        this, mColliderHakoniwa, mModelChangerHakoniwa, mHackKeeper);
    mCapManHeroEyesControl = new PlayerCapManHeroEyesControl(
        "キャップの目の制御",
        al::tryGetSubActor(al::getSubActor(mModelHolder->findModelActor("Normal"), "頭"),
                           "キャップの目"),
        al::getSubActor(mModelHolder->findModelActor("Normal2D"), "キャップの目[2D]"));

    mJudgeAirForceCount =
        new PlayerJudgeAirForceCount(this, mExternalVelocity, playerCollision);
    mJudgeCapCatchPop =
        new PlayerJudgeCapCatchPop(this, mInput, playerCollision, mModelChangerHakoniwa,
                                   mCapActionHistory, counterAfterCapCatch);
    mJudgeDeadWipeStart = new PlayerJudgeDeadWipeStart(this, mConst);
    mJudgeDirectRolling = new PlayerJudgeDirectRolling(
        this, mConst, mInput, playerCollision, mModelChangerHakoniwa,
        mCarryKeeper, mCounterForceRun);
    mJudgeEnableStandUp =
        new PlayerJudgeEnableStandUp(playerCollision, mColliderHakoniwa);
    mJudgeForceSlopeSlide =
        new PlayerJudgeForceSlopeSlide(this, mConst, playerCollision);
    mJudgeForceRolling = new PlayerJudgeForceRolling(this, playerCollision);
    mJudgeGrabCeil =
        new PlayerJudgeGrabCeil(this, mConst, playerCollision, mModelChangerHakoniwa,
                                mCarryKeeper, mExternalVelocity);
    mJudgeInWater1 = new PlayerJudgeInWater(
        this, mConst, playerCollision, mAreaChecker, mWaterSurfaceFinder,
        mColliderHakoniwa, mCounterForceRun, false, false, false);
    mJudgeInWater2 = new PlayerJudgeInWater(
        this, mConst, playerCollision, mAreaChecker, mWaterSurfaceFinder,
        mColliderHakoniwa, mCounterForceRun, false, false, true);
    mJudgeInWater3 = new PlayerJudgeInWater(
        this, mConst, playerCollision, mAreaChecker, mWaterSurfaceFinder,
        mColliderHakoniwa, nullptr, false, false, false);
    mJudgeInWater4 = new PlayerJudgeInWater(
        this, mConst, playerCollision, mAreaChecker, mWaterSurfaceFinder,
        mColliderHakoniwa, mCounterForceRun, false, true, false);
    mJudgeInvalidateInputFall =
        new PlayerJudgeInvalidateInputFall(this, mAreaChecker, playerCollision);
    mJudgeLongFall =
        new PlayerJudgeLongFall(this, mConst, mColliderHakoniwa, mHackKeeper,
                                mBindKeeper, mModelChangerHakoniwa);
    mJudgePoleClimb = new PlayerJudgePoleClimb(
        this, mConst, playerCollision, mModelChangerHakoniwa, mCarryKeeper,
        mExternalVelocity, mInput, mTrigger);
    mJudgePreInputCapThrow =
        new PlayerJudgePreInputCapThrow(mConst, mInput, mCarryKeeper, mHackCap);
    mJudgePreInputHackAction = new PlayerJudgePreInputHackAction(mConst, mInput);
    mHackCapJudgePreInputHoveringJump =
        new HackCapJudgePreInputHoveringJump(this, playerCollision, mHackCap, mInput);
    mJudgeSandSink = new PlayerJudgeSandSink(playerCollision, mSandSinkAffect);
    mJudgeSlopeSlide = new PlayerJudgeSlopeSlide(
        this, mConst, mInput, playerCollision, mModelChangerHakoniwa);
    mJudgeStartHipDrop = new PlayerJudgeStartHipDrop(
        mConst, mInput, mColliderHakoniwa, mModelChangerHakoniwa);
    mJudgeStartRise =
        new PlayerJudgeStartRise(this, mAreaChecker, mModelChangerHakoniwa);
    mJudgeStartRolling =
        new PlayerJudgeStartRolling(this, mConst, mInput, playerCollision,
                                    mModelChangerHakoniwa, mCarryKeeper);
    mJudgeStartSquat =
        new PlayerJudgeStartSquat(mInput, mCounterForceRun, mCarryKeeper);
    mJudgeWallCatch = new PlayerJudgeWallCatch(
        this, mConst, playerCollision, mModelChangerHakoniwa, mCarryKeeper,
        mExternalVelocity, mInput, mTrigger, mCounterForceRun);
    mJudgeWallCatchInputDir = new PlayerJudgeWallCatchInputDir(
        this, mConst, mInput, playerCollision, mModelChangerHakoniwa,
        mCarryKeeper, mExternalVelocity, mCounterForceRun,
        mColliderHakoniwa);
    mJudgeWallHitDown =
        new PlayerJudgeWallHitDown(this, playerCollision, mConst, mTrigger);
    mJudgeWallHitDownForceRun = new PlayerJudgeWallHitDownForceRun(
        this, playerCollision, mConst, mCounterForceRun, mTrigger);
    mJudgeWallHitDownRolling =
        new PlayerJudgeWallHitDownRolling(this, playerCollision, mConst, mTrigger);
    mJudgeWallKeep = new PlayerJudgeWallKeep(
        this, mConst, mInput, playerCollision, mModelChangerHakoniwa,
        mColliderHakoniwa, mCounterAfterUpperPunch, mWallActionHistory,
        mCarryKeeper, mTrigger, mCounterForceRun);
    mJudgeCameraSubjective =
        new PlayerJudgeCameraSubjective(this, playerCollision, mInput);
    mSpinCapAttack =
        new PlayerSpinCapAttack(mHackCap, mConst, mTrigger, mInput,
                                counterAfterCapCatch, mJudgePreInputCapThrow);
    mActionDiveInWater = new PlayerActionDiveInWater(
        mAnimator, mModelChangerHakoniwa, mCarryKeeper,
        new PlayerJudgeDiveInWater(this, mConst, mColliderHakoniwa,
                                   mColliderHakoniwa));
    mJudgeForceLand = new PlayerJudgeForceLand(mJudgeLongFall, mTrigger);
    mJudgeOutInWater =
        new PlayerJudgeOutInWater(this, mJudgeInWater1, mJudgeInWater4);
    mJudgePreInputJump =
        new PlayerJudgePreInputJump(mConst, mInput, mJudgeForceSlopeSlide);
    mJudgeStartWaterSurfaceRun =
        new PlayerJudgeStartWaterSurfaceRun(this, mWaterSurfaceFinder, mCounterForceRun);
    mJudgeSpeedCheckFall = new PlayerJudgeSpeedCheckFall(this, playerCollision, mConst,
                                                               mJudgeStartWaterSurfaceRun);
    mJudgeStartRun =
        new PlayerJudgeStartRun(this, mConst, playerCollision, mInput,
                                mCounterForceRun, mJudgeForceLand);

    al::initNerve(this, &Fall, 29);

    mStateWait = new PlayerStateWait(
        this, mConst, playerCollision, mModelChangerHakoniwa, mJointControlKeeper,
        mWaterSurfaceFinder, mColliderHakoniwa, mJudgeForceLand, mAnimator,
        mTrigger, mCapManHeroEyesControl,
        (PlayerJointParamCenterDynamics*)mJointControlKeeper->gap2[7],
        (PlayerJointParamGroundPose*)mJointControlKeeper->gap2[9]);
    mStateSandSink = new PlayerStateSandSink(this, mConst, mInput, mTrigger,
                                                   mColliderHakoniwa, mAnimator,
                                                   mJudgeStartRun, mJudgePreInputJump);
    mActorStateSandGeyser = new ActorStateSandGeyser(this);
    mStateSpinCap = new PlayerStateSpinCap(
        this, mConst, mInput, mCounterForceRun, mWaterSurfaceFinder,
        mColliderHakoniwa, mTrigger, mSpinCapAttack, mAnimator,
        (PlayerJointParamCapThrow*)mJointControlKeeper->gap2[4]);
    mStateSquat =
        new PlayerStateSquat(this, mConst, mAnimator, mInput, playerCollision,
                             mModelChangerHakoniwa, mTrigger, mJudgeStartSquat,
                             mJudgeStartRun, mJudgeEnableStandUp);
    mStateRunHakoniwa2D3D = new PlayerStateRunHakoniwa2D3D(
        this, mConst, this, mInput, playerCollision, mWaterSurfaceFinder,
        mCounterForceRun, mCounterQuickTurnJump, mTrigger, mAnimator,
        mEffect, (PlayerJointParamCenterDynamics*)mJointControlKeeper->gap2[7], isMoon);
    mStateSlope =
        new PlayerStateSlope(this, mConst, mInput, playerCollision, mAnimator);
    mStateRolling = new PlayerStateRolling(
        this, mConst, mInput, playerCollision, mTrigger, mAnimator,
        mEffect, mJudgeStartRolling, mJudgeEnableStandUp,
        mJudgePreInputJump, mJudgePreInputCapThrow, mJudgeForceRolling,
        mContinuousLongJump, mSeCtrl);
    mStateJump = new PlayerStateJump(
        this, mConst, mModelChangerHakoniwa, this, mInput,
        mJumpMessageRequest, new PlayerJudgeCapCatch(this, counterAfterCapCatch),
        mCounterForceRun, mJointControlKeeper, mColliderHakoniwa, mTrigger,
        mContinuousJump, mAnimator, mActionDiveInWater, mHackCap,
        new HackCapJudgeHoldHoveringJump(this, mColliderHakoniwa, mHackCap, mInput,
                                         mConst),
        isMoon);
    mStateCapCatchPop = new PlayerStateCapCatchPop(this, mConst, playerCollision,
                                                         mInput, mAnimator, mHackCap);
    mStateWallAir = new PlayerStateWallAir(
        this, mConst, mInput, mTrigger, mColliderHakoniwa,
        mJudgeWallKeep, mJudgePreInputJump, mAnimator, mWallActionHistory,
        mActionDiveInWater);
    mStateWallCatch = new PlayerStateWallCatch(
        this, mConst, mInput, getPlayerCollision(), mColliderHakoniwa,
        mModelHolder, mAreaChecker, mAnimator, mTrigger,
        mJudgePreInputJump, (PlayerJointParamGrab*)mJointControlKeeper->gap2[5]);
    mStateGrabCeil = new PlayerStateGrabCeil(
        this, mConst, mInput, mModelHolder, mTrigger, getPlayerCollision(),
        mAnimator, mJudgePreInputJump, mBodyHitSensor,
        (PlayerJointParamGrab*)mJointControlKeeper->gap2[5]);
    mStatePoleClimb = new PlayerStatePoleClimb(
        this, mConst, mInput, mTrigger, mModelHolder, getPlayerCollision(),
        mAnimator, mWallActionHistory,
        (PlayerJointParamHandLegAngle*)mJointControlKeeper->gap2[6], mJudgePreInputJump,
        mActionDiveInWater);
    mStateHipDrop = new PlayerStateHipDrop(
        this, mConst, mInput, mColliderHakoniwa, mAnimator, mTrigger);
    mStateHeadSliding =
        new PlayerStateHeadSliding(this, mConst, playerCollision, mInput,
                                   mActionDiveInWater, mAnimator);
    mStateLongJump = new PlayerStateLongJump(
        this, mConst, mInput, playerCollision, mTrigger, mAnimator,
        mContinuousLongJump, mActionDiveInWater);
    mStateFallHakoniwa =
        new PlayerStateFallHakoniwa(this, mConst, playerCollision, mInput,
                                    mTrigger, mAreaChecker, this, mAnimator);
    mStateRise =
        new PlayerStateRise(this, mConst, playerCollision, mInput, mAnimator);
    mStateSwim = new PlayerStateSwim(
        this, mConst, playerCollision, mInput, mTrigger, mCarryKeeper,
        mModelHolder, mExternalVelocity, mAnimator, mSpinCapAttack,
        mWaterSurfaceFinder, mEffect,
        (PlayerJointParamSwim*)mJointControlKeeper->gap2[8]);
    mStateDamageLife =
        new PlayerStateDamageLife(this, mConst, playerCollision, mInput,
                                  mColliderHakoniwa, mAnimator, mTrigger);
    mStateDamageSwim = new PlayerStateDamageSwim(
        this, mConst, playerCollision, mInput, mTrigger, mAnimator,
        mWaterSurfaceFinder, mEffect, mJudgeInWater1, mJudgeOutInWater);
    mStateDamageFire =
        new PlayerStateDamageFire(this, mConst, playerCollision, mInput,
                                  mAnimator, mJudgeSpeedCheckFall);
    mStatePress = new PlayerStatePress(this, playerCollision, mAnimator);
    mStateBind = new PlayerStateBind(this, mConst, mInput, mPuppet,
                                           playerCollision, mTrigger);
    mStateHack = new PlayerStateHack(this, mHackKeeper, mModelChangerHakoniwa,
                                           mAnimator, mHackCap);
    mStateEndHack = new PlayerStateEndHack(this, mConst, playerCollision, mInput,
                                                 mTrigger, mAnimator);
    mStateCameraSubjective = new PlayerStateCameraSubjective(
        this, *((al::CameraTicket**)mJudgeCameraSubjective + 4), mModelChangerHakoniwa,
        mCarryKeeper, mAnimator, mConst, playerCollision, mInput);
    mStateAbyss = new PlayerStateAbyss(this, mConst, mRecoverySafetyPoint,
                                             mColliderHakoniwa, mAnimator,
                                             mModelHolder->findModelActor("Normal2D"));

    /*
    al::initNerveState(this, mStateWait, &Wait, "待機");
    al::initNerveState(this, mStateSquat, &Squat, "しゃがみ");
    al::initNerveState(this, mStateRunHakoniwa2D3D, &Run, "走り");
    al::initNerveState(this, mStateSlope, &Slope, "坂すべり");
    al::initNerveState(this, mStateRolling, &Rolling, "ローリング");
    al::initNerveState(this, mStateSpinCap, &SpinCap, "帽子スピン");
    al::initNerveState(this, mStateJump, &Jump, "ジャンプ");
    al::initNerveState(this, mStateCapCatchPop, &CapCatchPop, "帽子キャッチ浮きあがり");
    al::initNerveState(this, mStateWallAir, &WallAir, "壁接触[空中]");
    al::initNerveState(this, mStateWallCatch, &WallCatch, "壁つかまり");
    al::initNerveState(this, mStateGrabCeil, &GrabCeil, "天井ぶら下がり");
    al::initNerveState(this, mStatePoleClimb, &PoleClimb, "ポールつかまり");
    al::initNerveState(this, mStateHipDrop, &HipDrop, "ヒップドロップ");
    al::initNerveState(this, mStateHeadSliding, &HeadSliding, "ヘッドスライディング");
    al::initNerveState(this, mStateLongJump, &LongJump, "幅跳び");
    al::initNerveState(this, mStateFallHakoniwa, &Fall, "落下");
    al::initNerveState(this, mStateSandSink, &SandSink, "砂沈み");
    al::initNerveState(this, mActorStateSandGeyser, &SandGeyser, "砂の間欠泉");
    al::initNerveState(this, mStateRise, &Rise, "浮き上がり");
    al::initNerveState(this, mStateSwim, &Swim, "泳ぎ");
    al::initNerveState(this, mStateDamageLife, &Damage, "ダメージ");
    al::initNerveState(this, mStateDamageSwim, &DamageSwim, "水中・水面ダメージ");
    al::initNerveState(this, mStateDamageFire, &DamageFire, "あちちダメージ");
    al::initNerveState(this, mStatePress, &Press, "圧死");
    al::initNerveState(this, mStateHack, &Hack, "憑依");
    al::initNerveState(this, mStateEndHack, &EndHack, "憑依解除");
    al::initNerveState(this, mStateBind, &Bind, "バインド");
    al::initNerveState(this, mStateCameraSubjective, &Camera, "主観カメラ");
    al::initNerveState(this, mStateAbyss, &Abyss, "奈落死");
    */
    al::initNerveState(this, mStateWait, &Wait, "Wait");
    al::initNerveState(this, mStateSquat, &Squat, "Squat");
    al::initNerveState(this, mStateRunHakoniwa2D3D, &Run, "RunHakoniwa2D3D");
    al::initNerveState(this, mStateSlope, &Slope, "Slope");
    al::initNerveState(this, mStateRolling, &Rolling, "Rolling");
    al::initNerveState(this, mStateSpinCap, &SpinCap, "SpinCap");
    al::initNerveState(this, mStateJump, &Jump, "Jump");
    al::initNerveState(this, mStateCapCatchPop, &CapCatchPop, "CapCatchPop");
    al::initNerveState(this, mStateWallAir, &WallAir, "WallAir");
    al::initNerveState(this, mStateWallCatch, &WallCatch, "WallCatch");
    al::initNerveState(this, mStateGrabCeil, &GrabCeil, "GrabCeil");
    al::initNerveState(this, mStatePoleClimb, &PoleClimb, "PoleClimb");
    al::initNerveState(this, mStateHipDrop, &HipDrop, "HipDrop");
    al::initNerveState(this, mStateHeadSliding, &HeadSliding, "HeadSliding");
    al::initNerveState(this, mStateLongJump, &LongJump, "LongJump");
    al::initNerveState(this, mStateFallHakoniwa, &Fall, "FallHakoniwa");
    al::initNerveState(this, mStateSandSink, &SandSink, "SandSink");
    al::initNerveState(this, mActorStateSandGeyser, &SandGeyser, "砂の間欠泉");
    al::initNerveState(this, mStateRise, &Rise, "Rise");
    al::initNerveState(this, mStateSwim, &Swim, "Swim");
    al::initNerveState(this, mStateDamageLife, &Damage, "DamageLife");
    al::initNerveState(this, mStateDamageSwim, &DamageSwim, "DamageSwim");
    al::initNerveState(this, mStateDamageFire, &DamageFire, "DamageFire");
    al::initNerveState(this, mStatePress, &Press, "Press");
    al::initNerveState(this, mStateHack, &Hack, "Hack");
    al::initNerveState(this, mStateEndHack, &EndHack, "EndHack");
    al::initNerveState(this, mStateBind, &Bind, "Bind");
    al::initNerveState(this, mStateCameraSubjective, &Camera, "CameraSubjective");
    al::initNerveState(this, mStateAbyss, &Abyss, "Abyss");

    mEquipmentUser->setPlayerStateRolling(mStateRolling);

    mOxygen = new PlayerOxygen();
    mOxygen->setup(
        mConst->getOxygenNoReduceFrame(), mConst->getOxygenReduceFrame(),
        mConst->getOxygenRecoveryFrame(), mConst->getOxygenDamageInterval());
    mOxygen->reset();

    PlayerJudgeInWater* iceWaterJudge = new PlayerJudgeInWater(
        this, mConst, playerCollision, mAreaChecker, mWaterSurfaceFinder,
        mColliderHakoniwa, mCounterForceRun, true, false, false);
    mCounterIceWater = new PlayerCounterIceWater(this, actorInfo, mConst,
                                                       mColliderHakoniwa, iceWaterJudge);
    mStainControl =
        new PlayerStainControl(this, mModelHolder->findModelActor("Normal"),
                               mModelChangerHakoniwa, mHackCap, mEffect);
    mJudgeRecoveryLifeFast = new PlayerJudgeRecoveryLifeFast(
        this, mDamageKeeper, mBindKeeper, playerCollision, mInput,
        mHackKeeper, mStateWait, mStateSwim, iceWaterJudge);

    mInfo = new PlayerInfo();
    mInfo->mModelChangerHakoniwa = mModelChangerHakoniwa;
    mInfo->mOxygen = mOxygen;
    mInfo->mBindKeeper = mBindKeeper;
    mInfo->mDamageKeeper = mDamageKeeper;
    mInfo->mDemoActionFlag = mDemoActionFlag;
    mInfo->mEquipmentUser = mEquipmentUser;
    mInfo->mHackCap = mHackCap;
    mInfo->mWorldEndBorderKeeper = mWorldEndBorderKeeper;
    mInfo->mFormSensorCollisionArranger = mFormSensorCollisionArranger;
    mInfo->mAnimator = mAnimator;
    mInfo->mInput = mInput;
    mInfo->mModelHolder = mModelHolder;
    mInfo->mCeilingCheck = mColliderHakoniwa;
    mInfo->mHackKeeper = mHackKeeper;
    mInfo->mCapManHeroEyesControl = mCapManHeroEyesControl;
    mInfo->mCarryKeeper = mCarryKeeper;
    mInfo->mJointControlKeeper = mJointControlKeeper;
    mInfo->mCounterIceWater = mCounterIceWater;
    mInfo->mStainControl = mStainControl;
    mInfo->mFootPrintHolder = mFootPrintHolder;
    mInfo->mBodyHitSensor = mBodyHitSensor;
    mInfo->mRecoverySafetyPoint = mRecoverySafetyPoint;
    mInfo->mCostumeInfo = costumeInfo;
    mInfo->mJudgeCameraInWater =
        new PlayerJudgeCameraInWater(this, mJudgeInWater1, mWaterSurfaceFinder);
    mInfo->mJudgeActiveCameraSubjective =
        new PlayerJudgeActiveCameraSubjective(mStateCameraSubjective);
    mInfo->mJudgeTalkGround = new PlayerJudgeTalkGround(
        this, mModelChangerHakoniwa, mHackKeeper, mCarryKeeper, playerCollision,
        mInput, mConst, mStateWait);
    mInfo->mJudgeTalkSwim =
        new PlayerJudgeTalkSwim(mHackKeeper, mCarryKeeper, mStateSwim);
    mInfo->mJudgeDead = new PlayerJudgeDead(this);
    mInfo->mJudgeDeadWipeStart = mJudgeDeadWipeStart;
    mInfo->mJudgeDrawForward =
        new PlayerJudgeDrawForward(this, mJudgeDeadWipeStart);
    mInfo->mJudgeNrvPoleClimb = new PlayerJudgeSameNerve(this, &PoleClimb);
    mInfo->mJudgeNrvGrabCeil = new PlayerJudgeSameNerve(this, &GrabCeil);
    mInfo->mJudgeNrvWallCatch = new PlayerJudgeSameNerve(this, &WallCatch);
    mInfo->mJudgeNrvSwim = new PlayerJudgeSameNerve(this, &Swim);
    mInfo->mJudgeFailureCameraSubjective = new PlayerJudgeFailureCameraSubjective(
        mInput, new PlayerJudgeSameNerve(this, &Camera));
    mInfo->mJudgeSafetyPointRecovery = new PlayerJudgeSafetyPointRecovery(
        new PlayerJudgeSameNerve(this, &Abyss), mStateAbyss);
    mInfo->mJudgeStatusPoleClimb = new PlayerJudgeStatusPoleClimb(
        new PlayerJudgeSameNerve(this, &PoleClimb), mStatePoleClimb);
    mInfo->mJudgePlaySwitchOnAreaWaitAnim =
        new PlayerJudgePlaySwitchOnAreaWaitAnim(mStateWait);
    mInfo->mJudgeSleep = new PlayerJudgeSleep(mStateWait);
    mInfo->mJudgeEnableGuideArrow = new PlayerJudgeEnableGuideArrow(mPuppet);
    mInfo->mJudgeEnablePeachAmiibo =
        new PlayerJudgeEnablePeachAmiibo(mStateDamageFire);
    mInfo->mIsMoon = isMoon;

    CapFunction::putOnCapPlayer(mHackCap, mAnimator);
    syncSensorAndCollision();
    makeActorAlive();
    rs::resetCollision(mColliderHakoniwa);
    mGaugeAir = new GaugeAir("酸素タイマー", al::getLayoutInitInfo(actorInfo));
    mWaterSurfaceShadow = new WaterSurfaceShadow(actorInfo, "WaterSurfaceShadow");
    mWaterSurfaceShadow->mMaxDistance = 1500.0f;
    mWaterSurfaceShadow->mEndFrame = 0.25f;
    mWaterSurfaceShadow->mStartFrame = 0.75f;
    mWaterSurfaceShadow->setScale(0.5f);
}

u32 PlayerActorHakoniwa::getPortNo() const {
    if (rs::isSeparatePlay(this))
        return PlayerInput::getSeparatePlay1P();
    else
        return PlayerActorBase::getPortNo();
}

IUsePlayerCollision* PlayerActorHakoniwa::getPlayerCollision() const {
    return mColliderHakoniwa;
}

PlayerHackKeeper* PlayerActorHakoniwa::getPlayerHackKeeper() const {
    return mHackKeeper;
}

bool PlayerActorHakoniwa::isEnableDemo() {
    CRASH
}

void PlayerActorHakoniwa::startDemo() {
    CRASH
}

void PlayerActorHakoniwa::endDemo() {
    CRASH
}

void PlayerActorHakoniwa::startDemoPuppetable() {
    CRASH
}

void PlayerActorHakoniwa::endDemoPuppetable() {
    CRASH
}

void PlayerActorHakoniwa::startDemoShineGet() {
    CRASH
}

void PlayerActorHakoniwa::endDemoShineGet() {
    CRASH
}

void PlayerActorHakoniwa::startDemoMainShineGet() {
    CRASH
}

void PlayerActorHakoniwa::endDemoMainShineGet() {
    CRASH
}

void PlayerActorHakoniwa::startDemoHack() {
    CRASH
}

void PlayerActorHakoniwa::endDemoHack() {
    CRASH
}

void PlayerActorHakoniwa::startDemoKeepBind() {
    CRASH
}

void PlayerActorHakoniwa::noticeDemoKeepBindExecute() {
    CRASH
}

void PlayerActorHakoniwa::endDemoKeepBind() {
    CRASH
}

void PlayerActorHakoniwa::startDemoKeepCarry() {
    CRASH
}

void PlayerActorHakoniwa::endDemoKeepCarry() {
    CRASH
}

al::DemoActor* PlayerActorHakoniwa::getDemoActor() {
    CRASH
}

PlayerAnimator* PlayerActorHakoniwa::getDemoAnimator() {
    CRASH
}

bool PlayerActorHakoniwa::isDamageStopDemo() const {
    CRASH
}

PlayerPuppet* PlayerActorHakoniwa::getPlayerPuppet(){CRASH}

PlayerInfo* PlayerActorHakoniwa::getPlayerInfo() const {
    return mInfo;
}

bool PlayerActorHakoniwa::checkDeathArea() {
    WARN_UNIMPL;
    return false;
}

void PlayerActorHakoniwa::sendCollisionMsg() {
    WARN_UNIMPL;
    mTrigger->clearCollisionTrigger();
    mJumpMessageRequest->clear();
}

bool PlayerActorHakoniwa::receivePushMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*,
                                         f32){CRASH}

ActorDimensionKeeper* PlayerActorHakoniwa::getActorDimensionKeeper() const {
    return mDimensionKeeper;
}

void PlayerActorHakoniwa::updateModelShadowDropLength() {
    CRASH
}

void PlayerActorHakoniwa::executeAfterCapTarget() {
    CRASH
}

void PlayerActorHakoniwa::syncSensorAndCollision() {
    if(mModelChangerHakoniwa->is2DModel())
        mFormSensorCollisionArranger->setFormModel2D();
    else
        mFormSensorCollisionArranger->setFormModel3D();

    if(al::isNerve(this, &Hack))
        mFormSensorCollisionArranger->setFormActionHack();
    else if(al::isNerve(this, &Squat) || al::isNerve(this, &Slope) || (al::isNerve(this, &Rolling) && mStateRolling->isRolling()) ||
        (al::isNerve(this, &Jump) && mStateJump->isFormSquat2D()) || (al::isNerve(this, &Damage) && mStateDamageLife->isFormSquat()) )
        mFormSensorCollisionArranger->setFormActionSquat();
    else if(al::isNerve(this, &WallCatch) && mStateWallCatch->isWallCatchForm())
        mFormSensorCollisionArranger->setFormActionWallGrab(mStateWallCatch->getWallCatchFront());
    else if(al::isNerve(this, &GrabCeil) && mStateGrabCeil->isFormGrabCeil())
        mFormSensorCollisionArranger->setFormActionGrabCeil(mStateGrabCeil->getGrabCeilFront());
    else if(al::isNerve(this, &PoleClimb) && mStatePoleClimb->isFormPoleClimb())
        mFormSensorCollisionArranger->setFormActionPoleClimb(mStatePoleClimb->getPoleClimbFront());
    else if(al::isNerve(this, &Swim))
        mFormSensorCollisionArranger->setFormActionSwim();
    // setFormActionRecovery
    else if(al::isNerve(this, &Abyss))
        mFormSensorCollisionArranger->setFormActionAbyss(al::getGravity(this));
    else if(al::isNerve(this, &Dead))
        mFormSensorCollisionArranger->setFormActionDead();
    else if(al::isNerve(this, &Bind))
        {CRASH}
    else
        mFormSensorCollisionArranger->setFormActionStandup();

    if(mSpinCapAttack->isValidAttackSensor(mAnimator))
        mFormSensorCollisionArranger->setFormAttackSensorSpin();
    else
        mFormSensorCollisionArranger->setFormAttackSensorNone();

    mFormSensorCollisionArranger->update();

    // TODO SandSinkAffect - stuff

    if(mTrigger->isOnCollisionExpandCheck())
        rs::resetCollisionExpandCheck(mColliderHakoniwa);
}

// void PlayerActorHakoniwa::checkDamageFromCollision() { WARN_UNIMPL; }
void PlayerActorHakoniwa::executePreMovementNerveChange() {
    if (PlayerFunction::isPlayerDeadStatus(this) || al::isNerve(this, &Demo))
        mBindKeeper->clearBindableSensor();
    else if (al::isNerve(this, &Hack))
        CRASH
    else if (al::isNerve(this, &Abyss))
        mBindKeeper->clearBindableSensor();
    else if (mBindKeeper->sendStartMsg())
        CRASH

    if (!PlayerFunction::isPlayerDeadStatus(this) && !al::isNerve(this, &Bind) &&
        !al::isNerve(this, &Demo) && !al::isNerve(this, &Hack) && !al::isNerve(this, &Abyss) &&
        rs::isPressedCollision(mColliderHakoniwa)) {
        CRASH
        al::setNerve(this, &Press);
        return;
    }

    if (al::isNerve(this, &SandSink) && mSandSinkAffect->isSinkDeathHeight() &&
        !PlayerFunction::isPlayerDeadStatus(this))
        CRASH

    if (rs::isActiveDemo(this) || al::isNerve(this, &Abyss) || al::isNerve(this, &Demo) ||
        PlayerFunction::isPlayerDeadStatus(this) || mBindKeeper->_8) {
        CRASH
    } else if (mHackCap->sendMsgStartHack(mBodyHitSensor)) {
        al::setNerve(this, &Hack);
        return;
    }

    if (al::isNerve(this, &Hack) && mHackKeeper->_70)
        CRASH

    if (mActorStateSandGeyser->mGeyserSensor && !al::isNerve(this, &SandGeyser))
        al::setNerve(this, &SandGeyser);

    if (mBindKeeper->_8)
        CRASH

    if (mHackKeeper->_70)
        CRASH

    if (mTrigger->isOnAnyDamage())
        CRASH

    if (mHitPush->mIsPush)
        CRASH

    if (!PlayerFunction::isPlayerDeadStatus(this) && !al::isNerve(this, &Rise) &&
        !al::isNerve(this, &Demo) && !al::isNerve(this, &Hack) && !al::isNerve(this, &Abyss) &&
        !mBindKeeper->_8 && rs::updateJudgeAndResult(mJudgeStartRise)) {
        al::setNerve(this, &Rise);
        return;
    }

    if (!PlayerFunction::isPlayerDeadStatus(this) && !mBindKeeper->_8 &&
        !al::isNerve(this, &Demo) && !al::isNerve(this, &Hack) && !al::isNerve(this, &Abyss) &&
        !al::isNerve(this, &Camera) && !al::isNerve(this, &Swim) &&
        rs::isTouchJumpCode(this, getPlayerCollision()))
        CRASH

    // TODO starting recovery if neccessary
}

// void PlayerActorHakoniwa::updateCarry() { WARN_UNIMPL; }

void sub_71004229D0(al::LiveActor* player, PlayerTrigger* trigger,
                    const IUsePlayerCollision* collision) {
    trigger->set(PlayerTrigger::EActionTrigger_val8);
    sead::Vector3f normal = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrGravityDir(&normal, player, collision);
    sead::Vector3f slideDir = {0.0f, 0.0f, 0.0f};
    if (rs::calcSlideDir(&slideDir, al::getGravity(player), normal)) {
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, slideDir, normal);
        al::updatePoseQuat(player, quat);
    }
    al::setNerve(player, &Rolling);
}

void PlayerActorHakoniwa::setNerveOnGround() {
    if (rs::updateJudgeAndResult(mJudgeStartWaterSurfaceRun)) {
        al::setNerve(this, &Run);
        return;
    } else if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        al::setNerve(this, &Swim);
        return;
    } else if ((mCounterForceRun->getCounter() < 1 &&
                !rs::isOnGroundRunAngle(this, mColliderHakoniwa, mConst)) ||
               !rs::isOnGround(this, mColliderHakoniwa)) {
        al::setNerve(this, &Fall);
        return;
    } else if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    } else if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    } else if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    } else if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    } else if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    } else if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        rs::cutVerticalVelocityGroundNormal(this, getPlayerCollision());
        al::setNerve(this, &Squat);
        return;
    } else if (rs::updateJudgeAndResult(mJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    } else if (rs::updateJudgeAndResult(mJudgeStartRun)) {
        al::setNerve(this, &Run);
        return;
    } else if (!rs::updateJudgeAndResult(mJudgeStartSquat) ||
               rs::isJudge(mJudgeForceLand)) {
        al::setNerve(this, &Wait);
        return;
    } else if (!rs::isJustLand(mColliderHakoniwa) || al::isNerve(this, &LongJump)) {
        al::setNerve(this, &Squat);
        return;
    } else {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        rs::cutVerticalVelocityGroundNormal(this, getPlayerCollision());
        al::setNerve(this, &Squat);
        return;
    }
}

void PlayerActorHakoniwa::startPlayerPuppet() {
    CRASH
}

void PlayerActorHakoniwa::cancelHackPlayerPuppetDemo() {
    CRASH
}

void PlayerActorHakoniwa::endPlayerPuppet() {
    CRASH
}

void PlayerActorHakoniwa::exeWait() {
    if (al::isFirstStep(this)) {
        mCapActionHistory->clearLandLimitStandAngle();
        rs::resetJudge(mJudgeSpeedCheckFall);
        rs::resetJudge(mJudgeStartRun);
    }

    mExternalVelocity->requestApplyLastGroundInertia();
    tryActionCapReturn();
    al::updateNerveState(this);
    if (al::isFirstStep(this) && mStateWait->isLandStain())
        WARN_UNIMPL;

    if (rs::updateJudgeAndResult(mJudgeWallCatchInputDir)) {
        mStateWallCatch->setup(
            mJudgeWallCatchInputDir->mCollisionParts, mJudgeWallCatchInputDir->mHitPos,
            -mJudgeWallCatchInputDir->mHitNormal2, mJudgeWallCatchInputDir->mHitNormal);
        al::setNerve(this, &WallCatch);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSpeedCheckFall)) {
        mExternalVelocity->cancelAndFeedbackLastGroundInertia(
            this, mConst->getJumpInertiaRate(), 0);
        mJudgeWallCatchInputDir->validateFallJudge();
        al::setNerve(this, &Fall);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }

    if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Squat);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        getPlayerCollision();
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        getPlayerCollision();
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }

    bool isEnableCancelAction = mStateWait->isEnableCancelAction();
    if (isEnableCancelAction) {
        if (rs::updateJudgeAndResult(mJudgeCameraSubjective)) {
            al::setNerve(this, &Camera);
            return;
        }
        if (tryActionCapSpinAttackImpl(true)) {
            al::setNerve(this, &SpinCap);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeStartRun)) {
            al::setNerve(this, &Run);
            return;
        }
    }

    if (mStateWait->isEnableCancelHipDropJump()) {
        if (rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
            mJumpMessageRequest->jumpType = PlayerJumpType::HipDropJump;
            al::setNerve(this, &Jump);
            return;
        }
        if (mJudgeStartRolling->judgeCancelHipDrop()) {
            mTrigger->set(PlayerTrigger::EActionTrigger_val17);
            al::setNerve(this, &Rolling);
            return;
        }
    }

    if (isEnableCancelAction) {
        if (rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
            mExternalVelocity->cancelAndFeedbackLastGroundInertia(
                this, mConst->getJumpInertiaRate(), false);
            mTrigger->set(PlayerTrigger::EActionTrigger_val21);
            al::setNerve(this, &Jump);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeStartSquat)) {
            al::setNerve(this, &Squat);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeInWater1)) {
            if (mActionDiveInWater->isDiveInWaterAnim())
                mTrigger->set(PlayerTrigger::EActionTrigger_val9);
            al::setNerve(this, &Swim);
            return;
        }
    }

    if (mColliderHakoniwa->isEnableStandUp() && rs::isPlayer2D(mHackCap) &&
        mHackCap->isEnableSpinAttack() && rs::isJudge(mHackCapJudgePreInputHoveringJump)) {
        rs::resetJudge(mHackCapJudgePreInputHoveringJump);
        *(((u8*)mHackCapJudgePreInputHoveringJump) + 44) = 1;
        mJumpMessageRequest->jumpType = PlayerJumpType::val_12;
        al::setNerve(this, &Jump);
        return;
    }

    if (mStateWait->tryConnectWait())
        rs::resetCollision(mColliderHakoniwa);
}

bool PlayerActorHakoniwa::tryActionCapReturn() {
    GameDataHolderAccessor accessor = {this};
    if (!GameDataFunction::isEnableCap(accessor) || rs::is2D(this) ||
        !mInput->isTriggerCapReturn() || !mHackCap->isRequestableReturn())
        return false;

    if (mCarryKeeper->isCarry()) {
        if (mSeparateCapFlag->someCheck())
            return false;
    }

    bool someFlag = false;
    if (!mHackCap->requestReturn(&someFlag)) {
        if (someFlag)
            al::startHitReaction(this, "帽子が戻せない");
        return false;
    }

    if (mSeparateCapFlag->someCheck())
        rs::resetJudge(mJudgePreInputCapThrow);
    else
        rs::resetJudge(mHackCapJudgePreInputSeparateThrow);
    return true;
}

bool PlayerActorHakoniwa::tryActionCapSpinAttack() {
    return tryActionCapSpinAttackImpl(true);
}

void PlayerActorHakoniwa::exeSquat() {
    if (al::isFirstStep(this)) {
        mCapActionHistory->clearLandLimitStandAngle();
        rs::resetJudge(mJudgeSpeedCheckFall);
    }

    tryActionSeparateCapThrow();
    if (al::updateNerveState(this)) {
        tryActionCapReturn();
        setNerveOnGround();
        return;
    }

    mExternalVelocity->requestApplyLastGroundInertia();
    if (rs::updateJudgeAndResult(mJudgeSpeedCheckFall)) {
        al::setNerve(this, &Fall);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        getPlayerCollision();
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        getPlayerCollision();
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeStartRolling)) {
        al::setNerve(this, &Rolling);
        return;
    }

    tryActionCapReturn();

    if (rs::updateJudgeAndResult(mJudgePreInputJump)) {
        if (mModelChangerHakoniwa->is2DModel()) {
            mJumpMessageRequest->jumpType = PlayerJumpType::val_0F;
            mJumpMessageRequest->isEnableStandUp =
                rs::updateJudgeAndResult(mJudgeEnableStandUp);
            al::setNerve(this, &Jump);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
            if (mStateSquat->isEnableLongJump()) {
                al::setNerve(this, &LongJump);
                return;
            }
            mJumpMessageRequest->jumpType = PlayerJumpType::val_0E;
            al::setNerve(this, &Jump);
            return;
        }
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        if (mActionDiveInWater->isDiveInWaterAnim())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return;
    }

    if (/*...*/ rs::isPlayer2D(mHackCap))
        CRASH
}

bool PlayerActorHakoniwa::tryActionSeparateCapThrow() {
    if (mSeparateCapFlag->someCheck() ||
        PlayerEquipmentFunction::isEquipmentNoCapThrow(mEquipmentUser))
        return false;

    if (!GameDataFunction::isEnableCap(this) || rs::is2D(this) || mHackCap->isRequestableReturn() ||
        !mHackCap->isEnableThrowSeparate() || !mSandSinkAffect->isEnableCapThrow())
        return false;

    if (!al::isNerve(this, &GrabCeil) && mColliderHakoniwa->getSafetyCeilSpace() <
                                             mConst->getSeparateEnableThrowHeight())
        return false;

    if (rs::judgeAndResetReturnTrue(mHackCapJudgePreInputSeparateJump)) {
        sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
        sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
        rs::calcGroundNormalOrGravityDir(&a2, this, mColliderHakoniwa);
        mInput->calcCapSeparateMoveInput(&a2, a3);
        if (!al::tryNormalizeOrZero(&a2) && !rs::calcAlongDirFront(&a2, this, a3))
            al::calcFrontDir(&a2, this);
        mHackCap->startThrowSeparatePlayJump(a2, a3, 1.0f);
        mJudgePreInputCapThrow->recordSeparateJudge();
        return true;
    }

    if (rs::judgeAndResetReturnTrue(mHackCapJudgePreInputSeparateThrow)) {
        sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
        sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
        rs::calcGroundNormalOrGravityDir(&a2, this, mColliderHakoniwa);
        mInput->calcCapSeparateMoveInput(&a2, a3);
        if (!al::tryNormalizeOrZero(&a2) && !rs::calcAlongDirFront(&a2, this, a3))
            al::calcFrontDir(&a2, this);
        mHackCap->startThrowSeparatePlay(a2, a3, 1.0f,
                                         sead::Mathf::max(al::calcSpeedExceptDir(this, a3),
                                                          mHackCap->calcSeparateHideSpeedH(a3)) >
                                             mConst->getDashFastBorderSpeed());
        mJudgePreInputCapThrow->recordSeparateJudge();
        return true;
    }

    return false;
}

void PlayerActorHakoniwa::exeRun() {
    if (al::isFirstStep(this)) {
        mCapActionHistory->clearLandLimitStandAngle();
        rs::resetJudge(mJudgeSpeedCheckFall);
    }

    tryActionCapReturn();
    if (al::updateNerveState(this)) {
        setNerveOnGround();
        if (al::isNerve(this, &Fall)) {
            mTrigger->set(PlayerTrigger::EActionTrigger_val14);
            mJudgeWallCatchInputDir->validateFallJudge();
        }
        return;
    }

    /* TODO
    if(mTrigger->isOn(PlayerTrigger::EMaterialChangeTrigger_val0) &&
    rs::isSubAnimHeadSlidingLand(mAnimator)) { mWetControl->recordPuddleRolling();
    }
    */

    mExternalVelocity->requestApplyLastGroundInertia();
    if (rs::updateJudgeAndResult(mJudgeWallCatchInputDir)) {
        mStateWallCatch->setup(
            mJudgeWallCatchInputDir->mCollisionParts, mJudgeWallCatchInputDir->mHitPos,
            -mJudgeWallCatchInputDir->mHitNormal2, mJudgeWallCatchInputDir->mHitNormal);
        al::setNerve(this, &WallCatch);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }

    if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Squat);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        getPlayerCollision();
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        getPlayerCollision();
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }

    if (tryActionCapSpinAttackImpl(true)) {
        if (mStateRunHakoniwa2D3D->isGroundSpin()) {
            if (mStateRunHakoniwa2D3D->isSpinClockwise())
                mTrigger->set(PlayerTrigger::EActionTrigger_val28);
            else
                mTrigger->set(PlayerTrigger::EActionTrigger_val27);
        }
        if (mStateRunHakoniwa2D3D->isRunWaterSurface())
            mTrigger->set(PlayerTrigger::EActionTrigger_val33);
        al::setNerve(this, &SpinCap);
        return;
    }

    sead::Vector3f v43 = {0.0f, 0.0f, 0.0f};
    if (mStateRunHakoniwa2D3D->tryTurnJump(mJudgePreInputJump, &v43)) {
        rs::resetJudge(mJudgePreInputJump);
        mExternalVelocity->cancelAndFeedbackLastGroundInertia(
            this, mConst->getJumpInertiaRate(), true);
        mJumpMessageRequest->jumpType = PlayerJumpType::TurnJump;
        mJumpMessageRequest->turnJumpAngle = v43;
        al::setNerve(this, &Jump);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgePreInputJump)) {
        mExternalVelocity->cancelAndFeedbackLastGroundInertia(
            this, mConst->getJumpInertiaRate(), true);
        if (mStateRunHakoniwa2D3D->isGroundSpin()) {
            mJumpMessageRequest->jumpType = PlayerJumpType::val_0C;
            mJumpMessageRequest->isSpinClockwise =
                mStateRunHakoniwa2D3D->isSpinClockwise();
        } else if (mStateRunHakoniwa2D3D->isRunDashFast()) {
            mJumpMessageRequest->jumpType = PlayerJumpType::val_03;
        } else {
            mTrigger->set(PlayerTrigger::EActionTrigger_val21);
        }

        if (mStateRunHakoniwa2D3D->isBrake2D()) {
            sead::Vector3f front = {0.0f, 0.0f, 0.0f};
            sead::Vector3f up = {0.0f, 0.0f, 0.0f};
            al::calcFrontDir(&front, this);
            al::calcUpDir(&up, this);
            sead::Quatf quat = sead::Quatf::unit;
            al::makeQuatFrontUp(&quat, -front, up);
            al::updatePoseQuat(this, quat);
            rs::resetCollisionPose(mColliderHakoniwa, quat);
        }

        al::setNerve(this, &Jump);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSpeedCheckFall) &&
        !mStateRunHakoniwa2D3D->isRunWaterSurface()) {
        mExternalVelocity->cancelAndFeedbackLastGroundInertia(
            this, mConst->getJumpInertiaRate(), false);
        mJudgeWallCatchInputDir->validateFallJudge();
        mTrigger->set(PlayerTrigger::EActionTrigger_val14);
        al::setNerve(this, &Fall);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeStartSquat)) {
        al::setNerve(this, &Squat);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeInWater1) &&
        !mStateRunHakoniwa2D3D->isRunWaterSurface()) {
        if (mActionDiveInWater->isDiveInWaterAnim())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeWallHitDownForceRun)) {
        mTrigger->set(PlayerTrigger::EActionTrigger_val10);
        if (mEquipmentUser->getEquipmentSensor() && mCounterForceRun->getCounter() >= 1)
            mEquipmentUser->cancelEquip();
        al::setNerve(this, &Damage);
        return;
    }

    if (rs::isPlayer2D(mHackCap)) {
        // and more checks, but this one is a killer criterion
        CRASH
    }
}

void PlayerActorHakoniwa::exeSlope() {
    tryActionSeparateCapThrow();
    tryActionCapReturn();
    if (al::updateNerveState(this)) {
        setNerveOnGround();
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    }

    if (mStateSlope->isEnableCancelSandSink() &&
        rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeStartRolling)) {
        al::setNerve(this, &Rolling);
        return;
    }

    if (rs::isCollidedGround(getPlayerCollision()) &&
        rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        mTrigger->set(PlayerTrigger::EActionTrigger_val29);
        if (mActionDiveInWater->isDiveInWaterAnim())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeInvalidateInputFall)) {
        mInput->convergedInUpdateToDisableInput = 1;
        mInput->mIsDisableInput = true;
        mInput->usedInUpdate = false;
        al::setNerve(this, &Fall);
        return;
    }
}

void PlayerActorHakoniwa::exeRolling() {
    tryActionSeparateCapThrow();
    if (rs::isCollidedGround(mColliderHakoniwa)) {
        // stain stuff
        mCapActionHistory->clearLandLimitStandAngle();
    }

    if (mTrigger->isOn(PlayerTrigger::EMaterialChangeTrigger_val0))
        mWetControl->recordPuddleRolling();

    if (al::updateNerveState(this)) {
        if (mStateRolling->isEndStandUp())
            mTrigger->set(PlayerTrigger::EActionTrigger_val25);
        setNerveOnGround();
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeInvalidateInputFall)) {
        mInput->convergedInUpdateToDisableInput = 1;
        mInput->mIsDisableInput = true;
        mInput->usedInUpdate = false;
        al::setNerve(this, &Fall);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        getPlayerCollision();
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        getPlayerCollision();
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }

    if (mStateRolling->isRollingJump() && tryActionCapSpinAttack()) {
        al::setNerve(this, &SpinCap);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        if (mActionDiveInWater->isDiveInWaterAnim())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeWallHitDownRolling)) {
        mTrigger->set(PlayerTrigger::EActionTrigger_val10);
        if (mEquipmentUser->getEquipmentSensor() && mCounterForceRun->getCounter() >= 1)
            mEquipmentUser->cancelEquip();
        al::setNerve(this, &Damage);
        return;
    }

    if (mStateRolling->isEndSquat()) {
        al::setNerve(this, &Squat);
        return;
    }
}

void PlayerActorHakoniwa::exeSpinCap() {
    tryActionSeparateCapThrow();
    if (al::updateNerveState(this)) {
        setNerveOnGround();
        return;
    }

    if (!mStateSpinCap->_78)
        mCapActionHistory->clearLandLimitStandAngle();

    if (mStateSpinCap->isEnableCancelHipDrop() &&
        rs::updateJudgeAndResult(mJudgeStartHipDrop)) {
        mStateSpinCap->cancelPoseInterpole();
        al::setNerve(this, &HipDrop);
        return;
    }

    if (mCarryKeeper->isCarry()) {
        setNerveOnGround();
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeInWater1) && mStateSpinCap->noticeInWater())
        mStateSwim->tryReactionWaterIn();

    if (mStateSpinCap->isEnableCancelAir()) {
        tryActionCapReturn();
        if (tryChangeNerveFromAir()) {
            if (al::isNerve(this, &WallCatch))
                mStateSpinCap->resetJoint();
            return;
        }

        if (rs::isLandGroundRunAngle(this, getPlayerCollision(), mConst)) {
            setNerveOnGround();
            return;
        }

        if (tryActionCapSpinAttackImpl(false))
            mSpinCapAttack->tryStartCapSpinAirMiss(mAnimator);
        return;
    }

    if (!mStateSpinCap->isEnableCancelGround())
        return;

    if (mStateSpinCap->isEnableReThrowCap() && tryActionCapSpinAttackImpl(true)) {
        if (mStateSpinCap->isWaterSurfaceRun())
            mTrigger->set(PlayerTrigger::EActionTrigger_val33);
        al::setNerve(this, &SpinCap);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeWallCatchInputDir)) {
        mStateWallCatch->setup(
            mJudgeWallCatchInputDir->mCollisionParts, mJudgeWallCatchInputDir->mHitPos,
            -mJudgeWallCatchInputDir->mHitNormal2, mJudgeWallCatchInputDir->mHitNormal);
        al::setNerve(this, &WallCatch);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        getPlayerCollision();
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        getPlayerCollision();
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }

    if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Squat);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeStartWaterSurfaceRun)) {
        al::setNerve(this, &Run);
        return;
    }

    if (!mStateSpinCap->isWaterSurfaceRun() &&
        rs::updateJudgeAndResult(mJudgeInWater1)) {
        if (mActionDiveInWater->isDiveInWaterAnim())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return;
    }

    if (rs::isOnGroundRunAngle(this, getPlayerCollision(), mConst) &&
        rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    }
}

bool PlayerActorHakoniwa::tryChangeNerveFromAir() {
    if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return true;
    }
    if (rs::updateJudgeAndResult(mJudgeInvalidateInputFall)) {
        mInput->convergedInUpdateToDisableInput = 1;
        mInput->mIsDisableInput = true;
        mInput->usedInUpdate = false;
        al::setNerve(this, &Fall);
        return true;
    }
    if (!al::isNerve(this, &SpinCap) && tryActionCapSpinAttackImpl(true)) {
        al::setNerve(this, &SpinCap);
        return true;
    }
    if (!al::isNerve(this, &LongJump) && rs::updateJudgeAndResult(mJudgeStartHipDrop)) {
        al::setNerve(this, &HipDrop);
        return true;
    }
    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return true;
    }
    if ((!al::isNerve(this, &GrabCeil) || mStateGrabCeil->isEnableNextGrabCeil()) &&
        rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return true;
    }
    if (al::isNerve(this, &LongJump) && rs::updateJudgeAndResult(mJudgeWallHitDown)) {
        mTrigger->set(PlayerTrigger::EActionTrigger_val10);
        if (mEquipmentUser->getEquipmentSensor() && mCounterForceRun->getCounter() >= 1)
            mEquipmentUser->cancelEquip();
        al::setNerve(this, &Damage);
        return true;
    }
    if (rs::updateJudgeAndResult(mJudgeWallCatch)) {
        mStateWallCatch->setup(
            mJudgeWallCatch->mCollidedWallPart, mJudgeWallCatch->mPosition,
            -mJudgeWallCatch->mCollidedWallNormal, mJudgeWallCatch->mNormalAtPos);
        al::setNerve(this, &WallCatch);
        return true;
    }
    if (!al::isNerve(this, &WallAir) && rs::updateJudgeAndResult(mJudgeWallKeep)) {
        al::setNerve(this, &WallAir);
        return true;
    }
    if (rs::updateJudgeAndResult(mJudgeCapCatchPop)) {
        al::setNerve(this, &CapCatchPop);
        return true;
    }
    if (rs::updateJudgeAndResult(mJudgeStartWaterSurfaceRun)) {
        al::setNerve(this, &Run);
        return true;
    }
    if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        if (mActionDiveInWater->isDiveInWaterAnim())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return true;
    }
    return false;
}

bool PlayerActorHakoniwa::tryActionCapSpinAttackMiss() {
    CRASH
}

void PlayerActorHakoniwa::exeJump() {
    if (al::isFirstStep(this)) {
        rs::resetJudge(mJudgeOutInWater);
        rs::resetJudge(mJudgePreInputJump);
    }
    tryActionCapReturn();
    bool isFormSquat2D = mStateJump->isFormSquat2D();
    if (al::updateNerveState(this)) {
        mStateJump->tryCountUpContinuousJump(mContinuousJump);
        if (mStateJump->isEndJumpDownFallLand())
            mTrigger->set(PlayerTrigger::EActionTrigger_val11);

        setNerveOnGround();
        if (isFormSquat2D && rs::updateJudgeAndResult(mJudgeStartSquat)) {
            if (al::isNerve(this, &Run)) {
                al::setNerve(this, &Squat);
                return;
            }
            if (al::isNerve(this, &Jump)) {
                mJumpMessageRequest->jumpType = PlayerJumpType::val_0F;
                mJumpMessageRequest->isEnableStandUp =
                    rs::updateJudgeAndResult(mJudgeEnableStandUp);
            }
        }
    } else {
        if (mStateJump->isHovering())
            CRASH  // set some unknown attribute of another judge
                if (mStateJump->isJumpBack() || mStateJump->isJumpSpinFlower())
                    mTrigger->set(PlayerTrigger::EActionTrigger_val30);
        if (rs::updateJudgeAndResult(mJudgeInvalidateInputFall))
            CRASH
        if (tryActionCapSpinAttackImpl(true)) {
            if (mStateJump->isJumpSpinGround()) {
                mTrigger->set(mStateJump->isJumpSpinGroundClockwise() ?
                                        PlayerTrigger::EActionTrigger_val28 :
                                        PlayerTrigger::EActionTrigger_val27);
            }
            al::setNerve(this, &SpinCap);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
            if (mCarryKeeper->isCarry())
                mCarryKeeper->startCancelAndRelease();
            al::setNerve(this, &Slope);
            return;
        }
        if (mStateJump->isEnableHipDropStart() &&
            rs::updateJudgeAndResult(mJudgeStartHipDrop)) {
            al::setNerve(this, &HipDrop);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
            mStatePoleClimb->setup(
                mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
                mJudgePoleClimb->_64, mJudgePoleClimb->_70,
                mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
                mJudgePoleClimb->mMaterialCodeCeiling);
            al::setNerve(this, &PoleClimb);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
            mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts,
                                        mJudgeGrabCeil->_48, mJudgeGrabCeil->_54,
                                        mJudgeGrabCeil->_60);
            al::setNerve(this, &GrabCeil);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeWallCatch)) {
            mStateWallCatch->setup(
                mJudgeWallCatch->mCollidedWallPart, mJudgeWallCatch->mPosition,
                -mJudgeWallCatch->mCollidedWallNormal, mJudgeWallCatch->mNormalAtPos);
            al::setNerve(this, &WallCatch);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeWallKeep)) {
            al::setNerve(this, &WallAir);
            return;
        }
        if (mStateJump->isJumpCapCatch() &&
            rs::updateJudgeAndResult(mJudgeCapCatchPop)) {
            al::setNerve(this, &CapCatchPop);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeStartWaterSurfaceRun)) {
            al::setNerve(this, &Run);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeOutInWater))
            CRASH
        if (rs::updateJudgeAndResult(mJudgeWallHitDownForceRun)) {
            mTrigger->set(PlayerTrigger::EActionTrigger_val10);
            if (mEquipmentUser->getEquipmentSensor() &&
                mCounterForceRun->getCounter() >= 1)
                mEquipmentUser->cancelEquip();
            al::setNerve(this, &Damage);
            return;
        }
        if (mStateJump->isEnableCancelCarryThrow() && mCarryKeeper->isThrowRelease())
            CRASH
    }
}

void PlayerActorHakoniwa::exeCapCatchPop() {
    CRASH
}

void PlayerActorHakoniwa::exeWallAir() {
    if (al::isFirstStep(this))
        mCapActionHistory->clearWallAirLimit();

    tryActionCapReturn();
    if (al::updateNerveState(this)) {
        setNerveOnGround();
        return;
    }

    if (mStateWallAir->isAir()) {
        if (tryChangeNerveFromAir())
            return;
    } else {
        if (tryActionCapSpinAttackImpl(true)) {
            mInput->resetAlongWall();
            mTrigger->set(PlayerTrigger::EActionTrigger_val0);
            mStateWallAir->startSlideSpinAttack();
            al::setNerve(this, &SpinCap);
            return;
        }
        if (mStateWallAir) {
            al::HitSensor* wallSensor = rs::tryGetCollidedWallSensor(mColliderHakoniwa);
            if (wallSensor)
                rs::sendMsgPlayerStartWallJump(wallSensor, mBodyHitSensor);
        }
    }

    if (rs::updateJudgeAndResult(mJudgeStartWaterSurfaceRun)) {
        al::setNerve(this, &Run);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        if (mActionDiveInWater->isDiveInWaterAnim())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeWallCatch)) {
        mStateWallCatch->setup(
            mJudgeWallCatch->mCollidedWallPart, mJudgeWallCatch->mPosition,
            -mJudgeWallCatch->mCollidedWallNormal, mJudgeWallCatch->mNormalAtPos);
        al::setNerve(this, &WallCatch);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeInvalidateInputFall)) {
        mInput->convergedInUpdateToDisableInput = 1;
        mInput->mIsDisableInput = true;
        mInput->usedInUpdate = false;
        al::setNerve(this, &Fall);
        return;
    }
}

void PlayerActorHakoniwa::exeWallCatch() {
    CRASH
}

void PlayerActorHakoniwa::exeGrabCeil() {
    CRASH
}

void PlayerActorHakoniwa::exePoleClimb() {
    CRASH
}

void PlayerActorHakoniwa::exeHipDrop() {
    if (al::isFirstStep(this))
        rs::resetJudge(mJudgeAirForceCount);

    if (rs::isCollidedGround(mColliderHakoniwa))
        mCapActionHistory->clearLandLimitStandAngle();

    if (mStateHipDrop->isLandTrigger()) {
        if (mEquipmentUser->getEquipmentSensor() && mCounterForceRun->getCounter() >= 1)
            mEquipmentUser->cancelEquip();
        if (rs::isCollidedGround(mColliderHakoniwa)) {
            // TODO stain and wetness
        }
    }

    tryActionSeparateCapThrow();
    if (al::updateNerveState(this))
        setNerveOnGround();

    if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }
    if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Squat);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        getPlayerCollision();
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeAirForceCount)) {
        al::setNerve(this, &Fall);
        return;
    }

    if (mStateHipDrop->isEnableLandCancel()) {
        if (rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
            mJumpMessageRequest->jumpType = PlayerJumpType::HipDropJump;
            al::setNerve(this, &Jump);
            return;
        }
        if (mJudgeStartRolling->judgeCancelHipDrop()) {
            mTrigger->set(PlayerTrigger::EActionTrigger_val17);
            al::setNerve(this, &Rolling);
            return;
        }
        if (tryActionCapSpinAttackImpl(true)) {
            mTrigger->set(PlayerTrigger::EActionTrigger_val18);
            al::setNerve(this, &SpinCap);
            return;
        }
    }

    if (mStateHipDrop->isEnableMove()) {
        if (tryActionCapSpinAttackImpl(true)) {
            al::setNerve(this, &SpinCap);
            return;
        }
        if (rs::updateJudgeAndResult(mJudgeStartRun)) {
            al::setNerve(this, &Run);
            return;
        }
        if (rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
            al::setNerve(this, &Jump);
            return;
        }
    }

    if (mStateHipDrop->isEnableHeadSliding() && !mCarryKeeper->isCarry()) {
        if (!rs::isOnGround(this, getPlayerCollision()) && mInput->isTriggerHeadSliding()) {
            al::setNerve(this, &HeadSliding);
            return;
        }
    }

    if (mStateHipDrop->isEnableInWater() && rs::updateJudgeAndResult(mJudgeInWater3)) {
        if (mEquipmentUser->getEquipmentSensor() && mCounterForceRun->getCounter() >= 1)
            mEquipmentUser->cancelEquip();
        mCounterForceRun->clearCounter();
        mTrigger->set(PlayerTrigger::EActionTrigger_val19);
        al::setNerve(this, &Swim);
        return;
    }
}

void PlayerActorHakoniwa::exeHeadSliding() {
    if (al::isFirstStep(this))
        rs::resetJudge(mJudgeAirForceCount);

    if (rs::isCollidedGround(mColliderHakoniwa))
        mCapActionHistory->clearLandLimitStandAngle();

    tryActionSeparateCapThrow();

    if (al::updateNerveState(this)) {
        if (rs::updateJudgeAndResult(mJudgeDirectRolling)) {
            mTrigger->set(PlayerTrigger::EActionTrigger_val8);
            al::setNerve(this, &Rolling);
            return;
        }
        mTrigger->set(PlayerTrigger::EActionTrigger_val16);
        setNerveOnGround();
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }

    if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Squat);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        if (mStateHeadSliding->isEnableDiveInWater())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        else
            mTrigger->set(PlayerTrigger::EActionTrigger_val15);
        al::setNerve(this, &Swim);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeStartWaterSurfaceRun)) {
        al::setNerve(this, &Run);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        getPlayerCollision();
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeWallHitDown)) {
        mTrigger->set(PlayerTrigger::EActionTrigger_val10);
        if (mEquipmentUser->getEquipmentSensor() && mCounterForceRun->getCounter() >= 1)
            mEquipmentUser->cancelEquip();
        al::setNerve(this, &Damage);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeAirForceCount)) {
        al::setNerve(this, &Fall);
        return;
    }
}

void PlayerActorHakoniwa::exeLongJump() {
    tryActionCapReturn();
    if (al::updateNerveState(this)) {
        if (rs::updateJudgeAndResult(mJudgeDirectRolling)) {
            mTrigger->set(PlayerTrigger::EActionTrigger_val8);
            al::setNerve(this, &Rolling);
            return;
        }
        setNerveOnGround();
    }
    tryChangeNerveFromAir();
}

void PlayerActorHakoniwa::exeFall() {
    al::updateNerveState(this);
    tryActionCapReturn();
    if (tryActionCapSpinAttackImpl(true)) {
        // result ignored
        rs::isJustLand(mColliderHakoniwa);
        if (rs::isLandGroundRunAngle(this, mColliderHakoniwa, mConst))
            mTrigger->set(PlayerTrigger::EActionTrigger_val1);
        else
            mTrigger->set(PlayerTrigger::EActionTrigger_val2);
        al::setNerve(this, &SpinCap);
        return;
    }
    if (mStateFallHakoniwa->isValidWallCatch()) {
        if (mJudgeWallCatchInputDir->mValidTimer >= 1 &&
            rs::updateJudgeAndResult(mJudgeWallCatchInputDir)) {
            mStateWallCatch->setup(
                mJudgeWallCatchInputDir->mCollisionParts, mJudgeWallCatchInputDir->mHitPos,
                -mJudgeWallCatchInputDir->mHitNormal2, mJudgeWallCatchInputDir->mHitNormal);
            al::setNerve(this, &WallCatch);
            return;
        }
    }
    if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
        sub_71004229D0(this, mTrigger, mColliderHakoniwa);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    }
    if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if (mCarryKeeper->isCarry())
            mCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Squat);
        return;
    }
    if (rs::isLandGroundRunAngle(this, getPlayerCollision(), mConst)) {
        if (mStateFallHakoniwa->flag2 && al::isFirstStep(this))
            mTrigger->set(PlayerTrigger::EActionTrigger_val24);
        setNerveOnGround();
        if (al::isNerve(this, &Run) && !rs::isJustLand(mColliderHakoniwa)) {
            al::alongVectorNormalH(al::getVelocityPtr(this), al::getVelocity(this),
                                   -al::getGravity(this),
                                   rs::getCollidedGroundNormal(mColliderHakoniwa));
        }
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeStartHipDrop)) {
        al::setNerve(this, &HipDrop);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
        mStatePoleClimb->setup(
            mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
            mJudgePoleClimb->_64, mJudgePoleClimb->_70,
            mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
            mJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
        getPlayerCollision();
        mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts, mJudgeGrabCeil->_48,
                                    mJudgeGrabCeil->_54, mJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    }
    if (!mStateFallHakoniwa->flag1 && !mStateFallHakoniwa->mInvalidateInputFallArea) {
        if (rs::updateJudgeAndResult(mJudgeWallKeep)) {
            al::setNerve(this, &WallAir);
            return;
        }
    }
    if (mStateFallHakoniwa->isValidWallCatch() &&
        rs::updateJudgeAndResult(mJudgeWallCatch)) {
        mStateWallCatch->setup(
            mJudgeWallCatch->mCollidedWallPart, mJudgeWallCatch->mPosition,
            -mJudgeWallCatch->mCollidedWallNormal, mJudgeWallCatch->mNormalAtPos);
        al::setNerve(this, &WallCatch);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeCapCatchPop)) {
        al::setNerve(this, &CapCatchPop);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeStartWaterSurfaceRun)) {
        al::setNerve(this, &Run);
        return;
    }
    if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        if (mActionDiveInWater->isDiveInWaterAnim())
            mTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return;
    }

    if (mStateFallHakoniwa->flag1 || mStateFallHakoniwa->mInvalidateInputFallArea) {
        mInput->convergedInUpdateToDisableInput = 1;
        mInput->mIsDisableInput = true;
        mInput->usedInUpdate = false;
        return;
    }

    if (rs::isPlayer2D(this)) {
        // and more checks, but this one is a killer criterion
        CRASH
    }
}

void PlayerActorHakoniwa::exeSandSink() {
    CRASH
}

void PlayerActorHakoniwa::exeSandGeyser() {
    CRASH
}

void PlayerActorHakoniwa::exeRise() {
    CRASH
}

void PlayerActorHakoniwa::exeSwim() {
    CRASH
}

void PlayerActorHakoniwa::exeDamage() {
    if (al::isFirstStep(this)) {
        rs::resetJudge(mJudgeSpeedCheckFall);
        rs::resetJudge(mJudgeAirForceCount);
        mCapActionHistory->clearLandLimitStandAngle();
    }

    tryActionSeparateCapThrow();
    if (al::updateNerveState(this)) {
        if (!PlayerFunction::isPlayerDeadStatus(this)) {
            setNerveOnGround();
            return;
        }
        al::setNerve(this, &Dead);
        return;
    }

    if (PlayerFunction::isPlayerDeadStatus(this))
        return;

    if (rs::updateJudgeAndResult(mJudgeInWater1)) {
        if (mStateDamageLife->isLand()) {
            al::setNerve(this, &Swim);
            return;
        }
        mTrigger->set(PlayerTrigger::EActionTrigger_val5);
        al::setNerve(this, &DamageSwim);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeAirForceCount)) {
        al::setNerve(this, &Fall);
        return;
    }

    if (mStateDamageLife->isLand()) {
        if (rs::updateJudgeAndResult(mJudgeForceSlopeSlide)) {
            if (mCarryKeeper->isCarry())
                mCarryKeeper->startCancelAndRelease();
            al::setNerve(this, &Slope);
            return;
        }

        if (rs::updateJudgeAndResult(mJudgeForceRolling)) {
            sead::Vector3f groundNormal = rs::getCollidedGroundNormal(mColliderHakoniwa);
            sead::Vector3f front = {0.0f, 0.0f, 0.0f};
            if (!rs::calcSlideDir(&front, al::getGravity(this), groundNormal))
                al::calcFrontDir(&front, this);

            sead::Quatf quat = sead::Quatf::unit;
            al::makeQuatFrontUp(&quat, front, groundNormal);
            al::updatePoseQuat(this, quat);
            sub_71004229D0(this, mTrigger, mColliderHakoniwa);
            return;
        }

        if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
            if (mCarryKeeper->isCarry())
                mCarryKeeper->startCancelAndRelease();
            mTrigger->set(PlayerTrigger::EActionTrigger_val12);
            al::setNerve(this, &Squat);
            return;
        }

        if (rs::updateJudgeAndResult(mJudgeSpeedCheckFall)) {
            al::setNerve(this, &Fall);
            return;
        }

        if (rs::updateJudgeAndResult(mJudgePoleClimb)) {
            mStatePoleClimb->setup(
                mJudgePoleClimb->mCollisionParts, mJudgePoleClimb->_58,
                mJudgePoleClimb->_64, mJudgePoleClimb->_70,
                mJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mJudgePoleClimb->_7C,
                mJudgePoleClimb->mMaterialCodeCeiling);
            al::setNerve(this, &PoleClimb);
            return;
        }

        if (rs::updateJudgeAndResult(mJudgeGrabCeil)) {
            getPlayerCollision();
            mStateGrabCeil->setup(mJudgeGrabCeil->mCollidedParts,
                                        mJudgeGrabCeil->_48, mJudgeGrabCeil->_54,
                                        mJudgeGrabCeil->_60);
            al::setNerve(this, &GrabCeil);
            return;
        }
    }

    if (mStateDamageLife->isEnableCancel() &&
        rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    }
}

void PlayerActorHakoniwa::exeDamageSwim() {
    CRASH
}

void PlayerActorHakoniwa::exeDamageFire() {
    CRASH
}

void PlayerActorHakoniwa::exePress() {
    CRASH
}

void PlayerActorHakoniwa::exeHack() {
    CRASH
}

void PlayerActorHakoniwa::exeEndHack() {
    CRASH
}

void PlayerActorHakoniwa::exeBind() {
    CRASH
}

bool PlayerActorHakoniwa::tryActionCapSpinAttackBindEnd() {
    CRASH
}

void PlayerActorHakoniwa::exeDemo() {
    CRASH
}

void PlayerActorHakoniwa::exeCamera() {
    CRASH
}

void PlayerActorHakoniwa::exeAbyss() {
    CRASH
}

void PlayerActorHakoniwa::exeDead() {
    CRASH
}

bool PlayerActorHakoniwa::tryActionCapSpinAttackImpl(bool a2) {
    if (PlayerEquipmentFunction::isEquipmentNoCapThrow(mEquipmentUser) || rs::is2D(this))
        return false;

    if (!GameDataFunction::isEnableCap(this)) {
        if (!mCarryKeeper->isCarry()) {
            if (GameDataFunction::isMeetCap(this) && rs::isJudge(mJudgePreInputCapThrow)) {
                rs::resetJudge(mJudgePreInputCapThrow);
                al::startHitReaction(this, "帽子がつかえない");
            }
        }
        return false;
    }

    bool v6 = false;
    bool v5 = !mSeparateCapFlag->someCheck();
    if (v5) {
        if (!tryActionSeparateCapThrow() && mInput->isTriggerSpinAttackSeparate()) {
            mCapActionHistory->recordLimitHeight();
            v6 = true;
        } else {
            return false;
        }
    }

    if ((!a2 && !mSpinCapAttack->isEnablePlaySpinCapMiss(mAnimator)) ||
        mCarryKeeper->isCarry() || !mSandSinkAffect->isEnableCapThrow() ||
        (mSeparateCapFlag->someCheck() && !rs::isJudge(mJudgePreInputCapThrow)))
        return false;

    if (v6)
        mJudgePreInputCapThrow->recordJudgeAndReset();

    if (!mHackCap->isRequestableReturn() && mHackCap->isEnableSpinAttack()) {
        if (!v5) {
            if (a2) {
                mHackCap->prepareCooperateThrow();
                mJudgePreInputCapThrow->recordCooperateAndReset();
            }
        } else {
            mJudgePreInputCapThrow->recordJudgeAndReset();
        }
        mCapActionHistory->recordLimitHeight();
        return true;
    }

    return v6;
}
