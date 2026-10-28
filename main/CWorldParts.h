#ifndef CWORLDPARTS_H
#define CWORLDPARTS_H

#include "CBlock.h"
#include "Common.h"
#include "gfx.h"
#include <stdint.h>
#include <stdlib.h>

class CWorldParts {
private:
  Surface *IMGBackground = NULL, *IMGBlocks = NULL;
  int SNDDelete, SNDError, SNDBlockSelect;
  CBlock *Items[NrOfCols][NrOfRows];
  SPoint Selects[4];
  int NumSelected, SelectedColor;
  uint32_t Time;
  bool NeedToKillBlocks, NeedToAddBlocks;
  void KillBlocks();
  void AddBlocks();

public:
  CWorldParts(Surface *AIMGBackground, Surface *AIMGBlocks,
              int ASNDDelete, int ASNDError, int ASNDBlockSelect);
  void NewGame();
  void Draw(Surface &Dst);
  void DeSelect(bool PlaySound);
  long Select(int PlayFieldX, int PlayFieldY);
  ~CWorldParts();
};

#endif
