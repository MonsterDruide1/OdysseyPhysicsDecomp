#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"
#include "Library/Stage/StageResourceList.h"

class CapMessageMoonNotifier : public al::LiveActor, public al::ISceneObj {
public:
    CapMessageMoonNotifier();
    void initialize(al::StageInfo const*,al::ActorInitInfo const&);
    // more

private:
    char filler[0x20];
};

static_assert(sizeof(CapMessageMoonNotifier) == 0x130);
