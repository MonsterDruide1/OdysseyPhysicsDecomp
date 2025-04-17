#include "Player/CollidedShapeResult.h"

#include "Library/Collision/KCollisionServer.h"
#include "Util/CollisionShapeFunction.h"

CollidedShapeResult::CollidedShapeResult(const CollisionShapeInfoBase* shapeInfo)
    : mShapeInfo(shapeInfo) {}

void CollidedShapeResult::setArrowHitInfo(const al::ArrowHitInfo& hitInfo) {
    if (hitInfo.hitInfo->collisionLocation != al::CollisionLocation::None)
        dbg_printf("CollidedShapeResult::setArrowHitInfo{tri.index=%d, unk=%f, mCollisionHitPos=(%f, %f, %f), unk3=(%f, %f, %f), mCollisionMovingReaction=(%f, %f, %f), mCollisionLocation=%d}\n",
            (*hitInfo)->mTriangle.mKCPrismData->triIndex, (*hitInfo)->unk, (*hitInfo)->mCollisionHitPos.x, (*hitInfo)->mCollisionHitPos.y, (*hitInfo)->mCollisionHitPos.z,
            (*hitInfo)->unk3.x, (*hitInfo)->unk3.y, (*hitInfo)->unk3.z, (*hitInfo)->mCollisionMovingReaction.x, (*hitInfo)->mCollisionMovingReaction.y, (*hitInfo)->mCollisionMovingReaction.z, (*hitInfo)->mCollisionLocation);
    *mArrowHitInfo.hitInfo = *hitInfo.hitInfo;
}

void CollidedShapeResult::setSphereHitInfo(const al::SphereHitInfo& hitInfo) {
    if (hitInfo.hitInfo->collisionLocation != al::CollisionLocation::None)
        dbg_printf("CollidedShapeResult::setSphereHitInfo{tri.index=%d, unk=%f, mCollisionHitPos=(%f, %f, %f), unk3=(%f, %f, %f), mCollisionMovingReaction=(%f, %f, %f), mCollisionLocation=%d}\n",
            (*hitInfo)->mTriangle.mKCPrismData->triIndex, (*hitInfo)->unk, (*hitInfo)->mCollisionHitPos.x, (*hitInfo)->mCollisionHitPos.y, (*hitInfo)->mCollisionHitPos.z,
            (*hitInfo)->unk3.x, (*hitInfo)->unk3.y, (*hitInfo)->unk3.z, (*hitInfo)->mCollisionMovingReaction.x, (*hitInfo)->mCollisionMovingReaction.y, (*hitInfo)->mCollisionMovingReaction.z, (*hitInfo)->mCollisionLocation);
    *mSphereHitInfo.hitInfo = *hitInfo.hitInfo;
}

void CollidedShapeResult::setDiskHitInfo(const al::DiskHitInfo& hitInfo) {
    *mDiskHitInfo.hitInfo = *hitInfo.hitInfo;
}

bool CollidedShapeResult::isArrow() const {
    return CollisionShapeFunction::isShapeArrow(mShapeInfo);
}

bool CollidedShapeResult::isSphere() const {
    return CollisionShapeFunction::isShapeSphere(mShapeInfo);
}

bool CollidedShapeResult::isDisk() const {
    return CollisionShapeFunction::isShapeDisk(mShapeInfo);
}

const al::ArrowHitInfo& CollidedShapeResult::getArrowHitInfo() const {
    return mArrowHitInfo;
}

const al::SphereHitInfo& CollidedShapeResult::getSphereHitInfo() const {
    return mSphereHitInfo;
}

const al::DiskHitInfo& CollidedShapeResult::getDiskHitInfo() const {
    return mDiskHitInfo;
}

const CollisionShapeInfoArrow* CollidedShapeResult::getShapeInfoArrow() const {
    return CollisionShapeFunction::getShapeInfoArrow(mShapeInfo);
}

const CollisionShapeInfoSphere* CollidedShapeResult::getShapeInfoSphere() const {
    return CollisionShapeFunction::getShapeInfoSphere(mShapeInfo);
}

const CollisionShapeInfoDisk* CollidedShapeResult::getShapeInfoDisk() const {
    return CollisionShapeFunction::getShapeInfoDisk(mShapeInfo);
}

void CollidedShapeResult::operator=(const CollidedShapeResult& other) {
    mShapeInfo = other.mShapeInfo;
    setArrowHitInfo(other.mArrowHitInfo);
    setSphereHitInfo(other.mSphereHitInfo);
    setDiskHitInfo(other.mDiskHitInfo);
}
