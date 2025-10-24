#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "gui_game.h"
#include "sound.h"

// Helper function to parse hexadecimal string to integer
int parseHexString(const char* hexStr) {
    int result = 0;
    int len = strlen(hexStr);
    
    for (int i = 0; i < len; i++) {
        result *= 16;
        char c = hexStr[i];
        if (c >= '0' && c <= '9') {
            result += c - '0';
        } else if (c >= 'A' && c <= 'F') {
            result += c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            result += c - 'a' + 10;
        }
    }
    
    return result;
}

// Menu states
typedef enum {
    MENU_MAIN,
    MENU_CONVERSION_SELECT,
    MENU_INPUT,
    MENU_GAME,
    MENU_QUIT
} MenuState;

typedef struct {
    MenuState currentMenu;
    int inputNumber;
    char inputBuffer[20];
    int inputLength;
    bool numberEntered;
    ConversionType conversionType;
} MenuSystem;

void renderMainMenu(SDL_Renderer* renderer, TTF_Font* font, MenuSystem* menu) {
    // Clear screen with gradient background
    setRenderColor(renderer, (Color){10, 10, 30, 255});
    SDL_RenderClear(renderer);

    // Draw title with glow effect
    renderText(renderer, font, "BINARY QUEST", WINDOW_WIDTH/2 - 120, 100, COLOR_CYAN);
    renderText(renderer, font, "Enhanced Edition", WINDOW_WIDTH/2 - 100, 140, COLOR_YELLOW);

    // Draw subtitle
    renderText(renderer, font, "Catch the falling binary bits!", WINDOW_WIDTH/2 - 150, 200, COLOR_WHITE);

    // Draw menu options
    renderButton(renderer, font, "Start New Game", WINDOW_WIDTH/2 - 100, 280, 200, 50, COLOR_BLUE, COLOR_WHITE);
    renderButton(renderer, font, "Instructions", WINDOW_WIDTH/2 - 100, 350, 200, 50, COLOR_GREEN, COLOR_WHITE);
    renderButton(renderer, font, "Quit", WINDOW_WIDTH/2 - 100, 420, 200, 50, COLOR_RED, COLOR_WHITE);

    // Draw version info
    renderText(renderer, font, "v2.0 - SDL2 Enhanced", 10, WINDOW_HEIGHT - 30, (Color){128, 128, 128, 255});
    
    SDL_RenderPresent(renderer);
}

void renderConversionSelectMenu(SDL_Renderer* renderer, TTF_Font* font, MenuSystem* menu) {
    // Clear screen with gradient background
    setRenderColor(renderer, (Color){10, 10, 30, 255});
    SDL_RenderClear(renderer);

    // Draw title
    renderText(renderer, font, "SELECT CONVERSION TYPE", WINDOW_WIDTH/2 - 150, 100, COLOR_CYAN);
    renderText(renderer, font, "Choose what to convert to Binary", WINDOW_WIDTH/2 - 150, 140, COLOR_WHITE);

    // Draw conversion type options
    renderButton(renderer, font, "Decimal to Binary", WINDOW_WIDTH/2 - 120, 200, 240, 50, COLOR_GREEN, COLOR_WHITE);
    renderButton(renderer, font, "Octal to Binary", WINDOW_WIDTH/2 - 120, 270, 240, 50, COLOR_BLUE, COLOR_WHITE);
    renderButton(renderer, font, "Hexadecimal to Binary", WINDOW_WIDTH/2 - 120, 340, 240, 50, COLOR_PURPLE, COLOR_WHITE);
    
    // Draw back button
    renderButton(renderer, font, "Back to Menu", WINDOW_WIDTH/2 - 100, 430, 200, 40, COLOR_RED, COLOR_WHITE);

    // Draw instructions
    renderText(renderer, font, "Click a button or press 1/2/3", WINDOW_WIDTH/2 - 140, 500, COLOR_YELLOW);
    
    SDL_RenderPresent(renderer);
}

void renderInputMenu(SDL_Renderer* renderer, TTF_Font* font, MenuSystem* menu) {
    setRenderColor(renderer, (Color){10, 10, 30, 255});
    SDL_RenderClear(renderer);

    // Show conversion type
    const char* conversionName = "";
    const char* inputHint = "";
    switch (menu->conversionType) {
        case CONVERSION_DECIMAL:
            conversionName = "Decimal to Binary";
            inputHint = "Enter a decimal number (e.g., 42)";
            break;
        case CONVERSION_OCTAL:
            conversionName = "Octal to Binary";
            inputHint = "Enter an octal number (e.g., 52 for octal)";
            break;
        case CONVERSION_HEXADECIMAL:
            conversionName = "Hexadecimal to Binary";
            inputHint = "Enter a hex number (e.g., 2A for 0x2A)";
            break;
    }
    
    renderText(renderer, font, conversionName, WINDOW_WIDTH/2 - 120, 150, COLOR_CYAN);
    renderText(renderer, font, "Enter a Number", WINDOW_WIDTH/2 - 100, 200, COLOR_YELLOW);
    renderText(renderer, font, inputHint, WINDOW_WIDTH/2 - 200, 240, COLOR_WHITE);

    // Draw input box
    SDL_Rect inputBox = {WINDOW_WIDTH/2 - 100, 300, 200, 40};
    setRenderColor(renderer, COLOR_WHITE);
    SDL_RenderDrawRect(renderer, &inputBox);
    
    setRenderColor(renderer, (Color){20, 20, 40, 255});
    SDL_RenderFillRect(renderer, &inputBox);

    // Draw input text
    if (menu->inputLength > 0) {
        renderText(renderer, font, menu->inputBuffer, WINDOW_WIDTH/2 - 90, 310, COLOR_WHITE);
    }

    // Draw cursor
    if ((SDL_GetTicks() / 500) % 2) {
        int cursorX = WINDOW_WIDTH/2 - 90 + menu->inputLength * 12;
        setRenderColor(renderer, COLOR_WHITE);
        SDL_RenderDrawLine(renderer, cursorX, 310, cursorX, 330);
    }

    renderText(renderer, font, "Press ENTER to start game", WINDOW_WIDTH/2 - 130, 380, COLOR_GREEN);
    renderText(renderer, font, "Press ESC to go back", WINDOW_WIDTH/2 - 110, 410, COLOR_YELLOW);

    SDL_RenderPresent(renderer);
}

void renderInstructions(SDL_Renderer* renderer, TTF_Font* font) {
    setRenderColor(renderer, (Color){10, 10, 30, 255});
    SDL_RenderClear(renderer);

    renderText(renderer, font, "HOW TO PLAY", WINDOW_WIDTH/2 - 80, 50, COLOR_CYAN);

    int y = 120;
    renderText(renderer, font, "• Enter a number to convert to binary", 100, y, COLOR_WHITE); y += 30;
    renderText(renderer, font, "• Use A/D or Arrow Keys to move your collector", 100, y, COLOR_WHITE); y += 30;
    renderText(renderer, font, "• Catch falling bits (1s and 0s) to score points", 100, y, COLOR_WHITE); y += 30;
    renderText(renderer, font, "• Green bits (1) and Red bits (0)", 100, y, COLOR_WHITE); y += 30;
    renderText(renderer, font, "• Collect power-ups for special abilities:", 100, y, COLOR_WHITE); y += 40;
    
    renderText(renderer, font, "  S - Speed Boost (Yellow)", 120, y, COLOR_YELLOW); y += 25;
    renderText(renderer, font, "  M - Score Multiplier (Orange)", 120, y, COLOR_ORANGE); y += 25;
    renderText(renderer, font, "  T - Slow Time (Purple)", 120, y, COLOR_PURPLE); y += 40;
    
    renderText(renderer, font, "• Don't let bits reach the bottom!", 100, y, COLOR_RED); y += 30;
    renderText(renderer, font, "• You have 3 lives", 100, y, COLOR_WHITE); y += 30;
    renderText(renderer, font, "• Press SPACE to pause, Q to quit", 100, y, COLOR_WHITE);

    renderText(renderer, font, "Press any key to return to menu", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT - 50, COLOR_GREEN);

    SDL_RenderPresent(renderer);
}

bool handleMenuInput(MenuSystem* menu, SDL_Event* event) {
    if (event->type == SDL_QUIT) {
        return false;
    }

    if (menu->currentMenu == MENU_INPUT) {
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.sym == SDLK_ESCAPE) {
                menu->currentMenu = MENU_MAIN;
                menu->inputLength = 0;
                menu->inputBuffer[0] = '\0';
            } else if (event->key.keysym.sym == SDLK_RETURN && menu->inputLength > 0) {
                // Parse input based on conversion type
                if (menu->conversionType == CONVERSION_HEXADECIMAL) {
                    menu->inputNumber = parseHexString(menu->inputBuffer);
                } else {
                    menu->inputNumber = atoi(menu->inputBuffer);
                }
                menu->numberEntered = true;
                menu->currentMenu = MENU_GAME;
            } else if (event->key.keysym.sym == SDLK_BACKSPACE && menu->inputLength > 0) {
                menu->inputLength--;
                menu->inputBuffer[menu->inputLength] = '\0';
            }
        } else if (event->type == SDL_TEXTINPUT) {
            char c = event->text.text[0];
            bool validChar = false;
            
            // Always accept digits 0-9
            if (c >= '0' && c <= '9') {
                // For octal, only accept 0-7
                if (menu->conversionType == CONVERSION_OCTAL) {
                    validChar = (c >= '0' && c <= '7');
                } else {
                    validChar = true;
                }
            }
            // For hexadecimal, also accept A-F and a-f
            else if (menu->conversionType == CONVERSION_HEXADECIMAL) {
                if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
                    validChar = true;
                    // Convert to uppercase for consistency
                    if (c >= 'a' && c <= 'f') {
                        c = c - 'a' + 'A';
                    }
                }
            }
            
            if (validChar && menu->inputLength < 18) {
                menu->inputBuffer[menu->inputLength] = c;
                menu->inputLength++;
                menu->inputBuffer[menu->inputLength] = '\0';
            }
        }
    } else if (menu->currentMenu == MENU_CONVERSION_SELECT) {
        if (event->type == SDL_KEYDOWN) {
            switch (event->key.keysym.sym) {
                case SDLK_1:
                    menu->conversionType = CONVERSION_DECIMAL;
                    menu->currentMenu = MENU_INPUT;
                    break;
                case SDLK_2:
                    menu->conversionType = CONVERSION_OCTAL;
                    menu->currentMenu = MENU_INPUT;
                    break;
                case SDLK_3:
                    menu->conversionType = CONVERSION_HEXADECIMAL;
                    menu->currentMenu = MENU_INPUT;
                    break;
                case SDLK_ESCAPE:
                    menu->currentMenu = MENU_MAIN;
                    break;
            }
        } else if (event->type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = event->button.x;
            int mouseY = event->button.y;
            
            // Check button clicks
            if (mouseX >= WINDOW_WIDTH/2 - 120 && mouseX <= WINDOW_WIDTH/2 + 120) {
                if (mouseY >= 200 && mouseY <= 250) {
                    menu->conversionType = CONVERSION_DECIMAL;
                    menu->currentMenu = MENU_INPUT;
                } else if (mouseY >= 270 && mouseY <= 320) {
                    menu->conversionType = CONVERSION_OCTAL;
                    menu->currentMenu = MENU_INPUT;
                } else if (mouseY >= 340 && mouseY <= 390) {
                    menu->conversionType = CONVERSION_HEXADECIMAL;
                    menu->currentMenu = MENU_INPUT;
                }
            }
            // Check back button
            if (mouseX >= WINDOW_WIDTH/2 - 100 && mouseX <= WINDOW_WIDTH/2 + 100 &&
                mouseY >= 430 && mouseY <= 470) {
                menu->currentMenu = MENU_MAIN;
            }
        }
    } else if (menu->currentMenu == MENU_MAIN) {
        if (event->type == SDL_KEYDOWN) {
            switch (event->key.keysym.sym) {
                case SDLK_1:
                case SDLK_RETURN:
                    menu->currentMenu = MENU_CONVERSION_SELECT;
                    break;
                case SDLK_2:
                    menu->currentMenu = MENU_MAIN; // Show instructions
                    break;
                case SDLK_3:
                case SDLK_q:
                    return false;
            }
        } else if (event->type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = event->button.x;
            int mouseY = event->button.y;
            
            // Check button clicks
            if (mouseX >= WINDOW_WIDTH/2 - 100 && mouseX <= WINDOW_WIDTH/2 + 100) {
                if (mouseY >= 280 && mouseY <= 330) {
                    menu->currentMenu = MENU_CONVERSION_SELECT;
                } else if (mouseY >= 350 && mouseY <= 400) {
                    // Show instructions (for now just stay in main menu)
                } else if (mouseY >= 420 && mouseY <= 470) {
                    return false;
                }
            }
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    
    if (!initSDL(&window, &renderer)) {
        return 1;
    }

    // Try to load a font (system font or fallback)
    // Try Windows fonts first
    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 18);
    if (!font) {
        font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 18);
    }
    if (!font) {
        font = TTF_OpenFont("C:\\Windows\\Fonts\\verdana.ttf", 18);
    }
    // Try Linux fonts
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 18);
    }
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/TTF/arial.ttf", 18);
    }
    // Try local font file
    if (!font) {
        font = TTF_OpenFont("font.ttf", 18);
    }
    if (!font) {
        printf("ERROR: Could not load any font! SDL_ttf Error: %s\n", TTF_GetError());
        printf("Please ensure arial.ttf or font.ttf is in the game folder.\n");
        // Continue without font - text won't render but game will still work
    }

    // Initialize menu sound system (but don't start music yet)
    SoundSystem menuSound = {0};
    soundInit(&menuSound);
    soundLoadMusic(&menuSound, "sounds");
    // Music will start when user clicks "Start New Game"

    MenuSystem menu = {0};
    menu.currentMenu = MENU_MAIN;
    menu.inputLength = 0;
    menu.inputBuffer[0] = '\0';
    menu.numberEntered = false;
    menu.conversionType = CONVERSION_DECIMAL; // Default to decimal

    GameState game = {0};
    bool quit = false;
    bool showInstructions = false;
    Uint32 lastTime = SDL_GetTicks();

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (menu.currentMenu == MENU_GAME && !game.gameOver) {
                handleInput(&game, &event);
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
            } else if (showInstructions) {
                if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
                    if (event.type == SDL_QUIT) {
                        quit = true;
                    } else {
                        showInstructions = false;
                    }
                }
            } else {
                if (!handleMenuInput(&menu, &event)) {
                    quit = true;
                }
                
                // Check for instructions
                if (menu.currentMenu == MENU_MAIN && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
                    showInstructions = true;
                } else if (menu.currentMenu == MENU_MAIN && event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (mouseX >= WINDOW_WIDTH/2 - 100 && mouseX <= WINDOW_WIDTH/2 + 100 &&
                        mouseY >= 350 && mouseY <= 400) {
                        showInstructions = true;
                    }
                }
            }
        }

        // Initialize game when number is entered
        if (menu.numberEntered) {
            initGame(&game, menu.inputNumber, menu.conversionType);
            // Play gamestart.mp3 once when user clicks "Start New Game"
            if (game.soundSystem) {
                soundPlayMusicOnce(game.soundSystem, MUSIC_MENU); // This plays gamestart.mp3 once
            }
            menu.numberEntered = false;
        }

        // Update game
        if (menu.currentMenu == MENU_GAME) {
            updateGame(&game, deltaTime);
            
            // Check if gamestart music finished, then start background music
            if (game.soundSystem && !soundIsMusicPlaying(game.soundSystem) && !game.gameOver) {
                soundPlayMusic(game.soundSystem, MUSIC_BACKGROUND);
            }
            
            // Return to menu when game is over and Q is pressed
            if (game.gameOver) {
                SDL_PumpEvents();
                const Uint8* keystate = SDL_GetKeyboardState(NULL);
                if (keystate[SDL_SCANCODE_Q]) {
                    menu.currentMenu = MENU_MAIN;
                    menu.inputLength = 0;
                    menu.inputBuffer[0] = '\0';
                }
            }
        }

        // Render
        if (showInstructions) {
            renderInstructions(renderer, font);
        } else if (menu.currentMenu == MENU_MAIN) {
            renderMainMenu(renderer, font, &menu);
        } else if (menu.currentMenu == MENU_CONVERSION_SELECT) {
            renderConversionSelectMenu(renderer, font, &menu);
        } else if (menu.currentMenu == MENU_INPUT) {
            renderInputMenu(renderer, font, &menu);
        } else if (menu.currentMenu == MENU_GAME) {
            renderGame(renderer, font, &game);
        }

        SDL_Delay(16); // Cap at ~60 FPS
    }

    if (font) {
        TTF_CloseFont(font);
    }
    
    // Cleanup menu sound system
    soundCleanup(&menuSound);
    
    cleanupSDL(window, renderer);
    
    return 0;
}
