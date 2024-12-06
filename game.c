#include "include/raylib.h"
#include <math.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()
#include <string.h> // For strlen()

typedef enum {
    MENU,
    GAME,
    INFO,
    GAME_OVER
} GameState;

int main()
{
    Music bgMusic;
    Texture2D menuBackground;
    Music menuMusic;
    
    // Initialization
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Catch_Fruits_Test1.exe");

    SetTargetFPS(144);

    // Audio initialization
    InitAudioDevice();
    menuMusic = LoadMusicStream("audios/BGM/menuSoundtrack.ogg");
    PlayMusicStream(menuMusic);
    SetMusicVolume(menuMusic, 0.5f);

    // Load custom font
    Font customFont = LoadFontEx("fonts/BitPotion.ttf", 32, 0, 0);

    // Menu Background initialization
    menuBackground = LoadTexture("textures_audios/pfp.png");

    const char *titleText = "Catch the Fruits";
    
    // Array of chat texts
    const char *chatTexts[] = {
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

    // Menu text
    const char *textMenu = "Menu";

    // Menu text measurements
    Vector2 textMenuSize = MeasureTextEx(customFont, textMenu, fontSize, 1);
    int textMenuWidth = textMenuSize.x;
    int textMenuHeight = textMenuSize.y;

    bool showChat = false; // State variable to track if the chat box should be displayed
    float elapsedTime = 0.0f;
    float scale  = 0.25f;
    int charIndex = 0; // Current character index for typing effect
    float charTime = 0.0f; // Time accumulator for typing effect

    srand(time(NULL)); // Seed the random number generator

    // Variables for the game
    bool gameStarted = false;
    bool gameOver = false;
    int fruitsCaught = 0;
    int fruitsMissed = 0;
    bool darkTwist = false;

    GameState gameState = MENU;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(menuMusic);
        elapsedTime += GetFrameTime();

        if (gameState == MENU)
        {
            // Check if any key is pressed except ESC
            for (int key = 32; key < 349; key++) // Key codes range from 32 to 348
            {
                if (key != KEY_ESCAPE && IsKeyPressed(key))
                {
                    gameState = GAME; // Transition to game state
                    break;
                }
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
            {
                Vector2 mousePosition = GetMousePosition();
                Rectangle menuPicBounds = {
                    (screenWidth - menuBackground.width * scale) / 4, // Move to the left
                    (screenHeight - textTitleHeight) / 2 - menuBackground.height * scale - (yOffset + 50),
                    menuBackground.width * scale,
                    menuBackground.height * scale
                };

                if (CheckCollisionPointRec(mousePosition, menuPicBounds))
                {
                    // PlaySound(); // Play the sound when the menuBackground is clicked
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
                    gameState = GAME;
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
                    // PlaySound(chatSound); // Play the chat sound effect
                }
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (gameState == MENU)
        {
            // Draw menu picture above the title, scaled down
            float menuPicX = (screenWidth - menuBackground.width * scale) / 4; // Move to the left
            float menuPicY = (screenHeight - textTitleHeight) / 2 - menuBackground.height * scale - (yOffset + 50);
            DrawTextureEx(menuBackground, (Vector2){ menuPicX, menuPicY }, 0.0f, scale, WHITE);

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

            // Draw menu buttons
            Rectangle playButton = { screenWidth / 2 - 100, screenHeight / 2, 200, 50 };
            Rectangle infoButton = { screenWidth / 2 - 100, screenHeight / 2 + 60, 200, 50 };
            Rectangle quitButton = { screenWidth / 2 - 100, screenHeight / 2 + 120, 200, 50 };

            DrawRectangleRec(playButton, LIGHTGRAY);
            DrawRectangleRec(infoButton, LIGHTGRAY);
            DrawRectangleRec(quitButton, LIGHTGRAY);

            DrawTextEx(customFont, "Play", (Vector2){ playButton.x + 70, playButton.y + 10 }, 30, 1, BLACK);
            DrawTextEx(customFont, "Info", (Vector2){ infoButton.x + 70, infoButton.y + 10 }, 30, 1, BLACK);
            DrawTextEx(customFont, "Quit", (Vector2){ quitButton.x + 70, quitButton.y + 10 }, 30, 1, BLACK);

            // Check button clicks
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePosition = GetMousePosition();
                if (CheckCollisionPointRec(mousePosition, playButton))
                {
                    gameState = GAME;
                }
                else if (CheckCollisionPointRec(mousePosition, infoButton))
                {
                    gameState = INFO;
                }
                else if (CheckCollisionPointRec(mousePosition, quitButton))
                {
                    CloseWindow();
                }
            }

            // Draw chat text if the chat box is active
            if (showChat)
            {
                float chatTextY = menuPicY + (menuBackground.height * scale / 2) - (fontChatSize / 2);
                DrawTextEx(customFont, TextSubtext(currentChatText, 0, charIndex), 
                           (Vector2){ menuPicX + menuBackground.width * scale + 10, chatTextY }, 
                           fontChatSize, 1, BLACK);
            }
        }
        else if (gameState == GAME)
        {
            // Game logic for catching fruits
            if (!gameStarted)
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
            else
            {
                if (darkTwist)
                {
                    DrawTextEx(customFont, "The game has taken a dark twist...", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "The game has taken a dark twist...", 32, 1).x / 2, screenHeight / 2 - 10 }, 32, 1, BLACK);
                }
                else
                {
                    DrawTextEx(customFont, "Game Over!", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Game Over!", 32, 1).x / 2, screenHeight / 2 - 10 }, 32, 1, BLACK);
                }

                DrawTextEx(customFont, "Press R to restart", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Press R to restart", 20, 1).x / 2, screenHeight / 2 + 40 }, 20, 1, BLACK);

                if (IsKeyPressed(KEY_R))
                {
                    gameStarted = false;
                    gameOver = false;
                    fruitsCaught = 0;
                    fruitsMissed = 0;
                    darkTwist = false;
                }
            }
        }
        else if (gameState == INFO)
        {
            DrawTextEx(customFont, "Information about the game", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Information about the game", 32, 1).x / 2, screenHeight / 2 - 10 }, 32, 1, BLACK);
            DrawTextEx(customFont, "Press ESC to return to menu", (Vector2){ screenWidth / 2 - MeasureTextEx(customFont, "Press ESC to return to menu", 20, 1).x / 2, screenHeight / 2 + 40 }, 20, 1, BLACK);

            if (IsKeyPressed(KEY_ESCAPE))
            {
                gameState = MENU;
            }
        }

        EndDrawing();
    }

    // De-Initialization
    StopMusicStream(menuMusic);
    UnloadFont(customFont);
    UnloadTexture(menuBackground);
    CloseAudioDevice(); // Close the audio device
    CloseWindow();

    return 0;
}