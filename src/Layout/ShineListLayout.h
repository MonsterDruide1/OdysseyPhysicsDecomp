#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include "Library/Layout/LayoutActor.h"

namespace al {
class IUseSceneObjHolder;
class LayoutInitInfo;
class PlayerHolder;
}

class ShineListLayout : al::LayoutActor {
public:
    ShineListLayout(al::LayoutInitInfo const&);
private:
    char filler[0xe0];
};

static_assert(sizeof(ShineListLayout) == 0x210);
