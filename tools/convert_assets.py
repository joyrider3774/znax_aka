#!/usr/bin/env python3
"""Converts the assets of the SDL2 version of Znax into what the Gamebuino AKA build uses.

    python tools/convert_assets.py <ffmpeg.exe>

Reads the original files from assets/ (graphics/, skins/, sound/, music/title.ogg and fonts/).
Writes:
  main/assets/assets.bin   the default skin, the fonts and the sound effects, in the LCD's own
                           pixel format, built into the firmware
  main/assets/assets.h     where each of them sits in assets.bin
  SD_files/ZNAX/           the folder that goes on the SD card: the other skins, the music,
                           meta.json's screen.bmp

Images become 16 bit pixels in the order the AKA's panel takes them (lcd_color_rgb in the
library: red in the low bits), with an 8 bit alpha plane when the image is not fully opaque.
The images the game gave a colour key get alpha 0 where they are magenta (255,0,255).

The skins are the same images, one file each in SD_files/ZNAX/skins/<skin>/<image>.bin:
a 4 byte header (width and height, 16 bit little endian), a byte that is 1 when an alpha
plane follows, 3 bytes of padding, the pixels and then the alpha plane.

The TrueType fonts are rendered once here at the sizes the game opened them at, into an alpha
bitmap per character, the AKA draws those in any colour.

Sounds are resampled to 44100 Hz mono 16 bit, the only format the library's WAV player takes;
the music is decoded from title.ogg by ffmpeg and written with the plain 44 byte WAV header
the library's player checks for.
"""
import os
import struct
import subprocess
import sys

from PIL import Image, ImageDraw, ImageFont

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIR = os.path.join(ROOT, "assets")
OUT_DIR = os.path.join(ROOT, "main", "assets")
SD_DIR = os.path.join(ROOT, "SD_files", "ZNAX")

# name in the code, file, whether the game set a magenta colour key on it (LoadGraphics)
IMAGES = [
    ("HIGHSCORES", "highscores", False),
    ("FIXEDTIMER1", "fixedtimer1", True),
    ("RELATIVETIMER1", "relativetimer1", True),
    ("FIXEDTIMER2", "fixedtimer2", True),
    ("RELATIVETIMER2", "relativetimer2", True),
    ("SELECTGAME", "selectgame", True),
    ("CREDITS", "credits", True),
    ("TITLESCREEN", "titlescreen", True),
    ("PLAY1", "play1", True),
    ("PLAY2", "play2", True),
    ("QUIT1", "quit1", True),
    ("QUIT2", "quit2", True),
    ("CREDITS2", "credits2", True),
    ("CREDITS1", "credits1", True),
    ("HIGHSCORES2", "highscores2", True),
    ("HIGHSCORES1", "highscores1", True),
    ("INTRO1", "intro1", False),
    ("INTRO2", "intro2", False),
    ("INTRO3", "intro3", False),
    ("TIMEOVER", "timeover", False),
    ("READY", "ready", False),
    ("GO", "go", False),
    ("CURSOR", "cursor", True),
    ("BLOCKS", "blocks", True),
    ("BACKGROUND", "background", False),
]

# name in the code, file, size the game opened it at (TTF_OpenFont)
FONTS = [
    ("FONT", "Roboto-Regular.ttf", 11),
    ("BIGFONT", "Roboto-Regular.ttf", 16),
    ("MONOFONT", "RobotoMono-Bold.ttf", 12),
]

# same order as the SND_ constants of the game
SOUNDS = ["readygo", "fiveminute", "oneminute", "three", "two", "one", "timeover", "menu", "select",
          "welcome", "goodbye", "delete", "blockselect", "error"]


def lcd_color(r, g, b):
    return (r >> 3) | ((g >> 2) << 5) | ((b >> 3) << 11)


class Blob:
    def __init__(self):
        self.data = bytearray()

    def align(self):
        while len(self.data) % 4:
            self.data.append(0)

    def add(self, raw):
        self.align()
        offset = len(self.data)
        self.data += raw
        return offset


def surface_planes(path, keyed):
    """pixels and alpha plane (None when opaque) of an image"""
    rgba = Image.open(path).convert("RGBA")
    w, h = rgba.size
    pixels = bytearray()
    alpha = bytearray()
    opaque = True
    for r, g, b, a in rgba.getdata():
        if keyed and (r, g, b) == (255, 0, 255):
            a = 0
        if a != 255:
            opaque = False
        pixels += struct.pack("<H", lcd_color(r, g, b) if a else 0)
        alpha.append(a)
    return w, h, bytes(pixels), (None if opaque else bytes(alpha))


def font_glyphs(blob, path, size):
    font = ImageFont.truetype(path, size)
    ascent, descent = font.getmetrics()
    height = ascent + descent
    entries = []
    for code in range(32, 127):
        ch = chr(code)
        advance = int(round(font.getlength(ch)))
        left, _, right, _ = font.getbbox(ch, anchor="la")
        x0 = min(0, left)
        gw = max(advance, right) - x0
        if gw <= 0 or ch == " ":
            entries.append("{%d, 0, 0, -1}" % advance)
            continue
        img = Image.new("L", (gw, height), 0)
        ImageDraw.Draw(img).text((-x0, 0), ch, font=font, fill=255, anchor="la")
        off = blob.add(img.tobytes())
        entries.append("{%d, %d, %d, %d}" % (advance, gw, x0, off))
    return height, font.font.height, entries


def ffmpeg_pcm(ffmpeg, path):
    """the file as 44100 Hz mono signed 16 bit samples"""
    return subprocess.run([ffmpeg, "-hide_banner", "-loglevel", "error", "-i", path,
                           "-ac", "1", "-ar", "44100", "-f", "s16le", "-acodec", "pcm_s16le", "-"],
                          check=True, stdout=subprocess.PIPE).stdout


def write_wav(path, pcm):
    with open(path, "wb") as f:
        f.write(struct.pack("<4sI4s4sIHHIIHH4sI", b"RIFF", 36 + len(pcm), b"WAVE", b"fmt ", 16, 1, 1,
                            44100, 88200, 2, 16, b"data", len(pcm)))
        f.write(pcm)


def write_bmp(path, im):
    """24 bit BMP, what the launcher shows next to the game's name"""
    im = im.convert("RGB")
    w, h = im.size
    row = (w * 3 + 3) & ~3
    with open(path, "wb") as f:
        f.write(struct.pack("<2sIHHI", b"BM", 54 + row * h, 0, 0, 54))
        f.write(struct.pack("<IiiHHIIiiII", 40, w, h, 1, 24, 0, row * h, 2835, 2835, 0, 0))
        for y in range(h - 1, -1, -1):
            line = bytearray()
            for x in range(w):
                r, g, b = im.getpixel((x, y))
                line += bytes((b, g, r))
            line += b"\0" * (row - w * 3)
            f.write(line)


def write_skin_image(path, w, h, pixels, alpha):
    with open(path, "wb") as f:
        f.write(struct.pack("<HHB3x", w, h, 1 if alpha else 0))
        f.write(pixels)
        if alpha:
            f.write(alpha)


def main():
    if len(sys.argv) != 2:
        print(__doc__)
        sys.exit(1)
    ffmpeg = sys.argv[1]
    gfx = os.path.join(SRC_DIR, "graphics")
    os.makedirs(OUT_DIR, exist_ok=True)
    blob = Blob()
    h = ["// Generated by tools/convert_assets.py from the SDL2 version's graphics, fonts and sounds.",
         "// Offsets are into assets.bin (dm_assets), -1 = not there.", "#pragma once", "#include <stdint.h>", "",
         "struct AssetSurface { int16_t w, h; int32_t pixels, alpha; };",
         "struct AssetGlyph { int8_t advance; int8_t w; int8_t x; int32_t alpha; };",
         "struct AssetFont { int16_t height, lineSkip; const AssetGlyph* glyphs; };",
         "struct AssetSound { int32_t samples; uint32_t count; };", ""]

    # the default skin, in the firmware
    names = []
    for name, file, keyed in IMAGES:
        w, hh, px, a = surface_planes(os.path.join(gfx, file + ".png"), keyed)
        pxo = blob.add(px)
        ao = blob.add(a) if a else -1
        h.append("static const AssetSurface ASSET_%s = {%d, %d, %d, %d};" % (name, w, hh, pxo, ao))
        names.append('"%s"' % file)
    h.append("")
    h.append("// the file names of the images, a skin on the SD card has <name>.bin for those it changes")
    h.append("static const char* const ASSET_IMAGE_NAMES[] = {%s};" % ", ".join(names))
    h.append("")

    for name, file, size in FONTS:
        height, line_skip, glyphs = font_glyphs(blob, os.path.join(SRC_DIR, "fonts", file), size)
        h.append("// %s at %d pixels, characters 32 to 126" % (file, size))
        h.append("static const AssetGlyph %s_GLYPHS[] = {\n    %s};" % (name, ",\n    ".join(glyphs)))
        h.append("static const AssetFont ASSET_%s = {%d, %d, %s_GLYPHS};" % (name, height, line_skip, name))
        h.append("")

    sounds = []
    for s in SOUNDS:
        pcm = ffmpeg_pcm(ffmpeg, os.path.join(SRC_DIR, "sound", s + ".wav"))
        sounds.append("{%d, %d}" % (blob.add(pcm), len(pcm) // 2))
    h.append("static const AssetSound ASSET_SOUNDS[] = {%s};" % ", ".join(sounds))
    h.append("")

    with open(os.path.join(OUT_DIR, "assets.bin"), "wb") as f:
        f.write(blob.data)
    with open(os.path.join(OUT_DIR, "assets.h"), "w", newline="\n") as f:
        f.write("\n".join(h))
    print("assets.bin: %d bytes" % len(blob.data))

    # the other skins, on the SD card
    skins_src = os.path.join(SRC_DIR, "skins")
    for skin in sorted(os.listdir(skins_src)):
        src = os.path.join(skins_src, skin)
        if not os.path.isdir(src):
            continue
        dst = os.path.join(SD_DIR, "skins", skin)
        os.makedirs(dst, exist_ok=True)
        for name, file, keyed in IMAGES:
            png = os.path.join(src, file + ".png")
            if os.path.exists(png):
                write_skin_image(os.path.join(dst, file + ".bin"), *surface_planes(png, keyed))
        print("skin %s" % skin)

    os.makedirs(os.path.join(SD_DIR, "music"), exist_ok=True)
    write_wav(os.path.join(SD_DIR, "music", "title.wav"), ffmpeg_pcm(ffmpeg, os.path.join(SRC_DIR, "music", "title.ogg")))
    write_bmp(os.path.join(SD_DIR, "screen.bmp"), Image.open(os.path.join(gfx, "titlescreen.png")).resize((160, 120), Image.LANCZOS))
    print("SD folder written to %s" % SD_DIR)


if __name__ == "__main__":
    main()
