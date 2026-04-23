#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Collision/KTriangle.h"
#include "Library/Collision/TriangleFilter.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "container/seadRingBuffer.h"
#include "math/seadMatrix.h"
#include "math/seadVectorFwd.h"

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

s32 CollisionParts::checkStrikeArrow(al::ArrowHitResultBuffer* results, const sead::Vector3f& unk1,
                                     const sead::Vector3f& unk2,
                                     const al::TriangleFilterBase* filter) {
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
        if (filter && filter->isInvalidTriangle(info.mTriangle)) {
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
        info.triangle.fillData(*this, hit.data, hit.header);
        if (filter && filter->isInvalidTriangle(info.triangle))
            continue;

        info._70 = v48 * hit.something;
        info.collisionHitPos.setMul(mBaseMtx, (hit.something * v61) + v62);
        v29++;
        info.collisionLocation = (al::CollisionLocation)hit.mCollisionLocation;
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

namespace {
class TriFilterMoveReaction : public TriangleFilterBase {
public:
    TriFilterMoveReaction(sead::Vector3f* a1, const TriangleFilterBase* a2) : _8(a1), _10(a2) {}

    bool isInvalidTriangle(const Triangle& triangle) const override {
        if (_10 && _10->isInvalidTriangle(triangle))
            return true;
        if (!_8)
            return false;
        return _8->dot(triangle.getFaceNormal()) > 0.0f;
    }

public:
    sead::Vector3f* _8;
    const TriangleFilterBase* _10;
};
}  // namespace

s32 CollisionParts::checkStrikeSphere(al::SphereHitResultBuffer* a2, const sead::Vector3f& a3,
                                      float a4, bool a5, const sead::Vector3f& x4_0,
                                      const al::TriangleFilterBase* a7) const {
    float z;                     // s1
    float y;                     // s2
    float x;                     // s3
    float mMtxScale;             // s7
    float v18;                   // s0
    float v19;                   // s17
    float v20;                   // s1
    float v21;                   // s7
    float v22;                   // s2
    float mPrevStep;             // s0
    s32 result;                  // w0
    sead::Vector3f* v25;         // x8
    float a5a;                   // [xsp+4h] [xbp-DCh] BYREF
    sead::Vector3f v27;          // [xsp+8h] [xbp-D8h] BYREF
    sead::Vector3f a3a;          // [xsp+18h] [xbp-C8h] BYREF
    al::SphereInterpolator v32;  // [xsp+40h] [xbp-A0h] BYREF
    sead::Vector3f v33;          // [xsp+70h] [xbp-70h] BYREF
    sead::Vector3f v34;          // [xsp+80h] [xbp-60h] BYREF
    sead::Vector3f v35;          // [xsp+90h] [xbp-50h] BYREF
    sead::Vector3f a4a;          // [xsp+A0h] [xbp-40h] BYREF

    auto a1 = this;

    z = a3.z;
    x = a3.x;
    y = a3.y;
    a4a.setMul(mBaseInvMtx, a3);
    mMtxScale = a1->mMtxScale;
    v35.x = 0.0;
    v35.y = 0.0;
    v35.z = 0.0;
    v18 = mMtxScale * a4;
    if (a5 && (!a1->someCounter || a1->mIsMoving)) {
        v34.x = a1->mPrevBaseInvMtx.m[0][3] +
                (float)((float)((float)(x * a1->mPrevBaseInvMtx.m[0][0]) +
                                (float)(y * a1->mPrevBaseInvMtx.m[0][1])) +
                        (float)(z * a1->mPrevBaseInvMtx.m[0][2]));
        v34.y = a1->mPrevBaseInvMtx.m[1][3] +
                (float)((float)((float)(x * a1->mPrevBaseInvMtx.m[1][0]) +
                                (float)(y * a1->mPrevBaseInvMtx.m[1][1])) +
                        (float)(z * a1->mPrevBaseInvMtx.m[1][2]));
        v34.z = a1->mPrevBaseInvMtx.m[2][3] +
                (float)((float)((float)(x * a1->mPrevBaseInvMtx.m[2][0]) +
                                (float)(y * a1->mPrevBaseInvMtx.m[2][1])) +
                        (float)(z * a1->mPrevBaseInvMtx.m[2][2]));
        v19 = (float)(a4a.z - v34.z) * a1->mBaseMtx.m[1][2];
        v20 = (float)((float)((float)(a4a.x - v34.x) * a1->mBaseMtx.m[2][0]) +
                      (float)((float)(a4a.y - v34.y) * a1->mBaseMtx.m[2][1])) +
              (float)((float)(a4a.z - v34.z) * a1->mBaseMtx.m[2][2]);
        v21 = (float)((float)(a4a.x - v34.x) * a1->mBaseMtx.m[1][0]) +
              (float)((float)(a4a.y - v34.y) * a1->mBaseMtx.m[1][1]);
        v33.x = (float)((float)((float)((float)(a4a.x - v34.x) * a1->mBaseMtx.m[0][0]) +
                                (float)((float)(a4a.y - v34.y) * a1->mBaseMtx.m[0][1])) +
                        (float)((float)(a4a.z - v34.z) * a1->mBaseMtx.m[0][2])) +
                x4_0.x;
        v33.y = (float)(v21 + v19) + x4_0.y;
        v22 = x4_0.z;
        memset(&v32, 0, sizeof(v32));
        v33.z = v20 + v22;
        v32.startInterp(v34, a4a, v18, v18, fminf(v18 * 0.9, 35.0));
        mPrevStep = v32.mPrevStep;
        TriFilterMoveReaction a6 = {&v33, a7};
        while (1) {
            if (mPrevStep == 1.0) {
                result = 0;
                if (v32.mCurrentStep == 1.0)
                    break;
            }
            v32.calcInterp(&a3a, &a5a, &v27);
            v35.x = (float)((float)(a1->mBaseMtx.m[0][0] * (float)-v27.x) -
                            (float)(a1->mBaseMtx.m[0][1] * v27.y)) -
                    (float)(a1->mBaseMtx.m[0][2] * v27.z);
            v35.y = (float)((float)(a1->mBaseMtx.m[1][0] * (float)-v27.x) -
                            (float)(a1->mBaseMtx.m[1][1] * v27.y)) -
                    (float)(a1->mBaseMtx.m[1][2] * v27.z);
            v35.z = (float)((float)(a1->mBaseMtx.m[2][0] * (float)-v27.x) -
                            (float)(a1->mBaseMtx.m[2][1] * v27.y)) -
                    (float)(a1->mBaseMtx.m[2][2] * v27.z);
            v25 = v32.mCurrentStep < 1.0 ? &v33 : 0LL;
            a6._8 = v25;
            result = checkStrikeSphereCore(a2, a3, a3a, v35, a5a, &a6);
            if (result)
                break;
            v32.nextStep();
            mPrevStep = v32.mPrevStep;
        }
    } else {
        return checkStrikeSphereCore(a2, a3, a4a, {0.0f, 0.0f, 0.0f}, v18, a7);
    }
    return result;
}

s32 CollisionParts::checkStrikeSphereCore(al::SphereHitResultBuffer* results,
                                          const sead::Vector3f& a2, const sead::Vector3f& a3,
                                          const sead::Vector3f& a4, float a5,
                                          const TriangleFilterBase* filter) const {
    buffer.clear();
    mKCollisionServer->checkSphere(&a3, a5, mMtxScale, results->capacity() - results->size(),
                                   &buffer);
    int v14 = 0;
    for (int i = 0; i < buffer.size(); i++) {
        al::KCHitInfo hit = buffer[i];

        sead::Vector3f sphereHitPos;
        alKCollisionFunc::calcSphereHitPos(&sphereHitPos, mKCollisionServer, a3, *hit.data, hit.header, hit.mCollisionLocation);

        // EDITED:
        al::SphereHitInfo info = {};

        info._80 = a2;
        info.collisionHitPos.setMul(mBaseMtx, sphereHitPos);
        info.triangle.fillData(*this, hit.data, hit.header);
        if (filter && filter->isInvalidTriangle(info.triangle))
            continue;

        info._70 = hit.something * mInvMtxScale;
        info.collisionMovingReaction = a4;
        info.collisionLocation = (al::CollisionLocation)hit.mCollisionLocation;
        parallelizeVec(&info.collisionMovingReaction, info.triangle.getFaceNormal(), info.collisionMovingReaction);
        v14++;
        results->pushBack(info);
    }
    return v14;
}

}  // namespace al
