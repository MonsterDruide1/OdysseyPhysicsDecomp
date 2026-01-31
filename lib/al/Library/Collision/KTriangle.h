#pragma once

#include "CollisionPartsTriangle.h"

namespace alKCollisionFunc {

void calcSphereHitPos(sead::Vector3<float>*, al::KCollisionServer const*,
                      sead::Vector3<float> const&, al::KCPrismData const&, al::KCPrismHeader const*,
                      u8);

}
