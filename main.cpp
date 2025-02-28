#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 50;
const int TOTAL_BUTTONS = 4;

const int MAX_PROJECTILES = 4;
const int MAX_ENEMIES = 5;

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
SDL_Texture *gBackgroundTexture = nullptr;
SDL_Texture *gBackgroundTextureStart = nullptr;

struct Button
{
    SDL_Rect position;
};

struct Projectile
{
    SDL_Rect rect;
    int speed;
    bool isActive;
    SDL_Texture *texture;
};

struct Enemy
{
    SDL_Rect rect;
    int speed;
    bool isActive;
    SDL_Texture *texture;
};

struct CursorPosition
{
    int x, y;
};

struct Boss
{
    SDL_Rect rect;
    int health;
    int speed;
    bool isActive;
    SDL_Texture *texture;
};

struct BossProjectile
{
    SDL_Rect rect;
    int speed;
    bool isActive;
    SDL_Texture *texture;
};

enum class GameState
{
    MENU,
    GAMEPLAY
};
GameState currentGameState = GameState::MENU;

Button buttons[TOTAL_BUTTONS];

bool init();
bool loadMedia();
void close();
SDL_Texture *loadTexture(const char *path);

bool quit = false;
SDL_Event e;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        gWindow = SDL_CreateWindow("Galaga Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr)
            {
                std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
                return false;
            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }
    return true;
}

bool loadMedia()
{
    gBackgroundTexture = loadTexture("visuals/menu.bmp");
    if (gBackgroundTexture == nullptr)
    {
        std::cerr << "Failed to load background texture!" << std::endl;
        return false;
    }
    buttons[0].position = {540, 270, BUTTON_WIDTH, BUTTON_HEIGHT};
    buttons[1].position = {390, 450, BUTTON_WIDTH + 300, BUTTON_HEIGHT};
    buttons[2].position = {480, 635, BUTTON_WIDTH + 120, BUTTON_HEIGHT};
    buttons[3].position = {540, 822, BUTTON_WIDTH, BUTTON_HEIGHT};

    return true;
}

SDL_Texture *loadTexture(const char *path)
{
    SDL_Texture *newTexture = nullptr;
    SDL_Surface *loadedSurface = SDL_LoadBMP(path);
    if (loadedSurface == nullptr)
    {
        std::cerr << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr)
        {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void close()
{
    SDL_DestroyTexture(gBackgroundTexture);
    gBackgroundTexture = nullptr;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    SDL_Quit();
}

bool checkCollision(const SDL_Rect &projectileRect, const SDL_Rect &enemyRect)
{
    return SDL_HasIntersection(&projectileRect, &enemyRect);
}

void renderGameOver(SDL_Renderer *renderer, SDL_Texture *gameOverTexture, SDL_Texture *digitTextures[], SDL_Texture *scoreTexture, int score)
{
    int digitWidth = 60;
    int digitHeight = 90;
    int scoreLength = score > 0 ? (int)log10(score) + 1 : 1;
    int scorePositionX = 980 / 2 - (scoreLength * digitWidth) / 2;
    int scorePositionY = 750 * 3 / 4;

    SDL_Rect gameOverRect = {980 / 2 - (scoreLength * digitWidth) / 2, 750 / 8, 980 / 2, 750 / 4};
    SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
    SDL_Rect scoreRect = {980 / 2 - (scoreLength * digitWidth) / 2, 750 / 2, 980 / 6, 750 / 6};
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    for (int i = 0; i < scoreLength; ++i)
    {
        int digit = (int)(score / pow(10, scoreLength - i - 1)) % 10;
        SDL_Rect digitRect = {scorePositionX + i * digitWidth, scorePositionY, digitWidth, digitHeight};
        SDL_RenderCopy(renderer, digitTextures[digit], NULL, &digitRect);
    }
}

CursorPosition getCurrentCursorPosition()
{
    CursorPosition pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

void clearEnemies(Enemy enemies[], int maxEnemies)
{
    for (int i = 0; i < maxEnemies; ++i)
    {
        enemies[i].isActive = false;
        enemies[i].rect.x = -100;
        enemies[i].rect.y = -100;
    }
}

void startGame()
{
    const int WIDTH = 980, HEIGHT = 750;
    bool boss_flag = true;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Armagedon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == nullptr)
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *backgroundSurface = SDL_LoadBMP("visuals/background.bmp");
    SDL_Surface *gameOverSurface = SDL_LoadBMP("visuals/gameover.bmp");
    SDL_Surface *scoreSurface = SDL_LoadBMP("visuals/score.bmp");
    SDL_Surface *planeSurface = SDL_LoadBMP("visuals/plane.bmp");
    SDL_Surface *projectileSurface = SDL_LoadBMP("visuals/projectile.bmp");
    SDL_Surface *enemySurface = SDL_LoadBMP("visuals/enemy.bmp");

    if (backgroundSurface == nullptr || gameOverSurface == nullptr || scoreSurface == nullptr || planeSurface == nullptr || projectileSurface == nullptr || enemySurface == nullptr)
    {
        std::cout << "Could not load image: " << SDL_GetError() << std::endl;
    }

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_Texture *gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_Texture *planeTexture = SDL_CreateTextureFromSurface(renderer, planeSurface);
    SDL_Texture *projectileTexture = SDL_CreateTextureFromSurface(renderer, projectileSurface);
    SDL_Texture *enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);

    SDL_FreeSurface(backgroundSurface);
    SDL_FreeSurface(gameOverSurface);
    SDL_FreeSurface(scoreSurface);

    SDL_FreeSurface(planeSurface);
    SDL_FreeSurface(projectileSurface);
    SDL_FreeSurface(enemySurface);

    const int NUM_DIGITS = 10;
    SDL_Texture *digitTextures[NUM_DIGITS];

    for (int i = 0; i < 10; ++i)
    {
        char imagePath[50];
        sprintf(imagePath, "visuals/digit_%d.bmp", i);
        std::cout << "Loading image: " << imagePath << std::endl;

        SDL_Surface *digitSurface = SDL_LoadBMP(imagePath);

        if (digitSurface == nullptr)
        {
            std::cout << "Could not load digit image: " << SDL_GetError() << std::endl;
        }
        else
        {
            digitTextures[i] = SDL_CreateTextureFromSurface(renderer, digitSurface);
            SDL_FreeSurface(digitSurface);
        }
    }

    SDL_ShowCursor(SDL_DISABLE);

    SDL_Event windowEvent;

    Projectile projectiles[MAX_PROJECTILES];

    Boss boss;
    boss.isActive = false;
    boss.rect = {WIDTH / 2 - 150, -300, 500, 325}; 
    boss.health = 100;
    boss.speed = 1;
    boss.texture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("visuals/enemy.bmp"));

    const int MAX_BOSS_PROJECTILES = 4;
    BossProjectile bossProjectiles[MAX_BOSS_PROJECTILES];

    for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
    {
        bossProjectiles[i].isActive = false;
        bossProjectiles[i].speed = 1;
        bossProjectiles[i].rect.w = 20;
        bossProjectiles[i].rect.h = 20;
        bossProjectiles[i].texture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("visuals/enemy.bmp"));
    }

    for (int i = 0; i < MAX_PROJECTILES; ++i)
    {
        projectiles[i].isActive = false;
        projectiles[i].speed = 1;
        projectiles[i].rect.w = 10;
        projectiles[i].rect.h = 10;
        projectiles[i].texture = projectileTexture;
    }

    Enemy enemies[MAX_ENEMIES];

    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        enemies[i].isActive = false;
        enemies[i].speed = 1;
        enemies[i].rect.w = rand() % 150 + 50;
        enemies[i].rect.h = rand() % 150 + 50;
        enemies[i].texture = enemyTexture;
    }

    int enemySpawnTimer = 0;
    const int ENEMY_SPAWN_INTERVAL = 60;
    int ENEMY_SPEED_DIVISOR = 1;
    int cnter = 0;

    int score = 0;
    const int ENEMY_SPEED = 1;

    bool gameRunning = true;
    bool gameOver = false;
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
            if (SDL_MOUSEBUTTONDOWN == windowEvent.type)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                int wingAXOffset = 5;
                int wingBXOffset = 60;
                int wingYOffset = 10;

                if (!projectiles[0].isActive && !projectiles[1].isActive)
                {
                    projectiles[0].isActive = true;
                    projectiles[1].isActive = true;

                    projectiles[0].rect.x = mouseX + wingAXOffset;
                    projectiles[0].rect.y = mouseY + wingYOffset;

                    projectiles[1].rect.x = mouseX + wingBXOffset;
                    projectiles[1].rect.y = mouseY + wingYOffset;
                }
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_Rect imageRect = {mouseX, mouseY, 75, 75};
        SDL_Rect imageRect2 = {mouseX, mouseY, 25, 25};

        SDL_RenderCopy(renderer, planeTexture, NULL, &imageRect);

        enemySpawnTimer++;
        if (enemySpawnTimer >= ENEMY_SPAWN_INTERVAL)
        {
            enemySpawnTimer = 0;

            for (int i = 0; i < MAX_ENEMIES; ++i)
            {
                if (!enemies[i].isActive)
                {
                    enemies[i].isActive = true;
                    enemies[i].rect.x = rand() % (WIDTH - enemies[i].rect.w);
                    enemies[i].rect.y = -enemies[i].rect.h;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_ENEMIES; ++i)
        {
            if (enemies[i].isActive && !boss.isActive)
            {
                if (score < 100 || i % 25 != 0)
                {
                    enemies[i].rect.y += ENEMY_SPEED / ENEMY_SPEED_DIVISOR;

                    if (i % 5 == 0)
                    {
                        enemies[i].rect.x += ENEMY_SPEED / ENEMY_SPEED_DIVISOR;
                    }
                    else if (i % 4 == 0)
                    {
                        enemies[i].rect.x -= ENEMY_SPEED / ENEMY_SPEED_DIVISOR;
                    }

                    if (ENEMY_SPEED_DIVISOR == 1)
                    {
                        ENEMY_SPEED_DIVISOR = 2;
                        cnter = 0;
                    }
                    else if (cnter == 12)
                    {
                        ENEMY_SPEED_DIVISOR = 1;
                        cnter = 0;
                    }
                    else
                    {
                        cnter++;
                    }
                }
                else
                {
                    CursorPosition pos = getCurrentCursorPosition();
                    float floatPosX = enemies[i].rect.x;
                    float floatPosY = enemies[i].rect.y;

                    int dx = pos.x - floatPosX;
                    int dy = pos.y - floatPosY;
                    float distance = sqrt(dx * dx + dy * dy);

                    if (distance > 0)
                    {
                        float nx = dx / distance; 
                        float ny = dy / distance;

                        floatPosX += nx * (ENEMY_SPEED / (float)ENEMY_SPEED_DIVISOR);
                        floatPosY += ny * (ENEMY_SPEED / (float)ENEMY_SPEED_DIVISOR);

                        enemies[i].rect.x = static_cast<int>(round(floatPosX));
                        enemies[i].rect.y = static_cast<int>(round(floatPosY));
                    }

                    if (ENEMY_SPEED_DIVISOR == 1)
                    {
                        ENEMY_SPEED_DIVISOR = 2;
                        cnter = 0;
                    }
                    else if (cnter == 12)
                    {
                        ENEMY_SPEED_DIVISOR = 1;
                        cnter = 0;
                    }
                    else
                    {
                        cnter++;
                    }
                }

                if (enemies[i].rect.y > HEIGHT)
                {
                    enemies[i].isActive = false;
                    enemies[i].rect.x = rand() % (WIDTH - enemies[i].rect.w);
                    enemies[i].rect.y = -enemies[i].rect.h;
                    enemies[i].rect.w = rand() % 150 + 50;
                    enemies[i].rect.h = rand() % 150 + 50;
                    continue;
                }

                SDL_RenderCopy(renderer, enemies[i].texture, NULL, &enemies[i].rect);
            }
            else if(!boss.isActive)
            {
                enemies[i].isActive = true;
                enemies[i].rect.x = rand() % (WIDTH - enemies[i].rect.w);
                enemies[i].rect.y = -enemies[i].rect.h;
                enemies[i].rect.w = rand() % 150 + 50;
                enemies[i].rect.h = rand() % 150 + 50;
            }

                if (score >= 30 && boss_flag)
                {
                    boss_flag = false;
                    boss.isActive = true;
                    clearEnemies(enemies, MAX_ENEMIES);
                    SDL_Delay(200);
                }

                if (boss.isActive)
                {
                    if (boss.rect.y < 50)
                    {
                        boss.rect.y += boss.speed / ENEMY_SPEED_DIVISOR;
                    }
                    else
                    {
                        static int direction = 1;
                        boss.rect.x += boss.speed * direction / ENEMY_SPEED_DIVISOR;
                        if (boss.rect.x <= 0 || boss.rect.x + boss.rect.w >= WIDTH)
                        {
                            direction *= -1;
                        }
                        if(boss.rect.x < 0 || boss.rect.x > 1280)
                        {
                            boss.rect.x = WIDTH / 2 - 150;
                        }

                        for (int j = 0; j < MAX_PROJECTILES; ++j)
                        {
                            if (projectiles[j].isActive && checkCollision(projectiles[j].rect, boss.rect))
                            {
                                projectiles[j].isActive = false;
                                boss.health -= 10;
                                if (boss.health <= 0)
                                {
                                    boss.isActive = false;
                                    std::cout << "Boss defeated!" << std::endl;
                                }
                            }
                        }
                    }

                    static int bossShootTimer = 0;
                    bossShootTimer++;
                    if (bossShootTimer >= 3600)
                    {
                        bossShootTimer = 0;

                        for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
                        {
                            if (!bossProjectiles[i].isActive)
                            {
                                bossProjectiles[i].isActive = true;
                                bossProjectiles[i].rect.x = boss.rect.x + boss.rect.w / 2 - bossProjectiles[i].rect.w / 2;
                                bossProjectiles[i].rect.y = boss.rect.y + boss.rect.h;
                                break;
                            }
                        }
                    }

                    if (ENEMY_SPEED_DIVISOR == 1)
                    {
                        ENEMY_SPEED_DIVISOR = 2;
                        cnter = 0;
                    }
                    else if (cnter == 12)
                    {
                        ENEMY_SPEED_DIVISOR = 1;
                        cnter = 0;
                    }
                    else
                    {
                        cnter++;
                    }

                    SDL_RenderCopy(renderer, boss.texture, NULL, &boss.rect);

                    for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
                    {
                        if (bossProjectiles[i].isActive)
                        {
                            bossProjectiles[i].rect.y += bossProjectiles[i].speed / ENEMY_SPEED_DIVISOR;
                            if (ENEMY_SPEED_DIVISOR == 1)
                            {
                                ENEMY_SPEED_DIVISOR = 2;
                                cnter = 0;
                            }
                            else if (cnter == 12)
                            {
                                ENEMY_SPEED_DIVISOR = 1;
                                cnter = 0;
                            }
                            else
                            {
                                cnter++;
                            }
                            if (bossProjectiles[i].rect.y > HEIGHT)
                            {
                                bossProjectiles[i].isActive = false;
                            }
                            SDL_RenderCopy(renderer, bossProjectiles[i].texture, NULL, &bossProjectiles[i].rect);

                            if (checkCollision(bossProjectiles[i].rect, imageRect))
                            {
                                std::cout << "Player hit by boss projectile!" << std::endl;
                                SDL_Delay(2000);
                                SDL_RenderClear(renderer);
                                renderGameOver(renderer, gameOverTexture, digitTextures, scoreTexture, score);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(2000);
                                score = 0;
                                SDL_DestroyTexture(backgroundTexture);
                                SDL_DestroyTexture(gameOverTexture);
                                SDL_DestroyTexture(scoreTexture);

                                if (boss.texture)
                                {
                                    SDL_DestroyTexture(boss.texture);
                                    boss.texture = nullptr;
                                }
                                for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
                                {
                                    if (bossProjectiles[i].texture)
                                    {
                                        SDL_DestroyTexture(bossProjectiles[i].texture);
                                        bossProjectiles[i].texture = nullptr;
                                    }
                                }
                                SDL_DestroyTexture(planeTexture);
                                SDL_DestroyTexture(projectileTexture);
                                SDL_DestroyTexture(enemyTexture);
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_Quit();
                            }
                        }
                    }
                }

            for (int j = 0; j < MAX_PROJECTILES; ++j)
            {
                if (projectiles[j].isActive && checkCollision(projectiles[j].rect, enemies[i].rect))
                {

                    projectiles[j].isActive = false; 
                    enemies[i].isActive = false;
                    score++;
                }
            }

            for (int i = 0; i < MAX_ENEMIES; ++i)
            {
                SDL_Rect scaledRect = {
                    enemies[i].rect.x,
                    enemies[i].rect.y,
                    7 * enemies[i].rect.w / 10,
                    7 * enemies[i].rect.h / 10};
                if (enemies[i].isActive && checkCollision(imageRect2, scaledRect))
                {
                    SDL_Delay(2000);
                    SDL_RenderClear(renderer);
                    renderGameOver(renderer, gameOverTexture, digitTextures, scoreTexture, score);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(2000);
                    score = 0;
                    SDL_DestroyTexture(backgroundTexture);
                    SDL_DestroyTexture(gameOverTexture);
                    SDL_DestroyTexture(scoreTexture);

                    if (boss.texture)
                    {
                        SDL_DestroyTexture(boss.texture);
                        boss.texture = nullptr;
                    }
                    for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
                    {
                        if (bossProjectiles[i].texture)
                        {
                            SDL_DestroyTexture(bossProjectiles[i].texture);
                            bossProjectiles[i].texture = nullptr;
                        }
                    }
                    SDL_DestroyTexture(planeTexture);
                    SDL_DestroyTexture(projectileTexture);
                    SDL_DestroyTexture(enemyTexture);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                }
            }
        }

        for (int i = 0; i < MAX_PROJECTILES; ++i)
        {
            if (projectiles[i].isActive)
            {
                projectiles[i].rect.y -= projectiles[i].speed;
                if (projectiles[i].rect.y + projectiles[i].rect.h < 0)
                {
                    projectiles[i].isActive = false;
                }
                else
                {
                    SDL_RenderCopy(renderer, projectiles[i].texture, NULL, &projectiles[i].rect);
                }
            }
        }

        int digitWidth = 20;
        int digitHeight = 30;
        int xPosition = WIDTH - digitWidth;

        int remainingScore = score;
        int digitCount = (score == 0) ? 1 : 0;

        while (remainingScore != 0)
        {
            remainingScore /= 10;
            digitCount++;
        }

        remainingScore = score;

        if (digitCount == 0)
        {
            SDL_Rect digitRect = {xPosition, 10, digitWidth, digitHeight};
            SDL_RenderCopy(renderer, digitTextures[0], NULL, &digitRect);
        }
        else
        {
            for (int i = 0; i < digitCount; ++i)
            {
                int digit = remainingScore / static_cast<int>(pow(10, i)) % 10;
                SDL_Rect digitRect = {xPosition, 10, digitWidth, digitHeight};
                SDL_RenderCopy(renderer, digitTextures[digit], NULL, &digitRect);
                xPosition -= digitWidth;
            }
        }

        SDL_RenderPresent(renderer);
    }
    if (boss.texture)
    {
        SDL_DestroyTexture(boss.texture);
        boss.texture = nullptr;
    }
    for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
    {
        if (bossProjectiles[i].texture)
        {
            SDL_DestroyTexture(bossProjectiles[i].texture);
            bossProjectiles[i].texture = nullptr;
        }
    }
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(planeTexture);
    SDL_DestroyTexture(projectileTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handleButtonClick(int buttonId)
{
    if (buttonId == 0)
    {
        close();
        currentGameState = GameState::GAMEPLAY;
        startGame();
        currentGameState = GameState::MENU;
    }
    else if (buttonId == TOTAL_BUTTONS - 1 && currentGameState == GameState::MENU)
    {
        quit = true;
    }
}

void runMenu()
{
    while (!quit && currentGameState == GameState::MENU)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for (int i = 0; i < TOTAL_BUTTONS; ++i)
                {
                    if (x > buttons[i].position.x && x < buttons[i].position.x + buttons[i].position.w &&
                        y > buttons[i].position.y && y < buttons[i].position.y + buttons[i].position.h)
                    {
                        std::cout << "Button " << i << " clicked" << std::endl;
                        handleButtonClick(i);
                    }
                }
            }
        }

        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, gBackgroundTexture, nullptr, nullptr);

        SDL_RenderPresent(gRenderer);
    }
}

int main(int argc, char *argv[])
{
    if (!init())
    {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else
    {
        if (!loadMedia())
        {
            std::cerr << "Failed to load media!" << std::endl;
        }
        else
        {
            runMenu();

            if (currentGameState == GameState::GAMEPLAY)
            {
                startGame();
            }
        }
        close();
    }
    return 0;
}
