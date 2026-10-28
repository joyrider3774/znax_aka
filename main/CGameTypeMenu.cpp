#include "CGameTypeMenu.h"
#include "sound.h"

// constructor of main menu will Load the graphics and set the current selection
// to 1 (io newgame)
CGameTypeMenu::CGameTypeMenu(Surface *AIMGTitleScreen, Surface *AIMGSelectGame,
                             Surface *AIMGFixedTimer1, Surface *AIMGFixedTimer2,
                             Surface *AIMGRelativeTimer1,
                             Surface *AIMGRelativeTimer2, int ASNDMenu) {
  Selection = Fixed;
  IMGTitleScreen = AIMGTitleScreen;
  IMGSelectGame = AIMGSelectGame;
  IMGFixedTimer1 = AIMGFixedTimer1;
  IMGFixedTimer2 = AIMGFixedTimer2;
  IMGRelativeTimer1 = AIMGRelativeTimer1;
  IMGRelativeTimer2 = AIMGRelativeTimer2;
  SNDMenu = ASNDMenu;
}

// Destructor will free the surface images
CGameTypeMenu::~CGameTypeMenu() {}

// Increase the selection if it goes to far set i to the first selection
void CGameTypeMenu::NextItem() {
  Selection++;
  if (Selection == 2)
    Selection = 0;
  PlaySound(SNDMenu);
}

// decrease the selection if it goes to low set it to the last selection
void CGameTypeMenu::PreviousItem() {
  Selection--;
  if (Selection == -1)
    Selection = 1;
  PlaySound(SNDMenu);
}

// Draw the main menu
void CGameTypeMenu::Draw(Surface &Dst) {
  // draw the title screen background
  BlitSurface(*IMGTitleScreen, NULL, Dst, 0, 0);
  BlitSurface(*IMGSelectGame, NULL, Dst, 68, 77);
  BlitSurface(Selection == Fixed ? *IMGFixedTimer2 : *IMGFixedTimer1, NULL, Dst, 68, 114);
  BlitSurface(Selection == Relative ? *IMGRelativeTimer2 : *IMGRelativeTimer1, NULL, Dst, 68, 151);
}
