#include "include/raylib.h"
#include "functions.h"
#include <math.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()
#include <string.h> // For strlen()
#include <stdio.h>

// Main game function
int main()  {
    // Initialization Stuff

    // Menu stuff and sound
    Texture2D menuBackground;
    Music menuMusic;
    Sound typingSound;
    Sound btnClick;
    Font customFont;

    // In-Game Sound
    Sound fruitCaughtSound;
    Sound gamePause;
    Sound playerCollision;

    // Overworld Textures
    Texture2D overworldBg;
    Texture2D overworldGrass;

    // Overworld Sound
    Music overworldMusic; // Background music for the overworld normal
    Music overworldMusic2; // Background music for the overworld mascot slightly angry
    Music overworldMusic3; // Background music for the overworld mascot angry

    // HorrorTextures
    Texture2D pentagram;

    // Player Textures
    Texture2D playerOverworldIdle;
    Texture2D playerOverworldRunLeft;
    Texture2D playerOverworldRunRight;
    

    // Mascot Textures
    // Texture2D overworldMascot1 = LoadTexture("textures/mascot/mascot1.png");
    // Texture2D overworldMascot2 = LoadTexture("textures/mascot/mascot2.png");


    int voicelineIndex = 0;
    bool briefingDone = false;
    float mascotY = 50.0f; // Initial Y position of the mascot
    float mascotYOffset = 0.0f; // Offset for floating effect
    float mascotFloatSpeed = 0.5f; // Speed of floating effect

    // Target fruits
    int targetFruits = rand() % 50 + 1; // Random number of fruits to gather (1 to 50)
    const char *voicelines[] = {
        "Welcome to the game! Your goal is to catch fruits for me.",
        "You need to gather %d fruits. Good luck!",
        "Get ready! The game will start soon."
    };

    // Initialization
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Catch_Fruits_Test1.exe");
    InitAudioDevice();
    SetTargetFPS(144);

    // Menu Stuff
    InitializeGame(&menuBackground, &menuMusic, &typingSound, &btnClick, &customFont);

    // Audio initialization
    PlayMusicStream(menuMusic);
    SetMusicVolume(menuMusic, 0.5f);

    const char *titleText = "Catch the Fruits"; // Title text

    typingSound = LoadSound("audios/menuVoice.ogg"); // Load typing sound
    SetSoundVolume(typingSound, 0.25f); // Set volume to 25%
    
    // Array of menu texts
    // Menu texts are displayed when the titleText is clicked
    const char *menuTexts[] = {
        "This is merely a test. Things might get weird",
        "Children's game is very chill and fun.",
        "When you hear a whisper in the game, open your earphones, and listen carefully",
        "You should try Jason's Roullete",
        "You can't escape from this game.",
        "You should try Adrian's Tetris",
        "The more you progress, the more it's not a game anymore.",
        "I love her",
        "I shouldn't have created it.",
        "Minecraft is a goated game",
        "If you see this, it's too late.",
        "Hello miss, if you see this, please give me HD hehe",
        "What have I done?"
    };
    int chatTextCount = sizeof(menuTexts) / sizeof(menuTexts[0]);
    const char *currentChatText = menuTexts[0]; // Initial chat text
    
    int fontTitleSize = 75;
    int fontChatSize = 25; // Larger font size for chat text
    int yOffset = 50;
    
    // Title text measurements
    Vector2 textTitleSize = MeasureTextEx(customFont, titleText, fontTitleSize, 1);
    int textTitleWidth = textTitleSize.x;
    int textTitleHeight = textTitleSize.y;

    // Menu text measurements
    Vector2 textMenuSize = MeasureTextEx(customFont, currentChatText, fontChatSize, 1);
    int textMenuWidth = textMenuSize.x;
    int textMenuHeight = textMenuSize.y;

    // Variables for glitch effect
    bool glitchEffect = false;
    float glitchTimer = 0.0f;
    float glitchDuration = 0.1f; // Duration of the glitch effect
    float glitchInterval = 10.0f; // Interval between glitch effects
    float glitchEffectTimer = 0.0f; // Timer for the glitch effect

    // Game Variables
    bool showChat = false; // State variable to track if the chat box should be displayed
    float elapsedTime = 0.0f;
    float scale  = 0.25f;
    int charIndex = 0; // Current character index for typing effect
    float charTime = 0.0f; // Time accumulator for typing effect

    srand(time(NULL)); // Seed the random number generator

    // In-Game Stuff
    // Variables for the game
    bool gameStarted = false;
    bool gameOver = false;
    int fruitsCaught = 0;
    int fruitsMissed = 0;
    bool darkTwist = false;

    // In-Game Sound
    // fruitCaughtSound = LoadSound("audios/catch.wav");
    // gamePause = LoadSound("audios/pause.wav");
    // playerCollision = LoadSound("audios/playerCollision.wav");

    // Mascot's Voicelines
    Sound voicelineSounds[3];
    voicelineSounds[0] = LoadSound("audios/Voicelines/Mascot/welcome.wav");
    voicelineSounds[1] = LoadSound("audios/Voicelines/Mascot/good_luck.wav");
    voicelineSounds[2] = LoadSound("audios/Voicelines/Mascot/get_ready.wav");

    // Check if voicelines are loaded
    for (int i = 0; i < 3; i++) {
        if (voicelineSounds[i].frameCount == 0) {
            printf("Failed to load voiceline %d\n", i);
        } else {
            printf("Loaded voiceline %d\n", i);
        }
    }
    float voicelineDelay = 0.5f; // Delay between voicelines in seconds
    float voicelineDelayTimer = 0.0f;
    bool voicelinePlaying = false;

    // Overworld Sound
    overworldMusic = LoadMusicStream("audios/BGM/overworldMusic1.ogg");
    overworldMusic2 = LoadMusicStream("audios/BGM/overworldMusic2.ogg");
    overworldMusic3 = LoadMusicStream("audios/BGM/overworldMusic3.ogg");

    // Load overworld textures
    overworldBg = LoadTexture("textures/world/overworld_bg.png");
    overworldGrass = LoadTexture("textures/world/grass.png");


    GameState gameState = MENU;

    while (!WindowShouldClose())    {
        UpdateMusicStream(menuMusic);
        elapsedTime += GetFrameTime();

        if (gameState == MENU) {
            glitchTimer += GetFrameTime();

            // Trigger glitch effect randomly
            if (glitchTimer >= glitchInterval)
            {
                glitchEffect = true;
                glitchTimer = 0.0f;
                glitchInterval = (float)(rand() % 10 + 5); // Random interval between 5 and 15 seconds
                glitchEffectTimer = 0.0f; // Reset the glitch effect timer
            }
        }

        BeginDrawing();

        // Apply glitch effect only in the main menu
        if (gameState == MENU && glitchEffect) {
            ClearBackground(BLACK);
            DrawRandomRedPixels(250, screenWidth, screenHeight); // Draw 100 random red pixels

            // Change title to "Who are you?" in red color
            const char *glitchTitleText = "Who are you?";
            int glitchTitleLength = strlen(glitchTitleText);
            for (int i = 0; i < glitchTitleLength; i++) {
                float waveOffset = sinf(elapsedTime * 2.0f + i * 0.5f) * 10.0f; // Adjust the wave speed and amplitude
                DrawTextEx(customFont, TextSubtext(glitchTitleText, i, 1), 
                        (Vector2){ (screenWidth - textTitleWidth) / 2 + i * (textTitleWidth / glitchTitleLength), 
                                    (screenHeight - textTitleHeight) / 4 - yOffset + waveOffset }, // Move title higher
                        fontTitleSize, 1, RED); // Change color to red
            }

            glitchEffectTimer += GetFrameTime();
            if (glitchEffectTimer >= glitchDuration) {
                glitchEffect = false; 
            }
        } else {
            ClearBackground(RAYWHITE);

            if (gameState == MENU) {
                float menuPicX = (screenWidth - menuBackground.width * scale) / 4; // Move to the left
                float menuPicY = (screenHeight - textTitleHeight) / 2 - menuBackground.height * scale - (yOffset + 50);
                DrawTextureEx(menuBackground, (Vector2){ menuPicX, menuPicY }, 0.0f, scale, WHITE);

                // Draw title text with wave effect
                // Using sine wave to create a wave effect on the title text
                const char *titleText = "Catch the Fruits";
                int titleLength = strlen(titleText);
                for (int i = 0; i < titleLength; i++) {
                    float waveOffset = sinf(elapsedTime * 2.0f + i * 0.5f) * 10.0f; // Adjust the wave speed and amplitude
                    DrawTextEx(customFont, TextSubtext(titleText, i, 1), 
                            (Vector2){ (screenWidth - textTitleWidth) / 2 + i * (textTitleWidth / titleLength), 
                                        (screenHeight - textTitleHeight) / 4 - yOffset + waveOffset }, // Move title higher
                            fontTitleSize, 1, BLACK);
                }

                // Draw chat text if the chat box is active
                if (showChat) {
                    float chatTextY = (screenHeight - textTitleHeight) / 4 + textTitleHeight + yOffset; // Position below title
                    DrawTextEx(customFont, TextSubtext(currentChatText, 0, charIndex), 
                            (Vector2){ (screenWidth - textMenuWidth) / 2, chatTextY }, 
                            fontChatSize, 1, BLACK);
                }

                // Draw menu buttons below the chat text
                int buttonGap = 20; // Gap between buttons
                Rectangle playButton = { screenWidth / 2 - 100, (screenHeight - textTitleHeight) / 4 + textTitleHeight + yOffset + textMenuHeight + 50, 200, 50 };
                Rectangle infoButton = { screenWidth / 2 - 100, playButton.y + playButton.height + buttonGap, 200, 50 };
                Rectangle quitButton = { screenWidth / 2 - 100, infoButton.y + infoButton.height + buttonGap, 200, 50 };

                Vector2 mousePosition = GetMousePosition();

                DrawRectangleRec(playButton, LIGHTGRAY);
                DrawRectangleRec(infoButton, LIGHTGRAY);
                DrawRectangleRec(quitButton, LIGHTGRAY);

                // Draw and handle hover effect for play button
                if (CheckCollisionPointRec(mousePosition, playButton)) {
                    DrawRectangleRec(playButton, LIGHTGRAY);
                    DrawRectangleLinesEx(playButton, 2, BLACK);
                    DrawTextEx(customFont, "Play", (Vector2){ playButton.x + 70, playButton.y + 10 }, 30, 1, BLACK);
                } else {
                    DrawRectangleRec(playButton, GRAY);
                    DrawTextEx(customFont, "Play", (Vector2){ playButton.x + 70, playButton.y + 10 }, 30, 1, BLACK);
                }

                // Draw and handle hover effect for info button
                if (CheckCollisionPointRec(mousePosition, infoButton)) {
                    DrawRectangleRec(infoButton, LIGHTGRAY);
                    DrawRectangleLinesEx(infoButton, 2, BLACK);
                    DrawTextEx(customFont, "Info", (Vector2){ infoButton.x + 70, infoButton.y + 10 }, 30, 1, BLACK);
                } else {
                    DrawRectangleRec(infoButton, GRAY);
                    DrawTextEx(customFont, "Info", (Vector2){ infoButton.x + 70, infoButton.y + 10 }, 30, 1, BLACK);
                }

                // Draw and handle hover effect for quit button
                if (CheckCollisionPointRec(mousePosition, quitButton)) {
                    DrawRectangleRec(quitButton, LIGHTGRAY);
                    DrawRectangleLinesEx(quitButton, 2, BLACK);
                    DrawTextEx(customFont, "Quit", (Vector2){ quitButton.x + 70, quitButton.y + 10 }, 30, 1, BLACK);
                } else {
                    DrawRectangleRec(quitButton, GRAY);
                    DrawTextEx(customFont, "Quit", (Vector2){ quitButton.x + 70, quitButton.y + 10 }, 30, 1, BLACK);
                }

                // Check button clicks
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePosition = GetMousePosition();
                    if (CheckCollisionPointRec(mousePosition, playButton)) {
                        PlaySound(btnClick);
                        gameState = GAME;
                    } else if (CheckCollisionPointRec(mousePosition, infoButton)) {
                        PlaySound(btnClick);
                        gameState = INFO;
                    } else if (CheckCollisionPointRec(mousePosition, quitButton)) {
                        PlaySound(btnClick);
                        CloseWindow();
                    } else if (CheckCollisionPointRec(mousePosition, (Rectangle){ (screenWidth - textTitleWidth) / 2, (screenHeight - textTitleHeight) / 4 - yOffset, textTitleWidth, textTitleHeight })) {
                        showChat = true; // Show the chat box

                        // Select a random chat text
                        int randomIndex = rand() % chatTextCount;
                        currentChatText = menuTexts[randomIndex];

                        // Reset typing effect
                        charIndex = 0;
                        charTime = 0.0f;
                    }
                }
            }
        }

        // Update typing effect
        if (showChat) {
            UpdateTypingEffect(&charTime, &charIndex, currentChatText, typingSound);
        }

        // Game state
        else if (gameState == GAME)
        {
            DrawWorld(overworldBg, overworldGrass, screenWidth, screenHeight);

            // Draw player's character
            Vector2 playerPosition = { screenWidth / 2 - playerOverworldIdle.width / 2, screenHeight - playerOverworldIdle.height - overworldGrass.height };
            DrawTexture(playerOverworldIdle, playerPosition.x, playerPosition.y, WHITE);

            // Draw mascot with floating effect
            // mascotYOffset = sinf(elapsedTime * mascotFloatSpeed) * 5.0f; 
            // Subtle floating effect
            // DrawTexture(overworldMascot1, screenWidth - overworldMascot1.width - 20, mascotY + mascotYOffset, WHITE);

            // Draw player's score
            DrawTextEx(customFont, TextFormat("Score: %d", fruitsCaught), (Vector2){ 20, 20 }, 30, 1, BLACK);

            // Briefing before the game starts
            if (!briefingDone) {
                const char *voiceline = TextFormat(voicelines[voicelineIndex], targetFruits);
                float voicelineY = screenHeight - 100; // Position at the bottom middle of the screen
                int voicelineWidth = MeasureTextEx(customFont, voiceline, 20, 1).x;
                int maxWidth = screenWidth - 40; // Adjust this value as needed

                // Wrap text if it exceeds the screen width
                if (voicelineWidth > maxWidth) {
                    int lineLength = maxWidth / MeasureTextEx(customFont, "W", 20, 1).x;
                    int lines = (voicelineWidth / maxWidth) + 1;
                    for (int i = 0; i < lines; i++) {
                        DrawTextEx(customFont, TextSubtext(voiceline, i * lineLength, lineLength), (Vector2){ (screenWidth - maxWidth) / 2, voicelineY + i * 25 }, 20, 1, BLUE);
                    }
                } else {
                    DrawTextEx(customFont, TextSubtext(voiceline, 0, charIndex), (Vector2){ (screenWidth - voicelineWidth) / 2, voicelineY }, 20, 1, YELLOW);
                }

                charTime += GetFrameTime();
                if (charTime >= 0.05f) { // Adjust the typing speed here
                    charTime = 0.0f;
                    if (charIndex < strlen(voiceline)) {
                        charIndex++;
                        if (!voicelinePlaying) {
                            printf("Playing voiceline %d\n", voicelineIndex); // Debug information
                            PlaySound(voicelineSounds[voicelineIndex]); // Play the voiceline sound effect
                            voicelinePlaying = true;
                        }
                    } else {
                        voicelineDelayTimer += GetFrameTime();
                        if (voicelineDelayTimer >= voicelineDelay) {
                            charIndex = 0;
                            voicelineIndex++;
                            voicelinePlaying = false;
                            voicelineDelayTimer = 0.0f;
                            if (voicelineIndex >= 3) {
                                briefingDone = true;

                                // Start countdown
                                for (int i = 3; i > 0; i--) {
                                    ClearBackground(RAYWHITE);
                                    DrawTextEx(customFont, TextFormat("%d", i), (Vector2){ screenWidth / 2 - 10, screenHeight / 2 - 10 }, 50, 1, BLACK);
                                    EndDrawing();
                                    WaitTime(1.0f);
                                    BeginDrawing();
                                }

                                gameStarted = true;
                            }
                        }
                    }
                }
            }

            // Game logic for catching fruits
            else if (!gameStarted)
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    gameStarted = true;
                }
            }

            else if (!gameOver)
            {
                // Game logic for catching fruits
                // If the player misses more than 7 fruits, trigger the dark twist
                if (fruitsMissed >= 7)
                {
                    darkTwist = true;
                    gameOver = true;
                }

                // Update game logic here
                // ...

                // Example: If the player catches a fruit
                // if (/* condition for catching a fruit */)
                // {
                //     fruitsCaught++;
                // }
                // // Example: If the player misses a fruit
                // else if (/* condition for missing a fruit */)
                // {
                //     fruitsMissed++;
                // }
            }
            // else
            // {
            //     if (darkTwist)
                
            // }
        }

        else if (gameState == INFO) {
            DrawInfo(customFont, screenWidth, screenHeight, btnClick, &gameState);
        }

        EndDrawing();
    }

    // De-Initialization
    void DeinitializeGame(Texture2D menuBackground, Music menuMusic, Sound typingSound, Sound btnClick, Font customFont, Texture2D overworldBackground, Texture2D grass, Music overworldMusic, Music overworldMusic2, Music overworldMusic3, Texture2D playerOverworldIdle, Texture2D playerOverworldRunLeft, Texture2D playerOverworldRunRight, Sound voicelineSounds[3]);


    return 0;
}

