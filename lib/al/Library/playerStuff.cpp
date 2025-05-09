#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Player/PlayerHolder.h"
#include "math/seadVectorFwd.h"
#include "playerUtil.h"

namespace al {

HitSensor* getHitSensor(al::LiveActor const* actor, char const* name) {
    return alActorSensorFunction::getSensorKeeper(actor)->getSensor(name);
}

void lerpVec(sead::Vector3<float>* out, sead::Vector3<float> const& a,
             sead::Vector3<float> const& b, float val) {
    *out = a + (b - a) * val;
}

PadRumbleKeeper* getPlayerPadRumbleKeeper(al::LiveActor const* actor, int index) {
    return actor->getSceneInfo()->playerHolder->getPadRumbleKeeper(index);
}

}  // namespace al

al::HitSensorKeeper* alActorSensorFunction::getSensorKeeper(al::LiveActor const* actor) {
    return actor->mHitSensorKeeper;
}
