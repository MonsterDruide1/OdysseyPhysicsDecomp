#pragma once

#include "Library/Nerve/NerveExecutor.h"
#include "prim/seadSafeString.h"

namespace al {
class LayoutActor;
class LayoutInitInfo;
}

class CommonVerticalList : public al::NerveExecutor {
public:
    CommonVerticalList(al::LayoutActor *,al::LayoutInitInfo const&,bool);
    void addStringData(sead::WFixedSafeString<512> const*,char const*);
    void addGroupAnimData(sead::FixedSafeString<64> const*,char const*);
private:
    char filler[0xc0];
};
static_assert(sizeof(CommonVerticalList) == 0xd0);
