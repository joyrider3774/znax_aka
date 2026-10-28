// The game's globals, the members of CGame every state shared
#ifndef COMMONVARS_H
#define COMMONVARS_H

#include "Common.h"
#include "CWorldParts.h"
#include "gfx.h"

// the images of the skin in use: the default ones, or a skin's from the card where it has them
extern Surface IMG[NrOfImages];
extern GameStates GameState;
extern int SelectedMusic, Volume, InstalledSkinsCount, SelectedSkin, Score;
extern char InstalledSkins[MaxSkins][64];
extern char SkinName[64];
extern CWorldParts *World;
extern GameTypes GameType;
extern SHighScore HighScores[2][10];
extern bool ShowFPS;

#endif
