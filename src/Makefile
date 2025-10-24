CC=gcc
CFLAGS=-Wall -Wextra -std=c99
CONSOLE_TARGET=BinaryQuest
GUI_TARGET=BinaryQuestGUI
CONSOLE_SOURCES=main.c game.c binary.c draw.c input.c
GUI_SOURCES=gui_main.c gui_game.c binary.c sound.c
SDL_LIBS=-lSDL2 -lSDL2_ttf -lSDL2_mixer -lm

# Windows cross-compilation settings
WIN_CC=x86_64-w64-mingw32-gcc
WIN_CFLAGS=-Wall -Wextra -std=c99 -D_WIN32
WIN_GUI_TARGET=BinaryGame.exe
WIN_SDL_LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -mwindows

# Default target builds both versions
all: $(CONSOLE_TARGET) $(GUI_TARGET)

# Console version (original)
$(CONSOLE_TARGET): $(CONSOLE_SOURCES)
	$(CC) $(CFLAGS) -o $(CONSOLE_TARGET) $(CONSOLE_SOURCES)

# GUI version (enhanced)
$(GUI_TARGET): $(GUI_SOURCES)
	$(CC) $(CFLAGS) -o $(GUI_TARGET) $(GUI_SOURCES) $(SDL_LIBS)

# Individual targets
console: $(CONSOLE_TARGET)

gui: $(GUI_TARGET)

# Windows cross-compilation target
windows: $(WIN_GUI_TARGET)

$(WIN_GUI_TARGET): $(GUI_SOURCES)
	@echo "Building Windows executable..."
	@echo "Note: Requires MinGW-w64 and Windows SDL2 libraries"
	$(WIN_CC) $(WIN_CFLAGS) -o $(WIN_GUI_TARGET) $(GUI_SOURCES) $(WIN_SDL_LIBS)

# Install SDL2 dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev

# Install Windows cross-compilation tools
install-mingw:
	@echo "Installing MinGW-w64 cross-compiler..."
	sudo apt-get update
	sudo apt-get install -y mingw-w64 gcc-mingw-w64-x86-64
	@echo ""
	@echo "MinGW installed! Now you need Windows SDL2 libraries."
	@echo "Download from: https://github.com/libsdl-org/SDL/releases"
	@echo "Extract to: /usr/x86_64-w64-mingw32/"

# Clean all targets
clean:
	rm -f $(CONSOLE_TARGET) $(GUI_TARGET) $(WIN_GUI_TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build both console and GUI versions (Linux)"
	@echo "  console      - Build console version only (Linux)"
	@echo "  gui          - Build GUI version only (Linux)"
	@echo "  windows      - Build Windows .exe (requires MinGW)"
	@echo "  install-deps - Install SDL2 dependencies (Linux)"
	@echo "  install-mingw - Install MinGW cross-compiler"
	@echo "  clean        - Remove all built files"
	@echo "  help         - Show this help"

.PHONY: all console gui windows clean install-deps install-mingw help
