#include <stdio.h>

#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "states.h"

void ShowHighScores() {
  int Teller = 0;
  char Text[250];
  int ScoreType = GameType;
  uint8_t alpha = 0;
  Color TextColor = {102, 115, 152};
  uint32_t AlphaTimer = GetTicks();
  while (GameState == GSShowHighScores) {
    MenuMusic();

    ReadButtons();

    if (ButNextSkin)
      NextSkin();

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButStart || ButA) {
      if (ScoreType == Fixed)
        ScoreType = Relative;
      else
        GameState = GSTitleScreen;
    }

    if (ButBack || ButB)
      GameState = GSTitleScreen;

    BlitSurface(IMG[IMGHighScores], NULL, Buffer, 0, 0);

    switch (ScoreType) {
    case Fixed:
      snprintf(Text, sizeof(Text), "Gametype: Fixed Timer");
      break;
    case Relative:
      snprintf(Text, sizeof(Text), "Gametype: Relative Timer");
      break;
    }

    WriteText(Buffer, font, Text, 105, 227, 0, TextColor);
    for (Teller = 0; Teller < 10; Teller++) {
      snprintf(Text, sizeof(Text), "%2d.%s", Teller + 1, HighScores[ScoreType][Teller].PName);
      WriteText(Buffer, MonoFont, Text, 50, (62 + Teller * 16), 0, TextColor);
      snprintf(Text, sizeof(Text), "%7d", HighScores[ScoreType][Teller].PScore);
      WriteText(Buffer, MonoFont, Text, 220, (62 + Teller * 16), 0, TextColor);
    }

    alpha = FadeAlpha(alpha, AlphaTimer);
    PresentBuffer(alpha);
    FramerateDelay();
  }
}
