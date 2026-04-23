#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/IUseCollision.h"

namespace alCollisionUtil {

s32 checkStrikeSphereMovingReaction(const al::IUseCollision* a1, const sead::Vector3f& a2, float a3,
                                    const sead::Vector3f& a4, const al::CollisionPartsFilterBase* a5,
                                    const al::TriangleFilterBase* a6) {
    auto* cd = a1->getCollisionDirector();
    cd->setPartsFilter(a5);
    cd->setTriFilter(a6);
    return cd->checkStrikeSphere(a2, a3, true, a4);
}

s32 getStrikeSphereInfoNum(const al::IUseCollision* a1) {
    return a1->getCollisionDirector()->getStrikeSphereInfoNum();
}

const al::SphereHitInfo& getStrikeSphereInfo(al::IUseCollision const* a1, unsigned int a2) {
    return *a1->getCollisionDirector()->getStrikeSphereInfo(a2);
}

}  // namespace alCollisionUtil
