#include "sound.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

bool soundInit(SoundSystem* sound) {
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    // Initialize volumes
    sound->masterVolume = MIX_MAX_VOLUME;
    sound->musicVolume = MIX_MAX_VOLUME / 2;
    sound->sfxVolume = MIX_MAX_VOLUME / 2;
    sound->soundEnabled = true;
    sound->musicEnabled = true;

    printf("🔊 Sound system initialized: soundEnabled=%d, musicEnabled=%d\n", 
           sound->soundEnabled, sound->musicEnabled);

    // Initialize arrays
    for (int i = 0; i < SOUND_COUNT; i++) {
        sound->soundEffects[i] = NULL;
    }
    for (int i = 0; i < MUSIC_COUNT; i++) {
        sound->music[i] = NULL;
    }

    return true;
}

void soundCleanup(SoundSystem* sound) {
    // Free sound effects
    for (int i = 0; i < SOUND_COUNT; i++) {
        if (sound->soundEffects[i]) {
            Mix_FreeChunk(sound->soundEffects[i]);
            sound->soundEffects[i] = NULL;
        }
    }

    // Free music
    for (int i = 0; i < MUSIC_COUNT; i++) {
        if (sound->music[i]) {
            Mix_FreeMusic(sound->music[i]);
            sound->music[i] = NULL;
        }
    }

    Mix_CloseAudio();
}

bool soundLoadEffects(SoundSystem* sound, const char* soundPath) {
    char filepath[256];
    
    // Check if sounds directory exists
    struct stat st;
    bool soundDirExists = (stat(soundPath, &st) == 0 && S_ISDIR(st.st_mode));

    // Load sound effects (using your actual files)
    snprintf(filepath, sizeof(filepath), "%s/pop.mp3", soundPath);
    sound->soundEffects[SOUND_BIT_COLLECT] = Mix_LoadWAV(filepath);

    snprintf(filepath, sizeof(filepath), "%s/wrong_bit.wav", soundPath);
    sound->soundEffects[SOUND_WRONG_BIT] = Mix_LoadWAV(filepath);

    snprintf(filepath, sizeof(filepath), "%s/level_complete.wav", soundPath);
    sound->soundEffects[SOUND_LEVEL_COMPLETE] = Mix_LoadWAV(filepath);

    snprintf(filepath, sizeof(filepath), "%s/powerup.wav", soundPath);
    sound->soundEffects[SOUND_POWERUP] = Mix_LoadWAV(filepath);

    snprintf(filepath, sizeof(filepath), "%s/game_over.wav", soundPath);
    sound->soundEffects[SOUND_GAME_OVER] = Mix_LoadWAV(filepath);

    snprintf(filepath, sizeof(filepath), "%s/menu_select.wav", soundPath);
    sound->soundEffects[SOUND_MENU_SELECT] = Mix_LoadWAV(filepath);

    // Only show warnings if sounds directory exists
    if (soundDirExists) {
        for (int i = 0; i < SOUND_COUNT; i++) {
            if (!sound->soundEffects[i]) {
                printf("Warning: Could not load sound effect %d: %s\n", i, Mix_GetError());
            } else {
                printf("✅ Loaded sound effect %d: %s\n", i, filepath);
            }
        }
    }

    return true;
}

bool soundLoadMusic(SoundSystem* sound, const char* musicPath) {
    char filepath[256];
    
    // Check if sounds directory exists
    struct stat st;
    bool soundDirExists = (stat(musicPath, &st) == 0 && S_ISDIR(st.st_mode));

    // Load music files (using your actual filenames)
    snprintf(filepath, sizeof(filepath), "%s/gamestart.mp3", musicPath);
    sound->music[MUSIC_MENU] = Mix_LoadMUS(filepath);

    snprintf(filepath, sizeof(filepath), "%s/background.mp3", musicPath);
    sound->music[MUSIC_BACKGROUND] = Mix_LoadMUS(filepath);

    snprintf(filepath, sizeof(filepath), "%s/gameover.mp3", musicPath);
    sound->music[MUSIC_GAME_OVER] = Mix_LoadMUS(filepath);

    // Only show warnings if sounds directory exists
    if (soundDirExists) {
        for (int i = 0; i < MUSIC_COUNT; i++) {
            if (!sound->music[i]) {
                printf("Warning: Could not load music %d: %s\n", i, Mix_GetError());
            }
        }
    }

    return true;
}

void soundPlayEffect(SoundSystem* sound, SoundEffect effect) {
    if (!sound->soundEnabled || !sound->soundEffects[effect]) {
        printf("❌ Cannot play sound effect %d: soundEnabled=%d, soundEffects[%d]=%p\n", 
               effect, sound->soundEnabled, effect, sound->soundEffects[effect]);
        return;
    }

    printf("🔊 Playing sound effect %d\n", effect);
    Mix_VolumeChunk(sound->soundEffects[effect], sound->sfxVolume);
    Mix_PlayChannel(-1, sound->soundEffects[effect], 0);
}

void soundPlayMusic(SoundSystem* sound, MusicType music) {
    if (!sound->soundEnabled || !sound->musicEnabled || !sound->music[music]) {
        return;
    }
    
    // Stop current music if playing
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    
    Mix_PlayMusic(sound->music[music], -1); // Loop indefinitely
    // Set volume with proper scaling
    int scaledVolume = (sound->masterVolume * sound->musicVolume) / MIX_MAX_VOLUME;
    Mix_VolumeMusic(scaledVolume);
}

void soundPlayMusicOnce(SoundSystem* sound, MusicType music) {
    if (!sound->soundEnabled || !sound->musicEnabled || !sound->music[music]) {
        return;
    }
    
    // Stop current music if playing
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    
    Mix_PlayMusic(sound->music[music], 0); // Play once (0 = no loop)
    // Set volume with proper scaling
    int scaledVolume = (sound->masterVolume * sound->musicVolume) / MIX_MAX_VOLUME;
    Mix_VolumeMusic(scaledVolume);
}

void soundStopMusic(SoundSystem* sound) {
    Mix_HaltMusic();
}

void soundPauseMusic(SoundSystem* sound) {
    Mix_PauseMusic();
}

void soundResumeMusic(SoundSystem* sound) {
    Mix_ResumeMusic();
}

void soundSetMasterVolume(SoundSystem* sound, int volume) {
    sound->masterVolume = volume;
    soundSetMusicVolume(sound, sound->musicVolume); // Update current volumes
    soundSetSFXVolume(sound, sound->sfxVolume);
}

void soundSetMusicVolume(SoundSystem* sound, int volume) {
    sound->musicVolume = volume;
    // Apply master volume scaling
    int scaledVolume = (sound->masterVolume * sound->musicVolume) / MIX_MAX_VOLUME;
    Mix_VolumeMusic(scaledVolume);
}

void soundSetSFXVolume(SoundSystem* sound, int volume) {
    sound->sfxVolume = volume;
    // Apply master volume scaling and update all loaded sound effects
    int scaledVolume = (sound->masterVolume * sound->sfxVolume) / MIX_MAX_VOLUME;
    for (int i = 0; i < SOUND_COUNT; i++) {
        if (sound->soundEffects[i]) {
            Mix_VolumeChunk(sound->soundEffects[i], scaledVolume);
        }
    }
}

void soundEnableMusic(SoundSystem* sound, bool enable) {
    sound->musicEnabled = enable;
    if (!enable) {
        Mix_PauseMusic();
    } else if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

void soundEnableSFX(SoundSystem* sound, bool enable) {
    sound->soundEnabled = enable;
}

// Utility functions
bool soundIsMusicPlaying(SoundSystem* sound) {
    return Mix_PlayingMusic() == 1;
}

bool soundIsMusicPaused(SoundSystem* sound) {
    return Mix_PausedMusic() == 1;
}

int soundGetMasterVolume(SoundSystem* sound) {
    return sound->masterVolume;
}

int soundGetMusicVolume(SoundSystem* sound) {
    return sound->musicVolume;
}

int soundGetSFXVolume(SoundSystem* sound) {
    return sound->sfxVolume;
}

bool soundIsMusicEnabled(SoundSystem* sound) {
    return sound->musicEnabled;
}

bool soundIsSFXEnabled(SoundSystem* sound) {
    return sound->soundEnabled;
}
