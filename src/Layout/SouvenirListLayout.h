#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include "Library/Layout/LayoutActor.h"

namespace al {
class IUseSceneObjHolder;
class LayoutInitInfo;
class PlayerHolder;
}

class SouvenirListLayout : al::LayoutActor {
public:
    SouvenirListLayout(al::LayoutInitInfo const&);
private:
    char filler[0x60];
};

static_assert(sizeof(SouvenirListLayout) == 0x190);
