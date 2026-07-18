#pragma once

#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Library/PostProcessing/PostProcessingFilter.h"

class ControllerGuideSnapShotCtrl : public al::NerveStateBase {
public:
    ControllerGuideSnapShotCtrl(char const*,al::LayoutInitInfo const&,al::PostProcessingFilter *);

private:
    char filler[0x50];
};
static_assert(sizeof(ControllerGuideSnapShotCtrl) == 0x68);
