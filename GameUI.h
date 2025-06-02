#ifndef GAME_UI_H
#define GAME_UI_H

#include <SDL2/SDL.h>

struct Button {
    SDL_Rect position;
};

struct CursorPosition {
    int x, y;
};

#endif // GAME_UI_H
