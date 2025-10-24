# 🎮 Binary Quest - Enhanced Edition v2.0

An educational game to learn binary number conversion through interactive gameplay!

## ✨ What's New in v2.0

### Multiple Conversion Types!
- **Decimal to Binary** - Convert regular numbers (e.g., 42 → 101010)
- **Octal to Binary** - Convert octal numbers (e.g., 52 → 101010)
- **Hexadecimal to Binary** - Convert hex numbers (e.g., 2A → 101010)

Choose your conversion type when starting a new game!

## 🎮 Quick Start

### **Cross-Platform Launcher (Recommended)**

#### **Python Launcher (Works on All Platforms)**
```bash
# Windows, Linux, macOS
python launch.py
```
or
```bash
python3 launch.py
```

#### **Platform-Specific Launchers**

**On Windows:**
```cmd
launch.bat
```
or double-click `launch.bat`

**On Linux/macOS:**
```bash
./launch.sh
```
or
```bash
bash launch.sh
```

---

## 📦 What's Included

### **Executables**
- **`src/BinaryGame.exe`** - Windows GUI version
- **`src/BinaryQuestGUI`** - Linux/macOS GUI version
- **`src/BinaryQuest`** - Linux/macOS console version

### **Launchers**
- **`launch.py`** - Universal Python launcher (all platforms)
- **`launch.bat`** - Windows batch launcher
- **`launch.sh`** - Linux/macOS shell launcher
- **`demo.sh`** - Interactive demo menu (Linux/macOS)
- **`run_gui.sh`** - Simple GUI launcher (Linux/macOS)

### **Assets**
- **`src/sounds/`** - Sound effects and music
  - `background.mp3` - Background music
  - `gamestart.mp3` - Game start sound
  - `gameover.mp3` - Game over sound
  - `pop.mp3` - Bit collection sound

---

## 🎯 How to Play

1. **Launch the game** using one of the methods above
2. **Click "Start New Game"** from the main menu
3. **Choose conversion type**:
   - Decimal to Binary (for numbers like 42)
   - Octal to Binary (for octal like 52)
   - Hexadecimal to Binary (for hex like 2A)
4. **Enter a number** based on your chosen type
5. **Watch it convert to binary** automatically
6. **Catch the falling bits** (0s and 1s) in the correct order
7. **Collect power-ups** for special abilities
8. **Avoid collecting 3 wrong bits** or it's game over!

### **Controls**
- **A / D** or **Arrow Keys** - Move collector left/right
- **SPACE** - Pause game
- **Q** - Quit game
- **ESC** - Go back in menus
- **Mouse** - Click menu buttons

---

## 🌟 Features

### **Educational**
- ✅ **Multiple conversion types** (Decimal, Octal, Hexadecimal to Binary)
- ✅ Interactive binary conversion learning
- ✅ Visual representation of binary numbers
- ✅ Real-time feedback on correct/incorrect catches
- ✅ Progressive difficulty with increasing number ranges
- ✅ Smart input validation for each conversion type

### **Gameplay**
- ✅ Beautiful SDL2 graphics with colors
- ✅ Power-ups (Speed Boost, Score Multiplier, Slow Time)
- ✅ Lives system (3 lives)
- ✅ Pause/Resume functionality
- ✅ High score tracking
- ✅ Particle effects

### **Audio**
- ✅ Background music
- ✅ Sound effects for actions
- ✅ Game start/over sounds

### **Interface**
- ✅ Main menu with mouse/keyboard support
- ✅ Instructions screen
- ✅ Number input screen
- ✅ Game over screen with statistics

---

## 🔧 Building from Source

### **Prerequisites**

**Linux/macOS:**
```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev gcc make

# macOS (with Homebrew)
brew install sdl2 sdl2_ttf sdl2_mixer
```

**Windows:**
- MinGW-w64 or Visual Studio
- SDL2, SDL2_ttf, SDL2_mixer development libraries

### **Build Commands**

```bash
cd src

# Build both versions
make all

# Build GUI version only
make gui

# Build console version only
make console

# Clean build files
make clean

# Install dependencies (Ubuntu/Debian)
make install-deps
```

---

## 🖥️ Platform Support

| Platform | GUI Version | Console Version | Launcher |
|----------|-------------|-----------------|----------|
| **Windows** | ✅ BinaryGame.exe | ❌ | launch.bat, launch.py |
| **Linux** | ✅ BinaryQuestGUI | ✅ BinaryQuest | launch.sh, launch.py |
| **macOS** | ✅ BinaryQuestGUI* | ✅ BinaryQuest* | launch.sh, launch.py |

*Requires recompilation with SDL2 installed

---

## 📁 Project Structure

```
Binary Quest/
├── src/
│   ├── BinaryGame.exe          # Windows executable
│   ├── BinaryQuestGUI          # Linux/macOS GUI executable
│   ├── BinaryQuest             # Linux/macOS console executable
│   ├── sounds/                 # Audio files
│   │   ├── background.mp3
│   │   ├── gamestart.mp3
│   │   ├── gameover.mp3
│   │   └── pop.mp3
│   ├── main.c                  # Console version entry point
│   ├── gui_main.c              # GUI version entry point
│   ├── game.c/h                # Core game logic
│   ├── gui_game.c/h            # GUI game logic
│   ├── binary.c/h              # Binary conversion
│   ├── sound.c/h               # Audio system
│   ├── input.c/h               # Input handling
│   ├── draw.c/h                # Console rendering
│   └── Makefile                # Build configuration
├── launch.py                   # Universal Python launcher
├── launch.bat                  # Windows launcher
├── launch.sh                   # Linux/macOS launcher
├── demo.sh                     # Interactive demo menu
├── run_gui.sh                  # Simple GUI launcher
├── HOW_TO_RUN.md              # Detailed running instructions
└── README.md                   # This file
```

---

## 🎓 Educational Use

Binary Quest is designed for:
- **Computer Science Education** - Teaching binary number systems
- **High School/University Courses** - Introduction to computing
- **Self-Learning** - Interactive way to understand binary
- **Coding Bootcamps** - Supplementary learning material

### **Learning Outcomes**
- Understand multiple number systems (Decimal, Octal, Hexadecimal)
- Master conversion to binary from different bases
- Visualize binary representation
- Improve retention through gamification
- Practice pattern recognition and quick thinking
- Learn the relationship between different number systems

---

## 🐛 Troubleshooting

### **"Permission denied" error (Linux/macOS)**
```bash
chmod +x launch.sh launch.py
chmod +x src/BinaryQuestGUI src/BinaryQuest
```

### **Missing sound files warnings**
The game will still work without sound files. To fix:
- Ensure `sounds/` folder is in `src/` directory
- Check that MP3 files exist: `background.mp3`, `gamestart.mp3`, `gameover.mp3`, `pop.mp3`

### **SDL2 library errors (Linux)**
```bash
# Install SDL2 libraries
sudo apt-get update
sudo apt-get install libsdl2-2.0-0 libsdl2-ttf-2.0-0 libsdl2-mixer-2.0-0

# Check what's missing
ldd src/BinaryQuestGUI
```

### **GUI doesn't appear (Linux)**
- Ensure you have a display server (X11 or Wayland)
- Don't run over SSH without X forwarding
- Check SDL2 installation: `sdl2-config --version`

### **Python launcher doesn't work**
```bash
# Make sure Python 3 is installed
python3 --version

# Run with full path
python3 /path/to/launch.py
```

---

## 🚀 Advanced Usage

### **Running Specific Versions**

**GUI Version (Direct):**
```bash
cd src
./BinaryQuestGUI          # Linux/macOS
BinaryGame.exe            # Windows
```

**Console Version (Direct):**
```bash
cd src
./BinaryQuest
```

### **Custom Build Options**

Edit `src/Makefile` to customize:
- Compiler flags
- Optimization levels
- SDL2 library paths
- Installation directories

---

## 📝 Game Mechanics

### **Binary Conversion**
- **Choose conversion type**: Decimal, Octal, or Hexadecimal
- **Decimal mode**: Enter numbers 0-9 (e.g., 42)
- **Octal mode**: Enter digits 0-7 (e.g., 52 for octal)
- **Hexadecimal mode**: Enter 0-9, A-F (e.g., 2A for hex)
- Game converts your input to binary representation
- Bits fall from top to bottom in the correct order

### **Scoring**
- **Correct bit catch**: +10 points
- **Power-up collection**: Bonus points
- **Combo multiplier**: Consecutive catches increase score

### **Power-ups**
- **🟡 Speed Boost (S)**: Move faster temporarily
- **🟠 Score Multiplier (M)**: Double points for a duration
- **🟣 Slow Time (T)**: Slow down falling bits

### **Wrong Bit System**
- You can collect up to 3 wrong bits
- Collecting the wrong bit in sequence counts as a mistake
- Visual feedback shows which bit you need next
- Game over after 3 wrong bits collected
- Missed bits (that fall past you) don't count as wrong

---

## 🤝 Contributing

This is an educational project. Feel free to:
- Report bugs
- Suggest features
- Improve documentation
- Add new game modes
- Create translations

---

## 📄 License

Educational project - Free to use for learning purposes.

---

## 🎮 Credits

**Developed as an educational tool for learning:**
- Binary number systems
- C programming
- SDL2 game development
- Modular software architecture

---

## 📞 Support

For issues or questions:
1. Check `HOW_TO_RUN.md` for detailed instructions
2. Review troubleshooting section above
3. Ensure all dependencies are installed
4. Try rebuilding from source

---

**Enjoy learning binary with Binary Quest! 🎯✨**
