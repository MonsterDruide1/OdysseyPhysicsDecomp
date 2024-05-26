#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class CollisionPartsFilterBase;
struct HitInfo;
class SpherePoseInterpolator;
}  // namespace al
class CollidedShapeResult;
class CollisionShapeKeeper;
class CollisionMultiShape;

class PlayerCollider : public al::HioNode, al::IUseCollision
{
public:
    PlayerCollider(al::CollisionDirector*, const sead::Matrix34f*, const sead::Vector3f*,
                   const sead::Vector3f*, bool);
    void onInvalidate();
    void setCollisionShapeKeeper(CollisionShapeKeeper*);
    void calcBoundingRadius(f32*) const;
    void setCollisionShapeScale(f32);
    void onCutCollideAffectDir(const sead::Vector3f&);
    void offCutCollideAffectDir();
    void clear();
    void calcCheckPos(sead::Vector3f*) const;
    void resetPose(const sead::Quatf&);
    sead::Vector3f collide(const sead::Vector3f&);
    bool calcMovePowerByContact(sead::Vector3f*, const sead::Vector3f&);
    void moveCollide(sead::Vector3f*, f32*, sead::Quatf*, const sead::Vector3f&, f32,
                     const sead::Quatf&, const sead::Vector3f&, f32, bool);
    bool findCollidePos(al::SpherePoseInterpolator*);
    void calcResultVec(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&);
    void calcGroundArrowAverage(bool*, sead::Vector3f*, bool*, sead::Vector3f*,
                                const CollisionShapeKeeper*);
    void calcResultVecArrow(sead::BitFlag32*, sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                            sead::Vector3f*, const CollidedShapeResult*);
    void calcResultVecSphere(sead::BitFlag32*, sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                             sead::Vector3f*, const CollidedShapeResult*);
    void calcResultVecDisk(sead::BitFlag32*, sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                           sead::Vector3f*, const CollidedShapeResult*);
    void collectHitInfoArray(const al::HitInfo&, s32);
    bool isNeedWallBorderCheck(const al::HitInfo&) const;
    void setWallBorderCheckTypeNone();
    void setWallBorderCheckTypeNoFace();
    void setWallBorderCheckTypeAll();
    void setCollisionPartsFilter(const al::CollisionPartsFilterBase*);
    void calcBoundingCenter(sead::Vector3f*) const;
    void validateCorrectMovePartsCheck();
    al::CollisionDirector* getCollisionDirector() const override;

    f32 get_70() const { return _70; }

    const sead::Vector3f& getCollidedGroundNormal() const { return mCollidedGroundNormal; }

    void set1b0(f32 value) { _1b0 = value; }

public:
  al::CollisionDirector *mCollisionDirector;
  const sead::Matrix34f *mMtxPtr;
  const sead::Vector3f *mTransPtr;
  const sead::Vector3f *mGravityPtr;
  sead::Vector3f mTrans;
  float mSize;
  sead::Matrix34f mMtx;
  al::HitInfo *info1;
  float val1;
  int pad1;
  al::HitInfo *info2;
  float val2;
  int pad2;
  al::HitInfo *info3;
  float val3;
  sead::Vector3f unk3;
  bool flag1;
  bool flag2;
  bool pads[2];
  sead::Vector3f mCollisionHitNormal;
  sead::Vector3f mCollisionHitPos;
  int mTimeInAir;
  sead::Matrix34f mCollidePosMtx;
  CollisionShapeKeeper *mCollisionShapeKeeper;
  float mCollisionShapeScale;
  int pad;
  CollisionMultiShape *mCollisionMultiShape;
  int someBitField;
  bool mIsInFastMoveCollisionArea;
  bool mIsValidGroundSupport;
  bool mIsDuringRecovery;
  bool pad3[1];
  sead::Vector3f mCutCollideAffectDir;
  int mWallBorderCheckType;
  const al::CollisionPartsFilterBase *mCollisionPartsFilter;
  sead::PtrArray<al::HitInfo const> ptrArraysChangeMe[3];
  al::HitInfo *someHitInfos;
  unsigned int numHitInfosAbove;
  int pad5;
  sead::PtrArray<al::HitInfo> anotherPtrArray;
  int sizeOfArrayBelowIs3;
  int pad4;
  float *someThreeFloats;
  int sizeOfArrayBelowIs3_2;
  int pad4_2;
  float *anotherThreeFloats;
  sead::Vector3f unk9;
  sead::Vector3f unk10;
  float unk11;
};
