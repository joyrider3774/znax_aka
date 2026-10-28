#include <stdio.h>

#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void ReadyGo() {
  uint8_t alpha = 0;
  uint32_t Time1 = GetTicks();
  uint32_t Time2 = GetTicks() + 500;
  World->NewGame();
  Score = 0;
  int Timer = 600, Counter = 0;
  char Text[500];
  Color TextColor = {255, 255, 255};
  if (GameType == Relative)
    Timer = 200;
  else
    Timer = 600;
  int FrameCount = 0, Fps = 0;

  if (MusicCount > 1) {
    SelectedMusic = 1 + rand() % (MusicCount - 1);
    PlayMusic(SelectedMusic);
  } else if (MusicCount == 1) {
    if (!MusicPlaying()) {
      SelectedMusic = 0;
      PlayMusic(SelectedMusic);
    }
  }
  uint32_t AlphaTimer = GetTicks();
  while (GameState == GSReadyGo) {
    GameMusic();

    ReadButtons();

    if (ButNextSkin)
      NextSkin();

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButBack || ButB)
      GameState = GSTitleScreen;

    World->Draw(Buffer);
    switch (Counter) {
    case 1:
      BlitSurface(IMG[IMGReady], NULL, Buffer, 46, 85);
      break;
    case 2:
      BlitSurface(IMG[IMGGo], NULL, Buffer, 93, 85);
      break;
    case 3:
      GameState = GSGame;
      break;
    }
    alpha = FadeAlpha(alpha, AlphaTimer);

    if (ShowFPS) {
      snprintf(Text, sizeof(Text), "FPS : %d", Fps);
      WriteText(Buffer, BigFont, Text, 247, 140, 0, TextColor);
    }

    snprintf(Text, sizeof(Text), "Timer:\n%2d:%02d\n\nScore:\n%d", Timer / 60, Timer % 60, Score);
    WriteText(Buffer, BigFont, Text, 247, 10, 0, TextColor);
    WriteText(Buffer, BigFont, "QUIT", 247, 180, 0, TextColor);

    PresentBuffer(alpha);
    FramerateDelay();

    FrameCount++;
    if (Time1 + 1000 < GetTicks()) {
      Fps = FrameCount;
      FrameCount = 0;
      Time1 = GetTicks();
    }

    if (Time2 < GetTicks()) {
      Counter++;
      if (Counter == 1)
        PlaySound(SND_READYGO);
      if (Counter == 2) {
        Time2 = GetTicks() + 400;
      } else
        Time2 = GetTicks() + 900;
    }
  }
}
