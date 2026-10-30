#include "CMainMenu.h"
#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

// Quit: the goodbye and then back to the launcher (main.cpp)
static void QuitGame() {
  GameState = GSQuit;
  HaltMusic();
  PlaySound(SND_GOODBYE);
  Delay(750);
}

void TitleScreen() {
  GameType = Fixed;
  uint8_t alpha = 0;
  CMainMenu *Menu =
      new CMainMenu(&IMG[IMGTitleScreen], &IMG[IMGPlay1], &IMG[IMGPlay2], &IMG[IMGHighScores1],
                    &IMG[IMGHighScores2], &IMG[IMGCredits1], &IMG[IMGCredits2], &IMG[IMGQuit1],
                    &IMG[IMGQuit2], SND_MENU);
  uint32_t AlphaTimer = GetTicks();
  while (GameState == GSTitleScreen) {
    MenuMusic();

    ReadButtons();

    if (ButNextSkin)
      NextSkin();

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButUp)
      Menu->PreviousItem();

    if (ButDown)
      Menu->NextItem();

    if (ButStart || ButA) {
      if (Menu->GetSelection() != 4)
        PlaySound(SND_SELECT);
      switch (Menu->GetSelection()) {
        case 1:
          GameState = GSGameTypeMenu;
          break;
        case 2:
          GameState = GSShowHighScores;
          break;
        case 3:
          GameState = GSCredits;
          break;
        case 4:
          QuitGame();
          break;
      }
    }

    Menu->Draw(Buffer);

    alpha = FadeAlpha(alpha, AlphaTimer);
    PresentBuffer(alpha);
    FramerateDelay();
  }
  delete Menu;
}
