#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "math/seadVector.h"

namespace al {

void normalize(sead::Vector3f* vec) {
    vec->normalize();
}

void normalize(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    out->normalize();
}

void makeQuatFrontUp(sead::Quatf* quat, const sead::Vector3f& front, const sead::Vector3f& up) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    al::makeMtxFrontUp(&mtx, front, up);
    mtx.toQuat(*quat);
}

void makeQuatUpFront(sead::Quatf* quat, const sead::Vector3f& up, const sead::Vector3f& front) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    al::makeMtxUpFront(&mtx, up, front);
    mtx.toQuat(*quat);
}

}
