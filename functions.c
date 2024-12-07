#include "functions.h"
#include <stdlib.h> // For rand()

// Random red pixels
void DrawRandomRedPixels(int count, int screenWidth, int screenHeight) {
    for (int i = 0; i < count; i++) {
        int x = rand() % screenWidth;
        int y = rand() % screenHeight;
        DrawPixel(x, y, RED);
    }
}