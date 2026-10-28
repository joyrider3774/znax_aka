# Znax for the Gamebuino AKA

A port of the SDL2 version of [Znax](https://github.com/joyrider3774/Znax) to the Gamebuino AKA.
Znax is a remake of a game by Nick Kouvaris: select 4 blocks of the same colour as the corners
of a rectangle, as big as you can, to clear every block inside it and score points before the
time runs out.

- the two game modes: Fixed Timer (10 minutes, score as much as you can) and Relative Timer
  (clearing blocks also adds time)
- high scores for both modes
- the skins of the SDL2 version (newblocks, numbers, portmaster, red), switched while playing
- the graphics, sounds and title music of the SDL2 version, at the same 320x240 the AKA's
  screen has

## Controls

| AKA | Action |
|---|---|
| D-pad / joystick | move in the menus, move the cursor |
| A | select a menu option, select a block |
| B | back, quit the game to the title screen |
| MENU | quit to the title screen; on the title screen: quit |
| L1 / R1 | volume down / up |
| C | next skin |
| D | next music track |

Entering a high score name: up and down change the letter, left and right move between
letters, A confirms and MENU cancels. To erase a letter choose the space.

**Quit** in the main menu goes back to the launcher; RUN switches the AKA off as in every game.

## Installing

Copy the `SD_files/ZNAX` folder to the root of the SD card and start Znax from the launcher.
The folder holds `firmware.bin`, `meta.json`, `screen.bmp`, the music, the skins and the
licences. The game writes its high scores (`highscore.dat`) and its volume and skin
(`settings.dat`) there.

Extra music: any 44100 Hz mono 16-bit WAV file in `ZNAX/music` is played during the game,
`title.wav` in the menus.

## Building

ESP-IDF 5.5 and the official [Gamebuino AKA library](https://github.com/jmp42/Gamebuino_AKA_lib),
which is used where it is checked out (`C:/github/Gamebuino_AKA_lib` by default, or set
`AKA_LIB_DIR`):

```
idf.py set-target esp32s3
idf.py build
```

`build/znax.bin` is copied to `SD_files/ZNAX/firmware.bin` by the build.

The graphics, skins, fonts, sounds and music are the SDL2 version's original files in
`assets/`, converted by `tools/convert_assets.py` (Pillow, and ffmpeg for the music):

```
python tools/convert_assets.py <ffmpeg>
```

It writes `main/assets/assets.bin` (the default skin, the fonts and the sounds, built into the
firmware) and the SD card folder's skins, music and `screen.bmp`. A skin on the card is a folder
in `ZNAX/skins` with a `.bin` file for each image it changes, the default image is used for the
others.

## Source layout

| File | What it is |
|---|---|
| `main/main.cpp` | start up and the state loop |
| `main/intro.cpp`, `titlescreen.cpp`, `gametypemenu.cpp`, `credits.cpp`, `readygo.cpp`, `game.cpp`, `timeover.cpp`, `gethighscorename.cpp`, `showhighscores.cpp` | one file per game state |
| `main/gamefuncs.cpp`, `commonvars.cpp` | what the states share: volume, settings, skins, high scores, music, the globals |
| `main/CBlock.cpp`, `CWorldParts.cpp`, `CSelector.cpp`, `CMainMenu.cpp`, `CGameTypeMenu.cpp` | the game's own classes |
| `main/gfx.cpp`, `sound.cpp`, `input.cpp`, `platform.cpp` | what SDL2 did, on the AKA library |

## Credits

Original (flash) game by Nick Kouvaris. GP2X and SDL2 versions and this port by Willems Davy
(Willems Soft). Music by Donskeeto. Roboto fonts by Christian Robertson (Google).

## License

MIT, see [LICENSE](LICENSE). The Roboto fonts are under the Apache License 2.0, see
[assets/fonts/LICENSE.Roboto.font.txt](assets/fonts/LICENSE.Roboto.font.txt).
