#pragma once

#include "Library/Nerve/NerveExecutor.h"
#include "prim/seadSafeString.h"

namespace al {
class LayoutActor;
class LayoutInitInfo;
}

class CommonHorizontalList : public al::NerveExecutor {
public:
    CommonHorizontalList(al::LayoutActor *,al::LayoutInitInfo const&,bool);
    void addStringData(sead::WFixedSafeString<512> const*,char const*);
    void setGroupAnimData(sead::FixedSafeString<64> const*,char const*);
private:
    char filler[0xb0];
};
static_assert(sizeof(CommonHorizontalList) == 0xc0);
