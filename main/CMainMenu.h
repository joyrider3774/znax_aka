#ifndef CMAINMENU_H
#define CMAINMENU_H

#include "Common.h"
#include "gfx.h"

class CMainMenu {
private:
  int Selection; // Holds what we currently selected in the main menu, 1 = new
                 // game, 2 = highscores, 3 = credits, 4 = quit
  Surface *IMGTitleScreen, *IMGPlay1, *IMGPlay2, *IMGHighScores1,
      *IMGHighScores2, *IMGCredits1, *IMGCredits2, *IMGQuit1, *IMGQuit2;
  int SNDMenu;

public:
  CMainMenu(Surface *AIMGTitleScreen, Surface *AIMGPlay1, Surface *AIMGPlay2,
            Surface *AIMGHighScores1, Surface *AIMGHighScores2,
            Surface *AIMGCredits1, Surface *AIMGCredits2, Surface *AIMGQuit1,
            Surface *AIMGQuit2, int ASNDMenu);
  void Draw(Surface &Dst);
  void NextItem();
  void PreviousItem();
  int GetSelection() { return Selection; }
  ~CMainMenu();
};

#endif
