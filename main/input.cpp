#include "input.h"

#include "gb_common.h"
#include "platform.h"

bool ButLeft = false, ButRight = false, ButUp = false, ButDown = false, ButNextSkin = false, ButVolDown = false,
     ButVolUp = false, ButNextMusic = false, ButBack = false, ButStart = false, ButA = false, ButB = false;

static uint16_t PreviousKeys = 0;

bool ReadButtons()
{
    const uint16_t Keys = PollKeys();
    const uint16_t Released = PreviousKeys & ~Keys;
    PreviousKeys = Keys;
    ButLeft = Released & GB_KEY_LEFT;
    ButRight = Released & GB_KEY_RIGHT;
    ButUp = Released & GB_KEY_UP;
    ButDown = Released & GB_KEY_DOWN;
    ButA = Released & GB_KEY_A;
    ButB = Released & GB_KEY_B;
    ButBack = Released & GB_KEY_MENU;
    ButVolDown = Released & GB_KEY_L1;
    ButVolUp = Released & GB_KEY_R1;
    ButNextSkin = Released & GB_KEY_C;
    ButNextMusic = Released & GB_KEY_D;
    ButStart = false;
    return Released != 0;
}
