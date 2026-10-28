#include "commonvars.h"

Surface IMG[NrOfImages];
GameStates GameState = GSIntro;
int SelectedMusic = 0, Volume = 128, InstalledSkinsCount = 0, SelectedSkin = -1, Score = 0;
char InstalledSkins[MaxSkins][64];
char SkinName[64];
CWorldParts *World = NULL;
GameTypes GameType = Fixed;
SHighScore HighScores[2][10];
bool ShowFPS = false;
