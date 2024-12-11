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
    srand(time(NULL)); // Seed the random number generator

    // Menu stuff and sound
    Texture2D menuBackground;
    Music menuMusic;
    Sound typingSound;
    Sound btnClick;
    Font customFont;

    // In-Game Stuff
    Texture2D fruit;

    // In-Game Sound
    Sound fruitCaughtSound;
    Sound gamePause;
    Sound playerRun;
    Sound countdownSound;

    // Overworld Textures
    Texture2D overworldBg;
    Texture2D overworldGrass;

    // Overworld Sound
    Music overworldMusic; // Background music for the overworld normal
    Music overworldMusic2; // Background music for the overworld mascot slightly angry
    Music overworldMusic3; // Background music for the overworld mascot angry

    // Horror Textures
    Texture2D pentagram;

    // Player Textures
    Texture2D playerOverworldIdle;
    Texture2D playerOverworldRunLeft;
    Texture2D playerOverworldRunRight;
    Texture2D playerTexture = playerOverworldIdle;

    // Mascot Textures
    Texture2D mascotNormal;
    Texture2D mascotAngry;
    Texture2D mascotVeryAngry1;
    Texture2D mascotVeryAngry2;
    Texture2D mascotJumpscare;
    Texture2D mascotTexture = mascotNormal;

    // Mascot Sound
    Sound jumpscareSound;

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
    float glitchDuration = 2.0f; // Duration of the glitch effect
    float glitchInterval = 10.0f; // Interval between glitch effects
    float glitchEffectTimer = 0.0f; // Timer for the glitch effect

    // Game Variables
    bool showChat = false; // State variable to track if the chat box should be displayed
    float elapsedTime = 0.0f;
    float scale  = 0.25f;
    int charIndex = 0; // Current character index for typing effect
    float charTime = 0.0f; // Time accumulator for typing effect

    // In-Game Stuff
    // Variables for the game
    bool gameStarted = false;
    bool gameOver = false;
    int fruitsCaught = 0;
    int fruitsMissed = 0;

    // In-Game Sound
    fruitCaughtSound = LoadSound("audios/catch.wav");
    gamePause = LoadSound("audios/pause.wav");
    playerRun = LoadSound("audios/mc_dirt_run.ogg");
    countdownSound = LoadSound("audios/countdown.ogg");

    // Overworld Sound
    overworldMusic = LoadMusicStream("audios/BGM/overworldMusic1.ogg");
    overworldMusic2 = LoadMusicStream("audios/BGM/overworldMusic2.ogg");
    overworldMusic3 = LoadMusicStream("audios/BGM/overworldMusic3.ogg");

    // Set Overworld Music Volume
    SetMusicVolume(overworldMusic, 0.3f); // Set volume to 30%
    SetMusicVolume(overworldMusic2, 0.3f); // Set volume to 30%
    SetMusicVolume(overworldMusic3, 0.3f); // Set volume to 30%

    // Load Overworld Textures
    overworldBg = LoadTexture("textures/world/overworld_bg.png");
    overworldGrass = LoadTexture("textures/world/grass.png");

    // Load player textures
    playerOverworldIdle = LoadTexture("textures/player/playerOverworld_idle.png");
    playerOverworldRunLeft = LoadTexture("textures/player/playerOverworld_runLeft.png");
    playerOverworldRunRight = LoadTexture("textures/player/playerOverworld_runRight.png");

    // Player variables
    Vector2 playerPosition = { screenWidth / 2 - playerOverworldIdle.width / 2, screenHeight - playerOverworldIdle.height - overworldGrass.height - 10 };
    Vector2 playerVelocity = { 0, 0 };
    float acceleration = 0.3f;
    float maxSpeed = 2.0f;
    float friction = 0.9f;
    float playerScale = 2.0f; // Character's size


    // Load fruit textures
    Texture2D fruits[10];
    for (int i = 0; i < 10; i++) {
        char fileName[20];
        sprintf(fileName, "textures/fruits/fruit%d.png", i);
        fruits[i] = LoadTexture(fileName);
    }

    // Fruit variables
    int fruitCount = 10;
    Vector2 fruitPositions[10];
    float fruitSpeeds[10];
    for (int i = 0; i < 10; i++) {
        fruitSpeeds[i] = (float)(rand() % 2 + 1) * 0.5f; // Random speed between 0.5 and 1
    }

    int initialFruitCount = 1; // Start with 1 falling fruit
    int maxFruitCount = 10; // Maximum number of falling fruits
    float difficultyIncreaseInterval = 10.0f; // Interval to increase difficulty
    float difficultyTimer = 0.0f; // Timer to track difficulty increase

    GenerateFruits(fruitPositions, fruitCount, screenWidth);

    // Mascot Textures
    mascotNormal = LoadTexture("textures/mascot/mascot1.png");
    mascotAngry = LoadTexture("textures/mascot/mascot2.png");
    mascotVeryAngry1 = LoadTexture("textures/mascot/mascot3_1.png");
    mascotVeryAngry2 = LoadTexture("textures/mascot/mascot3_2.png");
    mascotJumpscare = LoadTexture("textures/mascot/jumpscare.png");
    mascotTexture = mascotNormal;

    // Mascot Variables
    float mascotY = 50.0f; // Initial Y position of the mascot
    float mascotYOffset = 0.0f; // Offset for floating effect
    float mascotFloatSpeed = 0.2f; // Speed of floating effect 
    float mascotFloatAmplitude = 5.0f; // Amplitude of floating effect
    float mascotScale = 3.0f; // Scale factor for the mascot
    int voicelineIndex = 0;
    bool briefingDone = false;
    const char *currentVoiceline = NULL;

    // Mascot's Voicelines
    Sound voicelineSounds[7];
    voicelineSounds[0] = LoadSound("audios/Voicelines/Mascot/welcome.wav");
    voicelineSounds[1] = LoadSound("audios/Voicelines/Mascot/good_luck.wav");
    voicelineSounds[2] = LoadSound("audios/Voicelines/Mascot/get_ready.wav");
    voicelineSounds[3] = LoadSound("audios/Voicelines/Mascot/dont_again.wav");
    voicelineSounds[4] = LoadSound("audios/Voicelines/Mascot/warning.wav");
    voicelineSounds[5] = LoadSound("audios/Voicelines/Mascot/consequences.wav");
    voicelineSounds[6] = LoadSound("audios/Voicelines/Mascot/congratulation.wav");

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


    // Load Horror Sound
    jumpscareSound = LoadSound("audios/nightmare_jumpscare.ogg");


    GameState gameState = MENU;

    while (!WindowShouldClose()) {
    UpdateMusicStream(menuMusic);
    UpdateMusicStream(overworldMusic);
    UpdateMusicStream(overworldMusic2);
    UpdateMusicStream(overworldMusic3);
    elapsedTime += GetFrameTime();

    // Handle P key press to return to main menu and reset the game state
    if (IsKeyPressed(KEY_P)) {
        if (gameState == GAME || gameState == INFO) {
            gameState = MENU;
            // Reset game variables
            gameStarted = false;
            gameOver = false;
            fruitsCaught = 0;
            fruitsMissed = 0;
            initialFruitCount = 1;
            for (int i = 0; i < 10; i++) {
                fruitSpeeds[i] = (float)(rand() % 2 + 1) * 0.5f; // Reset fruit speeds
                fruitPositions[i].y = -(rand() % 800); // Reset fruit positions
                fruitPositions[i].x = rand() % (screenWidth - 32);
            }
            mascotTexture = mascotNormal; // Reset mascot texture
            currentVoiceline = NULL; // Reset current voiceline
            charIndex = 0; // Reset typing effect
            charTime = 0.0f; // Reset typing effect timer
            voicelinePlaying = false; // Reset voiceline playing state
            voicelineDelayTimer = 0.0f; // Reset voiceline delay timer

            // Stop any currently playing voicelines
            for (int i = 0; i < 7; i++) {
                StopSound(voicelineSounds[i]);
            }

            // Stop in-game music and play main menu music
            StopMusicStream(overworldMusic);
            StopMusicStream(overworldMusic2);
            StopMusicStream(overworldMusic3);
            PlayMusicStream(menuMusic);
        } else if (gameState == MENU) {
            break; // Exit the loop to close the window
        }
    }

    if (gameState == MENU) {
        glitchTimer += GetFrameTime();

        // Trigger glitch effect randomly
        if (glitchTimer >= glitchInterval) {
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
        DrawRandomRedPixels(250, screenWidth, screenHeight); // Draw 250 random red pixels

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
                DrawTextEx(customFont, "Play", (Vector2){ playButton.x + (playButton.width - MeasureTextEx(customFont, "Play", 30, 1).x) / 2, playButton.y + 10 }, 30, 1, BLACK);
            } else {
                DrawRectangleRec(playButton, GRAY);
                DrawTextEx(customFont, "Play", (Vector2){ playButton.x + (playButton.width - MeasureTextEx(customFont, "Play", 30, 1).x) / 2, playButton.y + 10 }, 30, 1, BLACK);
            }

            // Draw and handle hover effect for info button
            if (CheckCollisionPointRec(mousePosition, infoButton)) {
                DrawRectangleRec(infoButton, LIGHTGRAY);
                DrawRectangleLinesEx(infoButton, 2, BLACK);
                DrawTextEx(customFont, "Info", (Vector2){ infoButton.x + (infoButton.width - MeasureTextEx(customFont, "Info", 30, 1).x) / 2, infoButton.y + 10 }, 30, 1, BLACK);
            } else {
                DrawRectangleRec(infoButton, GRAY);
                DrawTextEx(customFont, "Info", (Vector2){ infoButton.x + (infoButton.width - MeasureTextEx(customFont, "Info", 30, 1).x) / 2, infoButton.y + 10 }, 30, 1, BLACK);
            }

            // Draw and handle hover effect for quit button
            if (CheckCollisionPointRec(mousePosition, quitButton)) {
                DrawRectangleRec(quitButton, LIGHTGRAY);
                DrawRectangleLinesEx(quitButton, 2, BLACK);
                DrawTextEx(customFont, "Quit", (Vector2){ quitButton.x + (quitButton.width - MeasureTextEx(customFont, "Quit", 30, 1).x) / 2, quitButton.y + 10 }, 30, 1, BLACK);
            } else {
                DrawRectangleRec(quitButton, GRAY);
                DrawTextEx(customFont, "Quit", (Vector2){ quitButton.x + (quitButton.width - MeasureTextEx(customFont, "Quit", 30, 1).x) / 2, quitButton.y + 10 }, 30, 1, BLACK);
            }

            // Check button clicks
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePosition = GetMousePosition();
                if (CheckCollisionPointRec(mousePosition, playButton)) {
                    PlaySound(btnClick);
                    gameState = GAME;
                    StopMusicStream(menuMusic);
                    PlayMusicStream(overworldMusic);
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

            // Update typing effect
            if (showChat) {
                UpdateTypingEffect(&charTime, &charIndex, currentChatText, typingSound);
            }
        }

        // Game state
        else if (gameState == GAME) {
            showChat = false; // Hide the chat box

            // Draw world
            DrawWorld(overworldBg, overworldGrass, screenWidth, screenHeight);

            // Update player movement
            UpdatePlayerMovement(&playerPosition, &playerVelocity, acceleration, maxSpeed, friction, screenWidth, &playerTexture, playerOverworldRunLeft, playerOverworldRunRight, playerOverworldIdle, playerRun);

            // Draw player's character 
            DrawTextureEx(playerTexture, playerPosition, 0.0f, playerScale, WHITE);

            // Update and draw fruits
            if (gameStarted) {
                UpdateFruitPositions(fruitPositions, initialFruitCount, fruitSpeeds, screenWidth, screenHeight, fruits[0].height, &fruitsCaught, &fruitsMissed, playerPosition, playerOverworldIdle.width * playerScale, playerOverworldIdle.height * playerScale, &gameOver, fruitCaughtSound, mascotJumpscare, jumpscareSound, customFont, voicelineSounds, 7, pentagram);

                for (int i = 0; i < initialFruitCount; i++) {
                    DrawTextureEx(fruits[i % 10], fruitPositions[i], 0.0f, 1.5f, WHITE); // Scale the fruits by 1.5
                }

                // Check for good ending condition
                if (fruitsCaught >= targetFruits) {
                    gameOver = true;
                    TriggerGoodEnding(mascotJumpscare, voicelineSounds[6], customFont, screenWidth, screenHeight, voicelineSounds, 7);
                }

                // Increase difficulty over time
                difficultyTimer += GetFrameTime();
                if (difficultyTimer >= difficultyIncreaseInterval) {
                    difficultyTimer = 0.0f;
                    if (initialFruitCount < maxFruitCount) {
                        initialFruitCount++;
                    }
                    for (int i = 0; i < initialFruitCount; i++) {
                        fruitSpeeds[i] += 0.02f; // Increase fruit speed more slowly
                    }
                }
            }

            // Draw player's score
            DrawTextEx(customFont, TextFormat("Score: %d", fruitsCaught), (Vector2){ 20, 20 }, 30, 1, BLACK);

            // Draw fruits
            for (int i = 0; i < fruitCount; i++) {
                DrawTexture(fruits[i % 10], fruitPositions[i].x, fruitPositions[i].y, WHITE);
            }

            // Update mascot texture based on missed fruits
            if (fruitsMissed >= 3 && fruitsMissed < 5) {
                if (mascotTexture.id != mascotAngry.id) {
                    mascotTexture = mascotAngry;
                    currentVoiceline = "You don't want to do that again.";
                    PlaySound(voicelineSounds[3]); // Play new voiceline
                    charIndex = 0; // Reset typing effect
                    charTime = 0.0f;
                    StopMusicStream(overworldMusic);
                    PlayMusicStream(overworldMusic2);
                }
            } else if (fruitsMissed >= 5) {
                if (mascotTexture.id != mascotVeryAngry1.id && mascotTexture.id != mascotVeryAngry2.id) {
                    mascotTexture = mascotVeryAngry1;
                    currentVoiceline = "I am warning you to stop failing.";
                    PlaySound(voicelineSounds[4]); // Play new voiceline
                    charIndex = 0; // Reset typing effect
                    charTime = 0.0f;
                    StopMusicStream(overworldMusic2);
                    PlayMusicStream(overworldMusic3);
                }
            }

            // Apply glitch effect when mascot is very angry
            if (fruitsMissed >= 5) {
                glitchTimer += GetFrameTime();
                if (glitchTimer >= glitchInterval) {
                    glitchEffect = true;
                    glitchTimer = 0.0f;
                    glitchEffectTimer = 0.0f; // Reset the glitch effect timer
                }

                if (glitchEffect) {
                    glitchEffectTimer += GetFrameTime();
                    if (glitchEffectTimer >= glitchDuration) {
                        glitchEffect = false;
                    } else {
                        // Randomly switch between mascotVeryAngry1 and mascotVeryAngry2
                        if (rand() % 2 == 0) {
                            mascotTexture = mascotVeryAngry1;
                        } else {
                            mascotTexture = mascotVeryAngry2;
                        }
                    }
                }
            }

            // Draw mascot with smoother floating effect and larger size
            mascotYOffset = sinf(elapsedTime * mascotFloatSpeed) * mascotFloatAmplitude; // Smoother floating effect
            DrawTextureEx(mascotTexture, (Vector2){ screenWidth - mascotTexture.width * mascotScale - 20, mascotY + mascotYOffset }, 0.0f, mascotScale, WHITE);

            // Draw current voiceline as subtitle
            if (currentVoiceline != NULL) {
                float voicelineY = screenHeight - 100; // Position at the bottom middle of the screen
                int voicelineWidth = MeasureTextEx(customFont, currentVoiceline, 20, 1).x;
                int maxWidth = screenWidth - 40; // Adjust this value as needed

                // Wrap text if it exceeds the screen width
                if (voicelineWidth > maxWidth) {
                    int lineLength = maxWidth / MeasureTextEx(customFont, "W", 20, 1).x;
                    int lines = (voicelineWidth / maxWidth) + 1;
                    for (int i = 0; i < lines; i++) {
                        DrawTextEx(customFont, TextSubtext(currentVoiceline, i * lineLength, lineLength), (Vector2){ (screenWidth - maxWidth) / 2, voicelineY + i * 25 }, 20, 1, YELLOW);
                    }
                } else {
                    DrawTextEx(customFont, TextSubtext(currentVoiceline, 0, charIndex), (Vector2){ (screenWidth - voicelineWidth) / 2, voicelineY }, 20, 1, WHITE);
                }

                charTime += GetFrameTime();
                if (charTime >= 0.05f) { // Adjust the typing speed here
                    charTime = 0.0f;
                    if (charIndex < strlen(currentVoiceline)) {
                        charIndex++;
                    }
                }
            }

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
                                PlaySound(countdownSound); // Play countdown sound
                                for (int i = 3; i > 0; i--) {
                                    BeginDrawing();
                                    ClearBackground(RAYWHITE);
                                    DrawWorld(overworldBg, overworldGrass, screenWidth, screenHeight);
                                    DrawTextureEx(playerTexture, playerPosition, 0.0f, playerScale, WHITE);
                                    DrawTextureEx(mascotTexture, (Vector2){ screenWidth - mascotTexture.width * mascotScale - 20, mascotY + mascotYOffset }, 0.0f, mascotScale, WHITE);
                                    DrawTextEx(customFont, TextFormat("%d", i), (Vector2){ screenWidth / 2 - 10, screenHeight / 2 - 10 }, 50, 1, BLACK);
                                    EndDrawing();
                                    WaitTime(1.0f);
                                }
                                StopSound(countdownSound); // Stop countdown sound

                                gameStarted = true;
                            }
                        }
                    }
                }
            }

             // Apply darkness and creepiness based on missed fruits
            if (fruitsMissed >= 3 && fruitsMissed < 5) {
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.2f)); // Slightly darker
            } else if (fruitsMissed >= 5) {
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.4f)); // Much darker
                DrawRandomRedPixels(100, screenWidth, screenHeight); // Add some random red pixels for creepiness
            }
        }

        else if (gameState == INFO) {
            DrawInfo(customFont, screenWidth, screenHeight, btnClick, &gameState);
        }

        EndDrawing();
        }
    }

    // De-Initialization
    DeinitializeGame(menuBackground, menuMusic, typingSound, btnClick, customFont, overworldBg, overworldGrass, overworldMusic, overworldMusic2, overworldMusic3, playerOverworldIdle, playerOverworldRunLeft, playerOverworldRunRight, fruits, fruitCount, mascotNormal, mascotAngry, mascotVeryAngry1, mascotVeryAngry2, mascotJumpscare, pentagram, voicelineSounds, 7, fruitCaughtSound, gamePause, playerRun, countdownSound, jumpscareSound);

    return 0;
}
