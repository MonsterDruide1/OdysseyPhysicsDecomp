#include "Player/PlayerActionTurnControl.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionFunction.h"
#include "math/seadQuat.h"

PlayerActionTurnControl::PlayerActionTurnControl(al::LiveActor* actor) : mActor(actor) {
    _48 = 0;
    arr = new f32[5];
    arr_size = 5;
    _44 = 0;
}

void PlayerActionTurnControl::setup(f32 turnAngleStart, f32 turnAngleFast, f32 turnAngleLimit,
                                    f32 turnAngleFastLimit, s32 turnAccelFrame,
                                    s32 turnAccelFrameFast, s32 turnBrakeFrame) {
    mTurnAngleStart = turnAngleStart;
    mTurnAngleFast = turnAngleFast;
    mTurnAngleLimit = turnAngleLimit;
    mTurnAngleFastLimit = turnAngleFastLimit;
    mTurnAccelFrame = turnAccelFrame;
    mTurnAccelFrameFast = turnAccelFrameFast;
    mTurnBrakeFrame = turnBrakeFrame;
}

void PlayerActionTurnControl::reset() {
    _44 = 0;
    _48 = 0;
    _30 = 0.0;
    _8 = 0.0;
    _c = {0.0f, 0.0f, 0.0f};
    _18 = {0.0f, 0.0f, 0.0f};
    _24 = {0.0f, 0.0f, 0.0f};
}

void PlayerActionTurnControl::update(const sead::Vector3f& a2, const sead::Vector3f& a3) {
    bool v12;
    bool v13;
    bool v14;
    bool v15;
    bool v16;
    bool v17;
    bool v18;
    float v19;
    float v20;
    int _48;
    int arr_size;
    int _44;
    int v24;
    int v25;
    int v26;
    int v27;
    int v28;
    int v29;
    float* arr;
    int v31;
    int v32;
    bool v33;
    int v34;
    int v35;
    int v36;
    float* v37;
    float v38;
    int v39;
    int v40;
    int v41;
    bool v42;
    bool v43;
    bool v44;
    bool isOppositeDir;
    float v48;
    float v49;
    float v50;
    sead::Vector3f* v51;
    float v53;
    float v54;
    float v55;
    const sead::Vector3f* v56;
    bool v57;
    bool v58;
    float v59;
    float v60;
    float v61;
    float lTurnAngleLimit;
    float v63;
    float v64;
    float lTurnAngleFastLimit;
    int lTurnAccelFrame;
    float v72;
    float v73;
    float v74;
    float v75;
    float cosAngle;
    float sinAngle;
    float v78;
    float v79;
    float v80;
    float v81;
    float v82;
    float v83;
    float v84;
    float v85;
    float v86;
    float v87;
    float v88;
    float v89;
    float v90;
    float v91;
    float lTurnAngleStart;
    const sead::Vector3f* v94;
    sead::Vector3f v67v;
    sead::Quatf v95q;
    sead::Vector3f v95v;
    sead::Vector3f v96;
    sead::Vector3f v97;
    sead::Vector3f v98;
    sead::Vector3f a3a;
    sead::Vector3f a1;
    sead::Vector3f* v103;

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, this->mActor);
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    al::calcUpDir(&up, this->mActor);
    al::alongVectorNormalH(&front, front, up, a3);
    al::tryNormalizeOrZero(&front);
    this->_50 = front;

    a1 = {0.0f, 0.0f, 0.0f};
    if (!al::isNearZero(this->_c, 0.001) || !al::isNearZero(this->_24, 0.001)) {
        al::alongVectorNormalH(&a1, this->_c, this->_24, a3);
        al::tryNormalizeOrZero(&a1);
    }

    v94 = &a3;
    sead::Vector3f gravity = al::getGravity(this->mActor);
    a3a = {0.0f, 0.0f, 0.0f};
    v12 = al::tryNormalizeOrZero(&a3a, a2);
    v13 = 0;
    if (up.dot(-gravity) < 0.087156)
        v13 = this->_89;

    v14 = al::isNearZero(front, 0.001);
    v15 = v13 || v14;
    v16 = al::isNearZero(a1, 0.001);
    v17 = v12 && !v16;
    v18 = v16 || !v12;
    v103 = (sead::Vector3f*)&a2;
    if (v18 || a3a.dot(a1) < 0.34202) {
        this->_44 = 0;
        this->_48 = 0;
        v19 = 0.0;
        this->_30 = 0.0;
    } else {
        if (v12 && !v16)
            v20 = sead::Mathf::abs(al::calcAngleDegree(a1, a3a));
        else
            v20 = 0.0;

        _48 = this->_48;
        arr_size = this->arr_size;
        _44 = this->_44;
        if (_48 < arr_size)
            goto LABEL_21;

        v24 = _48 - 1;
        if (_48 >= 1) {
            v25 = this->_44;
            if (v25 + 1 < arr_size)
                v26 = v25 + 1;
            else
                v26 = 0;

            _48 = v24;
            this->_44 = v26;
            this->_48 = v24;
            arr_size = this->arr_size;
            _44 = this->_44;
        }

        if (_48 < arr_size) {
        LABEL_21:
            v27 = _48 + 1;
            v28 = _44 + _48;
            if (v28 < arr_size)
                arr_size = 0;

            v29 = v28 - arr_size;
            arr = this->arr;
            this->_48 = v27;
            arr[v29] = v20;
        }

        v31 = this->_48;
        this->_30 = 0.0;
        if (v31 <= 5)
            v32 = v31;
        else
            v32 = 5;

        v33 = v31 - 1;  //__OFSUB__
        v34 = v31 - 1;
        if (v34 < 0 != v33) {
            v38 = 0.0;
        } else {
            v35 = this->arr_size;
            v36 = this->_44;
            v37 = this->arr;
            v38 = 0.0;
            v39 = 0;
            do {
                if (v34 + v36 >= v35)
                    v40 = v35;
                else
                    v40 = 0;

                v41 = v34 + v36 - v40;
                --v34;
                v38 = v37[v41] + v38;
                ++v39;
                this->_30 = v38;
            } while (v39 < v32);
        }

        v19 = v38 / 5.0;
        this->_30 = v38 / 5.0;
    }

    if (((v13 | v14 | v12 ^ 1) & 1) != 0) {
        v42 = 0;
        v43 = 0;
        v44 = v12;
    } else {
        isOppositeDir = PlayerActionFunction::isOppositeDir(a3a, front);
        v42 = 0;
        if (v18 || v17 && v19 <= 1.0) {
            v44 = v12;
            v43 = 0;
            if (isOppositeDir) {
                v43 = this->_88;
                v42 = !this->_88;
                v44 = v12 & v43;
            }
        } else {
            v44 = v12;
            v43 = 0;
        }
    }

    this->_69 = v43;
    this->_68 = v42;
    this->_6a = v13;
    v98 = {0.0f, 0.0f, 0.0f};
    if ((v44 & (v15 ^ 1) & 1) == 0) {
        if (v12) {
            if (v42) {
                v98 = front;
                this->_8 = 0.0;
                goto LABEL_101;
            }

            v98 = a3a;
            if (!v13)
                goto LABEL_101;
        } else {
            if (!v13) {
                if (v15)
                    al::calcFrontDir(&v98, this->mActor);
                else
                    v98 = front;

                this->_18.x = 0.0;
                this->_18.y = 0.0;
                this->_18.z = 0.0;
                this->_24.x = 0.0;
                this->_24.y = 0.0;
                this->_24.z = 0.0;
                this->_c.x = 0.0;
                this->_c.y = 0.0;
                this->_c.z = 0.0;
                this->_8 = PlayerActionFunction::brake(this->_8, this->mTurnBrakeFrame,
                                                       this->mTurnAngleLimit);
                goto LABEL_101;
            }

            al::calcFrontDir(&v98, this->mActor);
            if (up.dot(-gravity) < 0.0)
                v98 = -v98;

            this->_24.z = 0.0;
            this->_18.y = 0.0;
            this->_18.z = 0.0;
            this->_24.x = 0.0;
            this->_24.y = 0.0;
            this->_c.x = 0.0;
            this->_c.y = 0.0;
            this->_c.z = 0.0;
            this->_18.x = 0.0;
        }

        this->_8 = this->mTurnAngleFastLimit;
        goto LABEL_101;
    }

    if (v17)
        v48 = a1.dot(a3a);
    else
        v48 = front.dot(a3a);

    v49 = sead::Mathf::clamp(v48, -1.0, 1.0);
    v50 = sead::Mathf::acos(v49);
    v51 = v103;
    v53 = sead::Mathf::clamp(front.dot(a3a), -1.0, 1.0);
    v54 = sead::Mathf::acos(v53);
    v97.setCross(a1, a3a);
    al::tryNormalizeOrZero(&v97);
    if (!v17 || al::isNearZero(this->_18, 0.001))
        goto LABEL_61;

    v55 = sead::Mathf::rad2deg(v50);
    if (v97.dot(this->_18) <= 0.0) {
        lTurnAngleStart = this->mTurnAngleStart;
        if (lTurnAngleStart <= this->_8)
            lTurnAngleStart = this->_8;

        if (v55 >= lTurnAngleStart) {
            v57 = 0;
            v58 = 0;
            this->_8 = 0.0;
        } else {
            this->_8 = al::converge(this->_8, 0.0, v55);
            v57 = 0;
            v58 = v55 < this->mTurnAngleStart;
        }

        goto LABEL_62;
    }

    v95v.setCross(front, a3a);
    if (!al::tryNormalizeOrZero(&v95v)) {
    LABEL_61:
        v57 = 0;
        v58 = 0;

    LABEL_62:
        v56 = v94;
        goto LABEL_63;
    }

    v56 = &a3;
    if (v95v.dot(v97) >= 0.0) {
        v57 = 0;
        v58 = 0;
    } else {
        v57 = v19 > 10.0 || v55 > 3.0;
        v58 = v19 <= 10.0 && v55 <= 3.0;
    }

LABEL_63:
    v59 = sead::Mathf::rad2deg(v54);
    v60 = al::calcRate01(v51->length(), 0.0, 0.8);
    v61 = sead::Mathf::clamp(v60, 0.25, 1.0);

    lTurnAngleLimit = this->mTurnAngleLimit;
    if (v59 < this->mTurnAngleStart) {
        v63 = this->_8;

    LABEL_68:
        v64 = PlayerActionFunction::brake(v63, this->mTurnBrakeFrame, lTurnAngleLimit);
        goto LABEL_82;
    }

    v63 = this->_8;
    if (v59 >= this->mTurnAngleFast) {
        lTurnAngleFastLimit = this->mTurnAngleFastLimit;
        lTurnAccelFrame = this->mTurnAccelFrameFast;
    } else {
        lTurnAngleFastLimit = v61 * lTurnAngleLimit;
        if (v63 >= lTurnAngleFastLimit) {
            lTurnAngleLimit = this->mTurnAngleFastLimit;
            goto LABEL_68;
        }

        lTurnAccelFrame = this->mTurnAccelFrame;
    }

    v64 = PlayerActionFunction::accel(v63, lTurnAngleFastLimit,
                                      lTurnAngleFastLimit / lTurnAccelFrame);

LABEL_82:
    this->_8 = v64;
    v96 = {0.0f, 0.0f, 0.0f};
    if (v57 || al::isReverseDirection(front, a3a, 0.01)) {
        v67v = *v56;
        if (v97.dot(*v56) <= 0.0) {
            v67v = -v67v;
        }

        v72 = sead::Mathf::deg2rad(this->_8) * 0.5;
        v73 = v67v.x;
        v74 = v67v.y;
        v75 = v67v.z;
        cosAngle = cosf(v72);
        sinAngle = sinf(v72);
        v78 = v67v.x * sinAngle;
        v79 = v67v.y * sinAngle;
        v80 = v67v.z * sinAngle;
        v81 = ((v79 * front.z) - (v80 * front.y)) + (cosAngle * front.x);
        v82 = (cosAngle * front.y) + ((v80 * front.x) - (front.z * v78));
        v83 = (cosAngle * front.z) + ((v78 * front.y) - (v79 * front.x));
        v84 = (-(v78 * front.x) - (v79 * front.y)) - (front.z * v80);
        v98.x = ((v79 * v83) + ((cosAngle * v81) - (v80 * v82))) - (v78 * v84);
        v98.y = (((v80 * v81) + (cosAngle * v82)) - (v78 * v83)) - (v79 * v84);
        v98.z = ((cosAngle * v83) + ((v78 * v82) - (v79 * v81))) - (v80 * v84);
        al::tryNormalizeOrZero(&v98);
        v96 = v67v;
    } else {
        v95q = sead::Quatf::unit;
        al::makeQuatRotationLimit(&v95q, front, a3a, sead::Mathf::deg2rad(this->_8));
        v85 = ((v95q.y * front.z) - (v95q.z * front.y)) + (v95q.w * front.x);
        v86 = (front.y * v95q.w) + ((v95q.z * front.x) - (front.z * v95q.x));
        v87 = (front.z * v95q.w) + ((front.y * v95q.x) - (v95q.y * front.x));
        v88 = (-(front.x * v95q.x) - (v95q.y * front.y)) - (front.z * v95q.z);
        v98.x = ((v95q.y * v87) + ((v95q.w * v85) - (v95q.z * v86))) - (v95q.x * v88);
        v98.y = (((v85 * v95q.z) + (v86 * v95q.w)) - (v87 * v95q.x)) - (v88 * v95q.y);
        v98.z = (((v86 * v95q.x) - (v85 * v95q.y)) + (v87 * v95q.w)) - (v88 * v95q.z);
        al::normalize(&v98);
        v96.setCross(front, a3a);
        al::tryNormalizeOrZero(&v96);
        if (v58) {
            v89 = v96.x;
            v90 = v96.y;
            v91 = v96.z;
            if (v96.dot(this->_18) < 0.0) {
                v89 = -v96.x;
                v90 = -v96.y;
                v91 = -v96.z;
                v96 = -v96;
            }

            v73 = v89;
            v74 = v90;
            v75 = v91;
        } else {
            v73 = v96.x;
            v74 = v96.y;
            v75 = v96.z;
        }
    }

    this->_18.x = v73;
    this->_18.y = v74;
    this->_18.z = v75;
    this->_c = a3a;
    this->_24 = *v56;

LABEL_101:
    this->_5c = v98;
}

void PlayerActionTurnControl::calcTilt(sead::Vector3f*, const sead::Vector3f&, f32){
    CRASH} f32 PlayerActionTurnControl::calcTurnPowerRate(const sead::Vector3f& a2) const {
    f32 result = al::easeIn(al::calcRate01(_8, mTurnAngleStart, mTurnAngleLimit));
    if (a2.dot(_18) > 0.0)
        return -result;

    return result;
}
