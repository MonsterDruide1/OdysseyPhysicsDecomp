#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class IPlayerModelChanger;
class PlayerColliderHakoniwa;
class PlayerHackKeeper;

class PlayerFormSensorCollisionArranger {
public:
    PlayerFormSensorCollisionArranger(al::LiveActor*, PlayerColliderHakoniwa*,
                                      const IPlayerModelChanger*, const PlayerHackKeeper*);
    void setFormModel3D();
    void setFormModel2D();
    void setFormActionSquat();
    void setFormActionStandup();
    void setFormActionWallGrab(sead::Vector3<float> const&);
    void setFormActionGrabCeil(sead::Vector3<float> const&);
    void setFormActionPoleClimb(sead::Vector3<float> const&);
    void setFormActionSwim();
    void setFormActionHack();
    void setFormActionBind(bool);
    void setFormActionRecovery();
    void setFormActionAbyss(sead::Vector3<float> const&);
    void setFormActionDead();
    void setFormAttackSensorNone();
    void setFormAttackSensorSpin();
    void setFormAttackSensorTornado();
    void setCollisionShapeOffsetGround(float);
    const char* getHeadSensorName() const;
    const sead::Vector3f& getHeadPos() const;
    f32 getHeadRadius() const;
    const sead::Vector3f& getBodyPos() const;
    bool isEnableSafetyPointForm() const;
    void update();
    void syncForm();
    void validateAttackSensor();

private:
    al::LiveActor* mPlayer;
    PlayerColliderHakoniwa* mColliderHakoniwa;
    const IPlayerModelChanger* mModelChanger;
    const PlayerHackKeeper* mPlayerHackKeeper;
    int mFormModel = 0;
    int mFormAction = 0;
    int mFormAttack = 0;
    bool mIsValidForm = false;
    sead::Vector3f mFrontCutCollideAffectDir = {0.0f, 0.0f, 0.0f};
    bool mIsUpdateBind = false;
};
