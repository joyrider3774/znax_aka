#include "CGameTypeMenu.h"
#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void GameTypeMenu() {
  uint8_t alpha = 0;
  CGameTypeMenu *Menu =
      new CGameTypeMenu(&IMG[IMGTitleScreen], &IMG[IMGSelectGame], &IMG[IMGFixedTimer1],
                        &IMG[IMGFixedTimer2], &IMG[IMGRelativeTimer1], &IMG[IMGRelativeTimer2],
                        SND_MENU);
  uint32_t AlphaTimer = GetTicks();
  while (GameState == GSGameTypeMenu) {
    MenuMusic();

    ReadButtons();

    if (ButNextSkin)
      NextSkin();

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButUp)
      Menu->NextItem();

    if (ButDown)
      Menu->PreviousItem();

    if (ButStart || ButA) {
      PlaySound(SND_SELECT);
      switch (Menu->GetSelection()) {
        case Fixed:
          GameType = Fixed;
          break;
        case Relative:
          GameType = Relative;
          break;
      }
      GameState = GSReadyGo;
    }

    if (ButBack || ButB)
      GameState = GSTitleScreen;

    Menu->Draw(Buffer);

    alpha = FadeAlpha(alpha, AlphaTimer);
    PresentBuffer(alpha);
    FramerateDelay();
  }
  delete Menu;
}
