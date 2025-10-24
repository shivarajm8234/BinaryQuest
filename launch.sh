#!/bin/bash
# Binary Quest Launcher for Linux/Mac

echo "========================================"
echo "   Binary Quest - Enhanced Edition"
echo "========================================"
echo ""
echo "Starting GUI version..."
echo ""

cd src

if [ -f "./BinaryQuestGUI" ]; then
    ./BinaryQuestGUI
else
    echo "ERROR: BinaryQuestGUI not found!"
    echo "Please build the Linux version first:"
    echo "  cd src && make gui"
    exit 1
fi

cd ..
echo ""
echo "Thanks for playing!"
