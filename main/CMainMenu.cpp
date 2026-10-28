#include "CMainMenu.h"
#include "sound.h"

// constructor of main menu will Load the graphics and set the current selection
// to 1 (io newgame)
CMainMenu::CMainMenu(Surface *AIMGTitleScreen, Surface *AIMGPlay1,
                     Surface *AIMGPlay2, Surface *AIMGHighScores1,
                     Surface *AIMGHighScores2, Surface *AIMGCredits1,
                     Surface *AIMGCredits2, Surface *AIMGQuit1,
                     Surface *AIMGQuit2, int ASNDMenu) {
  Selection = 1;
  IMGTitleScreen = AIMGTitleScreen;
  IMGPlay1 = AIMGPlay1;
  IMGPlay2 = AIMGPlay2;
  IMGHighScores1 = AIMGHighScores1;
  IMGHighScores2 = AIMGHighScores2;
  IMGCredits1 = AIMGCredits1;
  IMGCredits2 = AIMGCredits2;
  IMGQuit1 = AIMGQuit1;
  IMGQuit2 = AIMGQuit2;
  SNDMenu = ASNDMenu;
}

CMainMenu::~CMainMenu() {}

// Increase the selection if it goes to far set i to the first selection
void CMainMenu::NextItem() {
  Selection++;
  if (Selection == 5)
    Selection = 1;
  PlaySound(SNDMenu);
}

// decrease the selection if it goes to low set it to the last selection
void CMainMenu::PreviousItem() {
  Selection--;
  if (Selection == 0)
    Selection = 4;
  PlaySound(SNDMenu);
}

// Draw the main menu
void CMainMenu::Draw(Surface &Dst) {
  BlitSurface(*IMGTitleScreen, NULL, Dst, 0, 0);
  BlitSurface(Selection == 1 ? *IMGPlay2 : *IMGPlay1, NULL, Dst, 98, 73);
  BlitSurface(Selection == 2 ? *IMGHighScores2 : *IMGHighScores1, NULL, Dst, 98, 105);
  BlitSurface(Selection == 3 ? *IMGCredits2 : *IMGCredits1, NULL, Dst, 98, 137);
  BlitSurface(Selection == 4 ? *IMGQuit2 : *IMGQuit1, NULL, Dst, 98, 169);
}
