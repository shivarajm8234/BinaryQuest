#!/bin/bash
# Simple Windows build script

echo "Building Windows .exe..."
cd src

# Try to build with MinGW
x86_64-w64-mingw32-gcc -Wall -Wextra -std=c99 \
    -o BinaryGame.exe \
    gui_main.c gui_game.c binary.c sound.c \
    -I/usr/x86_64-w64-mingw32/include/SDL2 \
    -L/usr/x86_64-w64-mingw32/lib \
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer \
    -mwindows

if [ $? -eq 0 ]; then
    echo "✅ SUCCESS! BinaryGame.exe created"
    ls -lh BinaryGame.exe
else
    echo "❌ Build failed. Installing SDL2 libraries for Windows..."
    echo "This requires downloading SDL2 development libraries for MinGW"
fi
