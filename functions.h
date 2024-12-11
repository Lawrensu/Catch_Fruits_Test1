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

void UpdatePlayerMovement(Vector2 *playerPosition, Vector2 *playerVelocity, float acceleration, float maxSpeed, float friction, int screenWidth, Texture2D *playerTexture, Texture2D playerOverworldRunLeft, Texture2D playerOverworldRunRight, Texture2D playerOverworldIdle, Sound playerRun);

void GenerateFruits(Vector2 *fruitPositions, int fruitCount, int screenWidth);

void UpdateFruitPositions(Vector2 *fruitPositions, int fruitCount, float *fruitSpeeds, int screenWidth, int screenHeight, int fruitHeight, int *fruitsCaught, int *fruitsMissed, Vector2 playerPosition, int playerWidth, int playerHeight, bool *gameOver, Sound fruitCaughtSound, Texture2D mascotJumpscare, Sound jumpscareSound, Font customFont, Sound voicelineSounds[], int voicelineCount, Texture2D pentagram);

void TriggerBadEnding(Texture2D mascotJumpscare, Sound jumpscareSound, Font customFont, int screenWidth, int screenHeight, Sound voicelineSounds[], int voicelineCount, Texture2D pentagram);

void DeinitializeGame(Texture2D menuBackground, Music menuMusic, Sound typingSound, Sound btnClick, Font customFont, Texture2D overworldBackground, Texture2D grass, Music overworldMusic, Music overworldMusic2, Music overworldMusic3, Texture2D playerOverworldIdle, Texture2D playerOverworldRunLeft, Texture2D playerOverworldRunRight, Texture2D fruits[], int fruitCount, Texture2D mascotNormal, Texture2D mascotAngry, Texture2D mascotVeryAngry1, Texture2D mascotVeryAngry2, Texture2D mascotJumpscare, Texture2D pentagram, Sound voicelineSounds[], int voicelineCount, Sound fruitCaughtSound, Sound gamePause, Sound playerRun, Sound countdownSound, Sound jumpscareSound);

void ShowJumpscare(Texture2D jumpscareTexture, int screenWidth, int screenHeight);

void ShowCongratulatoryMessage(Font customFont, int screenWidth, int screenHeight);

#endif // FUNCTIONS_H