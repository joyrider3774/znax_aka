#include <stdio.h>

#include "CSelector.h"
#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void Game() {
  CSelector *Selector;
  uint32_t Time1 = GetTicks();
  uint32_t ScoreTimer = 0;
  Selector = new CSelector(5, 5, &IMG[IMGCursor]);
  Color TextColor = {255, 255, 255};
  int AddToScore = 0, Timer = 150;
  char Text[500];
  if (GameType == Relative)
    Timer = 200;
  else
    Timer = 600;

  int FrameCount = 0, Fps = 0;
  uint32_t FpsTime = GetTicks();

  while (GameState == GSGame) {
    GameMusic();

    ReadButtons();

    if (ButBack || ButB)
      GameState = GSTitleScreen;

    if (ButA || ButStart) {
      AddToScore += World->Select(Selector->GetPosition().X, Selector->GetPosition().Y);
      if (AddToScore != 0) {
        ScoreTimer = GetTicks() + 700;
      }
    }

    if (ButLeft)
      Selector->SetPosition(Selector->GetPosition().X - 1, Selector->GetPosition().Y);

    if (ButRight)
      Selector->SetPosition(Selector->GetPosition().X + 1, Selector->GetPosition().Y);

    if (ButUp)
      Selector->SetPosition(Selector->GetPosition().X, Selector->GetPosition().Y - 1);

    if (ButDown)
      Selector->SetPosition(Selector->GetPosition().X, Selector->GetPosition().Y + 1);

    if (ButNextSkin)
      NextSkin();

    if (ButNextMusic) {
      if (MusicCount > 1) {
        SelectedMusic++;
        if (SelectedMusic >= MusicCount)
          SelectedMusic = 1;
        HaltMusic();
        PlayMusic(SelectedMusic);
      }
    }

    if (ButVolUp)
      IncVolume();

    if (ButVolDown)
      DecVolume();

    World->Draw(Buffer);
    Selector->Draw(Buffer);
    if (AddToScore != 0) {
      if (ScoreTimer > GetTicks()) {
        snprintf(Text, sizeof(Text), "+%d", AddToScore);
        WriteText(Buffer, font, Text, 247, 105, 0, TextColor);
        if (GameType == Relative) {
          snprintf(Text, sizeof(Text), "+%d", AddToScore / 200);
          WriteText(Buffer, font, Text, 247, 50, 0, TextColor);
        }
      } else {
        ScoreTimer = 0;
        Score += AddToScore;
        if (GameType == Relative)
          Timer += AddToScore / 200;
        AddToScore = 0;
      }
    }

    if (ShowFPS) {
      snprintf(Text, sizeof(Text), "FPS:\n%d", Fps);
      WriteText(Buffer, BigFont, Text, 247, 140, 0, TextColor);
    }

    snprintf(Text, sizeof(Text), "Timer:\n%2d:%02d\n\nScore:\n%d", Timer / 60, Timer % 60, Score);
    WriteText(Buffer, BigFont, Text, 247, 10, 0, TextColor);
    WriteText(Buffer, BigFont, "QUIT", 247, 180, 0, TextColor);

    PresentBuffer(255);
    FramerateDelay();

    if (Time1 + 1000 < GetTicks()) {
      Timer -= 1;
      switch (Timer) {
      case 300:
        PlaySound(SND_5MINUTE);
        break;
      case 60:
        PlaySound(SND_1MINUTE);
        break;
      case 3:
        PlaySound(SND_3);
        break;
      case 2:
        PlaySound(SND_2);
        break;
      case 1:
        PlaySound(SND_1);
        break;
      case 0:
        if (ScoreTimer != 0) {
          Score += AddToScore;
          if (GameType == Relative)
            Timer += AddToScore / 200;
        }
        if (Timer == 0) {
          PlaySound(SND_TIMEOVER);
          GameState = GSTimeOver;
        }
        break;
      }
      Time1 = GetTicks();
    }

    FrameCount++;
    if (FpsTime + 1000 < GetTicks()) {
      Fps = FrameCount;
      FrameCount = 0;
      FpsTime = GetTicks();
    }
  }

  delete Selector;
  HaltMusic();
}
