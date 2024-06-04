#pragma once

#include <basis/seadTypes.h>
#include "math/seadVectorFwd.h"

namespace al {
class LiveActor;
class IUseSceneObjHolder;
}  // namespace al

class PlayerInputFunction {
public:
    static bool isTriggerAction(const al::LiveActor* actor, s32 port);
    static bool isHoldAction(const al::LiveActor* actor, s32 port);
    static bool isReleaseAction(const al::LiveActor* actor, s32 port);

    static bool isTriggerJump(const al::LiveActor* actor, s32 port);
    static bool isHoldJump(const al::LiveActor* actor, s32 port);
    static bool isReleaseJump(const al::LiveActor* actor, s32 port);

    static bool isTriggerSubAction(const al::LiveActor* actor, s32 port);
    static bool isHoldSubAction(const al::LiveActor* actor, s32 port);

    static bool isTriggerTalk(const al::LiveActor* actor, s32 port);
    static bool isTriggerStartWorldWarp(const al::LiveActor* actor, s32 port);
    static bool isTriggerCancelWorldWarp(const al::LiveActor* actor, s32 port);

    static sead::Vector2f getMoveInputStick(al::LiveActor const*,int,int);

    static bool isTriggerCameraReset(al::LiveActor const*,int);
};

namespace rs {

bool isSeparatePlay(const al::IUseSceneObjHolder*);

}
