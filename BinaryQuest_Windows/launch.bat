@echo off
REM Binary Quest Launcher for Windows
echo ========================================
echo    Binary Quest - Enhanced Edition
echo ========================================
echo.
echo Starting GUI version...
echo.

cd src
if exist BinaryGame.exe (
    BinaryGame.exe
) else (
    echo ERROR: BinaryGame.exe not found!
    echo Please build the Windows version first.
    pause
)

cd ..
echo.
echo Thanks for playing!
pause
