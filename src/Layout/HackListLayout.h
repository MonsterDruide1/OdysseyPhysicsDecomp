#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include "Library/Layout/LayoutActor.h"

namespace al {
class IUseSceneObjHolder;
class LayoutInitInfo;
class PlayerHolder;
}

class HackListLayout : al::LayoutActor {
public:
    HackListLayout(al::LayoutInitInfo const&);
private:
    char filler[0x68];
};

static_assert(sizeof(HackListLayout) == 0x198);
