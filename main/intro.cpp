#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void Intro() {
  uint8_t alpha = 0;
  int IntroScreenNr = 1;
  uint32_t Time1 = GetTicks();
  uint32_t AlphaTimer = GetTicks();
  while (GameState == GSIntro) {
    // any key that comes up ends the intro
    if (ReadButtons())
      GameState = GSTitleScreen;

    switch (IntroScreenNr) {
    case 1:
      BlitSurface(IMG[IMGIntro1], NULL, Buffer, 0, 0);
      break;
    case 2:
      BlitSurface(IMG[IMGIntro2], NULL, Buffer, 0, 0);
      break;
    case 3:
      BlitSurface(IMG[IMGIntro3], NULL, Buffer, 0, 0);
      break;
    }

    alpha = FadeAlpha(alpha, AlphaTimer);
    PresentBuffer(alpha);
    FramerateDelay();

    if (Time1 + 3700 < GetTicks()) {
      AlphaTimer = GetTicks();
      alpha = 0;
      IntroScreenNr++;
      if (IntroScreenNr > 3)
        GameState = GSTitleScreen;
      Time1 = GetTicks();
    }
  }
  PlaySound(SND_WELCOME);
}
