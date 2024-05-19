#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"
#include "math/seadBoundBox.h"

enum class CollisionShapeId : u32 {
    Arrow,
    Sphere,
    Disk,
};

class CollisionShapeInfoBase : public al::HioNode {
public:
    CollisionShapeInfoBase(CollisionShapeId id, const char* name);

    virtual const sead::Vector3f& getBoundingCenter() const { return sead::Vector3f::zero; }

    virtual const sead::Vector3f& getBoundingCenterWorld() const { return sead::Vector3f::zero; }

    virtual f32 getBoundingRadius() const { return 0.0f; }

    virtual f32 getBoundingRadiusWorld() const { return 0.0f; }

    virtual f32 getCheckStepRange() const { return 100000.0f; }

    virtual f32 getCheckStepRangeWorld() const { return 100000.0f; }

    virtual void updateShapeOffset(const sead::Vector3f&) {}

    virtual void calcWorldShapeInfo(const sead::Matrix34f&, f32) {}

    virtual void calcRelativeShapeInfo(const sead::Matrix34f&) {}

    CollisionShapeId getId() const { return mId; }

    const char* getName() const { return mName; }

private:
    CollisionShapeId mId;
    const char* mName;
};

class CollisionShapeInfoArrow : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoArrow(const char*, const sead::Vector3f&, const sead::Vector3f&, f32, s32);

    const sead::Vector3f& getBoundingCenter() const override;
    const sead::Vector3f& getBoundingCenterWorld() const override;
    f32 getBoundingRadius() const override;
    f32 getBoundingRadiusWorld() const override;
    f32 getCheckStepRange() const override;
    f32 getCheckStepRangeWorld() const override;
    void updateShapeOffset(const sead::Vector3f&) override;
    void calcWorldShapeInfo(const sead::Matrix34f&, f32) override;
    void calcRelativeShapeInfo(const sead::Matrix34f&) override;

private:
    s32 a6;
    sead::Vector3f mBoundingCenter = {0.0f, 0.0f, 0.0f};
    f32 mBoundingRadius;
    sead::Vector3f mBoundingCenterWorld = {0.0f, 0.0f, 0.0f};
    f32 mBoundingRadiusWorld;
    sead::Vector3f a3;
    sead::Vector3f a4;
    f32 a5;
    sead::Vector3f vec1 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec2 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec3 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec4 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec5 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec6 = {0.0f, 0.0f, 0.0f};
    sead::BoundBox3f mAabb;
};

static_assert(sizeof(CollisionShapeInfoArrow) == 0xb8);

class CollisionShapeInfoSphere : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoSphere(const char*, f32, const sead::Vector3f&);

    const sead::Vector3f& getBoundingCenter() const override;
    const sead::Vector3f& getBoundingCenterWorld() const override;
    f32 getBoundingRadius() const override;
    f32 getBoundingRadiusWorld() const override;
    f32 getCheckStepRange() const override;
    f32 getCheckStepRangeWorld() const override;
    void updateShapeOffset(const sead::Vector3f&) override;
    void calcWorldShapeInfo(const sead::Matrix34f&, f32) override;
    void calcRelativeShapeInfo(const sead::Matrix34f&) override;

    // TODO: rename parameter
    void setSupportGround(const sead::Vector3f& up, f32 unk1) {
        mIsSupportGround = true;
        mUp = up;
        _48 = unk1;
    }

    void setIgnoreGround() { mIsIgnoreGround = true; }

    void set48(f32 value) { _48 = value; }

private:
    void* _18[5];
    s32 _40;
    bool mIsSupportGround;
    f32 _48;
    f32 _4c;
    sead::Vector3f mUp;
    sead::Vector3f _5c;
    bool mIsIgnoreGround;
};

static_assert(sizeof(CollisionShapeInfoSphere) == 0x70);

class CollisionShapeInfoDisk : public CollisionShapeInfoBase {
public:
    CollisionShapeInfoDisk(const char*, f32, const sead::Vector3f&, const sead::Vector3f&, f32);

    void updateDiskShape(f32, const sead::Vector3f&, f32);

    const sead::Vector3f& getBoundingCenter() const override;
    const sead::Vector3f& getBoundingCenterWorld() const override;
    f32 getBoundingRadius() const override;
    f32 getBoundingRadiusWorld() const override;
    f32 getCheckStepRange() const override;
    f32 getCheckStepRangeWorld() const override;
    void updateShapeOffset(const sead::Vector3f&) override;
    void calcWorldShapeInfo(const sead::Matrix34f&, f32) override;
    void calcRelativeShapeInfo(const sead::Matrix34f&) override;

    // TODO: rename parameter
    void setSupportGround(const sead::Vector3f& up, f32 unk1) {
        mIsSupportGround = true;
        mUp = up;
        _84 = unk1;
    }

    void setIgnoreGround() { mIsIgnoreGround = true; }

private:
    void* _18[13];
    bool mIsSupportGround;
    f32 _84;
    f32 _88;
    sead::Vector3f mUp;
    sead::Vector3f _98;
    bool mIsIgnoreGround = false;
};

static_assert(sizeof(CollisionShapeInfoDisk) == 0xA8);
