#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Collision/KTriangle.h"
#include "Library/Collision/TriangleFilter.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "container/seadRingBuffer.h"
#include "math/seadMatrix.h"
#include "math/seadVectorFwd.h"

#include <cstdio>

namespace al {

CollisionParts::CollisionParts(void* kclData, const void* bymlData) {
    mKCollisionServer = new KCollisionServer();
    mKCollisionServer->initKCollisionServer(kclData, bymlData);

    mPrevBaseMtx = sead::Matrix34f::ident;
    mBaseMtx = sead::Matrix34f::ident;
    mSyncMtx = sead::Matrix34f::ident;
    mPrevBaseInvMtx = sead::Matrix34f::ident;
    mBaseInvMtx = sead::Matrix34f::ident;

    calcMtxScale(&mMtxScaleVec, mBaseInvMtx);
    mMtxScale = (mMtxScaleVec.x + mMtxScaleVec.y + mMtxScaleVec.z) / 3.0f;
    mInvMtxScale = 1.0f / mMtxScale;
}

void CollisionParts::initParts(const sead::Matrix34f& mtx) {
    resetAllMtx(mtx);
    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    calcMtxScale(&scale, mtx);
    mKCollisionServer->calcFarthestVertexDistance();
    mBaseMtxScale = (scale.x + scale.y + scale.z) / 3.0f;
    mBoundingSphereRange = mBaseMtxScale * mKCollisionServer->mFarthestVertexDistance;
}

void CollisionParts::resetAllMtx(const sead::Matrix34f& mtx_param) {
    if (!bVar4 && !bVar5)
        return;

    sead::Matrix34f mtx = mtx_param;
    makeEqualScale(&mtx);

    mPrevBaseMtx = mtx;
    mBaseMtx = mtx;
    mSyncMtx = mtx;
    mPrevBaseInvMtx.setInverse(mPrevBaseMtx);
    mBaseInvMtx = mPrevBaseInvMtx;

    calcMtxScale(&mMtxScaleVec, mBaseInvMtx);
    mMtxScale = (mMtxScaleVec.x + mMtxScaleVec.y + mMtxScaleVec.z) / 3.0f;
    mInvMtxScale = 1.0f / mMtxScale;

    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    calcMtxScale(&scale, mBaseMtx);
    mBaseMtxScale = (scale.x + scale.y + scale.z) / 3.0f;
    mBoundingSphereRange = mBaseMtxScale * mKCollisionServer->mFarthestVertexDistance;
}

f32 CollisionParts::makeEqualScale(sead::Matrix34f* mtx) {
    sead::Vector3f scale;
    calcMtxScale(&scale, *mtx);
    sead::Vector3f v17 = {scale.z - scale.x, scale.x - scale.y, scale.y - scale.z};
    if (isNearZero(v17, 0.001f))
        return scale.x;

    f32 factor = 1.0f;
    sead::Vector3f vec;
    if (mForceCollisionFlag == ForceCollisionFlag::ScaleAverage) {
        factor = (scale.x + scale.y + scale.z) / 3.0f;
        vec = {factor / scale.x, factor / scale.y, factor / scale.z};
    } else if (mForceCollisionFlag == ForceCollisionFlag::ScaleOne) {
        vec = {1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z};
    }

    mtx->scaleBases(vec.x, vec.y, vec.z);
    return factor;
}

sead::FixedRingBuffer<KCHitInfo, 512> buffer;

s32 CollisionParts::checkStrikeArrow(al::ArrowHitResultBuffer* results, sead::Vector3f const& unk1,
                                     sead::Vector3f const& unk2,
                                     al::TriangleFilterBase const* filter) {
    f32 v48 = unk2.length();
    sead::Vector3f v62;
    v62.setMul(mBaseInvMtx, unk1);
    sead::Vector3f v61;
    v61.setMul(mBaseInvMtx, (unk1 + unk2));
    v61 -= v62;

    buffer.clear();
    u32 v60 = 0;
    mKCollisionServer->checkArrow(v62, v61, &buffer, &v60, results->capacity() - results->size());
    u32 v29 = 0;
    for (int i = 0; i < buffer.size(); i++) {
        /*al::ArrowHitInfo info = {};
        results->pushBack(info);

        al::KCHitInfo hit = buffer[i];
        info.mTriangle.fillData(*this, hit.mData, hit.mHeader);
        if (filter && !filter->isInvalidTriangle(info.mTriangle)) {
            results->popBack();
            continue;
        }

        info.unk = v48 * hit.something;
        info.collisionHitPos.setMul(mBaseMtx, (hit.something * v61) + v62);
        v29++;
        info.collisionLocation = hit.mCollisionLocation;*/

        // EDITED:
        al::ArrowHitInfo info = {};

        al::KCHitInfo hit = buffer[i];
        info.hitInfo->triangle.fillData(*this, hit.data, hit.header);
        if (filter && !filter->isInvalidTriangle(info.hitInfo->triangle)) {
            continue;
        }

        info.hitInfo->_70 = v48 * hit.something;
        info.hitInfo->collisionHitPos.setMul(mBaseMtx, (hit.something * v61) + v62);
        v29++;
        info.hitInfo->collisionLocation = (al::CollisionLocation)hit.mCollisionLocation;
        results->pushBack(info);
    }

    return v29;
}

void CollisionParts::syncMtx() {
    mSyncMtx = *mJointMtx;
    makeEqualScale(&mSyncMtx);
}

void CollisionParts::syncMtx(const sead::Matrix34f& mtx) {
    mSyncMtx = mtx;
    makeEqualScale(&mSyncMtx);
}

}  // namespace al
