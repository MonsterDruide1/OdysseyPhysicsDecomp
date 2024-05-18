#pragma once

#include <container/seadTList.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class ArrowHitResultBuffer;
class DiskHitResultBuffer;
struct HitInfo;
class HitSensor;
class KCollisionServer;
class LiveActor;
class SphereHitResultBuffer;
class TriangleFilterBase;

enum class ForceCollisionFlag : u8 {
    ScaleNone,
    ScaleAverage,
    ScaleOne,
};

class CollisionParts {
public:
    CollisionParts(void* kcl, const void* byml);

    void calcInvMtxScale();
    const LiveActor* getConnectedHost() const;
    void initParts(const sead::Matrix34f&);
    void resetAllMtx(const sead::Matrix34f&);
    void updateBoundingSphereRange(sead::Vector3f);
    void validateByUser();
    void invalidateByUser();
    void validateBySystem();
    void invalidateBySystem();
    void onJoinList();
    void makeEqualScale(sead::Matrix34f*);
    void resetAllMtxPrivate(const sead::Matrix34f&);
    void resetAllMtx();
    void updateBoundingSphereRange();
    void forceResetAllMtxAndSetUpdateMtxOneTime(const sead::Matrix34f&);
    void forceResetAllMtxAndSetUpdateMtxOneTime();
    void syncMtx(const sead::Matrix34f&);
    void syncMtx();
    void updateMtx();
    void updateScale();
    void updateBoundingSphereRangePrivate(f32);
    s32 checkBoundingSphereRange(const sead::Vector3f&, f32);
    s32 checkStrikePoint(HitInfo*, const sead::Vector3f&, const TriangleFilterBase*) const;
    s32 checkStrikeSphere(SphereHitResultBuffer*, const sead::Vector3f&, f32, bool,
                          const sead::Vector3f&, const TriangleFilterBase*) const;
    s32 checkStrikeSphereCore(SphereHitResultBuffer*, const sead::Vector3f&, const sead::Vector3f&,
                              const sead::Vector3f&, f32, const TriangleFilterBase*) const;
    s32 checkStrikeArrow(ArrowHitResultBuffer*, const sead::Vector3f&, const sead::Vector3f&,
                         const TriangleFilterBase*) const;
    s32 checkStrikeSphereForPlayer(SphereHitResultBuffer*, const sead::Vector3f&, f32,
                                   const TriangleFilterBase*) const;
    s32 checkStrikeSphereForPlayerCore(SphereHitResultBuffer*, const sead::Vector3f&,
                                       const sead::Vector3f&, const sead::Vector3f&,
                                       const sead::Vector3f&, f32, const TriangleFilterBase*) const;
    s32 checkStrikeDisk(DiskHitResultBuffer*, const sead::Vector3f&, f32, f32,
                        const sead::Vector3f&, const TriangleFilterBase*) const;
    s32 checkStrikeDiskCore(DiskHitResultBuffer*, const sead::Vector3f&, const sead::Vector3f&,
                            const sead::Vector3f&, f32, f32, const sead::Vector3f&,
                            const TriangleFilterBase*) const;
    void calcForceMovePower(sead::Vector3f*, const sead::Vector3f&) const;
    void calcForceRotatePower(sead::Quatf*) const;

    const sead::Matrix34f* getSyncCollisonMtx() const { return mSyncCollisionMtx; }

    void setSyncCollisionMtx(const sead::Matrix34f* mtx) { mSyncCollisionMtx = mtx; }

    const sead::Matrix34f& getBaseMtx() const { return mBaseMtx; }
    const sead::Matrix34f& getBaseInvMtx() const { return mBaseInvMtx; }
    const sead::Matrix34f& getPrevBaseMtx() const { return mPrevBaseMtx; }

    const sead::Matrix34f& getPrevBaseInvMtx() const { return mPrevBaseInvMtx; }

    void setPriority(s32 priority) { mPriority = priority; }

    KCollisionServer* getKCollisionServer() const { return mKCollisionServer; }

    HitSensor* getConnectedSensor() const { return mConnectedSensor; }

    void setConnectedSensor(HitSensor* sensor) { mConnectedSensor = sensor; }

    void setSpecialPurpose(const char* specialPurpose) { mSpecialPurpose = specialPurpose; }

    void setOptionalPurpose(const char* optionalPurpose) { mOptionalPurpose = optionalPurpose; }

    s32 get_15c() const { return getSomeCounter(); }

    bool isValidCollision() const { return isValidatedByUser && isValidatedBySystem; }

    int getSomeCounter() const { return someCounter; }

    bool isMoving() const { return mIsMoving; }

    void setIsMoving(bool isMoving) { mIsMoving = isMoving; }

    void set_16e(bool val) { bVar6 = val; }

    void setForceCollisionScaleNone() { mForceCollisionFlag = ForceCollisionFlag::ScaleNone; }

    void setForceCollisionScaleAverage() { mForceCollisionFlag = ForceCollisionFlag::ScaleAverage; }

    void setForceCollisionScaleOne() { mForceCollisionFlag = ForceCollisionFlag::ScaleOne; }

public:
    void* unk[2] = {nullptr, nullptr};
    CollisionParts* selfReference = this;
    sead::TList<CollisionParts*>* mPartsList = nullptr;
    union {
        const sead::Matrix34f* mJointMtx = nullptr;
        const sead::Matrix34f* mSyncCollisionMtx;
    };
    sead::Matrix34f mSyncMtx;
    sead::Matrix34f mBaseMtx;
    sead::Matrix34f mBaseInvMtx;
    sead::Matrix34f mPrevBaseMtx;
    sead::Matrix34f mPrevBaseInvMtx;
    sead::Vector3f mMtxScaleVec = {1.0f, 1.0f, 1.0f};
    f32 mMtxScale = 1.0f;
    f32 mInvMtxScale = 1.0f;
    s32 mPriority = -1;
    KCollisionServer* mKCollisionServer = nullptr;
    al::HitSensor* mConnectedSensor = nullptr;
    const char* mSpecialPurpose = nullptr;
    const char* mOptionalPurpose = nullptr;
    sead::Vector3f mMtxScaleVecAgain = {1.0f, 1.0f, 1.0f};
    int someCounter = 0;
    float mBoundingSphereRange = -1.0f;
    float mBaseMtxScale = 1.0f;
    bool isValidatedByUser = true;
    bool isValidatedBySystem = true;
    bool bVar3 = true;
    bool bVar4 = false;
    bool bVar5 = true;
    bool mIsMoving = false;
    union {
        ForceCollisionFlag mForceCollisionFlag;
        bool bVar6;
    };
    bool pad2;
};
static_assert(sizeof(CollisionParts) == 0x170);

}  // namespace al
