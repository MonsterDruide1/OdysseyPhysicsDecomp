#pragma once

#include <math/seadVector.h>

#include <container/seadPtrArray.h>

#include "Library/Collision/IUseCollision.h"

namespace al {
class CollisionDirector;
class CollisionParts;
class CollisionPartsFilterBase;
class Triangle;
}  // namespace al

namespace alCollisionUtil {
class SphereMoveHitInfo;
}  // namespace alCollisionUtil

class PlayerCollisionCheckSphereMove : public al::IUseCollision {
public:
    PlayerCollisionCheckSphereMove(al::CollisionDirector*, s32);

    al::CollisionDirector* getCollisionDirector() const override;

    bool checkSphereMove(const sead::Vector3f&, const sead::Vector3f&, f32);
    void gatherCollisionParts(al::CollisionParts*);
    u32 getNum() const;
    f32 getTValue(u32) const;
    const sead::Vector3f& getPos(u32) const;
    const sead::Vector3f& getNormal(u32) const;
    const al::CollisionParts* getCollisionParts(u32) const;
    const char* getMapCodeName(u32) const;
    const char* getWallCodeName(u32) const;
    const char* getMaterialCodeName(u32) const;

    void setCollisionPartsFilter(const al::CollisionPartsFilterBase* filter) {
        mCollisionPartsFilter = filter;
    }

private:
    al::CollisionDirector* mCollisionDirector;
    int unk;
    u32 mNum = 0;
    float* mTValues;
    sead::Vector3f* mPos;
    al::Triangle* mTriangles;
    alCollisionUtil::SphereMoveHitInfo* mSphereMoveHitInfos;
    void* gap;
    int* unkArrayType2;
    sead::PtrArray<al::CollisionParts> mCollisionParts;
    const al::CollisionPartsFilterBase* mCollisionPartsFilter;
};
