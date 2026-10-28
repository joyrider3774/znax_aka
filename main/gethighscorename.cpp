#include <stdio.h>
#include <string.h>

#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "input.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

void GetHighScoreName(char NameIn[21], int Place, int PScore) {
  char Name[21];
  char Msg[500];
  bool End = false, SubmitChanges = false;
  int Teller, MaxSelection = 0, Selection = 0, asci = 97;
  Color TextColor = {102, 115, 152}, TextColor1 = {255, 115, 152};
  snprintf(Name, sizeof(Name), "%s", NameIn);
  MaxSelection = strlen(NameIn);
  Name[Selection + 1] = '\0';
  if (MaxSelection == 0)
    Name[MaxSelection] = (char)asci;
  char Tekst[100];
  while (!End) {
    MenuMusic();

    ReadButtons();

    if (ButLeft) {
      if (Selection > 0) {
        Selection--;
        asci = (int)Name[Selection];
      }
    }

    if (ButRight) {
      if (Selection < 19) {
        Selection++;
        if (Selection > MaxSelection) {
          Name[Selection] = (char)97;
          Name[Selection + 1] = '\0';
          MaxSelection = Selection;
        }
        asci = (int)Name[Selection];
      }
    }

    if (ButUp) {
      asci++;
      if (asci == 123)
        asci = 32;
      if (asci == 33)
        (asci = 48);
      if (asci == 58)
        asci = 97;
      Name[Selection] = (char)asci;
    }

    if (ButDown) {
      asci--;
      if (asci == 96)
        asci = 57;
      if (asci == 47)
        asci = 32;
      if (asci == 31)
        asci = 122;
      Name[Selection] = (char)asci;
    }

    if (ButStart || ButA) {
      PlaySound(SND_SELECT);
      End = true;
      SubmitChanges = true;
    }

    if (ButBack) {
      End = true;
      SubmitChanges = false;
    }

    if (ButVolUp)
      IncVolume();

    if (ButVolDown)
      DecVolume();

    BlitSurface(IMG[IMGHighScores], NULL, Buffer, 0, 0);

    for (Teller = 0; Teller < 9; Teller++) {
      if (Teller < Place) {
        snprintf(Msg, sizeof(Msg), "%2d.%s", Teller + 1, HighScores[GameType][Teller].PName);
        WriteText(Buffer, MonoFont, Msg, 50, (62 + Teller * 16), 0, TextColor);
        snprintf(Msg, sizeof(Msg), "%7d", HighScores[GameType][Teller].PScore);
        WriteText(Buffer, MonoFont, Msg, 220, (62 + Teller * 16), 0, TextColor);
      } else {
        snprintf(Msg, sizeof(Msg), "%2d.%s", Teller + 2, HighScores[GameType][Teller].PName);
        WriteText(Buffer, MonoFont, Msg, 50, (62 + (Teller + 1) * 16), 0, TextColor);
        snprintf(Msg, sizeof(Msg), "%7d", HighScores[GameType][Teller].PScore);
        WriteText(Buffer, MonoFont, Msg, 220, (62 + (Teller + 1) * 16), 0, TextColor);
      }
    }
    snprintf(Msg, sizeof(Msg), "%2d.%s", Place + 1, Name);
    WriteText(Buffer, MonoFont, Msg, 50, (62 + Place * 16), 0, TextColor1);
    snprintf(Msg, sizeof(Msg), "%7d", PScore);
    WriteText(Buffer, MonoFont, Msg, 220, (62 + Place * 16), 0, TextColor1);
    if (Selection > 0) {
      snprintf(Tekst, sizeof(Tekst), "    ");
      for (Teller = 1; Teller < Selection; Teller++) {
        strcat(Tekst, " ");
      };
      strcat(Tekst, "_");
    } else
      snprintf(Tekst, sizeof(Tekst), "   _");
    WriteText(Buffer, MonoFont, Tekst, 50, (64 + (Place) * 16), 2, TextColor1);
    WriteText(Buffer, font, "Use Up,Down,Left,right. A = Confirm, Menu = Cancel", 30, 227, 0, TextColor);

    PresentBuffer(255);
    FramerateDelay();
  }
  Name[MaxSelection + 1] = '\0';
  while ((Name[0] == ' ') && (MaxSelection > -1)) {
    for (Teller = 0; Teller < MaxSelection; Teller++)
      Name[Teller] = Name[Teller + 1];
    MaxSelection--;
  }
  if (MaxSelection > -1)
    while ((Name[MaxSelection] == ' ') && (MaxSelection > 0)) {
      Name[MaxSelection] = '\0';
      MaxSelection--;
    }
  if (!SubmitChanges)
    snprintf(NameIn, 21, "%s", " ");
  else
    snprintf(NameIn, 21, "%s", Name);
}
