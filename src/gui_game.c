#include "gui_game.h"
#include "binary.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Color definitions
const Color COLOR_WHITE = {255, 255, 255, 255};
const Color COLOR_BLACK = {0, 0, 0, 255};
const Color COLOR_RED = {255, 0, 0, 255};
const Color COLOR_GREEN = {0, 255, 0, 255};
const Color COLOR_BLUE = {0, 0, 255, 255};
const Color COLOR_YELLOW = {255, 255, 0, 255};
const Color COLOR_CYAN = {0, 255, 255, 255};
const Color COLOR_MAGENTA = {255, 0, 255, 255};
const Color COLOR_ORANGE = {255, 165, 0, 255};
const Color COLOR_PURPLE = {128, 0, 128, 255};

bool initSDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Binary Quest - Enhanced Edition",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void cleanupSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

bool initGame(GameState* game, int number, ConversionType conversionType) {
    srand(time(NULL));
    
    game->originalNumber = number;
    game->conversionType = conversionType;
    
    // Clear bits array before conversion
    for (int i = 0; i < MAX_BITS; i++) {
        game->bits[i] = 0;
    }
    
    game->bitCount = convertToBinary(number, game->bits, MAX_BITS, conversionType);
    game->currentBitIndex = 0;
    game->expectedBitIndex = 0;
    game->score = 0;
    game->level = 1;
    game->collectedCount = 0;
    game->gameOver = false;
    game->paused = false;
    game->gameSpeed = 1.0f;
    game->nextBitTimer = 0;
    game->powerUpSpawnTimer = 0;
    game->showBinaryResult = false;
    game->levelComplete = false;
    game->wrongBitPenalty = 0;
    game->continuousFallTimer = 0;
    game->screenShakeTimer = 0;
    game->screenShakeIntensity = 0.0f;
    game->transitionTimer = 0;
    game->isTransitioning = false;
    game->nextLevelNumber = 0;
    game->wrongBitCount = 0; // Initialize wrong bit counter
    game->minNumber = 1;
    game->maxNumber = 50; // Start with smaller numbers

    // Initialize sound system
    game->soundSystem = malloc(sizeof(SoundSystem));
    if (game->soundSystem) {
        soundInit(game->soundSystem);
        // Try to load sound effects (optional - game works without them)
        soundLoadEffects(game->soundSystem, "sounds");
        soundLoadMusic(game->soundSystem, "sounds");
        // Don't start music automatically - wait for user to start game
    }

    // Initialize player
    game->player.x = GAME_AREA_X + GAME_AREA_WIDTH / 2 - PLAYER_WIDTH / 2;
    game->player.width = PLAYER_WIDTH;
    game->player.speed = 300.0f;
    game->player.lives = 3;
    game->player.hasSpeedBoost = false;
    game->player.hasScoreMultiplier = false;
    game->player.hasSlowTime = false;
    // Initialize particles
    game->particleCount = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        game->particles[i].life = 0.0f;
    }

    // Initialize falling bits
    for (int i = 0; i < MAX_FALLING_BITS; i++) {
        game->fallingBits[i].active = false;
    }

    // Initialize power-ups
    for (int i = 0; i < 3; i++) {
        game->powerUps[i].active = false;
    }

    return true;
}

void setRenderColor(SDL_Renderer* renderer, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void spawnBit(GameState* game) {
    // Spawn bits continuously, not just in order
    for (int i = 0; i < MAX_FALLING_BITS; i++) {
        if (!game->fallingBits[i].active) {
            game->fallingBits[i].active = true;
            game->fallingBits[i].x = GAME_AREA_X + rand() % (GAME_AREA_WIDTH - BIT_SIZE);
            game->fallingBits[i].y = GAME_AREA_Y;
            
            // Spawn random bits from the binary representation
            int randomBitIndex = rand() % game->bitCount;
            game->fallingBits[i].value = game->bits[randomBitIndex];
            game->fallingBits[i].speed = 80.0f + game->level * 15.0f;
            
            // Color based on bit value
            if (game->fallingBits[i].value == 1) {
                game->fallingBits[i].color = COLOR_GREEN;
            } else {
                game->fallingBits[i].color = COLOR_RED;
            }
            
            break;
        }
    }
}

void spawnPowerUp(GameState* game) {
    for (int i = 0; i < 3; i++) {
        if (!game->powerUps[i].active) {
            game->powerUps[i].active = true;
            game->powerUps[i].x = GAME_AREA_X + rand() % (GAME_AREA_WIDTH - POWERUP_SIZE);
            game->powerUps[i].y = GAME_AREA_Y;
            game->powerUps[i].type = rand() % 3;
            game->powerUps[i].speed = 80.0f;
            game->powerUps[i].duration = 300; // 5 seconds at 60 FPS
            break;
        }
    }
}

bool isCorrectBit(GameState* game, int bitValue) {
    if (game->expectedBitIndex >= game->bitCount) {
        return false; // All bits already collected
    }
    return game->bits[game->expectedBitIndex] == bitValue;
}

void generateNewLevel(GameState* game) {
    game->level++;

    // Increase difficulty by expanding number range (max 1000)
    if (game->level <= 3) {
        game->maxNumber = 100;
    } else if (game->level <= 6) {
        game->maxNumber = 250;
    } else if (game->level <= 10) {
        game->maxNumber = 500;
    } else {
        game->maxNumber = 1000; // Maximum limit as requested
    }

    // Generate random number for new level
    int newNumber;
    
    if (game->conversionType == CONVERSION_OCTAL) {
        // For octal, generate a valid octal number (no digits 8 or 9)
        // Generate a decimal number first, then convert to octal representation
        int maxOctal = game->maxNumber;
        if (maxOctal > 777) maxOctal = 777; // Max valid octal in this range
        
        do {
            newNumber = game->minNumber + rand() % (maxOctal - game->minNumber + 1);
            // Check if all digits are valid octal (0-7)
            int temp = newNumber;
            bool validOctal = true;
            while (temp > 0) {
                if (temp % 10 >= 8) {
                    validOctal = false;
                    break;
                }
                temp /= 10;
            }
            if (validOctal || newNumber == 0) break;
        } while (true);
    } else {
        newNumber = game->minNumber + rand() % (game->maxNumber - game->minNumber + 1);
    }

    // Reset level immediately instead of using transition
    resetLevel(game, newNumber);
}

void resetLevel(GameState* game, int newNumber) {
    game->originalNumber = newNumber;
    
    // Clear bits array before conversion to avoid old data
    for (int i = 0; i < MAX_BITS; i++) {
        game->bits[i] = 0;
    }
    
    game->bitCount = convertToBinary(newNumber, game->bits, MAX_BITS, game->conversionType);
    game->currentBitIndex = 0;
    game->expectedBitIndex = 0;
    game->collectedCount = 0;
    game->levelComplete = false;
    game->showBinaryResult = false;
    game->continuousFallTimer = 0;
    game->isTransitioning = false;
    
    // Clear collected bits array
    for (int i = 0; i < MAX_BITS; i++) {
        game->collectedBits[i] = 0;
    }
    
    // Clear all falling bits
    for (int i = 0; i < MAX_FALLING_BITS; i++) {
        game->fallingBits[i].active = false;
    }
    
    // Increase game speed slightly
    game->gameSpeed = 1.0f + (game->level - 1) * 0.1f;
}

void checkCollisions(GameState* game) {
    float playerLeft = game->player.x;
    float playerRight = game->player.x + game->player.width;
    float playerTop = GAME_AREA_Y + GAME_AREA_HEIGHT - PLAYER_HEIGHT;
    float playerBottom = GAME_AREA_Y + GAME_AREA_HEIGHT;

    // Check bit collisions
    for (int i = 0; i < MAX_FALLING_BITS; i++) {
        if (game->fallingBits[i].active) {
            float bitLeft = game->fallingBits[i].x;
            float bitRight = game->fallingBits[i].x + BIT_SIZE;
            float bitTop = game->fallingBits[i].y;
            float bitBottom = game->fallingBits[i].y + BIT_SIZE;

            // Check collision with player
            if (bitRight > playerLeft && bitLeft < playerRight &&
                bitBottom > playerTop && bitTop < playerBottom) {
                
                // Check if this is the correct bit in sequence
                if (isCorrectBit(game, game->fallingBits[i].value)) {
                    // Correct bit collected
                    game->collectedBits[game->collectedCount++] = game->fallingBits[i].value;
                    game->expectedBitIndex++;
                    
                    int points = 10;
                    if (game->player.hasScoreMultiplier) points *= 2;
                    game->score += points;
                    
                    // Spawn particles for correct bit collection
                    spawnParticles(game, game->fallingBits[i].x + BIT_SIZE/2,
                                 game->fallingBits[i].y + BIT_SIZE/2,
                                 PARTICLE_BIT_COLLECT, 8);
                    
                    // Play sound effect
                    if (game->soundSystem) {
                        soundPlayEffect(game->soundSystem, SOUND_BIT_COLLECT);
                        printf("🔊 Playing pop sound for bit collection!\n");
                    }
                    
                    // Check if level is complete
                    if (game->expectedBitIndex >= game->bitCount) {
                        game->levelComplete = true;
                        game->score += 50 * game->level; // Bonus for completing level
                        spawnParticles(game, game->fallingBits[i].x + BIT_SIZE/2,
                                     game->fallingBits[i].y + BIT_SIZE/2,
                                     PARTICLE_LEVEL_COMPLETE, 15);
                        
                        // Play level complete sound
                        if (game->soundSystem) {
                            soundPlayEffect(game->soundSystem, SOUND_LEVEL_COMPLETE);
                        }
                    }
                } else {
                    // Wrong bit collected - penalty
                    game->wrongBitCount++;
                    game->wrongBitPenalty = 60; // Show penalty indicator for 1 second
                    
                    // Trigger screen shake for wrong bit
                    triggerScreenShake(game, 5.0f);
                    
                    // Spawn particles for wrong bit
                    spawnParticles(game, game->fallingBits[i].x + BIT_SIZE/2,
                                 game->fallingBits[i].y + BIT_SIZE/2,
                                 PARTICLE_WRONG_BIT, 12);
                    
                    // Play wrong bit sound
                    if (game->soundSystem) {
                        soundPlayEffect(game->soundSystem, SOUND_WRONG_BIT);
                    }
                    
                    // End game after 3 wrong bits
                    if (game->wrongBitCount >= 3) {
                        game->gameOver = true;
                        // Stop screen shake immediately when game ends
                        game->screenShakeTimer = 0;
                        game->screenShakeIntensity = 0.0f;
                        // Play game over sound and music (once, not looping)
                        if (game->soundSystem) {
                            soundPlayEffect(game->soundSystem, SOUND_GAME_OVER);
                            soundPlayMusicOnce(game->soundSystem, MUSIC_GAME_OVER);
                        }
                    }
                }
                game->fallingBits[i].active = false;
            }
            // Check if bit reached bottom - just remove it, no penalty
            else if (game->fallingBits[i].y > GAME_AREA_Y + GAME_AREA_HEIGHT) {
                game->fallingBits[i].active = false;
                // No penalty for missed bits - only wrong bit collection matters
            }
        }
    }

    // Check power-up collisions
    for (int i = 0; i < 3; i++) {
        if (game->powerUps[i].active) {
            float powerUpLeft = game->powerUps[i].x;
            float powerUpRight = game->powerUps[i].x + POWERUP_SIZE;
            float powerUpTop = game->powerUps[i].y;
            float powerUpBottom = game->powerUps[i].y + POWERUP_SIZE;

            if (powerUpRight > playerLeft && powerUpLeft < playerRight &&
                powerUpBottom > playerTop && powerUpTop < playerBottom) {
                
                // Activate power-up
                switch (game->powerUps[i].type) {
                    case 0: // Speed boost
                        game->player.hasSpeedBoost = true;
                        game->player.powerUpTimer[0] = game->powerUps[i].duration;
                        spawnParticles(game, game->powerUps[i].x + POWERUP_SIZE/2,
                                     game->powerUps[i].y + POWERUP_SIZE/2,
                                     PARTICLE_POWERUP, 10);
                        break;
                    case 1: // Score multiplier
                        game->player.hasScoreMultiplier = true;
                        game->player.powerUpTimer[1] = game->powerUps[i].duration;
                        spawnParticles(game, game->powerUps[i].x + POWERUP_SIZE/2,
                                     game->powerUps[i].y + POWERUP_SIZE/2,
                                     PARTICLE_POWERUP, 10);
                        break;
                    case 2: // Slow time
                        game->player.hasSlowTime = true;
                        game->player.powerUpTimer[2] = game->powerUps[i].duration;
                        game->gameSpeed = 0.5f;
                        spawnParticles(game, game->powerUps[i].x + POWERUP_SIZE/2,
                                     game->powerUps[i].y + POWERUP_SIZE/2,
                                     PARTICLE_POWERUP, 10);
                        break;
                }
                // Play power-up sound
                if (game->soundSystem) {
                    soundPlayEffect(game->soundSystem, SOUND_POWERUP);
                }
                game->powerUps[i].active = false;
                game->score += 5;
            }
            // Remove if reached bottom
            else if (game->powerUps[i].y > GAME_AREA_Y + GAME_AREA_HEIGHT) {
                game->powerUps[i].active = false;
            }
        }
    }
}

void updatePowerUps(GameState* game, float deltaTime) {
    for (int i = 0; i < 3; i++) {
        if (game->player.powerUpTimer[i] > 0) {
            game->player.powerUpTimer[i]--;
            if (game->player.powerUpTimer[i] <= 0) {
                switch (i) {
                    case 0:
                        game->player.hasSpeedBoost = false;
                        break;
                    case 1:
                        game->player.hasScoreMultiplier = false;
                        break;
                    case 2:
                        game->player.hasSlowTime = false;
                        game->gameSpeed = 1.0f;
                        break;
                }
            }
        }
    }
}

void updateGame(GameState* game, float deltaTime) {
    if (game->gameOver || game->paused) return;

    deltaTime *= game->gameSpeed;

    // Update power-ups
    updatePowerUps(game, deltaTime);
    
    // Update penalty indicator
    if (game->wrongBitPenalty > 0) {
        game->wrongBitPenalty--;
    }

    // Update particles
    updateParticles(game, deltaTime);

    // Update screen shake (but not when game is over)
    if (game->screenShakeTimer > 0 && !game->gameOver) {
        game->screenShakeTimer--;
        if (game->screenShakeTimer <= 0) {
            game->screenShakeIntensity = 0.0f;
        }
    } else if (game->gameOver) {
        // Stop screen shake immediately when game ends
        game->screenShakeTimer = 0;
        game->screenShakeIntensity = 0.0f;
    }

    // Update level transition
    updateTransition(game, deltaTime);

    // Handle level completion
    if (game->levelComplete && !game->isTransitioning) {
        game->continuousFallTimer += deltaTime * 60;
        if (game->continuousFallTimer >= 120) { // Wait 2 seconds before next level
            generateNewLevel(game);
        }
        return; // Don't spawn new bits during level transition
    }

    // Spawn new bits continuously (faster spawning)
    game->nextBitTimer += deltaTime * 60;
    int spawnRate = 30 - (game->level * 2); // Faster spawning at higher levels
    if (spawnRate < 10) spawnRate = 10; // Minimum spawn rate
    
    if (game->nextBitTimer >= spawnRate) {
        spawnBit(game);
        game->nextBitTimer = 0;
    }

    // Spawn power-ups occasionally
    game->powerUpSpawnTimer += deltaTime * 60;
    if (game->powerUpSpawnTimer >= 300) { // Every 5 seconds
        if (rand() % 3 == 0) { // 33% chance
            spawnPowerUp(game);
        }
        game->powerUpSpawnTimer = 0;
    }

    // Update falling bits
    for (int i = 0; i < MAX_FALLING_BITS; i++) {
        if (game->fallingBits[i].active) {
            game->fallingBits[i].y += game->fallingBits[i].speed * deltaTime;
        }
    }

    // Update power-ups
    for (int i = 0; i < 3; i++) {
        if (game->powerUps[i].active) {
            game->powerUps[i].y += game->powerUps[i].speed * deltaTime;
        }
    }

    // Check collisions
    checkCollisions(game);

    // Level completion is now handled in collision detection
}

void handleInput(GameState* game, SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                if (game->player.x > GAME_AREA_X) {
                    float speed = game->player.speed;
                    if (game->player.hasSpeedBoost) speed *= 1.5f;
                    game->player.x -= speed * 0.016f; // Assume 60 FPS
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                if (game->player.x < GAME_AREA_X + GAME_AREA_WIDTH - game->player.width) {
                    float speed = game->player.speed;
                    if (game->player.hasSpeedBoost) speed *= 1.5f;
                    game->player.x += speed * 0.016f;
                }
                break;
            case SDLK_SPACE:
                game->paused = !game->paused;
                break;
            case SDLK_q:
                game->gameOver = true;
                break;
        }
    }
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, 
                int x, int y, Color color) {
    if (!font) return;
    
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, sdlColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &destRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

void renderButton(SDL_Renderer* renderer, TTF_Font* font, const char* text,
                  int x, int y, int width, int height, Color bgColor, Color textColor) {
    // Draw button background
    setRenderColor(renderer, bgColor);
    SDL_Rect buttonRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &buttonRect);
    
    // Draw button border
    setRenderColor(renderer, COLOR_WHITE);
    SDL_RenderDrawRect(renderer, &buttonRect);
    
    // Draw button text (centered)
    if (font) {
        int textW, textH;
        TTF_SizeText(font, text, &textW, &textH);
        int textX = x + (width - textW) / 2;
        int textY = y + (height - textH) / 2;
        renderText(renderer, font, text, textX, textY, textColor);
    }
}

void renderGame(SDL_Renderer* renderer, TTF_Font* font, GameState* game) {
    // Clear screen with dark blue background
    setRenderColor(renderer, (Color){20, 20, 40, 255});
    SDL_RenderClear(renderer);

    // Apply screen shake (but not when game is over)
    int shakeX = 0, shakeY = 0;
    if (game->screenShakeTimer > 0 && !game->gameOver) {
        shakeX = (rand() % (int)(game->screenShakeIntensity * 2)) - game->screenShakeIntensity;
        shakeY = (rand() % (int)(game->screenShakeIntensity * 2)) - game->screenShakeIntensity;
    }

    // Draw game area border
    setRenderColor(renderer, COLOR_WHITE);
    SDL_Rect gameArea = {GAME_AREA_X + shakeX - 2, GAME_AREA_Y + shakeY - 2, GAME_AREA_WIDTH + 4, GAME_AREA_HEIGHT + 4};
    SDL_RenderDrawRect(renderer, &gameArea);

    // Draw game area background
    setRenderColor(renderer, (Color){10, 10, 20, 255});
    SDL_Rect gameAreaBg = {GAME_AREA_X + shakeX, GAME_AREA_Y + shakeY, GAME_AREA_WIDTH, GAME_AREA_HEIGHT};
    SDL_RenderFillRect(renderer, &gameAreaBg);

    // Draw title
    char title[100];
    sprintf(title, "BINARY QUEST - Enhanced Edition");
    renderText(renderer, font, title, 200, 20, COLOR_CYAN);

    // Draw score and info
    char scoreText[200];
    sprintf(scoreText, "Score: %d | Level: %d | Wrong Bits: %d/3 | Range: %d-%d", 
            game->score, game->level, game->wrongBitCount, game->minNumber, game->maxNumber);
    renderText(renderer, font, scoreText, 50 + shakeX, 50 + shakeY, COLOR_WHITE);

    // Draw original number with conversion type
    char numberText[150];
    const char* conversionName = "";
    switch (game->conversionType) {
        case CONVERSION_DECIMAL: conversionName = "Decimal"; break;
        case CONVERSION_OCTAL: conversionName = "Octal"; break;
        case CONVERSION_HEXADECIMAL: conversionName = "Hexadecimal"; break;
    }
    sprintf(numberText, "%s: %d -> Binary: ", conversionName, game->originalNumber);
    for (int i = 0; i < game->bitCount; i++) {
        char bit[2];
        sprintf(bit, "%d", game->bits[i]);
        strcat(numberText, bit);
    }
    renderText(renderer, font, numberText, 50 + shakeX, WINDOW_HEIGHT - 110 + shakeY, COLOR_CYAN);

    // Draw collected bits
    char collectedText[200] = "Collected: ";
    for (int i = 0; i < game->collectedCount; i++) {
        char bit[2];
        sprintf(bit, "%d", game->collectedBits[i]);
        strcat(collectedText, bit);
    }
    renderText(renderer, font, collectedText, 50 + shakeX, WINDOW_HEIGHT - 80 + shakeY, COLOR_GREEN);
    
    // Show next expected bit
    if (game->expectedBitIndex < game->bitCount) {
        char expectedText[100];
        sprintf(expectedText, "Next bit needed: %d", game->bits[game->expectedBitIndex]);
        renderText(renderer, font, expectedText, 50 + shakeX, WINDOW_HEIGHT - 50 + shakeY, COLOR_ORANGE);
    }

    // Draw power-up status
    int powerUpY = 100;
    if (game->player.hasSpeedBoost) {
        renderText(renderer, font, "SPEED BOOST!", WINDOW_WIDTH - 150, powerUpY, COLOR_YELLOW);
        powerUpY += 25;
    }
    if (game->player.hasScoreMultiplier) {
        renderText(renderer, font, "2X SCORE!", WINDOW_WIDTH - 150, powerUpY, COLOR_MAGENTA);
        powerUpY += 25;
    }
    if (game->player.hasSlowTime) {
        renderText(renderer, font, "SLOW TIME!", WINDOW_WIDTH - 150, powerUpY, COLOR_PURPLE);
        powerUpY += 25;
    }
    
    // Show penalty indicator
    if (game->wrongBitPenalty > 0) {
        char penaltyText[150];
        sprintf(penaltyText, "WRONG BIT! (%d/3) - Expected: %d", 
                game->wrongBitCount, 
                game->expectedBitIndex < game->bitCount ? game->bits[game->expectedBitIndex] : -1);
        renderText(renderer, font, penaltyText, WINDOW_WIDTH/2 - 120 + shakeX, 150 + shakeY, COLOR_RED);
    }
    
    // Show level completion
    if (game->levelComplete) {
        renderText(renderer, font, "LEVEL COMPLETE!", WINDOW_WIDTH/2 - 80 + shakeX, 200 + shakeY, COLOR_GREEN);
        char bonusText[50];
        sprintf(bonusText, "Bonus: +%d points", 50 * game->level);
        renderText(renderer, font, bonusText, WINDOW_WIDTH/2 - 80 + shakeX, 230 + shakeY, COLOR_CYAN);
    }

    if (!game->gameOver) {
        // Draw falling bits
        for (int i = 0; i < MAX_FALLING_BITS; i++) {
            if (game->fallingBits[i].active) {
                setRenderColor(renderer, game->fallingBits[i].color);
                SDL_Rect bitRect = {
                    (int)game->fallingBits[i].x + shakeX,
                    (int)game->fallingBits[i].y + shakeY,
                    BIT_SIZE,
                    BIT_SIZE
                };
                SDL_RenderFillRect(renderer, &bitRect);
                
                // Draw bit value
                char bitText[2];
                sprintf(bitText, "%d", game->fallingBits[i].value);
                renderText(renderer, font, bitText, 
                          (int)game->fallingBits[i].x + 8 + shakeX, 
                          (int)game->fallingBits[i].y + 5 + shakeY, 
                          COLOR_WHITE);
            }
        }

        // Draw power-ups
        for (int i = 0; i < 3; i++) {
            if (game->powerUps[i].active) {
                Color powerUpColor;
                char powerUpSymbol[2];
                switch (game->powerUps[i].type) {
                    case 0: powerUpColor = COLOR_YELLOW; strcpy(powerUpSymbol, "S"); break;
                    case 1: powerUpColor = COLOR_ORANGE; strcpy(powerUpSymbol, "M"); break;
                    case 2: powerUpColor = COLOR_PURPLE; strcpy(powerUpSymbol, "T"); break;
                }
                
                setRenderColor(renderer, powerUpColor);
                SDL_Rect powerUpRect = {
                    (int)game->powerUps[i].x + shakeX,
                    (int)game->powerUps[i].y + shakeY,
                    POWERUP_SIZE,
                    POWERUP_SIZE
                };
                SDL_RenderFillRect(renderer, &powerUpRect);
                
                renderText(renderer, font, powerUpSymbol, 
                          (int)game->powerUps[i].x + 8 + shakeX, 
                          (int)game->powerUps[i].y + 5 + shakeY, 
                          COLOR_BLACK);
            }
        }

        // Draw player
        setRenderColor(renderer, COLOR_CYAN);
        SDL_Rect playerRect = {
            (int)game->player.x + shakeX,
            GAME_AREA_Y + GAME_AREA_HEIGHT - PLAYER_HEIGHT + shakeY,
            game->player.width,
            PLAYER_HEIGHT
        };
        SDL_RenderFillRect(renderer, &playerRect);

        // Render particles
        renderParticles(renderer, game);
    }

    // Draw pause screen
    if (game->paused) {
        setRenderColor(renderer, (Color){0, 0, 0, 128});
        SDL_Rect pauseOverlay = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(renderer, &pauseOverlay);
        
        renderText(renderer, font, "PAUSED", WINDOW_WIDTH/2 - 50 + shakeX, WINDOW_HEIGHT/2 - 20 + shakeY, COLOR_WHITE);
        renderText(renderer, font, "Press SPACE to continue", WINDOW_WIDTH/2 - 120 + shakeX, WINDOW_HEIGHT/2 + 20 + shakeY, COLOR_WHITE);
    }

    // Draw game over screen
    if (game->gameOver) {
        setRenderColor(renderer, (Color){0, 0, 0, 200});
        SDL_Rect gameOverOverlay = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(renderer, &gameOverOverlay);
        
        renderText(renderer, font, "GAME OVER", WINDOW_WIDTH/2 - 80 + shakeX, WINDOW_HEIGHT/2 - 80 + shakeY, COLOR_RED);
        
        char finalScore[100];
        sprintf(finalScore, "Final Score: %d", game->score);
        renderText(renderer, font, finalScore, WINDOW_WIDTH/2 - 80 + shakeX, WINDOW_HEIGHT/2 - 40 + shakeY, COLOR_WHITE);
        
        char wrongBitsText[100];
        sprintf(wrongBitsText, "Wrong bits collected: %d/3", game->wrongBitCount);
        renderText(renderer, font, wrongBitsText, WINDOW_WIDTH/2 - 100 + shakeX, WINDOW_HEIGHT/2 - 10 + shakeY, COLOR_RED);
        
        char levelText[100];
        sprintf(levelText, "Levels completed: %d", game->level - 1);
        renderText(renderer, font, levelText, WINDOW_WIDTH/2 - 80 + shakeX, WINDOW_HEIGHT/2 + 20 + shakeY, COLOR_CYAN);
        
        renderText(renderer, font, "Press Q to quit", WINDOW_WIDTH/2 - 80 + shakeX, WINDOW_HEIGHT/2 + 50 + shakeY, COLOR_WHITE);
    }

    // Draw controls
    renderText(renderer, font, "Controls: A/D or Arrow Keys to move, SPACE to pause, Q to quit", 
               50 + shakeX, WINDOW_HEIGHT - 20 + shakeY, COLOR_WHITE);

    SDL_RenderPresent(renderer);
}

void spawnParticles(GameState* game, float x, float y, ParticleType type, int count) {
    for (int i = 0; i < count && game->particleCount < MAX_PARTICLES; i++) {
        Particle* p = &game->particles[game->particleCount++];
        p->x = x;
        p->y = y;
        p->life = 1.0f;
        p->size = 2.0f + rand() % 4;

        // Set particle properties based on type
        switch (type) {
            case PARTICLE_BIT_COLLECT:
                p->color = (Color){0, 255, 100, 255}; // Green
                p->vx = (rand() % 200 - 100) * 0.1f;
                p->vy = (rand() % 100 - 150) * 0.1f;
                break;
            case PARTICLE_WRONG_BIT:
                p->color = (Color){255, 50, 50, 255}; // Red
                p->vx = (rand() % 300 - 150) * 0.1f;
                p->vy = (rand() % 200 - 250) * 0.1f;
                p->size *= 1.5f; // Larger particles for wrong bits
                break;
            case PARTICLE_LEVEL_COMPLETE:
                p->color = (Color){255, 215, 0, 255}; // Gold
                p->vx = (rand() % 400 - 200) * 0.1f;
                p->vy = (rand() % 300 - 400) * 0.1f;
                p->size *= 2.0f; // Larger particles for level complete
                break;
            case PARTICLE_POWERUP:
                p->color = (Color){255, 255, 0, 255}; // Yellow
                p->vx = (rand() % 150 - 75) * 0.1f;
                p->vy = (rand() % 150 - 200) * 0.1f;
                break;
        }
    }
}

void updateParticles(GameState* game, float deltaTime) {
    for (int i = game->particleCount - 1; i >= 0; i--) {
        Particle* p = &game->particles[i];

        // Update position
        p->x += p->vx;
        p->y += p->vy;

        // Update life
        p->life -= deltaTime * 2.0f; // 0.5 second lifetime

        // Apply gravity
        p->vy += 50.0f * deltaTime;

        // Remove dead particles
        if (p->life <= 0.0f) {
            // Move last particle to current position
            *p = game->particles[--game->particleCount];
        }
    }
}

void triggerScreenShake(GameState* game, float intensity) {
    game->screenShakeTimer = SCREEN_SHAKE_DURATION;
    game->screenShakeIntensity = intensity;
}

void startLevelTransition(GameState* game, int nextNumber) {
    game->isTransitioning = true;
    game->transitionTimer = TRANSITION_DURATION;
    game->nextLevelNumber = nextNumber;
}

void updateTransition(GameState* game, float deltaTime) {
    if (game->isTransitioning) {
        game->transitionTimer--;
        if (game->transitionTimer <= 0) {
            game->isTransitioning = false;
            resetLevel(game, game->nextLevelNumber);
        }
    }
}

void renderParticles(SDL_Renderer* renderer, GameState* game) {
    for (int i = 0; i < game->particleCount; i++) {
        Particle* p = &game->particles[i];
        if (p->life > 0.0f) {
            // Set particle color with alpha based on life
            Color color = p->color;
            color.a = (Uint8)(p->life * 255);
            setRenderColor(renderer, color);
            
            // Draw particle as a small rectangle
            SDL_Rect particleRect = {
                (int)(p->x - p->size/2),
                (int)(p->y - p->size/2),
                (int)p->size,
                (int)p->size
            };
            SDL_RenderFillRect(renderer, &particleRect);
        }
    }
}
