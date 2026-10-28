#ifndef CSELECTOR_H
#define CSELECTOR_H

#include "Common.h"
#include "gfx.h"

class CSelector {
private:
  SPoint SelectedPoint, CurrentPoint;

  Surface *IMGCursor;

public:
  bool HasSelection;
  CSelector(const int PlayFieldXin, const int PlayFieldYin,
            Surface *AIMGCursor);
  ~CSelector();
  void SetPosition(const int PlayFieldXin, const int PlayFieldYin);
  void Select();
  SPoint GetSelection();
  SPoint GetPosition();
  void DeSelect();
  void Draw(Surface &Dst);
};

#endif
