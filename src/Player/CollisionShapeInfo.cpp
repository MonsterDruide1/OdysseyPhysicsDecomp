#include "Player/CollisionShapeInfo.h"
#include "math/seadMatrix.h"

CollisionShapeInfoBase::CollisionShapeInfoBase(CollisionShapeId id, const char* name)
    : mId(id), mName(name) {}

CollisionShapeInfoSphere::CollisionShapeInfoSphere(char const* name, float boundingRadius,
                                                   const sead::Vector3f& boundingCenter)
    : CollisionShapeInfoBase(CollisionShapeId::Sphere, name), mBoundingRadius(boundingRadius), mBoundingCenter(boundingCenter) {
    calcWorldShapeInfo(sead::Matrix34f::ident, 1.0f);
}
const sead::Vector3f& CollisionShapeInfoSphere::getBoundingCenter() const {
    return mBoundingCenter;
}
const sead::Vector3f& CollisionShapeInfoSphere::getBoundingCenterWorld() const {
    return mBoundingCenterWorld;
}
f32 CollisionShapeInfoSphere::getBoundingRadius() const {
    return mBoundingRadius;
}
f32 CollisionShapeInfoSphere::getBoundingRadiusWorld() const {
    return mBoundingRadiusWorld;
}
f32 CollisionShapeInfoSphere::getCheckStepRange() const {
    return mBoundingRadius;
}
f32 CollisionShapeInfoSphere::getCheckStepRangeWorld() const {
    return mBoundingRadiusWorld;
}
void CollisionShapeInfoSphere::updateShapeOffset(const sead::Vector3f& offset) {
    mBoundingCenter = offset;
}
void CollisionShapeInfoSphere::calcWorldShapeInfo(const sead::Matrix34f& mtx, f32 scale) {
    mBoundingCenterWorld.setMul(mtx, mBoundingCenter * scale);
    mBoundingRadiusWorld = mBoundingRadius * scale;
    unk2 = unk1 * scale;
    mWorldShapeInfo.setRotated(mtx, mUp);
}
void CollisionShapeInfoSphere::calcRelativeShapeInfo(const sead::Matrix34f& mtx) {
    mRelativeShapeInfo.setMul(mtx, mBoundingCenterWorld);
}
