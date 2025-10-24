#ifndef GUI_GAME_H
#define GUI_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "sound.h"
#include "binary.h"

// Screen dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GAME_AREA_WIDTH 600
#define GAME_AREA_HEIGHT 500
#define GAME_AREA_X 100
#define GAME_AREA_Y 80

// Game constants
#define MAX_BITS 32
#define PLAYER_WIDTH 60
#define PLAYER_HEIGHT 20
#define BIT_SIZE 30
#define MAX_FALLING_BITS 5
#define POWERUP_SIZE 25

// Colors
typedef struct {
    Uint8 r, g, b, a;
} Color;

extern const Color COLOR_WHITE;
extern const Color COLOR_BLACK;
extern const Color COLOR_RED;
extern const Color COLOR_GREEN;
extern const Color COLOR_BLUE;
extern const Color COLOR_YELLOW;
extern const Color COLOR_CYAN;
extern const Color COLOR_MAGENTA;
extern const Color COLOR_ORANGE;
extern const Color COLOR_PURPLE;

// Game structures
typedef struct {
    float x, y;
    int value;
    bool active;
    float speed;
    Color color;
} FallingBit;

typedef struct {
    float x, y;
    int type; // 0: speed boost, 1: score multiplier, 2: slow time
    bool active;
    float speed;
    int duration;
} PowerUp;

typedef struct {
    float x;
    int width;
    float speed;
    int lives;
    bool hasSpeedBoost;
    bool hasScoreMultiplier;
    bool hasSlowTime;
    int powerUpTimer[3]; // timers for each power-up type
} Player;

#define MAX_PARTICLES 50
#define SCREEN_SHAKE_DURATION 30
#define TRANSITION_DURATION 60

// Particle types
typedef enum {
    PARTICLE_BIT_COLLECT,
    PARTICLE_WRONG_BIT,
    PARTICLE_LEVEL_COMPLETE,
    PARTICLE_POWERUP
} ParticleType;

typedef struct {
    float x, y;
    float vx, vy; // velocity
    float life; // remaining life (0-1)
    Color color;
    ParticleType type;
    float size;
} Particle;

typedef struct {
    int originalNumber;
    int bits[MAX_BITS];
    int bitCount;
    int currentBitIndex;
    int expectedBitIndex; // Track which bit we expect next
    FallingBit fallingBits[MAX_FALLING_BITS];
    PowerUp powerUps[3];
    Player player;
    int score;
    int level;
    int collectedBits[MAX_BITS];
    int collectedCount;
    bool gameOver;
    bool paused;
    float gameSpeed;
    int nextBitTimer;
    int powerUpSpawnTimer;
    bool showBinaryResult;
    bool levelComplete;
    int wrongBitPenalty;
    int wrongBitCount; // Track total wrong bits collected
    int continuousFallTimer;
    int minNumber; // Minimum number for current level
    int maxNumber; // Maximum number for current level
    ConversionType conversionType; // Type of conversion (decimal, octal, hex)

    // Visual effects
    Particle particles[MAX_PARTICLES];
    int particleCount;
    int screenShakeTimer;
    float screenShakeIntensity;
    int transitionTimer;
    bool isTransitioning;
    int nextLevelNumber; // For level transitions
    
    // Sound system
    SoundSystem* soundSystem;
} GameState;

// Function declarations
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
void cleanupSDL(SDL_Window* window, SDL_Renderer* renderer);
bool initGame(GameState* game, int number, ConversionType conversionType);
void updateGame(GameState* game, float deltaTime);
void renderGame(SDL_Renderer* renderer, TTF_Font* font, GameState* game);
void handleInput(GameState* game, SDL_Event* event);
void spawnBit(GameState* game);
void spawnPowerUp(GameState* game);
void checkCollisions(GameState* game);
void updatePowerUps(GameState* game, float deltaTime);
void generateNewLevel(GameState* game);
void resetLevel(GameState* game, int newNumber);
bool isCorrectBit(GameState* game, int bitValue);
void spawnParticles(GameState* game, float x, float y, ParticleType type, int count);
void updateParticles(GameState* game, float deltaTime);
void triggerScreenShake(GameState* game, float intensity);
void startLevelTransition(GameState* game, int nextNumber);
void updateTransition(GameState* game, float deltaTime);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, 
                int x, int y, Color color);
void renderButton(SDL_Renderer* renderer, TTF_Font* font, const char* text,
                  int x, int y, int width, int height, Color bgColor, Color textColor);
void setRenderColor(SDL_Renderer* renderer, Color color);
void renderParticles(SDL_Renderer* renderer, GameState* game);

#endif
