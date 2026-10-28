// What the states have in common, the members of CGame that were not a state: volume, settings,
// skins, high scores, music and the fade a screen starts with
#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include <stdint.h>

void IncVolume();
void DecVolume();
// the volume (0 to 128) and the skin, in DATA_DIR/settings.dat
void LoadSettings();
void SaveSettings();
void SearchForSkins();
// the images of SkinName, the default ones where the skin has none
void LoadGraphics();
void NextSkin();
void LoadHighScores();
void SaveHighScores();
// the title music, which the menus keep playing
void MenuMusic();
// the other music, at random, while a game is on
void GameMusic();
// the alpha a screen fades in with, AlphaTimer milliseconds after it started; 255 when it is done
uint8_t FadeAlpha(uint8_t alpha, uint32_t AlphaTimer);

#endif
