#pragma once

#include <nn/types.h>
#include "Player/PlayerActorHakoniwa.h"

template <typename T>
s32 getStateBitMap(u128* state, int bits, T* type);

inline u128 getPlayerStateBitMap(PlayerActorHakoniwa* player) {
    u128 state = 0;
    s32 size = getStateBitMap(&state, 0, player);
    if(size >= 128) CRASH;
    if(size != 96) CRASH;  // requires updating the gym-side code!
    return state;
}
