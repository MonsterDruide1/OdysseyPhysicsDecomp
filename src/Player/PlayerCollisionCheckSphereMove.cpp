#include "Player/PlayerCollisionCheckSphereMove.h"
#include "Library/Collision/CollisionUtil.h"
#include "prim/seadDelegate.h"

PlayerCollisionCheckSphereMove::PlayerCollisionCheckSphereMove(al::CollisionDirector* director,
                                                               s32 num)
    : unk(num) {
    mCollisionDirector = director;
    mTValues = new f32[num];
    mPos = new sead::Vector3f[num];
    mTriangles = new al::Triangle[num];
    mSphereMoveHitInfos = new alCollisionUtil::SphereMoveHitInfo[num];
    unkArrayType2 = new int[num];
    mCollisionParts.allocBuffer(100, nullptr);
}

bool PlayerCollisionCheckSphereMove::checkSphereMove(const sead::Vector3f& a2,
                                                     const sead::Vector3f& a3, f32 a4) {
    sead::PtrArray<al::CollisionParts>* p_mCollisionParts;
    float y;
    float v10;
    float z;
    float v12;
    float v13;
    sead::PtrArray<al::CollisionParts>* CollisionPartsPtrArray;
    unsigned int mNum;
    unsigned int i;
    unsigned int v17;
    alCollisionUtil::SphereMoveHitInfo* v18;
    sead::Vector3f* v19;
    float x;
    alCollisionUtil::SphereMoveHitInfo* v21;
    al::Triangle* v22;
    unsigned int v23;
    unsigned int v24;
    int v25;
    unsigned int v26;
    int* unkArrayType2;
    float* mTValues;
    u32 v29;
    unsigned int v30;
    int* v31;
    int v32;
    u32 v33;
    int* v34;
    u32 v35;
    int v36;
    sead::Vector3f v38;

    mCollisionParts.mPtrNum = 0;
    p_mCollisionParts = &mCollisionParts;
    sead::Delegate1<PlayerCollisionCheckSphereMove, al::CollisionParts*> delegate = {
        this, &PlayerCollisionCheckSphereMove::gatherCollisionParts};
    y = a3.y;
    v10 = (float)(y * 0.5) + a2.y;
    z = a3.z;
    v12 = a3.x * a3.x;
    v13 = a2.z;
    v38.x = (float)(a3.x * 0.5) + a2.x;
    v38.y = v10;
    v38.z = (float)(z * 0.5) + v13;
    auto* a1 = this;
    alCollisionUtil::searchCollisionParts(
        this, v38, (float)(sqrtf((float)(v12 + (float)(y * y)) + (float)(z * z)) * 0.5) + a4,
        delegate, this->mCollisionPartsFilter);
    CollisionPartsPtrArray = alCollisionUtil::getCollisionPartsPtrArray(this);
    alCollisionUtil::validateCollisionPartsPtrArray(this, p_mCollisionParts);
    WARN_UNIMPL;
    if(false) {
        // requires implementing Collision with Sphere (currently only Arrow)
        a1->mNum = alCollisionUtil::checkStrikeSphereMove(this, a1->mSphereMoveHitInfos, a1->unk, a2,
                                                        a4, a3, a1->mCollisionPartsFilter, 0LL);
    } else {
        a1->mNum = 0;
    }
    if (CollisionPartsPtrArray)
        alCollisionUtil::validateCollisionPartsPtrArray(a1, CollisionPartsPtrArray);
    else
        alCollisionUtil::invalidateCollisionPartsPtrArray(a1);

    mNum = a1->mNum;
    if (mNum) {
        for (i = 0; i < mNum; ++i) {
            v17 = i;
            a1->mTValues[v17] = a1->mSphereMoveHitInfos[i]._0;
            v18 = &a1->mSphereMoveHitInfos[i];
            v19 = &a1->mPos[i];
            *v19 = v18->_8;
            v21 = &a1->mSphereMoveHitInfos[i];
            v22 = &a1->mTriangles[i];
            v22->mKCPrismHeader = v21->mTriangle.mKCPrismHeader;
            v22->mKCPrismData = v21->mTriangle.mKCPrismData;
            v22->mCollisionParts = v21->mTriangle.mCollisionParts;
            v22->mFaceNormal.x = v21->mTriangle.mFaceNormal.x;
            v22->mFaceNormal.y = v21->mTriangle.mFaceNormal.y;
            v22->mFaceNormal.z = v21->mTriangle.mFaceNormal.z;
            v22->mEdgeNormals[0].x = v21->mTriangle.mEdgeNormals[0].x;
            v22->mEdgeNormals[0].y = v21->mTriangle.mEdgeNormals[0].y;
            v22->mEdgeNormals[0].z = v21->mTriangle.mEdgeNormals[0].z;
            v22->mEdgeNormals[1].x = v21->mTriangle.mEdgeNormals[1].x;
            v22->mEdgeNormals[1].y = v21->mTriangle.mEdgeNormals[1].y;
            v22->mEdgeNormals[1].z = v21->mTriangle.mEdgeNormals[1].z;
            v22->mEdgeNormals[2].x = v21->mTriangle.mEdgeNormals[2].x;
            v22->mEdgeNormals[2].y = v21->mTriangle.mEdgeNormals[2].y;
            v22->mEdgeNormals[2].z = v21->mTriangle.mEdgeNormals[2].z;
            v22->mVerts[0].x = v21->mTriangle.mVerts[0].x;
            v22->mVerts[0].y = v21->mTriangle.mVerts[0].y;
            v22->mVerts[0].z = v21->mTriangle.mVerts[0].z;
            v22->mVerts[1].x = v21->mTriangle.mVerts[1].x;
            v22->mVerts[1].y = v21->mTriangle.mVerts[1].y;
            v22->mVerts[1].z = v21->mTriangle.mVerts[1].z;
            v22->mVerts[2].x = v21->mTriangle.mVerts[2].x;
            v22->mVerts[2].y = v21->mTriangle.mVerts[2].y;
            v22->mVerts[2].z = v21->mTriangle.mVerts[2].z;
            a1->unkArrayType2[v17] = i;
            mNum = a1->mNum;
        }

        if (mNum >= 2) {
            v23 = 0;
            v24 = 1;
            v25 = -1;
            do {
                v26 = v23 + 1;
                if (v23 + 1 < mNum) {
                    unkArrayType2 = a1->unkArrayType2;
                    mTValues = a1->mTValues;
                    if (mNum == v23 + 2) {
                        v29 = v24;
                        v30 = v23;
                    } else {
                        v31 = &unkArrayType2[v24 + 1];
                        v29 = v24;
                        v30 = v23;
                        do {
                            if (mTValues[unkArrayType2[v30]] > mTValues[*(v31 - 1)])
                                v30 = v29;

                            v32 = *v31;
                            v31 += 2;
                            if (mTValues[unkArrayType2[v30]] > mTValues[v32])
                                v30 = v29 + 1;

                            v29 += 2LL;
                        } while (mNum - ((mNum + v25) & 1) != v29);
                    }

                    if (((mNum + ~v23) & 1) != 0 &&
                        mTValues[unkArrayType2[v30]] > mTValues[unkArrayType2[v29]])
                        v30 = v29;

                    if (v30 != v23) {
                        v33 = v23;
                        v34 = a1->unkArrayType2;
                        v35 = v30;
                        v33 *= 4LL;
                        v36 = *(int*)((char*)v34 + v33);
                        *(int*)((char*)v34 + v33) = v34[v35];
                        a1->unkArrayType2[v35] = v36;
                        mNum = a1->mNum;
                    }
                }

                ++v24;
                --v25;
                v23 = v26;
            } while (v26 < mNum - 1);
        }
    }

    return mNum != 0;
}

void PlayerCollisionCheckSphereMove::gatherCollisionParts(al::CollisionParts* parts) {
    mCollisionParts.pushBack(parts);
}

u32 PlayerCollisionCheckSphereMove::getNum() const {
    return mNum;
}

f32 PlayerCollisionCheckSphereMove::getTValue(u32 index) const {
    return mTValues[unkArrayType2[index]];
}

const sead::Vector3f& PlayerCollisionCheckSphereMove::getPos(u32 index) const {
    return mPos[unkArrayType2[index]];
}

const sead::Vector3f& PlayerCollisionCheckSphereMove::getNormal(u32 index) const {
    return mTriangles[unkArrayType2[index]].getNormal(0);
}

const al::CollisionParts* PlayerCollisionCheckSphereMove::getCollisionParts(u32 index) const {
    return mTriangles[unkArrayType2[index]].mCollisionParts;
}

const char* PlayerCollisionCheckSphereMove::getMapCodeName(u32 index) const {
    return al::getFloorCodeName(mTriangles[unkArrayType2[index]]);
}

const char* PlayerCollisionCheckSphereMove::getWallCodeName(u32 index) const {
    return al::getWallCodeName(mTriangles[unkArrayType2[index]]);
}

const char* PlayerCollisionCheckSphereMove::getMaterialCodeName(u32 index) const {
    return al::getMaterialCodeName(mTriangles[unkArrayType2[index]]);
}

al::CollisionDirector* PlayerCollisionCheckSphereMove::getCollisionDirector() const {
    return mCollisionDirector;
}
