#include "include/raylib.h"
#include <math.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()
#include <string.h> // For strlen()

int main()
{
    // Initialization
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Catching_Fruits_Test1.exe");
    SetTargetFPS(144);

    // Audio initialization
    InitAudioDevice();

    // Load custom font
    Font customFont = LoadFontEx("fonts/Tickerpixel-monoitalic.otf", 32, 0, 0);

    // Picture initialization
    Texture2D menuBg = LoadTexture("path/to/menuBg.png");

    const char *titleText = "Catch Fruits";
    
    // Array of chat texts
    const char *chatTexts[] = {
        "~wsg chat.",
        "~Guys, am I cool or cool?",
    };
    int chatTextCount = sizeof(chatTexts) / sizeof(chatTexts[0]);
    const char *currentChatText = chatTexts[0]; // Initial chat text

    int fontTitleSize = 75;
    int fontChatSize = 25; // Larger font size for chat text
    int yOffset = 50;
    
    // Title text measurements
    Vector2 textTitleSize = MeasureTextEx(customFont, titleText, fontTitleSize, 1);
    int textTitleWidth = textTitleSize.x;
    int textTitleHeight = textTitleSize.y;

    // Start game text measurements
    Vector2 textStartSize = MeasureTextEx(customFont, "Press SPACE to start the game", 20, 1);
    int textStartWidth = textStartSize.x;
    
    // Game state variables
    bool startMenu = true;
    bool showChat = false; // State variable to track if the chat box should be displayed
    float elapsedTime = 0.0f;
    float scale  = 0.25f;
    int charIndex = 0; // Current character index for typing effect
    float charTime = 0.0f; // Time accumulator for typing effect

    srand(time(NULL)); // Seed the random number generator

    while (!WindowShouldClose())
    {
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
                Rectangle menuBgBounds = {
                    (screenWidth - menuBg.width * scale) / 4, // Move to the left
                    (screenHeight - textTitleHeight) / 2 - menuBg.height * scale - (yOffset + 50),
                    menuBg.width * scale,
                    menuBg.height * scale
                };

                if (CheckCollisionPointRec(mousePosition, menuBgBounds))
                {
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
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLACK); // Set background to dark

        if (startMenu)
        {
            // Draw menu background above the title, scaled down
            float menuBgX = (screenWidth - menuBg.width * scale) / 4; // Move to the left
            float menuBgY = (screenHeight - textTitleHeight) / 2 - menuBg.height * scale - (yOffset + 50);
            DrawTextureEx(menuBg, (Vector2){ menuBgX, menuBgY }, 0.0f, scale, WHITE);

            // Draw title text with wave effect
            int titleLength = strlen(titleText);
            for (int i = 0; i < titleLength; i++)
            {
                float waveOffset = sinf(elapsedTime * 2.0f + i * 0.5f) * 10.0f; // Adjust the wave speed and amplitude
                DrawTextEx(customFont, TextSubtext(titleText, i, 1), 
                           (Vector2){ (screenWidth - textTitleWidth) / 2 + i * (textTitleWidth / titleLength), 
                                      (screenHeight - textTitleHeight) / 2 - yOffset + waveOffset }, 
                           fontTitleSize, 1, WHITE); // Set title text color to white
            }

            // Draw chat text if the chat box is active
            if (showChat)
            {
                float chatTextY = menuBgY + (menuBg.height * scale / 2) - (fontChatSize / 2);
                DrawTextEx(customFont, TextSubtext(currentChatText, 0, charIndex), 
                           (Vector2){ menuBgX + menuBg.width * scale + 10, chatTextY }, 
                           fontChatSize, 1, WHITE); // Set chat text color to white
            }
        }
        else
        {
            // Draw the start game text
            DrawTextEx(customFont, "Press SPACE to start the game", (Vector2){ screenWidth / 2 - textStartWidth / 2, screenHeight / 2 - 10 }, 20, 1, WHITE);

            // Start the game
            if (IsKeyPressed(KEY_SPACE))
            {
                // Transition to game state
            }
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(customFont);
    UnloadTexture(menuBg);
    CloseAudioDevice(); // Close the audio device
    CloseWindow();

    return 0;
}