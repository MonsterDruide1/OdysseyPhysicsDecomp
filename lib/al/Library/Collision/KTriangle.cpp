#include "Library/Collision/KTriangle.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Math/MathLengthUtil.h"
#include "math/seadMatrix.h"
#include "math/seadVectorFwd.h"

namespace al {

HitInfo::HitInfo() = default;

bool HitInfo::isCollisionAtFace() const {
    return mCollisionLocation == 1;
}

}  // namespace al
