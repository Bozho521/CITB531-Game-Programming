#ifndef GAME_STATE_H
#define GAME_STATE_H

enum class GameState {
    MENU,
    PLAY,
    HOWTOPLAY,
    CREDITS,
    GAME_OVER,
    QUIT
};

struct HitEffect {
    int x, y;
    int timer;
    bool active;
};

#endif // GAME_STATE_H
