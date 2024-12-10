#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "include/raylib.h"

// Game state enumeration
typedef enum {
    MENU,
    GAME,
    INFO,
    GAME_OVER
} GameState;

// Function declarations
void DrawRandomRedPixels(int count, int screenWidth, int screenHeight);

void UpdateTypingEffect(float *charTime, int *charIndex, const char *currentChatText, Sound typingSound);

void DrawInfo(Font customFont, int screenWidth, int screenHeight, Sound btnClick, GameState *gameState);

void LoadWorldTextures(Texture2D *overworldBg, Texture2D *grass);

void DrawWorld(Texture2D overworldBg, Texture2D grass, int screenWidth, int screenHeight);

void InitializeGame(Texture2D *menuBackground, Music *menuMusic, Sound *typingSound, Sound *btnClick, Font *customFont);

void DeinitializeGame(Texture2D menuBackground, Music menuMusic, Sound typingSound, Sound btnClick, Font customFont, Texture2D overworldBackground, Texture2D grass, Music overworldMusic, Music overworldMusic2, Music overworldMusic3, Texture2D playerOverworldIdle, Texture2D playerOverworldRunLeft, Texture2D playerOverworldRunRight, Sound voicelineSounds[3]);

void ShowJumpscare(Texture2D jumpscareTexture, int screenWidth, int screenHeight);

void ShowCongratulatoryMessage(Font customFont, int screenWidth, int screenHeight);

#endif // FUNCTIONS_H