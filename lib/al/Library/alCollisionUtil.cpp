#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/IUseCollision.h"

namespace alCollisionUtil {

s32 checkStrikeSphereMovingReaction(const al::IUseCollision* a1, const sead::Vector3f& a2, float a3,
                                    const sead::Vector3f& a4,
                                    const al::CollisionPartsFilterBase* a5,
                                    const al::TriangleFilterBase* a6) {
    auto* cd = a1->getCollisionDirector();
    cd->setPartsFilter(a5);
    cd->setTriFilter(a6);
    return cd->checkStrikeSphere(a2, a3, true, a4);
}

s32 getStrikeSphereInfoNum(const al::IUseCollision* a1) {
    return a1->getCollisionDirector()->getStrikeSphereInfoNum();
}

const al::SphereHitInfo& getStrikeSphereInfo(const al::IUseCollision* a1, unsigned int a2) {
    return *a1->getCollisionDirector()->getStrikeSphereInfo(a2);
}

al::CollisionParts* getStrikeArrowCollisionParts(const al::IUseCollision* a1,
                                                       sead::Vector3f* a2, const sead::Vector3f& a3,
                                                       const sead::Vector3f& a4,
                                                       const al::CollisionPartsFilterBase* a5,
                                                       const al::TriangleFilterBase* a6) {
    al::Triangle tri;
    const al::ArrowHitInfo* hitInfo = nullptr;
    if (!alCollisionUtil::getFirstPolyOnArrow(a1, &hitInfo, a3, a4, a5, a6))
        return nullptr;
    if (a2)
        *a2 = hitInfo->collisionHitPos;
    return const_cast<al::CollisionParts*>(hitInfo->triangle.getCollisionParts());
}

}  // namespace alCollisionUtil
