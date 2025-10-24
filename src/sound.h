#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

// Sound effect types
typedef enum {
    SOUND_BIT_COLLECT,
    SOUND_WRONG_BIT,
    SOUND_LEVEL_COMPLETE,
    SOUND_POWERUP,
    SOUND_GAME_OVER,
    SOUND_MENU_SELECT,
    SOUND_COUNT
} SoundEffect;

// Music types
typedef enum {
    MUSIC_MENU,
    MUSIC_BACKGROUND,
    MUSIC_GAME_OVER,
    MUSIC_COUNT
} MusicType;

typedef struct {
    Mix_Chunk* soundEffects[SOUND_COUNT];
    Mix_Music* music[MUSIC_COUNT];
    int masterVolume;
    int musicVolume;
    int sfxVolume;
    bool soundEnabled;
    bool musicEnabled;
} SoundSystem;

// Function declarations
bool soundInit(SoundSystem* sound);
void soundCleanup(SoundSystem* sound);
bool soundLoadEffects(SoundSystem* sound, const char* soundPath);
bool soundLoadMusic(SoundSystem* sound, const char* musicPath);

// Playback functions
void soundPlayEffect(SoundSystem* sound, SoundEffect effect);
void soundPlayMusic(SoundSystem* sound, MusicType music);
void soundPlayMusicOnce(SoundSystem* sound, MusicType music);
void soundStopMusic(SoundSystem* sound);
void soundPauseMusic(SoundSystem* sound);
void soundResumeMusic(SoundSystem* sound);

// Volume controls
void soundSetMasterVolume(SoundSystem* sound, int volume);
void soundSetMusicVolume(SoundSystem* sound, int volume);
void soundSetSFXVolume(SoundSystem* sound, int volume);

// Settings
void soundEnableMusic(SoundSystem* sound, bool enable);
void soundEnableSFX(SoundSystem* sound, bool enable);

// Utility functions
bool soundIsMusicPlaying(SoundSystem* sound);
bool soundIsMusicPaused(SoundSystem* sound);
int soundGetMasterVolume(SoundSystem* sound);
int soundGetMusicVolume(SoundSystem* sound);
int soundGetSFXVolume(SoundSystem* sound);
bool soundIsMusicEnabled(SoundSystem* sound);
bool soundIsSFXEnabled(SoundSystem* sound);

#endif
