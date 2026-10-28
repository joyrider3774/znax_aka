#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "states.h"

void Credits() {
  uint8_t alpha = 0;
  uint32_t AlphaTimer = GetTicks();
  while (GameState == GSCredits) {
    MenuMusic();

    ReadButtons();

    if (ButNextSkin)
      NextSkin();

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButA || ButB || ButBack || ButStart)
      GameState = GSTitleScreen;

    BlitSurface(IMG[IMGTitleScreen], NULL, Buffer, 0, 0);
    BlitSurface(IMG[IMGCredits], NULL, Buffer, 48, 76);

    alpha = FadeAlpha(alpha, AlphaTimer);
    PresentBuffer(alpha);
    FramerateDelay();
  }
}
