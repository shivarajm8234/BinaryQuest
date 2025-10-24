#!/bin/bash

# Binary Quest Enhanced Edition - Demo Script
echo "Binary Quest Enhanced Edition - GUI Version"
echo "============================================"
echo ""
echo "Welcome to the enhanced GUI version of Binary Quest!"
echo ""
echo " Project Structure:"
echo "   src/BinaryQuestGUI  - Enhanced GUI version"
echo ""
echo "Available Options:"
echo ""
echo "1) Play GUI Version"
echo "2) Show Help"
echo "3) Exit"
echo ""

while true; do
    read -p "Choose an option (1-3): " choice
    
    case $choice in
        1)
            echo ""
            echo "Starting Binary Quest GUI..."
            echo "   A beautiful windowed game with:"
            echo "   • Colorful graphics"
            echo "   • Power-ups (Speed, Score Multiplier, Slow Time)"
            echo "   • Interactive menus"
            echo "   • Lives system"
            echo "   Controls: A/D or Arrow Keys, SPACE to pause, Q to quit"
            echo ""
            (cd src && ./BinaryQuestGUI)
            echo ""
            ;;
        2)
            echo ""
            echo " Binary Quest Enhanced Edition Help"
            echo "====================================="
            echo ""
            echo "How to Play:"
            echo "• Enter a number to convert to binary"
            echo "• Catch falling bits (1s and 0s) IN CORRECT ORDER"
            echo "• Green bits = 1, Red bits = 0"
            echo "• Collect power-ups for special abilities"
            echo "• WARNING: Game ends after 3 wrong bits!"
            echo ""
            echo " GUI Version Features:"
            echo "• Beautiful SDL2 graphics"
            echo "• Power-ups: Speed Boost, Score Multiplier, Slow Time"
            echo "• Lives system (3 lives)"
            echo "• Interactive menus"
            echo "• Pause functionality"
            echo "• Real-time statistics"
            echo ""
            echo " Technical Details:"
            echo "• SDL2-powered modern game engine"
            echo "• Cross-platform Linux support"
            echo "• Educational binary conversion game"
            echo ""
            ;;
        3)
            echo ""
            echo " Thanks for playing Binary Quest Enhanced Edition!"
            echo "   Have fun catching those binary bits!"
            exit 0
            ;;
        *)
            echo " Invalid option. Please choose 1-3."
            ;;
    esac
done


