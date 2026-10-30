// The game's buttons on the AKA's keys.
//
// The game acted on a key or a controller button when it came up (SDL_KEYUP,
// SDL_CONTROLLERBUTTONUP) and set one But* flag for that frame. ReadButtons does the same with
// the AKA's keys, following the layout the game gave a controller:
//   d-pad / joystick  ButLeft, ButRight, ButUp, ButDown
//   A                 ButA     select, confirm
//   B                 ButB     back
//   MENU              ButBack  quit to the title screen (Escape, the controller's back)
//   L1 / R1           ButVolDown, ButVolUp
//   C                 ButNextSkin
//   D                 ButNextMusic
#ifndef INPUT_H
#define INPUT_H

extern bool ButLeft, ButRight, ButUp, ButDown, ButNextSkin, ButVolDown, ButVolUp, ButNextMusic, ButBack,
    ButStart, ButA, ButB;

// sets the But* flags of the keys that came up since the last call, true when any did
bool ReadButtons();

#endif
