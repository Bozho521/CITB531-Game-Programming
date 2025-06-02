#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

#include <SDL2/SDL.h>

struct Player
{
    SDL_Rect rect;
};

struct PlayerProjectile {
    SDL_Rect rect;
    int speed;
    bool isActive;
    SDL_Texture *texture;
};

struct Enemy {
    SDL_Rect rect;
    int speed;
    bool isActive;
    SDL_Texture* texture;
};

struct MiniBoss {
    SDL_Rect rect;
    int health;
    int speed;
    bool isActive;
    SDL_Texture* texture;
};

struct MiniBossProjectile {
    SDL_Rect rect;
    int speed;
    bool isActive;
    SDL_Texture* texture;
};

struct MegaBossPart {
    SDL_Texture* texture;
    SDL_Rect rect;
    bool isActive;
    int speed;
};

struct MegaBoss {
    MegaBossPart body;
    MegaBossPart leftArm;
    MegaBossPart rightArm;
    int health;
    bool isActive;
};

struct Portal
{
    SDL_Rect rect;
    SDL_Texture *texture;
    bool isActive;
};

struct Star
{
    int x, y, speed;
};

struct Laser
{
    SDL_Rect rect;
    SDL_Color color;
    bool isActive;
};

#endif // GAME_ENTITIES_H
