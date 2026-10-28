#include "gamefuncs.h"

#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "assets.h"
#include "commonvars.h"
#include "gfx.h"
#include "platform.h"
#include "sound.h"

// the default skin, built into the firmware, in the order of the Images enum
static const AssetSurface *const DefaultImages[NrOfImages] = {
    &ASSET_HIGHSCORES, &ASSET_FIXEDTIMER1, &ASSET_RELATIVETIMER1, &ASSET_FIXEDTIMER2,
    &ASSET_RELATIVETIMER2, &ASSET_SELECTGAME, &ASSET_CREDITS, &ASSET_TITLESCREEN,
    &ASSET_PLAY1, &ASSET_PLAY2, &ASSET_QUIT1, &ASSET_QUIT2, &ASSET_CREDITS2,
    &ASSET_CREDITS1, &ASSET_HIGHSCORES2, &ASSET_HIGHSCORES1, &ASSET_INTRO1,
    &ASSET_INTRO2, &ASSET_INTRO3, &ASSET_TIMEOVER, &ASSET_READY, &ASSET_GO,
    &ASSET_CURSOR, &ASSET_BLOCKS, &ASSET_BACKGROUND};

// increase the volume with 8
void IncVolume() {
  if (Volume < 128) {
    Volume += 8;
    SetVolume(Volume);
    SaveSettings();
  }
}

// decrease the volume with 8
void DecVolume() {
  if (Volume > 0) {
    Volume -= 8;
    SetVolume(Volume);
    SaveSettings();
  }
}

void LoadSettings() {
  FILE *SettingsFile;
  SkinName[0] = '\0';
  SettingsFile = fopen(DATA_DIR "/settings.dat", "r");
  if (SettingsFile) {
    if ((fscanf(SettingsFile, "Volume=%d\n", &Volume) != 1) || (Volume < 0) || (Volume > 128))
      Volume = 128;
    SetVolume(Volume);
    if (fscanf(SettingsFile, "Skin=%63s\n", SkinName) != 1)
      SkinName[0] = '\0';
    for (int Teller = 0; Teller < InstalledSkinsCount; Teller++) {
      if (strcmp(InstalledSkins[Teller], SkinName) == 0) {
        SelectedSkin = Teller;
        break;
      } else
        SelectedSkin = -1;
    }
    fclose(SettingsFile);
  } else {
    SetVolume(128);
  }
}

void SaveSettings() {
  FILE *SettingsFile;
  SettingsFile = fopen(DATA_DIR "/settings.dat", "w");
  if (SettingsFile) {
    fprintf(SettingsFile, "Volume=%d\n", Volume);
    fprintf(SettingsFile, "Skin=%s\n", SkinName);
    fclose(SettingsFile);
  }
}

static int CompareNames(const void *a, const void *b) {
  return strcasecmp((const char *)a, (const char *)b);
}

void SearchForSkins() {
  int Teller = 0;
  DIR *Directory = opendir(DATA_DIR "/skins");
  if (Directory) {
    struct dirent *Entry;
    while ((Entry = readdir(Directory)) && (Teller < MaxSkins)) {
      if ((Entry->d_type == DT_DIR) && strncmp(".", Entry->d_name, 1) &&
          (strlen(Entry->d_name) < sizeof(InstalledSkins[0]))) {
        snprintf(InstalledSkins[Teller], sizeof(InstalledSkins[0]), "%s", Entry->d_name);
        Teller++;
      }
    }
    closedir(Directory);
  }
  InstalledSkinsCount = Teller;
  qsort(InstalledSkins, InstalledSkinsCount, sizeof(InstalledSkins[0]), CompareNames);
  SelectedSkin = -1;
}

void LoadGraphics() {
  char FileName[160];
  for (int Teller = 0; Teller < NrOfImages; Teller++) {
    FreeSurface(IMG[Teller]);
    IMG[Teller].loaded = false;
    if (SkinName[0]) {
      snprintf(FileName, sizeof(FileName), DATA_DIR "/skins/%s/%s.bin", SkinName,
               ASSET_IMAGE_NAMES[Teller]);
      IMG[Teller] = LoadSurfaceFile(FileName);
    }
    if (!IMG[Teller].loaded)
      IMG[Teller] = SurfaceFromAsset(*DefaultImages[Teller]);
  }
}

void NextSkin() {
  Color TextColor = {255, 255, 255};
  if (InstalledSkinsCount > 0) {
    char Text[128];
    SelectedSkin++;
    if (SelectedSkin >= InstalledSkinsCount)
      SelectedSkin = -1;
    if (SelectedSkin == -1) {
      SkinName[0] = '\0';
      snprintf(Text, sizeof(Text), "Please Wait Loading Skin:\nDefault Skin");
    } else {
      snprintf(SkinName, sizeof(SkinName), "%s", InstalledSkins[SelectedSkin]);
      snprintf(Text, sizeof(Text), "Please Wait Loading Skin:\n%s", SkinName);
    }

    BoxRGBA(Buffer, 0, 0, 319, 48, 255, 255, 255, 200);
    BoxRGBA(Buffer, 1, 1, 318, 47, 0, 0, 0, 200);
    WriteText(Buffer, BigFont, Text, 5, 0, 0, TextColor);
    PresentBuffer(255);

    LoadGraphics();
    SaveSettings();
  }
}

void LoadHighScores() {
  FILE *FHighScores;
  FHighScores = fopen(DATA_DIR "/highscore.dat", "rb");
  if (FHighScores &&
      (fread(HighScores[Fixed], sizeof(HighScores[Fixed][0]), 10, FHighScores) == 10) &&
      (fread(HighScores[Relative], sizeof(HighScores[Relative][0]), 10, FHighScores) == 10)) {
    fclose(FHighScores);
  } else {
    if (FHighScores)
      fclose(FHighScores);
    for (int Teller = 0; Teller < 10; Teller++) {
      snprintf(HighScores[Relative][Teller].PName, sizeof(HighScores[0][0].PName), "%s", "joyrider");
      HighScores[Relative][Teller].PScore = 0;
      snprintf(HighScores[Fixed][Teller].PName, sizeof(HighScores[0][0].PName), "%s", "joyrider");
      HighScores[Fixed][Teller].PScore = 0;
    }
  }
}

void SaveHighScores() {
  FILE *FHighScores;
  FHighScores = fopen(DATA_DIR "/highscore.dat", "wb");
  if (FHighScores) {
    fwrite(HighScores[Fixed], sizeof(HighScores[Fixed][0]), 10, FHighScores);
    fwrite(HighScores[Relative], sizeof(HighScores[Relative][0]), 10, FHighScores);
    fclose(FHighScores);
  }
}

void MenuMusic() {
  if (MusicCount > 0)
    if (!MusicPlaying()) {
      SelectedMusic = 0;
      PlayMusic(SelectedMusic);
    }
}

void GameMusic() {
  if (!MusicPlaying()) {
    if (MusicCount > 1) {
      SelectedMusic = 1 + rand() % (MusicCount - 1);
      PlayMusic(SelectedMusic);
    } else if (MusicCount == 1) {
      SelectedMusic = 0;
      PlayMusic(SelectedMusic);
    }
  }
}

uint8_t FadeAlpha(uint8_t alpha, uint32_t AlphaTimer) {
  if (alpha < MaxAlpha) {
    double Value = trunc(MaxAlpha * ((double)(GetTicks() - AlphaTimer) / MaxAlphaTime));
    alpha = (Value > MaxAlpha) ? MaxAlpha : (uint8_t)Value;
    if (alpha + AlphaIncrease >= MaxAlpha)
      alpha = 255;
  }
  return alpha;
}
