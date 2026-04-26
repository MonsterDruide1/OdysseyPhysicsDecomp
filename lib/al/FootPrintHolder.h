#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/IUseAreaObj.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {
class FootPrintServer;

class FootPrintHolder {
public:
    FootPrintHolder(al::LiveActor*, char const*, al::HitSensor*, al::FootPrintServer*);

public:
    void* size[0x58 / 8];
};

}  // namespace al
