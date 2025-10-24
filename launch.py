#!/usr/bin/env python3
"""
Binary Quest - Cross-Platform Launcher
Works on Windows, Linux, and macOS
"""

import os
import sys
import platform
import subprocess

def clear_screen():
    """Clear the terminal screen"""
    os.system('cls' if os.name == 'nt' else 'clear')

def print_banner():
    """Print the game banner"""
    print("=" * 50)
    print("   Binary Quest - Enhanced Edition")
    print("=" * 50)
    print()

def get_executable():
    """Determine which executable to run based on platform"""
    system = platform.system()
    
    if system == "Windows":
        exe_path = os.path.join("src", "BinaryGame.exe")
        exe_name = "BinaryGame.exe"
    elif system in ["Linux", "Darwin"]:  # Darwin is macOS
        exe_path = os.path.join("src", "BinaryQuestGUI")
        exe_name = "BinaryQuestGUI"
    else:
        return None, None, f"Unsupported platform: {system}"
    
    return exe_path, exe_name, None

def main():
    """Main launcher function"""
    clear_screen()
    print_banner()
    
    # Get the correct executable for this platform
    exe_path, exe_name, error = get_executable()
    
    if error:
        print(f"❌ ERROR: {error}")
        sys.exit(1)
    
    # Check if executable exists
    if not os.path.exists(exe_path):
        print(f"❌ ERROR: {exe_name} not found!")
        print()
        print("Please build the game first:")
        if platform.system() == "Windows":
            print("  1. Open Visual Studio or MinGW")
            print("  2. Build the Windows version")
        else:
            print("  cd src")
            print("  make gui")
        print()
        sys.exit(1)
    
    # Make executable on Unix-like systems
    if platform.system() != "Windows":
        try:
            os.chmod(exe_path, 0o755)
        except:
            pass
    
    # Run the game
    print(f"🎮 Starting {exe_name}...")
    print()
    print("Controls:")
    print("  • A/D or Arrow Keys - Move left/right")
    print("  • SPACE - Pause")
    print("  • Q - Quit")
    print()
    print("-" * 50)
    print()
    
    try:
        # Change to src directory and run
        original_dir = os.getcwd()
        os.chdir("src")
        
        if platform.system() == "Windows":
            subprocess.run([exe_name])
        else:
            subprocess.run([f"./{exe_name}"])
        
        os.chdir(original_dir)
        
    except KeyboardInterrupt:
        print("\n\n⚠️  Game interrupted by user")
    except Exception as e:
        print(f"\n\n❌ ERROR running game: {e}")
        sys.exit(1)
    
    print()
    print("=" * 50)
    print("Thanks for playing Binary Quest! 🎮")
    print("=" * 50)

if __name__ == "__main__":
    main()
