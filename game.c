#include "include/raylib.h"
#include <math.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()
#include <string.h> // For strlen()

int main()
{
    Music music;
    Texture2D menuPic;
    Sound bruh;
    Sound chatSound;
    
    // Initialization
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Test");

    SetTargetFPS(144);

    // Audio initialization
    InitAudioDevice();
    music = LoadMusicStream("textures_audios/JoJo_s Bizarre Adventure_ Diamond is Unbreakable OST - Italian Restaurant.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.25f);
    bruh = LoadSound("textures_audios/vineboom.ogg");
    SetSoundVolume(bruh, 0.25f);
    chatSound = LoadSound("textures_audios/clink.ogg");
    SetSoundVolume(chatSound, 0.25f);

    // Load custom font
    Font customFont = LoadFontEx("fonts/Tickerpixel-monoitalic.otf", 32, 0, 0);

    // Menu Pic initialization
    menuPic = LoadTexture("textures_audios/pfp.png");

    const char *titleText = "Test";
    const char *textMenu = "press any key to start";
    
    // Array of chat texts
    const char *chatTexts[] = {
        "~wsg chat.",
        "~Guys, am I cool or cool?",
        "~Jarvis, unpiss my pants.",
        "~You guys should play deepwoken ong.",
        "~Soo...2024 actually wasn't my year.",
        "~It's official, I've gone insane.",
        "~I feel so skibidi.",
        "~I just lost my dawg.",
        "~Those who know...",
        "~I understand it now.",
        "~The voices in my walls are telling me I get 0 bitches."
    };
    int chatTextCount = sizeof(chatTexts) / sizeof(chatTexts[0]);
    const char *currentChatText = chatTexts[0]; // Initial chat text

    int fontSize = 20;
    int fontTitleSize = 75;
    int fontChatSize = 25; // Larger font size for chat text
    int yOffset = 50;
    
    // Title text measurements
    Vector2 textTitleSize = MeasureTextEx(customFont, titleText, fontTitleSize, 1);
    int textTitleWidth = textTitleSize.x;
    int textTitleHeight = textTitleSize.y;

    // Menu text measurements
    Vector2 textMenuSize = MeasureTextEx(customFont, textMenu, fontSize, 1);
    int textMenuWidth = textMenuSize.x;
    int textMenuHeight = textMenuSize.y;

    bool startMenu = true;
    bool showChat = false; // State variable to track if the chat box should be displayed
    float elapsedTime = 0.0f;
    float scale  = 0.25f;
    int charIndex = 0; // Current character index for typing effect
    float charTime = 0.0f; // Time accumulator for typing effect

    srand(time(NULL)); // Seed the random number generator

    // Variables for Russian Roulette game
    bool gameStarted = false;
    bool gameOver = false;
    bool playerAlive = true;
    bool aiAlive = true;
    bool playerTurn = true; // Flag to determine whose turn it is
    int bulletPosition = rand() % 6;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);
        elapsedTime += GetFrameTime();

        if (startMenu)
        {
            // Check if any key is pressed except ESC
            for (int key = 32; key < 349; key++) // Key codes range from 32 to 348
            {
                if (key != KEY_ESCAPE && IsKeyPressed(key))
                {
                    startMenu = false; // Transition to game state
                    break;
                }
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
            {
                Vector2 mousePosition = GetMousePosition();
                Rectangle menuPicBounds = {
                    (screenWidth - menuPic.width * scale) / 4, // Move to the left
                    (screenHeight - textTitleHeight) / 2 - menuPic.height * scale - (yOffset + 50),
                    menuPic.width * scale,
                    menuPic.height * scale
                };

                if (CheckCollisionPointRec(mousePosition, menuPicBounds))
                {
                    PlaySound(bruh); // Play the sound when the menuPic is clicked
                    showChat = true; // Show the chat box

                    // Select a random chat text
                    int randomIndex = rand() % chatTextCount;
                    currentChatText = chatTexts[randomIndex];

                    // Reset typing effect
                    charIndex = 0;
                    charTime = 0.0f;
                }
                else
                {
                    startMenu = false;
                }
            }
        }

        // Update typing effect
        if (showChat)
        {
            charTime += GetFrameTime();
            if (charTime >= 0.05f) // Adjust the typing speed here
            {
                charTime = 0.0f;
                if (charIndex < strlen(currentChatText))
                {
                    charIndex++;
                    PlaySound(chatSound); // Play the chat sound effect
                }
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (startMenu)
        {
            // Draw menu picture above the title, scaled down
            float menuPicX = (screenWidth - menuPic.width * scale) / 4; // Move to the left
            float menuPicY = (screenHeight - textTitleHeight) / 2 - menuPic.height * scale - (yOffset + 50);
            DrawTextureEx(menuPic, (Vector2){ menuPicX, menuPicY }, 0.0f, scale, WHITE);

            // Draw title text with wave effect
            int titleLength = strlen(titleText);
            for (int i = 0; i < titleLength; i++)
            {
                float waveOffset = sinf(elapsedTime * 2.0f + i * 0.5f) * 10.0f; // Adjust the wave speed and amplitude
                DrawTextEx(customFont, TextSubtext(titleText, i, 1), 
                           (Vector2){ (screenWidth - textTitleWidth) / 2 + i * (textTitleWidth / titleLength), 
                                      (screenHeight - textTitleHeight) / 2 - yOffset + waveOffset }, 
                           fontTitleSize, 1, BLACK);
            }

            // Draw menu text centered below the title
            DrawTextEx(customFont, textMenu, (Vector2){ (screenWidth - textMenuWidth) / 2, (screenHeight - textMenuHeight) / 2 + yOffset }, fontSize, 1, BLACK);

            // Draw chat text if the chat box is active
            if (showChat)
            {
                float chatTextY = menuPicY + (menuPic.height * scale / 2) - (fontChatSize / 2);
                DrawTextEx(customFont, TextSubtext(currentChatText, 0, charIndex), 
                           (Vector2){ menuPicX + menuPic.width * scale + 10, chatTextY }, 
                           fontChatSize, 1, BLACK);
            }
        }
        else if (!gameStarted)
        {
            // Start the Russian Roulette game
            if (IsKeyPressed(KEY_SPACE))
            {
                gameStarted = true;
            }
        }
        else if (!gameOver)
        {
            if (playerTurn)
            {
                DrawTextEx(customFont, "Press SPACE to pull the trigger", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Press SPACE to pull the trigger", 20, 1).x / 2, screenHeight / 2 - 10 }, 20, 1, BLACK);

                if (IsKeyPressed(KEY_SPACE))
                {
                    int triggerPull = rand() % 6;
                    if (triggerPull == bulletPosition)
                    {
                        playerAlive = false;
                        gameOver = true;
                        PlaySound(bruh);
                    }
                    else
                    {
                        PlaySound(chatSound);
                        playerTurn = false; // Switch to AI's turn
                    }
                }
            }
            else
            {
                DrawTextEx(customFont, "AI's turn...", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "AI's turn...", 20, 1).x / 2, screenHeight / 2 - 10 }, 20, 1, BLACK);

                // Simulate AI pulling the trigger after a short delay
                if (elapsedTime > 1.0f)
                {
                    int triggerPull = rand() % 6;
                    if (triggerPull == bulletPosition)
                    {
                        aiAlive = false;
                        gameOver = true;
                        PlaySound(bruh);
                    }
                    else
                    {
                        PlaySound(chatSound);
                        playerTurn = true; // Switch back to player's turn
                    }
                    elapsedTime = 0.0f; // Reset elapsed time for next turn
                }
            }
        }
        else
        {
            if (playerAlive)
            {
                DrawTextEx(customFont, "You survived!", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "You survived!", 32, 1).x / 2, screenHeight / 2 - 10 }, 32, 1, BLACK);
            }
            else
            {
                DrawTextEx(customFont, "You died!", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "You died!", 32, 1).x / 2, screenHeight / 2 - 10 }, 32, 1, BLACK);
            }

            if (!aiAlive)
            {
                DrawTextEx(customFont, "AI died!", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "AI died!", 32, 1).x / 2, screenHeight / 2 + 40 }, 32, 1, BLACK);
            }

            DrawTextEx(customFont, "Press R to restart", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Press R to restart", 20, 1).x / 2, screenHeight / 2 + 80 }, 20, 1, BLACK);

            if (IsKeyPressed(KEY_R))
            {
                gameStarted = false;
                gameOver = false;
                playerAlive = true;
                aiAlive = true;
                playerTurn = true;
                bulletPosition = rand() % 6;
            }
        }

        EndDrawing();
    }

    // De-Initialization
    StopMusicStream(music);
    UnloadFont(customFont);
    UnloadTexture(menuPic);
    UnloadSound(bruh);
    UnloadSound(chatSound);
    CloseAudioDevice(); // Close the audio device
    CloseWindow();

    return 0;
}