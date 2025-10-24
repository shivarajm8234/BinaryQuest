# How to Run Binary Quest

## ⚠️ Important Note About Executables

- **`BinaryGame.exe`** - Windows executable (won't work on Linux)
- **`BinaryQuest`** - Linux console version ✅
- **`BinaryQuestGUI`** - Linux GUI version ✅

## 🎮 Running the Game

### **Method 1: Using the Demo Script (Recommended)**
```bash
./demo.sh
```
Then choose option **1** to play the GUI version.

### **Method 2: Quick GUI Launch**
```bash
./run_gui.sh
```

### **Method 3: Direct Execution**
```bash
cd src
./BinaryQuestGUI
```

### **Method 4: Console Version**
```bash
cd src
./BinaryQuest
```

## 🔧 If GUI Doesn't Start

### Check SDL2 Dependencies
```bash
ldd src/BinaryQuestGUI
```

### Install Missing Dependencies (if needed)
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### Rebuild if Necessary
```bash
cd src
make clean
make gui
```

## 🎯 Game Controls

### GUI Version
- **Mouse**: Click menu buttons
- **A/D or Arrow Keys**: Move collector left/right
- **SPACE**: Pause game
- **Q**: Quit game
- **ESC**: Go back in menus

### Console Version
- **A/D**: Move collector left/right
- **Q**: Quit game

## 🎵 Sound

The GUI version includes:
- Background music (`sounds/background.mp3`)
- Game start sound (`sounds/gamestart.mp3`)
- Game over sound (`sounds/gameover.mp3`)

Make sure the `sounds/` directory is in the `src/` folder.

## 📝 Gameplay

1. Enter a number (e.g., 42)
2. The number is converted to binary
3. Catch the falling bits (0s and 1s) in order
4. Collect power-ups for bonuses
5. Don't miss 3 bits or it's game over!

## 🐧 Platform Support

- **Linux**: ✅ Full support (both console and GUI)
- **Windows**: ❌ Use BinaryGame.exe (not included in Linux build)
- **macOS**: ⚠️ Requires recompilation with SDL2

## 🔨 Building from Source

```bash
cd src

# Build both versions
make all

# Or build individually
make console    # Console version only
make gui        # GUI version only
```

## 💡 Troubleshooting

### "Permission denied" error
```bash
chmod +x demo.sh run_gui.sh
chmod +x src/BinaryQuest src/BinaryQuestGUI
```

### "No such file or directory" for sounds
```bash
# Make sure you're in the project root, not inside src/
cd "/home/kiyotoka/Downloads/Binary Quest(EL Project)"
./run_gui.sh
```

### GUI window doesn't appear
- Check if you have a display server running (X11 or Wayland)
- Try running from terminal, not SSH
- Check SDL2 installation: `sdl2-config --version`

---

**Enjoy playing Binary Quest! 🎮✨**
