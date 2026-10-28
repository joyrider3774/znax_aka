#include <stdio.h>
#include <string.h>

#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "states.h"

void TimeOver() {
  char Name[21];
  int Teller1, Teller2;
  uint8_t alpha = 0;
  uint32_t AlphaTimer = GetTicks();
  uint32_t Time2 = GetTicks() + 1250;
  World->DeSelect(false);
  memset(Name, '\0', 21);
  while (GameState == GSTimeOver) {
    ReadButtons();

    if (ButBack || ButStart || ButA)
      GameState = GSShowHighScores;

    if (ButNextSkin)
      NextSkin();

    if (ButVolUp)
      IncVolume();

    if (ButVolDown)
      DecVolume();

    World->Draw(Buffer);
    BlitSurface(IMG[IMGTimeOver], NULL, Buffer, 23, 85);

    alpha = FadeAlpha(alpha, AlphaTimer);
    PresentBuffer(alpha);
    FramerateDelay();
    if (Time2 < GetTicks()) {
      GameState = GSShowHighScores;
    }
  }
  for (Teller1 = 0; Teller1 < 10; Teller1++)
    if (HighScores[GameType][Teller1].PScore < Score) {
      GetHighScoreName(Name, Teller1, Score);
      for (Teller2 = 8; Teller2 >= Teller1; Teller2--)
        HighScores[GameType][Teller2 + 1] = HighScores[GameType][Teller2];
      snprintf(HighScores[GameType][Teller1].PName, sizeof(HighScores[0][0].PName), "%s", Name);
      HighScores[GameType][Teller1].PScore = Score;
      // the SDL2 version wrote them when it quit, the AKA is just switched off
      SaveHighScores();
      break;
    }
}
