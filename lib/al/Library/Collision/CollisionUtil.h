#pragma once

#include <math/seadVector.h>
#include "Library/Collision/KTriangle.h"
#include "container/seadPtrArray.h"
#include "math/seadBoundBox.h"
#include "prim/seadDelegate.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"

namespace al {
class CollisionPartsFilterBase;
class TriangleFilterBase;
class CollisionParts;
class IUseCollision;
class HitInfo;
class ArrowHitInfo;

bool isNearCollideSphereAabb(const sead::Vector3f&, f32, const sead::BoundBox3f&);

bool checkHitSegmentSphere(sead::Vector3<float> const&, sead::Vector3<float> const&,
                           sead::Vector3<float> const&, float, sead::Vector3<float>*,
                           sead::Vector3<float>*);

bool isFloorCode(const al::HitInfo*, const char*);
bool isFloorCode(const al::Triangle&, const char*);
bool isWallCode(const al::HitInfo*, const char*);

const char* getFloorCodeName(const al::Triangle&);
const char* getWallCodeName(const al::Triangle&);
const char* getMaterialCodeName(const al::Triangle&);

}  // namespace al

namespace alCollisionUtil {

bool isFarAway(const al::CollisionParts&, const sead::Vector3f&, f32);
s32 checkStrikeArrow(al::IUseCollision const*, sead::Vector3<float> const&,
                      sead::Vector3<float> const&, al::CollisionPartsFilterBase const*,
                      al::TriangleFilterBase const*);
bool isCollisionMoving(const al::HitInfo*);
const sead::Vector3f& getCollisionHitPos(const al::HitInfo*);
const sead::Vector3f& getCollisionHitNormal(const al::HitInfo*);
const al::CollisionParts* getCollisionHitParts(const al::HitInfo*);
const sead::Vector3f& getCollisionMovingReaction(const al::HitInfo*);
bool getHitPosAndNormalOnArrow(al::IUseCollision const*, sead::Vector3<float>*,
                               sead::Vector3<float>*, sead::Vector3<float> const&,
                               sead::Vector3<float> const&, al::CollisionPartsFilterBase const*,
                               al::TriangleFilterBase const*);

bool getFirstPolyOnArrow(al::IUseCollision const*, al::ArrowHitInfo const**,
                         sead::Vector3<float> const&, sead::Vector3<float> const&,
                         al::CollisionPartsFilterBase const*, al::TriangleFilterBase const*);
bool getFirstPolyOnArrow(al::IUseCollision const*, sead::Vector3<float>*, al::Triangle*,
                         sead::Vector3<float> const&, sead::Vector3<float> const&, char const*);
bool getFirstPolyOnArrow(al::IUseCollision const*, sead::Vector3<float>*, al::Triangle*,
                         sead::Vector3<float> const&, sead::Vector3<float> const&,
                         al::CollisionPartsFilterBase const*, al::TriangleFilterBase const*);

const al::ArrowHitInfo& getStrikeArrowInfo(al::IUseCollision const*, u32);

bool getLastPolyOnArrow(const al::IUseCollision*, const al::ArrowHitInfo**, const sead::Vector3f&,
                        const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                        const al::TriangleFilterBase*);

void searchCollisionParts(const al::IUseCollision*, const sead::Vector3f&, float,
                          sead::IDelegate1<al::CollisionParts*>&,
                          const al::CollisionPartsFilterBase*);

sead::PtrArray<al::CollisionParts>* getCollisionPartsPtrArray(const al::IUseCollision*);
void validateCollisionPartsPtrArray(const al::IUseCollision*, sead::PtrArray<al::CollisionParts>*);
void invalidateCollisionPartsPtrArray(const al::IUseCollision*);

s32 checkStrikeSphereMove(const al::IUseCollision*, SphereMoveHitInfo*, s32, const sead::Vector3f&,
                          f32, const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                          const al::TriangleFilterBase*);

}  // namespace alCollisionUtil
