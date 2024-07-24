
#include "Player/PlayerFormSensorCollisionArranger.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathLengthUtil.h"
#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerColliderHakoniwa.h"
#include "Player/PlayerHackKeeper.h"
#include "Stuff.h"

PlayerFormSensorCollisionArranger::PlayerFormSensorCollisionArranger(
    al::LiveActor* player, PlayerColliderHakoniwa* collider,
    const IPlayerModelChanger* modelChanger, const PlayerHackKeeper* hackKeeper)
    : mPlayer(player), mColliderHakoniwa(collider), mModelChanger(modelChanger),
      mPlayerHackKeeper(hackKeeper) {}

void PlayerFormSensorCollisionArranger::setFormModel3D() {
    if (mFormModel == 1)
        return;
    mFormModel = 1;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormModel2D() {
    if (mFormModel == 2)
        return;
    mFormModel = 2;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionSquat() {
    if (mFormAction == 2)
        return;
    mFormAction = 2;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionStandup() {
    if (mFormAction == 1)
        return;
    mFormAction = 1;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionWallGrab(
    const sead::Vector3f& cutCollideAffectDir) {
    if (mFormAction == 4 &&
        al::isNearDirection(mFrontCutCollideAffectDir, cutCollideAffectDir, 0.01f))
        return;
    mFormAction = 4;
    mFrontCutCollideAffectDir = cutCollideAffectDir;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionGrabCeil(
    const sead::Vector3f& cutCollideAffectDir) {
    if (mFormAction == 5 &&
        al::isNearDirection(mFrontCutCollideAffectDir, cutCollideAffectDir, 0.01f))
        return;
    mFormAction = 5;
    mFrontCutCollideAffectDir = cutCollideAffectDir;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionPoleClimb(
    const sead::Vector3f& cutCollideAffectDir) {
    if (mFormAction == 6 &&
        al::isNearDirection(mFrontCutCollideAffectDir, cutCollideAffectDir, 0.01f))
        return;
    mFormAction = 6;
    mFrontCutCollideAffectDir = cutCollideAffectDir;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionSwim() {
    if (mFormAction == 3)
        return;
    mFormAction = 3;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionHack() {
    if (mFormAction == 7)
        return;
    mFormAction = 7;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionBind(bool isUpdateBind) {
    if (mFormAction == 8 && mIsUpdateBind == isUpdateBind)
        return;
    mFormAction = 8;
    mIsUpdateBind = isUpdateBind;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionRecovery() {
    if (mFormAction == 9)
        return;
    mFormAction = 9;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionAbyss(
    const sead::Vector3f& cutCollideAffectDir) {
    if (mFormAction == 10 &&
        al::isNearDirection(mFrontCutCollideAffectDir, cutCollideAffectDir, 0.01f))
        return;
    mFormAction = 10;
    mFrontCutCollideAffectDir = cutCollideAffectDir;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormActionDead() {
    if (mFormAction == 11)
        return;
    mFormAction = 11;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormAttackSensorNone() {
    if (mFormAttack == 0)
        return;
    mFormAttack = 0;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormAttackSensorSpin() {
    if (mFormAttack == 1)
        return;
    mFormAttack = 1;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setFormAttackSensorTornado() {
    if (mFormAttack == 2)
        return;
    mFormAttack = 2;
    mIsValidForm = true;
}

void PlayerFormSensorCollisionArranger::setCollisionShapeOffsetGround(f32 offset) {
    if (mModelChanger->is2DModel())
        return;

    if (al::isNearZero(offset, 0.001f))
        mColliderHakoniwa->validateGroundSupport();
    else
        mColliderHakoniwa->invalidateGroundSupport();
    CollisionShapeUtil::setShapeOffsetAllArrow(mColliderHakoniwa, sead::Vector3f::ey * offset);
}

const char* PlayerFormSensorCollisionArranger::getHeadSensorName() const {
    CRASH;
}

const sead::Vector3f& PlayerFormSensorCollisionArranger::getHeadPos() const {
    CRASH;
}

f32 PlayerFormSensorCollisionArranger::getHeadRadius() const {
    CRASH;
}

const sead::Vector3f& PlayerFormSensorCollisionArranger::getBodyPos() const {
    CRASH;
}

bool PlayerFormSensorCollisionArranger::isEnableSafetyPointForm() const {
    return (this->mFormAction & 0xFFFFFFFE) != 4;
}

void PlayerFormSensorCollisionArranger::update() {
    if (mIsValidForm)
        syncForm();
}

void PlayerFormSensorCollisionArranger::syncForm() {
    al::invalidateHitSensors(mPlayer);
    if (mFormModel == 2) {
        // 2D
        CRASH
    } else if (mFormModel == 1) {
        // 3D
        // HITSENSOR => ignored
    }

    switch (mFormAction) {
    case 1:
    case 8:
        if (mFormModel == 2)
            mColliderHakoniwa->changeCollision2DNormal();
        else
            mColliderHakoniwa->changeCollisionNormal();
        break;
    case 2:
        if (mFormModel == 2)
            mColliderHakoniwa->changeCollision2DMini();
        else
            mColliderHakoniwa->changeCollisionMini();
        break;
    case 3:
        mColliderHakoniwa->changeCollisionSwim();
        break;
    case 4:
        mColliderHakoniwa->changeCollisionWallGrab(mFrontCutCollideAffectDir);
        break;
    case 5:
        mColliderHakoniwa->changeCollisionGrabCeil(mFrontCutCollideAffectDir);
        break;
    case 6:
        mColliderHakoniwa->changeCollisionPoleClimb(mFrontCutCollideAffectDir);
        break;
    case 7:
        mColliderHakoniwa->changeCollisionHack(mPlayerHackKeeper->getCollisionPartsFilter());
        break;
    case 9:
        mColliderHakoniwa->changeCollisionRecovery(mFormModel == 2);
        break;
    case 10:
        mColliderHakoniwa->changeCollisionAbyss(mFrontCutCollideAffectDir);
        break;
    case 11:
        mColliderHakoniwa->changeCollisionNormal();
        break;
    }

    setCollisionShapeOffsetGround(0.0f);
    mIsValidForm = false;
}

void PlayerFormSensorCollisionArranger::validateAttackSensor() {
    CRASH;
}
