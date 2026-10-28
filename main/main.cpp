// Znax for the Gamebuino AKA, ported from Willems Davy's SDL2 version (CGame.cpp there).
//
// The game is the SDL2 one at its own 320x240, the size of the AKA's screen: the same screens,
// skins, timers and scores, with the window, the renderer's scaling and the mouse and touch
// input of the PC left out. What sat under it changed: SDL2 is gfx.*, SDL2_mixer is sound.*,
// the keyboard and game controller are input.*, and the files live in the ZNAX folder on the
// SD card. CGame's states are a file each, its other members are in gamefuncs.*.
//
// Keys: d-pad or joystick, A select, B back, MENU quit to the menu (on the title screen: quit),
// L1/R1 the volume, C the next skin, D the next music.

#include <stdlib.h>

#include "commonvars.h"
#include "gamefuncs.h"
#include "gfx.h"
#include "platform.h"
#include "sound.h"
#include "states.h"

static void Quit() {
  HaltMusic();
  SaveHighScores();
  SaveSettings();
  ReturnToLoader();
}

static void Run() {
  PlatformInit();
  GfxInit();
  MakeDataDirs();
  SoundInit();
  srand(GetTicks());
  SearchForSkins();
  LoadSettings();
  LoadGraphics();
  LoadHighScores();
  World = new CWorldParts(&IMG[IMGBackground], &IMG[IMGBlocks], SND_DELETE, SND_ERROR, SND_BLOCKSELECT);
  while (GameState != GSQuit) {
    switch (GameState) {
    case GSGame:
      Game();
      break;
    case GSTitleScreen:
      TitleScreen();
      break;
    case GSIntro:
      Intro();
      break;
    case GSReadyGo:
      ReadyGo();
      break;
    case GSTimeOver:
      TimeOver();
      break;
    case GSCredits:
      Credits();
      break;
    case GSGameTypeMenu:
      GameTypeMenu();
      break;
    case GSShowHighScores:
      ShowHighScores();
      break;
    default:
      break;
    }
  }
  Quit();
}

extern "C" void app_main(void) {
  Run();
}
