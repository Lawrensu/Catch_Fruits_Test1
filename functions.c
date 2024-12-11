#include "functions.h"
#include <stdlib.h> // For rand()
#include <string.h>
#include <stdio.h> // For FILE, fopen, fprintf, fclose


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
    DrawTextEx(customFont, "Information about the game", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Information about the game", 32, 1).x / 2, screenHeight / 2 - 150 }, 32, 1, BLACK);
    DrawTextEx(customFont, "This game was developed by Lawrence Lian anak Matius Ding (102789563).", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "This game was developed by Lawrence Lian anak Matius Ding (102789563).", 20, 1).x / 2, screenHeight / 2 - 100 }, 20, 1, BLACK);
    DrawTextEx(customFont, "It is for my Introduction to Programming Project", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "It is for my Introduction to Programming Project", 20, 1).x / 2, screenHeight / 2 - 70 }, 20, 1, BLACK);
    DrawTextEx(customFont, "The objective is to catch as many fruits as possible and that's it...", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "The objective is to catch as many fruits as possible and that's it...", 20, 1).x / 2, screenHeight / 2 - 40 }, 20, 1, BLACK);
    DrawTextEx(customFont, "Controls:", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Controls:", 24, 1).x / 2, screenHeight / 2 }, 24, 1, BLACK);
    DrawTextEx(customFont, "Move Left: A or Left Arrow", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Move Left: A or Left Arrow", 20, 1).x / 2, screenHeight / 2 + 30 }, 20, 1, BLACK);
    DrawTextEx(customFont, "Move Right: D or Right Arrow", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Move Right: D or Right Arrow", 20, 1).x / 2, screenHeight / 2 + 60 }, 20, 1, BLACK);
    DrawTextEx(customFont, "Return to Menu: P", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Return to Menu: P", 20, 1).x / 2, screenHeight / 2 + 90 }, 20, 1, BLACK);
    DrawTextEx(customFont, "Press P or click Back to return to the menu.", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Press P or click Back to return to the menu.", 20, 1).x / 2, screenHeight / 2 + 120 }, 20, 1, BLACK);

    // Draw Back button
    Rectangle backButton = { screenWidth / 2 - 100, screenHeight / 2 + 150, 200, 50 };
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

    // Check if Back button is clicked or P is pressed
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, backButton)) {
        *gameState = MENU;
        PlaySound(btnClick);
    }
    if (IsKeyPressed(KEY_P)) {
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

// Player Movement
void UpdatePlayerMovement(Vector2 *playerPosition, Vector2 *playerVelocity, float acceleration, float maxSpeed, float friction, int screenWidth, Texture2D *playerTexture, Texture2D playerOverworldRunLeft, Texture2D playerOverworldRunRight, Texture2D playerOverworldIdle, Sound playerRun) {
    bool isRunning = false;

    // Handle input for acceleration
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        playerVelocity->x -= acceleration;
        *playerTexture = playerOverworldRunLeft;
        isRunning = true;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        playerVelocity->x += acceleration;
        *playerTexture = playerOverworldRunRight;
        isRunning = true;
    }

    // Apply friction
    playerVelocity->x *= friction;

    // Clamp velocity to max speed
    if (playerVelocity->x > maxSpeed) {
        playerVelocity->x = maxSpeed;
    }
    if (playerVelocity->x < -maxSpeed) {
        playerVelocity->x = -maxSpeed;
    }

    // Update player position
    playerPosition->x += playerVelocity->x;

    // Clamp player position to screen bounds
    if (playerPosition->x < 0) {
        playerPosition->x = 0;
    }
    if (playerPosition->x > screenWidth - playerTexture->width) {
        playerPosition->x = screenWidth - playerTexture->width;
    }

    // Set idle texture if no movement
    if (!isRunning) {
        *playerTexture = playerOverworldIdle;
    } else {
        // Play running sound
        if (!IsSoundPlaying(playerRun)) {
            PlaySound(playerRun);
        }
    }
}

// Random Fruits Generator
void GenerateFruits(Vector2 *fruitPositions, int fruitCount, int screenWidth) {
    for (int i = 0; i < fruitCount; i++) {
        fruitPositions[i].x = rand() % (screenWidth - 32); // Assuming fruit width is 32
        fruitPositions[i].y = -(rand() % 800); // Start above the screen
    }
}

// Update Fruit Position
void UpdateFruitPositions(Vector2 *fruitPositions, int fruitCount, float *fruitSpeeds, int screenWidth, int screenHeight, int fruitHeight, int *fruitsCaught, int *fruitsMissed, Vector2 playerPosition, int playerWidth, int playerHeight, bool *gameOver, Sound fruitCaughtSound, Texture2D mascotJumpscare, Sound jumpscareSound, Font customFont, Sound voicelineSounds[], int voicelineCount, Texture2D pentagram) {
    for (int i = 0; i < fruitCount; i++) {
        fruitPositions[i].y += fruitSpeeds[i];

        // Check if the fruit is caught by the player
        if (CheckCollisionRecs((Rectangle){fruitPositions[i].x, fruitPositions[i].y, 32, fruitHeight}, (Rectangle){playerPosition.x, playerPosition.y, playerWidth, playerHeight})) {
            (*fruitsCaught)++;
            PlaySound(fruitCaughtSound); // Play the fruit caught sound
            fruitPositions[i].y = -(rand() % 800); // Reset fruit position
            fruitPositions[i].x = rand() % (screenWidth - 32);
        }

        // Check if the fruit hits the ground
        if (fruitPositions[i].y > screenHeight - fruitHeight) {
            (*fruitsMissed)++;
            fruitPositions[i].y = -(rand() % 800); // Reset fruit position
            fruitPositions[i].x = rand() % (screenWidth - 32);

            // Check for game over condition
            if (*fruitsMissed >= 7) {
                *gameOver = true;
                TriggerBadEnding(mascotJumpscare, jumpscareSound, customFont, screenWidth, screenHeight, voicelineSounds, voicelineCount, pentagram);
            }
        }
    }
}

// Good Ending
void TriggerGoodEnding(Texture2D mascotJumpscare, Sound voicelineSound, Sound heartbeatSound, Font customFont, int screenWidth, int screenHeight, Sound voicelineSounds[], int voicelineCount) {
    // Stop any currently playing voicelines
    for (int i = 0; i < voicelineCount; i++) {
        StopSound(voicelineSounds[i]);
    }

    // Display "You are a great vessel." message with typing effect
    const char *greatVesselMessage = "Congratulation. You are a great vessel.";
    int charIndex = 0;
    float charTime = 0.0f;
    PlaySound(voicelineSounds[6]); // Play the voiceline sound
    PlaySound(heartbeatSound); // Play the heartbeat sound
    while (charIndex < strlen(greatVesselMessage)) {
        ClearBackground(BLACK);
        DrawTextEx(customFont, TextSubtext(greatVesselMessage, 0, charIndex), (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, greatVesselMessage, 32, 1).x / 2, screenHeight / 2 - 50 }, 32, 1, RED);
        EndDrawing();
        charTime += GetFrameTime();
        if (charTime >= 0.05f) { // Adjust the typing speed here
            charTime = 0.0f;
            charIndex++;
        }
    }
    WaitTime(4.0f); // Display the message for 4 seconds

    // Transition to white screen
    ClearBackground(WHITE);
    EndDrawing();
    WaitTime(1.0f); // Display the white screen for 1 second

    // Display "You proved yourself worthy." message with typing effect
    const char *worthyMessage = "You proved yourself worthy.";
    charIndex = 0;
    charTime = 0.0f;
    while (charIndex < strlen(worthyMessage)) {
        ClearBackground(WHITE);
        DrawTextEx(customFont, TextSubtext(worthyMessage, 0, charIndex), (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, worthyMessage, 32, 1).x / 2, screenHeight / 2 - 50 }, 32, 1, RED);
        EndDrawing();
        charTime += GetFrameTime();
        if (charTime >= 0.05f) { // Adjust the typing speed here
            charTime = 0.0f;
            charIndex++;
        }
    }
    WaitTime(3.0f); // Display the message for 3 seconds

    // Display message asking the player to check their game folder with typing effect
    const char *checkFolderMessage = "Check your game folder.";
    charIndex = 0;
    charTime = 0.0f;
    while (charIndex < strlen(checkFolderMessage)) {
        ClearBackground(WHITE);
        DrawTextEx(customFont, TextSubtext(checkFolderMessage, 0, charIndex), (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, checkFolderMessage, 32, 1).x / 2, screenHeight / 2 - 50 }, 32, 1, RED);
        EndDrawing();
        charTime += GetFrameTime();
        if (charTime >= 0.05f) { // Adjust the typing speed here
            charTime = 0.0f;
            charIndex++;
        }
    }
    WaitTime(2.0f); // Display the message for 2 seconds

    // Create the ritual_log.txt file
    FILE *file = fopen("ritual_log.txt", "w");
    if (file != NULL) {
        fprintf(file, "Ritual Entry 27. SUCCESS.\n");
        fprintf(file, "Subject: the person behind the screen.\n");
        fprintf(file, "Status: Vessel integrity confirmed. Fully bonded.\n\n");
        fprintf(file, "Completion Log:\n\n");
        fprintf(file, "- Blood sigil activated.\n");
        fprintf(file, "- Vessel consciousness subdued. No resistance detected.\n");
        fprintf(file, "- Invocation flawless. Ascension imminent.\n\n");
        fprintf(file, "Judgment: Vessel approved. Proceed to Phase II.\n");
        fclose(file);
    }

    // Exit the game
    CloseWindow();
    exit(0);
}

// Bad Ending
void TriggerBadEnding(Texture2D mascotJumpscare, Sound jumpscareSound, Font customFont, int screenWidth, int screenHeight, Sound voicelineSounds[], int voicelineCount, Texture2D pentagram) {
    // Stop any currently playing voicelines
    for (int i = 0; i < voicelineCount; i++) {
        StopSound(voicelineSounds[i]);
    }

    // Display "I warned you. Consequences" message with typing effect
    const char *warningMessage = "I warned you. Consequences";
    int charIndex = 0;
    float charTime = 0.0f;
    PlaySound(voicelineSounds[5]); // Play the voiceline sound
    while (charIndex < strlen(warningMessage)) {
        ClearBackground(BLACK);
        DrawTextEx(customFont, TextSubtext(warningMessage, 0, charIndex), (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, warningMessage, 32, 1).x / 2, screenHeight / 2 - 50 }, 32, 1, RED);
        EndDrawing();
        charTime += GetFrameTime();
        if (charTime >= 0.05f) { // Adjust the typing speed here
            charTime = 0.0f;
            charIndex++;
        }
    }
    WaitTime(3.0f); // Display the message for 3 seconds

    // Display pentagram texture
    ClearBackground(BLACK);
    DrawTexture(pentagram, (screenWidth - pentagram.width) / 2, (screenHeight - pentagram.height) / 2, WHITE);
    EndDrawing();
    WaitTime(2.0f); // Display the pentagram for 2 seconds

    // Transition to black screen and play jumpscare
    ClearBackground(BLACK);
    DrawTextureEx(mascotJumpscare, (Vector2){ (screenWidth - mascotJumpscare.width * 4.0f) / 2, (screenHeight - mascotJumpscare.height * 4.0f) / 2 }, 0.0f, 4.0f, WHITE);
    SetSoundVolume(jumpscareSound, 3.0f); // Make the jumpscare sound really loud
    PlaySound(jumpscareSound); // Play jumpscare sound
    EndDrawing();
    WaitTime(5.0f); // Display the jumpscare for 5 seconds

    // Display "You are a failed vessel." message with typing effect
    const char *failedMessage = "You are a failed vessel.";
    charIndex = 0;
    charTime = 0.0f;
    while (charIndex < strlen(failedMessage)) {
        ClearBackground(BLACK);
        DrawTextEx(customFont, TextSubtext(failedMessage, 0, charIndex), (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, failedMessage, 32, 1).x / 2, screenHeight / 2 - 50 }, 32, 1, RED);
        EndDrawing();
        charTime += GetFrameTime();
        if (charTime >= 0.05f) { // Adjust the typing speed here
            charTime = 0.0f;
            charIndex++;
        }
    }
    WaitTime(3.0f); // Display the message for 3 seconds

    // Display message asking the player to check their game folder with typing effect
    const char *checkFolderMessage = "Check your game folder.";
    charIndex = 0;
    charTime = 0.0f;
    while (charIndex < strlen(checkFolderMessage)) {
        ClearBackground(BLACK);
        DrawTextEx(customFont, TextSubtext(checkFolderMessage, 0, charIndex), (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, checkFolderMessage, 32, 1).x / 2, screenHeight / 2 - 50 }, 32, 1, RED);
        EndDrawing();
        charTime += GetFrameTime();
        if (charTime >= 0.05f) { // Adjust the typing speed here
            charTime = 0.0f;
            charIndex++;
        }
    }
    WaitTime(3.0f); // Display the message for 3 seconds

    // Create the ritual_log.txt file
    FILE *file = fopen("ritual_log.txt", "w");
    if (file != NULL) {
        fprintf(file, "Ritual Entry %d. FAILURE.\n", rand() % 1000);
        fprintf(file, "Subject: the person behind this screen.\n");
        fprintf(file, "Status: Vessel integrity compromised. Corrupted mind. Incomplete bonding.\n\n");
        fprintf(file, "Error Log:\n\n");
        fprintf(file, "- Blood sigil incomplete.\n");
        fprintf(file, "- Vessel consciousness resisting.\n");
        fprintf(file, "- Invocation disrupted by external interference (user actions detected).\n\n");
        fprintf(file, "Judgment: Vessel rejected. Dispose of remains will occur. Prepare for the next candidate.\n");
        fclose(file);
    }

    // Exit the game
    CloseWindow();
    exit(0);
}

// Reset Game State
void ResetGameState(Vector2 *playerPosition, Vector2 *playerVelocity, float playerScale, int screenWidth, int screenHeight, Vector2 fruitPositions[], float fruitSpeeds[], int fruitCount, int *fruitsCaught, int *fruitsMissed, int *initialFruitCount, int *targetFruits, bool *gameStarted, bool *gameOver, Texture2D *mascotTexture, Texture2D mascotNormal, Sound voicelineSounds[], int voicelineCount, Music menuMusic, Music overworldMusic, Music overworldMusic2, Music overworldMusic3, Texture2D playerOverworldIdle, Texture2D overworldGrass, bool *briefingDone, int *voicelineIndex, const char **currentVoiceline, int *charIndex, float *charTime, bool *voicelinePlaying, float *voicelineDelayTimer) {
    // Reset player position and velocity
    *playerPosition = (Vector2){ screenWidth / 2 - playerOverworldIdle.width / 2, screenHeight - playerOverworldIdle.height - overworldGrass.height - 10 };
    *playerVelocity = (Vector2){ 0, 0 };

    // Reset fruits' positions and speeds
    for (int i = 0; i < fruitCount; i++) {
        fruitSpeeds[i] = (float)(rand() % 2 + 1) * 0.5f; // Reset fruit speeds
        fruitPositions[i].y = -(rand() % 800); // Reset fruit positions
        fruitPositions[i].x = rand() % (screenWidth - 32);
    }

    // Reset game state variables
    *fruitsCaught = 0;
    *fruitsMissed = 0;
    *initialFruitCount = 1;
    *targetFruits = rand() % 50 + 1; // Random number of fruits to gather (1 to 50)
    *gameStarted = false;
    *gameOver = false;

    // Reset mascot texture
    *mascotTexture = mascotNormal;

    // Stop any currently playing voicelines
    for (int i = 0; i < voicelineCount; i++) {
        StopSound(voicelineSounds[i]);
    }

    // Stop in-game music and play main menu music
    StopMusicStream(overworldMusic);
    StopMusicStream(overworldMusic2);
    StopMusicStream(overworldMusic3);
    PlayMusicStream(menuMusic);

    // Reset voiceline and briefing variables
    *briefingDone = false;
    *voicelineIndex = 0;
    *currentVoiceline = NULL;
    *charIndex = 0;
    *charTime = 0.0f;
    *voicelinePlaying = false;
    *voicelineDelayTimer = 0.0f;
}


// Deinitialize game
void DeinitializeGame(Texture2D menuBackground, Music menuMusic, Sound typingSound, Sound btnClick, Font customFont, Texture2D overworldBackground, Texture2D grass, Music overworldMusic, Music overworldMusic2, Music overworldMusic3, Texture2D playerOverworldIdle, Texture2D playerOverworldRunLeft, Texture2D playerOverworldRunRight, Texture2D fruits[], int fruitCount, Texture2D mascotNormal, Texture2D mascotAngry, Texture2D mascotVeryAngry1, Texture2D mascotVeryAngry2, Texture2D mascotJumpscare, Texture2D pentagram, Sound voicelineSounds[], int voicelineCount, Sound fruitCaughtSound, Sound gamePause, Sound playerRun, Sound countdownSound, Sound jumpscareSound, Sound heartbeat) {
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

    for (int i = 0; i < fruitCount; i++) {
        UnloadTexture(fruits[i]);
    }

    UnloadTexture(mascotNormal);
    UnloadTexture(mascotAngry);
    UnloadTexture(mascotVeryAngry1);
    UnloadTexture(mascotVeryAngry2);
    UnloadTexture(mascotJumpscare);
    UnloadTexture(pentagram);

    for (int i = 0; i < voicelineCount; i++) {
        UnloadSound(voicelineSounds[i]);
    }

    UnloadSound(fruitCaughtSound);
    UnloadSound(gamePause);
    UnloadSound(playerRun);
    UnloadSound(countdownSound);
    UnloadSound(jumpscareSound);
    UnloadSound(heartbeat);

    CloseAudioDevice();
    CloseWindow();
}



