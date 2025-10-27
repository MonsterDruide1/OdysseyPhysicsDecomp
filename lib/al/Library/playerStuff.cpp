#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Player/PlayerHolder.h"

namespace al {

HitSensor* getHitSensor(al::LiveActor const* actor, char const* name) {
    return alActorSensorFunction::getSensorKeeper(actor)->getSensor(name);
}

}  // namespace al

al::HitSensorKeeper* alActorSensorFunction::getSensorKeeper(al::LiveActor const* actor) {
    return actor->mHitSensorKeeper;
}
