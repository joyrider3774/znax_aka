#ifndef COMMON_H
#define COMMON_H

enum GameStates {
  GSQuit,
  GSIntro,
  GSGame,
  GSTitleScreen,
  GSTimeOver,
  GSReadyGo,
  GSCredits,
  GSGameTypeMenu,
  GSShowHighScores
};

enum GameTypes { Fixed = 0, Relative = 1 };

const int NrOfRows = 10, NrOfCols = 10, TileWidth = 23, TileHeight = 23,
          NrOfBlockColors = 5;

// were CGame's own constants
const int MaxSkins = 100, MaxAlpha = 245, AlphaIncrease = 5, MaxAlphaTime = 1500,
          SND_READYGO = 0, SND_5MINUTE = 1, SND_1MINUTE = 2, SND_3 = 3, SND_2 = 4,
          SND_1 = 5, SND_TIMEOVER = 6, SND_MENU = 7, SND_SELECT = 8,
          SND_WELCOME = 9, SND_GOODBYE = 10, SND_DELETE = 11,
          SND_BLOCKSELECT = 12, SND_ERROR = 13;

// the images, in the order of ASSET_IMAGE_NAMES (tools/convert_assets.py)
enum Images {
  IMGHighScores, IMGFixedTimer1, IMGRelativeTimer1, IMGFixedTimer2, IMGRelativeTimer2,
  IMGSelectGame, IMGCredits, IMGTitleScreen, IMGPlay1, IMGPlay2, IMGQuit1, IMGQuit2,
  IMGCredits2, IMGCredits1, IMGHighScores2, IMGHighScores1, IMGIntro1, IMGIntro2,
  IMGIntro3, IMGTimeOver, IMGReady, IMGGo, IMGCursor, IMGBlocks, IMGBackground,
  NrOfImages
};

struct SPoint {
  int X, Y;
};

struct SHighScore {
  char PName[21];
  int PScore;
};

#endif
