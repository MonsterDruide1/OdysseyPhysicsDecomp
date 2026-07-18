#pragma once

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Library/Audio/IUseAudioKeeper.h"

class CollectBgmPlayer : public al::HioNode, public al::ISceneObj {
public:
    void init(al::IUseAudioKeeper const*);
    void prepare(void);
    void start(char const*,char const*);
    void stop(int);
    bool isPlaying(char const*,char const*) const;

private:
    char filler[0x18];
};

static_assert(sizeof(CollectBgmPlayer) == 0x20);
