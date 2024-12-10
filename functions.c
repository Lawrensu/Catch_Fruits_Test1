#include "functions.h"
#include <stdlib.h> // For rand()
#include <string.h>

// Random red pixels
void DrawRandomRedPixels(int count, int screenWidth, int screenHeight) {
    for (int i = 0; i < count; i++) {
        int x = rand() % screenWidth;
        int y = rand() % screenHeight;
        DrawPixel(x, y, RED);
    }
}

void UpdateTypingEffect(float *charTime, int *charIndex, const char *currentChatText, Sound typingSound) {
    *charTime += GetFrameTime();
    if (*charTime >= 0.05f) { // Adjust the typing speed here
        *charTime = 0.0f;
        if (*charIndex < strlen(currentChatText)) {
            (*charIndex)++;
            PlaySound(typingSound); // Play the typing sound effect
        }
    }
}

void DrawInfo(Font customFont, int screenWidth, int screenHeight, Sound btnClick, GameState *gameState) {
    DrawTextEx(customFont, "Information about the game", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Information about the game", 32, 1).x / 2, screenHeight / 2 - 100 }, 32, 1, BLACK);
    DrawTextEx(customFont, "This game was developed by Lawrence Lian anak Matius Ding (102789563).", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "This game was developed by Lawrence Lian anak Matius Ding (102789563).", 20, 1).x / 2, screenHeight / 2 - 50 }, 20, 1, BLACK);
    DrawTextEx(customFont, "It is for my Introduction to Programming Project", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "It is for my Introduction to Programming Project", 20, 1).x / 2, screenHeight / 2 - 20 }, 20, 1, BLACK);
    DrawTextEx(customFont, "The objective is to catch as many fruits as possible and that's it...", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "The objective is to catch as many fruits as possible and that's it...", 20, 1).x / 2, screenHeight / 2 + 10 }, 20, 1, BLACK);
    DrawTextEx(customFont, "Press ESC or click Back to return to the menu.", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Press ESC or click Back to return to the menu.", 20, 1).x / 2, screenHeight / 2 + 50 }, 20, 1, BLACK);

    // Draw Back button
    Rectangle backButton = { screenWidth / 2 - 100, screenHeight / 2 + 100, 200, 50 };
    Vector2 mousePosition = GetMousePosition();

    // Draw and handle hover effect for back button
    if (CheckCollisionPointRec(mousePosition, backButton)) {
        DrawRectangleRec(backButton, LIGHTGRAY);
        DrawRectangleLinesEx(backButton, 2, BLACK);
        DrawTextEx(customFont, "Back", (Vector2){ backButton.x + 70, backButton.y + 10 }, 30, 1, BLACK);
    } else {
        DrawRectangleRec(backButton, GRAY);
        DrawTextEx(customFont, "Back", (Vector2){ backButton.x + 70, backButton.y + 10 }, 30, 1, BLACK);
    }

    // Check if Back button is clicked or ESC is pressed
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, backButton)) {
        *gameState = MENU;
        PlaySound(btnClick);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        *gameState = MENU;
        PlaySound(btnClick);
    }
}

// Load world textures
void LoadWorldTextures(Texture2D *overworldBg, Texture2D *grass) {
    *overworldBg = LoadTexture("textures/world/overworld_bg.png");
    *grass = LoadTexture("textures/world/grass.png");
}

// Draw world
void DrawWorld(Texture2D overworldBg, Texture2D grass, int screenWidth, int screenHeight) {
    // Draw background scaled to fit the screen
    DrawTexturePro(overworldBg, (Rectangle){0, 0, overworldBg.width, overworldBg.height}, (Rectangle){0, 0, screenWidth, screenHeight}, (Vector2){0, 0}, 0.0f, WHITE);

    // Draw grass tiles to cover the bottom of the screen
    for (int x = 0; x < screenWidth; x += grass.width) {
        DrawTexture(grass, x, screenHeight - grass.height, WHITE);
    }
}

// Initialize game
void InitializeGame(Texture2D *menuBackground, Music *menuMusic, Sound *typingSound, Sound *btnClick, Font *customFont) {
    *menuBackground = LoadTexture("textures_audios/pfp.png");
    *menuMusic = LoadMusicStream("audios/BGM/menuSoundtrack.ogg");
    *typingSound = LoadSound("audios/menuVoice.ogg");
    *btnClick = LoadSound("audios/btnClicked.wav");
    *customFont = LoadFontEx("fonts/BitPotion.ttf", 32, 0, 0);
}

// Deinitialize game
void DeinitializeGame(Texture2D menuBackground, Music menuMusic, Sound typingSound, Sound btnClick, Font customFont, Texture2D overworldBackground, Texture2D grass, Music overworldMusic, Music overworldMusic2, Music overworldMusic3, Texture2D playerOverworldIdle, Texture2D playerOverworldRunLeft, Texture2D playerOverworldRunRight, Sound voicelineSounds[3]) {
    UnloadTexture(menuBackground);
    UnloadMusicStream(menuMusic);
    UnloadSound(typingSound);
    UnloadSound(btnClick);
    UnloadFont(customFont);

    UnloadTexture(overworldBackground);
    UnloadTexture(grass);

    UnloadMusicStream(overworldMusic);
    UnloadMusicStream(overworldMusic2);
    UnloadMusicStream(overworldMusic3);

    UnloadTexture(playerOverworldIdle);
    UnloadTexture(playerOverworldRunLeft);
    UnloadTexture(playerOverworldRunRight);
    
    for (int i = 0; i < 3; i++) {
        UnloadSound(voicelineSounds[i]);
    }

    CloseAudioDevice();
    CloseWindow();
}

// Show jumpscare
void ShowJumpscare(Texture2D jumpscareTexture, int screenWidth, int screenHeight) {
    ClearBackground(BLACK);
    DrawTexture(jumpscareTexture, (screenWidth - jumpscareTexture.width) / 2, (screenHeight - jumpscareTexture.height) / 2, WHITE);
    EndDrawing();
    WaitTime(2.0f); // Display the jumpscare for 2 seconds
}

// Show congratulatory message
void ShowCongratulatoryMessage(Font customFont, int screenWidth, int screenHeight) {
    ClearBackground(BLACK);
    DrawTextEx(customFont, "Congratulations. You are a great vessel.", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Congratulations. You are a great vessel.", 32, 1).x / 2, screenHeight / 2 - 50 }, 32, 1, WHITE);
    EndDrawing();
    WaitTime(2.0f); // Display the message for 2 seconds
}