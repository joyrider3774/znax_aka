#ifndef CGAMETYPEMENU_H
#define CGAMETYPEMENU_H

#include "Common.h"
#include "gfx.h"

class CGameTypeMenu {
private:
  int Selection;
  Surface *IMGTitleScreen, *IMGSelectGame, *IMGFixedTimer1, *IMGFixedTimer2,
      *IMGRelativeTimer1, *IMGRelativeTimer2;
  int SNDMenu;

public:
  CGameTypeMenu(Surface *AIMGTitleScreen, Surface *AIMGSelectGame,
                Surface *AIMGFixedTimer1, Surface *AIMGFixedTimer2,
                Surface *AIMGRelativeTimer1, Surface *AIMGRelativeTimer2,
                int ASNDMenu);
  void Draw(Surface &Dst);
  void NextItem();
  void PreviousItem();
  int GetSelection() { return Selection; }
  ~CGameTypeMenu();
};

#endif
