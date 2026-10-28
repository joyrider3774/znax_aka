#ifndef CBLOCK_H
#define CBLOCK_H

#include "gfx.h"

class CBlock {
private:
  int Color, AnimCounter, AnimBase, AnimDelay, AnimDelayCounter, AnimPhases,
      AnimPhase, PlayFieldX, PlayFieldY;
  bool bNeedToKill, bSelected;
  Surface *IMGBlocks;

public:
  CBlock(int PlayFieldXin, int PlayFieldYin, int ColorIn,
         Surface *AIMGBlocks);
  void Select();
  void DeSelect();
  void Kill();
  void Draw(Surface &Dst);
  int GetColor();
  bool IsSelected();
  bool NeedToKill();
  ~CBlock();
};

#endif
