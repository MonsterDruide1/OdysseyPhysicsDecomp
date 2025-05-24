#include "Player/CollisionShapeInfo.h"
#include "Library/Math/MathUtil.h"
#include "math/seadMatrix.h"

#include <cstdio>

CollisionShapeInfoBase::CollisionShapeInfoBase(CollisionShapeId id, const char* name)
    : mId(id), mName(name) {}

CollisionShapeInfoSphere::CollisionShapeInfoSphere(char const* name, float boundingRadius,
                                                   const sead::Vector3f& boundingCenter)
    : CollisionShapeInfoBase(CollisionShapeId::Sphere, name), mBoundingRadius(boundingRadius),
      mBoundingCenter(boundingCenter) {
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

CollisionShapeInfoArrow::CollisionShapeInfoArrow(const char* name, const sead::Vector3f& a3,
                                                 const sead::Vector3f& a4, f32 a5, s32 a6)
    : CollisionShapeInfoBase(CollisionShapeId::Arrow, name), a3(a3), a4(a4), a5(a5),
      mArrowIndex(a6) {
    // inlined call to updateShapeOffset?
    /*mBoundingCenter = (a3 + sead::Vector3f{0.0f, 0.0f, 0.0f}) + (a4 * 0.5f);
    mBoundingRadius = a4.length() * 0.5f;*/
    updateShapeOffset({0.0f, 0.0f, 0.0f});
    calcWorldShapeInfo(sead::Matrix34f::ident, 1.0f);
}

const sead::Vector3f& CollisionShapeInfoArrow::getBoundingCenter() const {
    return mBoundingCenter;
}

const sead::Vector3f& CollisionShapeInfoArrow::getBoundingCenterWorld() const {
    return mBoundingCenterWorld;
}

f32 CollisionShapeInfoArrow::getBoundingRadius() const {
    return mBoundingRadius;
}

f32 CollisionShapeInfoArrow::getBoundingRadiusWorld() const {
    return mBoundingRadiusWorld;
}

f32 CollisionShapeInfoArrow::getCheckStepRange() const {
    return mBoundingRadius;
}

f32 CollisionShapeInfoArrow::getCheckStepRangeWorld() const {
    return mBoundingRadiusWorld;
}

void CollisionShapeInfoArrow::updateShapeOffset(const sead::Vector3f& offset) {
    vec1 = offset;
    sead::Vector3f a3 = this->a3;
    sead::Vector3f a4 = this->a4;
    mBoundingCenter = (a3 + offset) + (a4 * 0.5f);
    mBoundingRadius = a4.length() * 0.5f;
}

void CollisionShapeInfoArrow::calcWorldShapeInfo(const sead::Matrix34f& mtx, f32 scale) {
    vec2.setMul(mtx, (a3 + vec1) * scale);
    vec4.setRotated(mtx, a4 * scale);
    mBoundingCenterWorld.setMul(mtx, mBoundingCenter * scale);
    vec3 = vec2 + vec4;
    mBoundingRadiusWorld = mBoundingRadius * scale;
    al::calcArrowAabb(&mAabb, vec2, vec3);
}

void CollisionShapeInfoArrow::calcRelativeShapeInfo(const sead::Matrix34f& mtx) {
    vec5.setMul(mtx, vec2);
    vec6.setRotated(mtx, vec4);
}

CollisionShapeInfoDisk::CollisionShapeInfoDisk(char const* name, float a1,
                                               sead::Vector3<float> const& a3,
                                               sead::Vector3<float> const& a4, float a5)
    : CollisionShapeInfoBase(CollisionShapeId::Disk, name) {
    _28 = a1;
    _3C = a4;
    _48 = a5;
    mBoundingRadius = sead::Mathf::sqrt(a1 * a1 + a5 * a5);
    mCheckStepRange = sead::Mathf::min(_28, 2 * _48);
    calcWorldShapeInfo(sead::Matrix34f::ident, 1.0f);
}

void CollisionShapeInfoDisk::updateDiskShape(f32 a1, const sead::Vector3f& a2, f32 a3) {
    _28 = a1;
    _3C = a2;
    _48 = a3;
    mBoundingRadius = sqrtf((float)(a3 * a3) + (float)(a1 * a1));
    mCheckStepRange = sead::Mathf::min(_28, 2 * _48);
}

const sead::Vector3f& CollisionShapeInfoDisk::getBoundingCenter() const {
    return mBoundingCenter;
}

const sead::Vector3f& CollisionShapeInfoDisk::getBoundingCenterWorld() const {
    return mBoundingCenterWorld;
}

f32 CollisionShapeInfoDisk::getBoundingRadius() const {
    return mBoundingRadius;
}

f32 CollisionShapeInfoDisk::getBoundingRadiusWorld() const {
    return mBoundingRadiusWorld;
}

f32 CollisionShapeInfoDisk::getCheckStepRange() const {
    return mCheckStepRange;
}

f32 CollisionShapeInfoDisk::getCheckStepRangeWorld() const {
    return mCheckStepRangeWorld;
}

void CollisionShapeInfoDisk::updateShapeOffset(const sead::Vector3f& a1) {
    mBoundingCenter = a1;
}

void CollisionShapeInfoDisk::calcWorldShapeInfo(const sead::Matrix34f& a2, f32 a3) {
    mBoundingCenterWorld.setMul(a2, mBoundingCenter * a3);
    _5C.setRotated(a2, _3C);
    al::tryNormalizeOrZero(&_5C);
    _2C = _28 * a3;
    _4C = _48 * a3;
    f32 v12 = mBoundingRadius * a3;
    mCheckStepRangeWorld = mCheckStepRange * a3;
    f32 more = unk1;
    mBoundingRadiusWorld = v12;
    _88 = more * a3;
    _98.setRotated(a2, mUp);
}

void CollisionShapeInfoDisk::calcRelativeShapeInfo(const sead::Matrix34f& a2) {
    _68.setMul(a2, mBoundingCenterWorld);
    _74.setRotated(a2, _5C);
    al::tryNormalizeOrZero(&_74);
}
