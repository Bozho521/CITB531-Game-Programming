#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <windows.h>

#include "Globals.h"

using std::cin;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "[ERROR] SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        gWindow = SDL_CreateWindow("Galaga Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr)
        {
            std::cerr << "[ERROR] Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr)
            {
                std::cerr << "[ERROR] Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
                return false;
            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }
    return true;
}

// YOAN res?
// bool loadMedia()
// {
//     gBackgroundTexture = loadTexture("visuals/menu.bmp");
//     if (gBackgroundTexture == nullptr)
//     {
//         std::cerr << "Failed to load background texture!" << std::endl;
//         return false;
//     }

//     gHowToPlayTexture = loadTexture("visuals/how_to_play.bmp");
//     if (gHowToPlayTexture == nullptr)
//     {
//         std::cerr << "Failed to load 'How To Play' texture!" << std::endl;
//         return false;
//     }

//     gCredits = loadTexture("visuals/credits.bmp");
//     if (gCredits == nullptr)
//     {
//         std::cerr << "Failed to load credits texture!" << std::endl;
//         return false;
//     }

//     // Constants for buttons
//     const int buttonWidth = BUTTON_WIDTH + 100;  // You can adjust this as needed
//     const int buttonHeight = BUTTON_HEIGHT;
//     const int numButtons = 4;
//     const int verticalSpacing = 50;

//     // Calculate starting Y coordinate to center all buttons vertically
//     int totalHeight = numButtons * buttonHeight + (numButtons - 1) * verticalSpacing;
//     int startY = (SCREEN_HEIGHT - totalHeight) / 2;

//     // Horizontal center for buttons
//     int centerX = (SCREEN_WIDTH - buttonWidth) / 2;

//     // Set button positions dynamically
//     for (int i = 0; i < numButtons; ++i)
//     {
//         buttons[i].position = {
//             centerX,
//             startY + i * (buttonHeight + verticalSpacing),
//             buttonWidth,
//             buttonHeight
//         };
//     }

//     return true;
// }

bool loadMedia()
{
    gMenu = loadTexture("visuals/menu.bmp");
    if (gMenu == nullptr)
    {
        std::cerr << "[ERROR] Failed to load background texture!" << std::endl;
        return false;
    }

    gHowToPlayTexture = loadTexture("visuals/how_to_play.bmp");
    if (gHowToPlayTexture == nullptr)
    {
        std::cerr << "[ERROR] Failed to load 'How To Play' texture!" << std::endl;
        return false;
    }

    gCredits = loadTexture("visuals/credits.bmp");
    if (gCredits == nullptr)
    {
        std::cerr << "[ERROR] Failed to load credits texture!" << std::endl;
        return false;
    }

    buttons[0].position = {810, 303, BUTTON_WIDTH, BUTTON_HEIGHT};
    buttons[1].position = {585, 506, BUTTON_WIDTH + 300, BUTTON_HEIGHT};
    buttons[2].position = {720, 713, BUTTON_WIDTH + 120, BUTTON_HEIGHT};
    buttons[3].position = {810, 924, BUTTON_WIDTH, BUTTON_HEIGHT};

    return true;
}

void close()
{
    SDL_DestroyTexture(gMenu);
    gMenu = nullptr;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    SDL_Quit();
}

void runMenu()
{
    // PlaySound(TEXT("sfx/menumusic.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
                        handleButtonClick(i);
                    }
                }
            }
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, gMenu, nullptr, nullptr);
        SDL_RenderPresent(gRenderer);
    }
}

void runHowToPlay()
{
    SDL_Rect footerBox = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 70, 200, 50};

    bool running = true;
    SDL_Event e;

    while (running && currentGameState == GameState::HOWTOPLAY)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= footerBox.x && x <= footerBox.x + footerBox.w &&
                    y >= footerBox.y && y <= footerBox.y + footerBox.h)
                {
                    currentGameState = GameState::MENU;
                    running = false;
                }
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                currentGameState = GameState::MENU;
                running = false;
            }
        }

        if (gRenderer && gHowToPlayTexture)
        {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, gHowToPlayTexture, nullptr, nullptr);
            SDL_RenderPresent(gRenderer);
        }
    }
}

void runCredits()
{
    SDL_Rect footerBox = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 70, 200, 50};

    bool running = true;
    SDL_Event e;

    while (running && currentGameState == GameState::CREDITS)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= footerBox.x && x <= footerBox.x + footerBox.w &&
                    y >= footerBox.y && y <= footerBox.y + footerBox.h)
                {
                    currentGameState = GameState::MENU;
                    running = false;
                }
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                currentGameState = GameState::MENU;
                running = false;
            }
        }

        if (gRenderer && gCredits)
        {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, gCredits, nullptr, nullptr);
            SDL_RenderPresent(gRenderer);
        }
    }
}

void startGame()
{
    SDL_ShowCursor(SDL_DISABLE);

    loadGame(gRenderer, gWindow);
    gameLogic(gRenderer, gWindow, SCREEN_WIDTH, SCREEN_HEIGHT, miniBoss_flag, megaBoss_flag);

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void loadGame(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_Surface *backgroundSurface = SDL_LoadBMP("visuals/background.bmp");
    SDL_Surface *gameOverSurface = SDL_LoadBMP("visuals/gameover.bmp");
    SDL_Surface *gameOverBackgroundSurface = SDL_LoadBMP("visuals/gameOverBackground.bmp");
    SDL_Surface *scoreSurface = SDL_LoadBMP("visuals/playerScore.bmp");
    SDL_Surface *planeSurface = SDL_LoadBMP("visuals/plane.bmp");
    SDL_Surface *projectileSurface = SDL_LoadBMP("visuals/projectile.bmp");
    SDL_Surface *enemySurface = SDL_LoadBMP("visuals/enemy.bmp");
    SDL_Surface *portalSurface = SDL_LoadBMP("visuals/portal_in.bmp");
    SDL_Surface *portalOutSurface = SDL_LoadBMP("visuals/portal_out.bmp");
    SDL_Surface *cautionSignSurface = SDL_LoadBMP("visuals/caution.bmp");


    if (!backgroundSurface || !gameOverSurface || !scoreSurface || !planeSurface ||
        !projectileSurface || !enemySurface || !portalSurface || !portalOutSurface)
    {
        std::cerr << "[ERROR] Failed to load one or more images: " << SDL_GetError() << std::endl;
        exit(1);
    }

    gBackgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    gGameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    gGameOverBackground = SDL_CreateTextureFromSurface(renderer, gameOverBackgroundSurface);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    planeTexture = SDL_CreateTextureFromSurface(renderer, planeSurface);
    projectileTexture = SDL_CreateTextureFromSurface(renderer, projectileSurface);
    enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
    cautionTexture = SDL_CreateTextureFromSurface(renderer, cautionSignSurface);
    
    portal_in.texture = SDL_CreateTextureFromSurface(renderer, portalSurface);
    if (!portal_in.texture)
    {
        std::cerr << "[ERROR] Failed to create portal_in texture: " << SDL_GetError() << std::endl;
        exit(1);
    }

    portal_out.texture = SDL_CreateTextureFromSurface(renderer, portalOutSurface);
    if (!portal_out.texture)
    {
        std::cerr << "[ERROR] Failed to create portal_out texture: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_FreeSurface(backgroundSurface);
    SDL_FreeSurface(gameOverSurface);
    SDL_FreeSurface(gameOverBackgroundSurface);
    SDL_FreeSurface(scoreSurface);
    SDL_FreeSurface(planeSurface);
    SDL_FreeSurface(projectileSurface);
    SDL_FreeSurface(enemySurface);
    SDL_FreeSurface(portalSurface);
    SDL_FreeSurface(portalOutSurface);
}

void gameLogic(SDL_Renderer *renderer, SDL_Window *window, int SCREEN_WIDTH, int SCREEN_HEIGHT, bool miniBoss_flag, bool megaBoss_flag)
{
    bool onCollision = false;
    int cnter = 0;

    player.rect = {100, 100, 32, 32};

    const int NUM_DIGITS = 10;
    SDL_Texture *digitTextures[NUM_DIGITS];

    for (int i = 0; i < 10; ++i)
    {
        char imagePath[50];
        sprintf(imagePath, "visuals/digit_%d.bmp", i);
        SDL_Surface *digitSurface = SDL_LoadBMP(imagePath);

        if (digitSurface == nullptr)
        {
            std::cerr << "[ERROR] Could not load digit image: " << SDL_GetError() << std::endl;
        }
        else
        {
            digitTextures[i] = SDL_CreateTextureFromSurface(renderer, digitSurface);
            SDL_FreeSurface(digitSurface);
        }
    }

    SDL_ShowCursor(SDL_DISABLE);

    SDL_Event windowEvent;

    PlayerProjectile playerProjectiles[MAX_PROJECTILES];

    // MiniBoss miniBoss;
    miniBoss.isActive = false;
    miniBoss.rect = {SCREEN_WIDTH / 2 - 150, -300, 500, 325};
    miniBoss.health = 100;
    miniBoss.speed = 1;
    miniBoss.texture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("visuals/miniBoss.bmp"));

    // MegaBoss megaBoss;
    SDL_Texture *megaBossBodyTex = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("visuals/megaBoss_body.bmp"));
    SDL_Texture *leftArmTex = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("visuals/left_arm.bmp"));
    SDL_Texture *rightArmTex = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("visuals/right_arm.bmp"));
    initMegaBoss(megaBoss, megaBossBodyTex, leftArmTex, rightArmTex);

    // Initialize miniBoss playerProjectiles
    for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
    {
        miniBossProjectiles[i].isActive = false;
        miniBossProjectiles[i].speed = 1;
        miniBossProjectiles[i].rect.w = 20;
        miniBossProjectiles[i].rect.h = 20;
        miniBossProjectiles[i].texture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("visuals/miniBossProjectile.bmp"));
    }

    for (int i = 0; i < MAX_PROJECTILES; ++i)
    {
        playerProjectiles[i].isActive = false;
        playerProjectiles[i].speed = 1;
        playerProjectiles[i].rect.w = 10;
        playerProjectiles[i].rect.h = 10;
        playerProjectiles[i].texture = projectileTexture;
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

    bool gameRunning = true;
    bool gameOver = false;
    Uint32 lastShotTime = 0;
    const Uint32 cooldownTime = 500;
    while (true)
    {
        if(newgame_flag)
        {
            SDL_ShowCursor(SDL_DISABLE);
            newgame_flag = false;
            miniBoss_flag = true;
            megaBoss_flag = true;
            playerScore = 0;
        }

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

                Uint32 currentTime = SDL_GetTicks();
                if (currentTime >= lastShotTime + cooldownTime)
                {
                    lastShotTime = currentTime;

                    int wingAXOffset = 5;
                    int wingBXOffset = 60;
                    int wingYOffset = 10;

                    if (!playerProjectiles[0].isActive && !playerProjectiles[1].isActive)
                    {
                        playerProjectiles[0].isActive = true;
                        playerProjectiles[1].isActive = true;

                        playerProjectiles[0].rect.x = mouseX + wingAXOffset;
                        playerProjectiles[0].rect.y = mouseY + wingYOffset;

                        playerProjectiles[1].rect.x = mouseX + wingBXOffset;
                        playerProjectiles[1].rect.y = mouseY + wingYOffset;

                        if (playerScore > 110 && playerScore <= 200)
                        {
                            playerProjectiles[2].isActive = true;
                            playerProjectiles[3].isActive = true;
                            playerProjectiles[4].isActive = true;
                            playerProjectiles[5].isActive = true;

                            playerProjectiles[2].rect.x = mouseX + wingAXOffset - 35;
                            playerProjectiles[2].rect.y = mouseY + wingYOffset;

                            playerProjectiles[3].rect.x = mouseX + wingBXOffset + 35;
                            playerProjectiles[3].rect.y = mouseY + wingYOffset;

                            playerProjectiles[4].rect.x = mouseX + wingAXOffset - 60;
                            playerProjectiles[4].rect.y = mouseY + wingYOffset;

                            playerProjectiles[5].rect.x = mouseX + wingBXOffset + 60;
                            playerProjectiles[5].rect.y = mouseY + wingYOffset;
                        }
                    }
                }
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, gBackgroundTexture, NULL, NULL);

        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (effects[i].active)
            {
                float progress = 1.0f - (static_cast<float>(effects[i].timer) / 180.0f); // 30

                int spread = static_cast<int>(progress * 40);     // how far circles fly out
                int radius = static_cast<int>(5 + progress * 15); // grow radius from 5 to 15

                SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255); // explosion orange

                drawCircle(renderer, effects[i].x, effects[i].y, radius);          // center
                drawCircle(renderer, effects[i].x + spread, effects[i].y, radius); // right
                drawCircle(renderer, effects[i].x - spread, effects[i].y, radius); // left
                drawCircle(renderer, effects[i].x, effects[i].y + spread, radius); // down
                drawCircle(renderer, effects[i].x, effects[i].y - spread, radius); // up

                effects[i].timer--;
                if (effects[i].timer <= 0)
                {
                    effects[i].active = false;
                }
            }
        }

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
                    enemies[i].rect.x = rand() % (SCREEN_WIDTH - enemies[i].rect.w);
                    enemies[i].rect.y = -enemies[i].rect.h;
                    break;
                }
            }
        }

        int x0, y0;
        bool exitPortalFlag;
        if (playerScore == 0)
        {
            exitPortalFlag = true;
        }
        for (int i = 0; i < MAX_ENEMIES; ++i)
        {
            if (enemies[i].isActive && !miniBoss.isActive && !megaBoss.isActive)
            {
                if (playerScore < 100 || i % 25 != 0)
                {
                    enemies[i].rect.y += ENEMY_SPEED / ENEMY_SPEED_DIVISOR;

                    if (i % 7 == 0 && playerScore > 50)
                    {
                        CursorPosition cursor = getCurrentCursorPosition();
                        teleportEnemy(enemies[i], 980, 750, cursor, portal_in);
                        if (exitPortalFlag)
                        {
                            do
                            {
                                x0 = rand() % SCREEN_WIDTH;
                                y0 = rand() % SCREEN_HEIGHT;
                                int dx = x0 - mouseX;
                                int dy = y0 - mouseY;
                                int distance = sqrt(dx * dx + dy * dy);

                                if (distance >= 150)
                                    break;
                            } while (true);
                            // Spawn portal at enemy's new location
                            portal_out.isActive = true;
                            portal_out.rect.x = x0;
                            portal_out.rect.y = y0;
                            portal_out.rect.w = 64; // set dimensions if needed
                            portal_out.rect.h = 64;

                            exitPortalFlag = false;
                        }
                        SDL_RenderCopy(renderer, enemies[i].texture, NULL, &enemies[i].rect);
                        if (portal_in.isActive && portal_in.texture)
                        {
                            SDL_RenderCopy(renderer, portal_in.texture, NULL, &portal_in.rect);
                        }
                        if (portal_out.isActive && portal_out.texture)
                        {
                            SDL_RenderCopy(renderer, portal_out.texture, NULL, &portal_out.rect);
                        }
                    }
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

                if (enemies[i].rect.y > SCREEN_HEIGHT)
                {
                    enemies[i].isActive = false;
                    enemies[i].rect.x = rand() % (SCREEN_WIDTH - enemies[i].rect.w);
                    enemies[i].rect.y = -enemies[i].rect.h;
                    enemies[i].rect.w = rand() % 150 + 50;
                    enemies[i].rect.h = rand() % 150 + 50;
                    continue;
                }

                SDL_RenderCopy(renderer, enemies[i].texture, NULL, &enemies[i].rect);
            }
            else if (!miniBoss.isActive && !megaBoss.isActive)
            {
                enemies[i].isActive = true;
                enemies[i].rect.x = rand() % (SCREEN_WIDTH - enemies[i].rect.w);
                enemies[i].rect.y = -enemies[i].rect.h;
                enemies[i].rect.w = rand() % 150 + 50;
                enemies[i].rect.h = rand() % 150 + 50;
            }

            if (playerScore >= 10 && miniBoss_flag)
            {
                miniBoss_flag = false;
                miniBoss.isActive = true;
                clearEnemies(enemies, MAX_ENEMIES);

                SDL_Delay(200);
            }

            if (playerScore >= 150 && megaBoss_flag)
            {
                megaBoss_flag = false;
                megaBoss.isActive = true;
                clearEnemies(enemies, MAX_ENEMIES);
                SDL_Delay(200);
            }

            if (megaBoss.isActive)
            {
                // Render megaBoss body (face)
                drawWhiteBorder(renderer, megaBoss.body.rect); //debugsquare
                SDL_RenderCopy(renderer, megaBoss.body.texture, NULL, &megaBoss.body.rect);

                // Projectile collision with body
                for (int j = 0; j < MAX_PROJECTILES; ++j)
                {
                    if (playerProjectiles[j].isActive && checkEnemiesCollision(playerProjectiles[j].rect, megaBoss.body.rect))
                    {
                        playerProjectiles[j].isActive = false;
                        megaBoss.health -= 5;

                        if (megaBoss.health <= 0)
                        {
                            megaBoss.isActive = false;
                            std::cout << " [INFO] Mega MiniBoss defeated!" << std::endl;
                        }
                    }
                }

                // Phase detection
                int phase = 1;
                if (megaBoss.health < 200)
                    phase = 2;
                if (megaBoss.health < 100)
                    phase = 3;

                // Cooldowns to slow movement updates
                static int leftCooldown = 0;
                static int rightCooldown = 0;
                leftCooldown++;
                rightCooldown++;

                // Movement speed (pixels per update)
                const int speed = 1;

                // Movement update triggers (frames)
                const int moveTrigger = 15;

                // Static state for left arm
                static int leftPattern = -1;   // 0=top->bottom,1=bottom->top,2=left->right,3=right->left
                static float leftAngle = 0;

                // Static state for right arm
                static int rightPattern = -1;
                static float rightAngle = 0;

                // Static state for caution sign before left arm appears
                static bool cautionPhaseActive = true; // true means caution sign blinking
                static int cautionBlinkCount = 0;
                static int cautionBlinkTimer = 0;
                const int cautionBlinkInterval = 15; // frames per show/hide toggle
                const int cautionMaxBlinks = 7;      // total toggles (so ~3-4 full blink cycles)

                // Initialize patterns randomly once on first update
                if (leftPattern == -1)
                {
                    leftPattern = rand() % 4;
                    switch(leftPattern)
                    {
                        case 0: // top->bottom
                            megaBoss.leftArm.rect.x = rand() % (SCREEN_WIDTH - megaBoss.leftArm.rect.w);
                            megaBoss.leftArm.rect.y = -megaBoss.leftArm.rect.h - 10;
                            break;
                        case 1: // bottom->top
                            megaBoss.leftArm.rect.x = rand() % (SCREEN_WIDTH - megaBoss.leftArm.rect.w);
                            megaBoss.leftArm.rect.y = SCREEN_HEIGHT + 10;
                            break;
                        case 2: // left->right
                            megaBoss.leftArm.rect.x = -megaBoss.leftArm.rect.w - 10;
                            megaBoss.leftArm.rect.y = rand() % (SCREEN_HEIGHT / 2 - megaBoss.leftArm.rect.h);
                            break;
                        case 3: // right->left
                            megaBoss.leftArm.rect.x = SCREEN_WIDTH + 10;
                            megaBoss.leftArm.rect.y = rand() % (SCREEN_HEIGHT / 2 - megaBoss.leftArm.rect.h);
                            break;
                    }
                }

                if (rightPattern == -1)
                {
                    rightPattern = rand() % 4;
                    switch(rightPattern)
                    {
                        case 0:
                            megaBoss.rightArm.rect.x = rand() % (SCREEN_WIDTH - megaBoss.rightArm.rect.w);
                            megaBoss.rightArm.rect.y = -megaBoss.rightArm.rect.h - 10;
                            break;
                        case 1:
                            megaBoss.rightArm.rect.x = rand() % (SCREEN_WIDTH - megaBoss.rightArm.rect.w);
                            megaBoss.rightArm.rect.y = SCREEN_HEIGHT + 10;
                            break;
                        case 2:
                            megaBoss.rightArm.rect.x = -megaBoss.rightArm.rect.w - 10;
                            megaBoss.rightArm.rect.y = rand() % (SCREEN_HEIGHT / 2 - megaBoss.rightArm.rect.h);
                            break;
                        case 3:
                            megaBoss.rightArm.rect.x = SCREEN_WIDTH + 10;
                            megaBoss.rightArm.rect.y = rand() % (SCREEN_HEIGHT / 2 - megaBoss.rightArm.rect.h);
                            break;
                    }
                }

                // --- CAUTION SIGN LOGIC for LEFT ARM ---

                if (cautionPhaseActive)
                {
                    cautionBlinkTimer++;

                    if (cautionBlinkTimer >= cautionBlinkInterval)
                    {
                        cautionBlinkTimer = 0;
                        cautionBlinkCount++;
                    }

                    // Render caution sign blinking
                    // Show on even counts, hide on odd counts
                    if ((cautionBlinkCount % 2) == 0)
                    {
                        // Center caution sign on left arm's initial position
                        int cautionW, cautionH;
                        SDL_QueryTexture(cautionTexture, NULL, NULL, &cautionW, &cautionH);

                        SDL_Rect cautionRect;
                        cautionRect.w = cautionW;
                        cautionRect.h = cautionH;
                        cautionRect.x = megaBoss.leftArm.rect.x + (megaBoss.leftArm.rect.w / 2) - (cautionW / 2);
                        cautionRect.y = megaBoss.leftArm.rect.y + (megaBoss.leftArm.rect.h / 2) - (cautionH / 2);

                        SDL_RenderCopy(renderer, cautionTexture, NULL, &cautionRect);
                    }

                    // Once blinking completed, move to normal left arm movement
                    if (cautionBlinkCount >= cautionMaxBlinks)
                    {
                        cautionPhaseActive = false;
                        leftCooldown = moveTrigger; // So it moves immediately on next update
                    }
                }
                else
                {
                    // LEFT ARM movement and rendering

                    if (leftCooldown >= moveTrigger)
                    {
                        switch(leftPattern)
                        {
                            case 0: // top->bottom
                                megaBoss.leftArm.rect.y += speed;
                                if (megaBoss.leftArm.rect.y > SCREEN_HEIGHT)
                                {
                                    megaBoss.leftArm.rect.x = rand() % (SCREEN_WIDTH - megaBoss.leftArm.rect.w);
                                    megaBoss.leftArm.rect.y = -megaBoss.leftArm.rect.h - 10;

                                    // Reset caution phase for next appearance
                                    cautionPhaseActive = true;
                                    cautionBlinkCount = 0;
                                    cautionBlinkTimer = 0;
                                }
                                break;

                            case 1: // bottom->top
                                megaBoss.leftArm.rect.y -= speed;
                                if (megaBoss.leftArm.rect.y + megaBoss.leftArm.rect.h < 0)
                                {
                                    megaBoss.leftArm.rect.x = rand() % (SCREEN_WIDTH - megaBoss.leftArm.rect.w);
                                    megaBoss.leftArm.rect.y = SCREEN_HEIGHT + 10;

                                    // Reset caution phase for next appearance
                                    cautionPhaseActive = true;
                                    cautionBlinkCount = 0;
                                    cautionBlinkTimer = 0;
                                }
                                break;

                            case 2: // left->right
                                megaBoss.leftArm.rect.x += speed;
                                if (megaBoss.leftArm.rect.x > SCREEN_WIDTH)
                                {
                                    megaBoss.leftArm.rect.x = -megaBoss.leftArm.rect.w - 10;
                                    megaBoss.leftArm.rect.y = rand() % (SCREEN_HEIGHT / 2 - megaBoss.leftArm.rect.h);

                                    // Reset caution phase for next appearance
                                    cautionPhaseActive = true;
                                    cautionBlinkCount = 0;
                                    cautionBlinkTimer = 0;
                                }
                                break;

                            case 3: // right->left
                                megaBoss.leftArm.rect.x -= speed;
                                if (megaBoss.leftArm.rect.x + megaBoss.leftArm.rect.w < 0)
                                {
                                    megaBoss.leftArm.rect.x = SCREEN_WIDTH + 10;
                                    megaBoss.leftArm.rect.y = rand() % (SCREEN_HEIGHT / 2 - megaBoss.leftArm.rect.h);

                                    // Reset caution phase for next appearance
                                    cautionPhaseActive = true;
                                    cautionBlinkCount = 0;
                                    cautionBlinkTimer = 0;
                                }
                                break;
                        }

                        leftAngle += 0.1f;
                        if (leftAngle > 360) leftAngle -= 360;

                        leftCooldown = 0;

                        if (!godmode && checkEnemiesCollision(megaBoss.leftArm.rect, imageRect))
                        {
                            std::cout << "[INFO] Player hit by Left Arm!" << std::endl;
                            onCollision = true;
                            clearEnemies(enemies, MAX_ENEMIES);
                            terminateGame(renderer, digitTextures, playerScore, onCollision);
                        }
                    }

                    drawWhiteBorder(renderer, megaBoss.leftArm.rect); //debugsquare
                    SDL_RenderCopyEx(renderer, megaBoss.leftArm.texture, NULL, &megaBoss.leftArm.rect, leftAngle, NULL, SDL_FLIP_NONE);
                }

                // RIGHT ARM movement and rendering (unchanged)
                if (rightCooldown >= moveTrigger)
                {
                    switch(rightPattern)
                    {
                        case 0: // top->bottom
                            megaBoss.rightArm.rect.y += speed;
                            if (megaBoss.rightArm.rect.y > SCREEN_HEIGHT)
                            {
                                megaBoss.rightArm.rect.x = rand() % (SCREEN_WIDTH - megaBoss.rightArm.rect.w);
                                megaBoss.rightArm.rect.y = -megaBoss.rightArm.rect.h - 10;
                            }
                            break;

                        case 1: // bottom->top
                            megaBoss.rightArm.rect.y -= speed;
                            if (megaBoss.rightArm.rect.y + megaBoss.rightArm.rect.h < 0)
                            {
                                megaBoss.rightArm.rect.x = rand() % (SCREEN_WIDTH - megaBoss.rightArm.rect.w);
                                megaBoss.rightArm.rect.y = SCREEN_HEIGHT + 10;
                            }
                            break;

                        case 2: // left->right
                            megaBoss.rightArm.rect.x += speed;
                            if (megaBoss.rightArm.rect.x > SCREEN_WIDTH)
                            {
                                megaBoss.rightArm.rect.x = -megaBoss.rightArm.rect.w - 10;
                                megaBoss.rightArm.rect.y = rand() % (SCREEN_HEIGHT / 2 - megaBoss.rightArm.rect.h);
                            }
                            break;

                        case 3: // right->left
                            megaBoss.rightArm.rect.x -= speed;
                            if (megaBoss.rightArm.rect.x + megaBoss.rightArm.rect.w < 0)
                            {
                                megaBoss.rightArm.rect.x = SCREEN_WIDTH + 10;
                                megaBoss.rightArm.rect.y = rand() % (SCREEN_HEIGHT / 2 - megaBoss.rightArm.rect.h);
                            }
                            break;
                    }

                    rightAngle -= 0.1f;
                    if (rightAngle < 0) rightAngle += 360;

                    rightCooldown = 0;

                    if (!godmode && checkEnemiesCollision(megaBoss.rightArm.rect, imageRect))
                    {
                        megaBoss.leftArm.isActive = false;
                        megaBoss.body.isActive = false;
                        megaBoss.rightArm.isActive = false;
                        megaBoss.isActive = false;

                        std::cout << "[INFO] Player hit by Right Arm!" << std::endl;
                        onCollision = true;
                        // megaBoss.isActive = false;
                        clearEnemies(enemies, MAX_ENEMIES);
                        terminateGame(renderer, digitTextures, playerScore, onCollision);
                    }
                }

                drawWhiteBorder(renderer, megaBoss.rightArm.rect); //debugsquare
                SDL_RenderCopyEx(renderer, megaBoss.rightArm.texture, NULL, &megaBoss.rightArm.rect, rightAngle, NULL, SDL_FLIP_NONE);
            }

            if (miniBoss.isActive)
            {
                // Initial vertical entry
                if (miniBoss.rect.y < 50)
                {
                    miniBoss.rect.y += miniBoss.speed / ENEMY_SPEED_DIVISOR;
                }
                else
                {
                    // Static movement variables
                    static int direction = 1;
                    static int wobbleOffset = 0;
                    static bool wobbleRight = true;
                    static int wobbleCounter = 0;
                    static int dashCooldown = 0;

                    // Horizontal movement with direction and dash
                    miniBoss.rect.x += (miniBoss.speed * direction) / MINI_BOSS_SPEED_DIVISOR_HORIZONTAL;

                    // Wobble effect vertically
                    wobbleCounter++;
                    if (wobbleCounter >= 2)
                    {
                        if (wobbleRight)
                        {
                            wobbleOffset++;
                            if (wobbleOffset > 4) wobbleRight = false;
                        }
                        else
                        {
                            wobbleOffset--;
                            if (wobbleOffset < -4) wobbleRight = true;
                        }
                        wobbleCounter = 0;
                    }
                    miniBoss.rect.y += wobbleOffset % 2;

                    // Bounce off edges
                    if (miniBoss.rect.x <= 0 || miniBoss.rect.x + miniBoss.rect.w >= SCREEN_WIDTH)
                    {
                        direction *= -1;
                    }

                    // Reposition if way off screen
                    if (miniBoss.rect.x < -200 || miniBoss.rect.x > SCREEN_WIDTH + 200)
                    {
                        miniBoss.rect.x = SCREEN_WIDTH / 2 - miniBoss.rect.w / 2;
                        miniBoss.rect.y -= 50;
                    }

                    // Random dash trigger
                    if (dashCooldown <= 0 && rand() % 480 == 0)
                    {
                        MINI_BOSS_SPEED_DIVISOR_HORIZONTAL = 1;
                        dashCooldown = 30;
                    }
                    else if (dashCooldown > 0)
                    {
                        dashCooldown--;
                        if (dashCooldown == 0)
                        {
                            MINI_BOSS_SPEED_DIVISOR_HORIZONTAL = 2;
                        }
                    }

                    // Handle player projectiles hitting the miniBoss
                    for (int j = 0; j < MAX_PROJECTILES; ++j)
                    {
                        if (playerProjectiles[j].isActive && checkEnemiesCollision(playerProjectiles[j].rect, miniBoss.rect))
                        {
                            playerProjectiles[j].isActive = false;
                            miniBoss.health -= 10;

                            if (miniBoss.health <= 0)
                            {
                                miniBoss.isActive = false;
                                std::cout << "[INFO] MiniBoss defeated!" << std::endl;
                            }
                        }
                    }
                }

                // Shooting projectiles from miniBoss
                static int bossShootTimer = 0;
                bossShootTimer++;
                if (bossShootTimer >= 3600)
                {
                    bossShootTimer = 0;

                    for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
                    {
                        if (!miniBossProjectiles[i].isActive)
                        {
                            miniBossProjectiles[i].isActive = true;
                            miniBossProjectiles[i].rect.x = miniBoss.rect.x + miniBoss.rect.w / 2 - miniBossProjectiles[i].rect.w / 2;
                            miniBossProjectiles[i].rect.y = miniBoss.rect.y + miniBoss.rect.h;
                            break;
                        }
                    }
                }

                // Animate miniBoss
                SDL_RenderCopy(renderer, miniBoss.texture, NULL, &miniBoss.rect);

                // Handle miniBoss projectile movement and collisions
                static int projectileSpeedToggleCounter = 0;
                for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i)
                {
                    if (miniBossProjectiles[i].isActive)
                    {
                        miniBossProjectiles[i].rect.y += miniBossProjectiles[i].speed / MINI_BOSS_SPEED_DIVISOR;

                        // Toggle projectile speed every 60 frames
                        projectileSpeedToggleCounter++;
                        if (projectileSpeedToggleCounter >= 60)
                        {
                            MINI_BOSS_SPEED_DIVISOR = (MINI_BOSS_SPEED_DIVISOR == 1) ? 2 : 1;
                            projectileSpeedToggleCounter = 0;
                        }

                        if (miniBossProjectiles[i].rect.y > SCREEN_HEIGHT)
                        {
                            miniBossProjectiles[i].isActive = false;
                        }

                        SDL_RenderCopy(renderer, miniBossProjectiles[i].texture, NULL, &miniBossProjectiles[i].rect);

                        if (!godmode && checkEnemiesCollision(miniBossProjectiles[i].rect, imageRect))
                        {
                            std::cout << "[INFO] Player hit by miniBoss projectile!" << std::endl;
                            onCollision = true;
                            miniBoss_flag = true;
                            clearEnemies(enemies, MAX_ENEMIES);
                            terminateGame(renderer, digitTextures, playerScore, onCollision);
                        }
                    }
                }

                // ENEMY_SPEED_DIVISOR toggling (used elsewhere)
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

            for (int j = 0; j < MAX_PROJECTILES; ++j)
            {
                if (playerScore > 200 && playerProjectiles[j].isActive)
                {
                    if (checkEnemiesCollision(playerProjectiles[j].rect, enemies[i].rect))
                    {
                        for (int ii = 0; ii < MAX_ENEMIES; ++ii)
                        {
                            SDL_Rect explosionRect = {
                                playerProjectiles[j].rect.x - 400,
                                playerProjectiles[j].rect.y - 400,
                                playerProjectiles[j].rect.w + 400,
                                playerProjectiles[j].rect.h + 400};

                            if (checkEnemiesCollision(explosionRect, enemies[ii].rect))
                            {
                                playerProjectiles[j].isActive = false;
                                enemies[ii].isActive = false;
                                // PlaySound(TEXT("sfx/death.wav"), NULL, SND_FILENAME | SND_ASYNC);
                                playerScore++;
                            }
                        }
                    }
                }
                else if (playerProjectiles[j].isActive && checkEnemiesCollision(playerProjectiles[j].rect, enemies[i].rect))
                {

                    // PlaySound(TEXT("sfx/enemydies.wav"), NULL, SND_FILENAME | SND_ASYNC);
                    effects[i].x = enemies[i].rect.x + enemies[i].rect.w / 2;
                    effects[i].y = enemies[i].rect.y + enemies[i].rect.h / 2;
                    effects[i].timer = 180;
                    effects[i].active = true;

                    playerProjectiles[j].isActive = false;
                    enemies[i].isActive = false;
                    playerScore++;
                }
            }
            if (!godmode)
            {
                for (int i = 0; i < MAX_ENEMIES; ++i)
                {
                    SDL_Rect scaledRect = {
                        enemies[i].rect.x,
                        enemies[i].rect.y,
                        7 * enemies[i].rect.w / 10,
                        7 * enemies[i].rect.h / 10};
                    if (enemies[i].isActive && checkEnemiesCollision(imageRect2, scaledRect))
                    {
                        // PlaySound(TEXT("sfx/death.wav"), NULL, SND_FILENAME | SND_ASYNC);
                        onCollision = true;
                        clearEnemies(enemies, MAX_ENEMIES);
                        terminateGame(renderer, digitTextures, playerScore, onCollision);
                    }
                }
            }
        }

        for (int i = 0; i < MAX_PROJECTILES; ++i)
        {
            if (playerProjectiles[i].isActive)
            {
                playerProjectiles[i].rect.y -= playerProjectiles[i].speed;
                if (playerProjectiles[i].rect.y + playerProjectiles[i].rect.h < 0)
                {
                    playerProjectiles[i].isActive = false;
                }
                else
                {
                    SDL_RenderCopy(renderer, playerProjectiles[i].texture, NULL, &playerProjectiles[i].rect);
                }
            }
        }

        if (portal_in.isActive && SDL_HasIntersection(&imageRect2, &portal_in.rect))
        {
            teleportPlayer(window, SCREEN_WIDTH, SCREEN_HEIGHT, x0, y0);
            portal_in.isActive = false;
            portal_out.isActive = false;
            exitPortalFlag = true;

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &player.rect);
        }

        int digitWidth = 20;
        int digitHeight = 30;
        int xPosition = SCREEN_WIDTH - digitWidth;

        int remainingScore = playerScore;
        int digitCount = (playerScore == 0) ? 1 : 0;

        while (remainingScore != 0)
        {
            remainingScore /= 10;
            digitCount++;
        }

        remainingScore = playerScore;

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
        SDL_RenderClear(renderer);
    }
    terminateGame(renderer, digitTextures, playerScore, onCollision);
}

void initStars()
{
    for (int i = 0; i < NUM_STARS; ++i)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = rand() % 3 + 1;
    }
}

void renderStars(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < NUM_STARS; ++i)
    {
        SDL_RenderDrawPoint(renderer, stars[i].x, stars[i].y);

        stars[i].y += stars[i].speed;

        if (stars[i].y > SCREEN_HEIGHT)
        {
            stars[i].y = 0;
            stars[i].x = rand() % SCREEN_WIDTH;
        }
    }
}

void spawnPortal(Portal &portal, int screenWidth, int screenHeight)
{
    portal.isActive = true;
    portal.rect.x = rand() % (screenWidth - portal.rect.w);
    portal.rect.y = rand() % (screenHeight - portal.rect.h);
}

void handlePortalCollision(SDL_Rect &playerRect, Portal &portal, int screenWidth, int screenHeight)
{
    if (SDL_HasIntersection(&playerRect, &portal.rect))
    {
        playerRect.x = rand() % (screenWidth - playerRect.w);
        playerRect.y = rand() % (screenHeight - playerRect.h);

        portal.isActive = false;
        std::cout << "[INFO] Player teleported!" << std::endl;
    }
}

void teleportPlayer(SDL_Window *window, int screenWidth, int screenHeight, int newX, int newY)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_WarpMouseInWindow(window, newX, newY);
    // PlaySound(TEXT("sfx/teleportPlayer.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void teleportEnemy(Enemy &enemy, int screenWidth, int screenHeight, const CursorPosition &cursor, Portal &portal)
{
    int minDistance = 250;
    static Uint32 lastTeleportTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime > lastTeleportTime + 2000)
    {
        int newX, newY;
        do
        {
            newX = rand() % (screenWidth - enemy.rect.w);
            newY = rand() % (screenHeight - enemy.rect.h);

            int dx = newX - cursor.x;
            int dy = newY - cursor.y;
            int distance = sqrt(dx * dx + dy * dy);

            if (distance >= minDistance)
                break;

        } while (true);

        enemy.rect.x = newX;
        enemy.rect.y = newY;

        // PlaySound(TEXT("sfx/teleport.wav"), NULL, SND_FILENAME | SND_ASYNC);

        portal.isActive = true;
        portal.rect.x = newX;
        portal.rect.y = newY;
        portal.rect.w = 64;
        portal.rect.h = 64;

        lastTeleportTime = currentTime;
    }
}

void screenShake(SDL_Renderer *renderer, int intensity, int duration)
{
    for (int i = 0; i < duration; ++i)
    {
        int offsetX = rand() % (intensity * 2 + 1) - intensity;
        int offsetY = rand() % (intensity * 2 + 1) - intensity;

        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH + offsetX, SCREEN_HEIGHT + offsetY);
        SDL_Delay(10);
    }

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void renderHealthBar(SDL_Renderer *renderer, const MiniBoss &miniBoss)
{
    int barWidth = miniBoss.rect.w;
    int barHeight = 10;
    int barX = miniBoss.rect.x;
    int barY = miniBoss.rect.y - barHeight - 150;

    float healthPercentage = static_cast<float>(miniBoss.health) / 100.0f;

    SDL_Rect fullHealthBar = {barX, barY, barWidth, barHeight};
    SDL_Rect currentHealth = {barX, barY, static_cast<int>(barWidth * healthPercentage), barHeight};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &fullHealthBar);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &currentHealth);
}

void initMegaBoss(MegaBoss &megaBoss, SDL_Texture *bodyTex, SDL_Texture *leftArmTex, SDL_Texture *rightArmTex)
{
    megaBoss.body.texture = bodyTex;
    megaBoss.body.rect = {SCREEN_WIDTH / 2 - 150, 0, 250, 250};
    megaBoss.body.isActive = true;
    megaBoss.body.speed = 0;

    megaBoss.leftArm.texture = leftArmTex;
    megaBoss.leftArm.rect = {-200, 100, 200, 600};
    megaBoss.leftArm.isActive = true;
    megaBoss.leftArm.speed = 4;

    megaBoss.rightArm.texture = rightArmTex;
    megaBoss.rightArm.rect = {SCREEN_WIDTH, 100, 200, 600};
    megaBoss.rightArm.isActive = true;
    megaBoss.rightArm.speed = 4;

    megaBoss.health = 300;
    megaBoss.isActive = false;
}


bool checkEnemiesCollision(const SDL_Rect &projectileRect, const SDL_Rect &enemyRect)
{
    return SDL_HasIntersection(&projectileRect, &enemyRect);
}

void createExplosion(int x, int y, SDL_Texture *explosionTexture)
{
    SDL_Rect explosionRect = {x, y, 50, 50};
    SDL_RenderCopy(gRenderer, explosionTexture, NULL, &explosionRect);
    SDL_Delay(100);
}

void clearEnemies(Enemy enemies[], int maxEnemies)
{
    miniBoss_flag = true;
    megaBoss_flag = true;

    for (int i = 0; i < maxEnemies; ++i)
    {
        enemies[i].isActive = false;
        enemies[i].rect.x = -100;
        enemies[i].rect.y = -100;
    }
}

void renderGameOver(SDL_Renderer *renderer, SDL_Texture *digitTextures[], SDL_Texture *scoreTexture, int playerScore)
{
    
    SDL_ShowCursor(SDL_ENABLE);

    const int digitWidth = 60;
    const int digitHeight = 90;

    int scoreLength = playerScore > 0 ? (int)log10(playerScore) + 1 : 1;

    SDL_RenderCopy(gRenderer, gGameOverBackground, nullptr, nullptr);

    SDL_Rect gameOverRect = {
        SCREEN_WIDTH / 2 - 300 / 2,
        SCREEN_HEIGHT / 8,
        300,
        100};
    SDL_RenderCopy(renderer, gGameOverTexture, NULL, &gameOverRect);

    SDL_Rect scoreRect = {
        SCREEN_WIDTH / 2 - 100 / 2,
        gameOverRect.y + gameOverRect.h + 20,
        100,
        50};
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    int scorePositionX = SCREEN_WIDTH / 2 - (scoreLength * digitWidth) / 2;
    int scorePositionY = scoreRect.y + scoreRect.h + 20;

    for (int i = 0; i < scoreLength; ++i)
    {
        int digit = (int)(playerScore / pow(10, scoreLength - i - 1)) % 10;
        SDL_Rect digitRect = {
            scorePositionX + i * digitWidth,
            scorePositionY,
            digitWidth,
            digitHeight};
        SDL_RenderCopy(renderer, digitTextures[digit], NULL, &digitRect);
    }
}

void terminateGame(SDL_Renderer *renderer, SDL_Texture *digitTextures[], int playerScore, bool onCollision)
{
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    if (!onCollision)
    {
        SDL_Quit();
        return;
    }

    megaBoss.leftArm.isActive = false;
    megaBoss.body.isActive = false;
    megaBoss.rightArm.isActive = false;
    megaBoss.isActive = false;

    std::cout << "[INFO] Game Over" << std::endl;

    SDL_RenderClear(renderer);
    renderGameOver(renderer, digitTextures, scoreTexture, playerScore);

    SDL_Rect playAgainButton = {
        SCREEN_WIDTH / 2 - 100,
        SCREEN_HEIGHT - 200,  
        200,
        50
    };

    SDL_Rect quitButton = {
        SCREEN_WIDTH / 2 - 100,
        playAgainButton.y + playAgainButton.h + 20, 
        200,
        50
    };

    SDL_RenderPresent(renderer);

    bool waiting = true;
    SDL_Event event;

    while (waiting)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                currentGameState = GameState::QUIT;
                waiting = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= playAgainButton.x && x <= playAgainButton.x + playAgainButton.w &&
                    y >= playAgainButton.y && y <= playAgainButton.y + playAgainButton.h)
                {
                    std::cout << "[Info] Play Again clicked.\n";
                    currentGameState = GameState::MENU;
                    playerScore = 0;
                    waiting = false;
                }
                else if (x >= quitButton.x && x <= quitButton.x + quitButton.w &&
                         y >= quitButton.y && y <= quitButton.y + quitButton.h)
                {
                    std::cout << "[Info] Quit clicked.\n";
                    quitGame();
                }
            }
        }
    }
    if(waiting == false)
    {
        loadMedia();
        runMenu();
    }
}

void drawCircle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, cx + x, cy + y);
            }
        }
    }
}

void handleButtonClick(int buttonId)
{
    if (currentGameState != GameState::MENU)
    {
        std::cerr << "[Error] Button click ignored: Current state is not MENU.\n";
        return;
    }

    switch (buttonId)
    {
    case 0:
        std::cout << "[Info] Button 0 (PLAY) clicked.\n";
        currentGameState = GameState::PLAY;
        if(!newgame_flag)
        {
            newgame_flag = true;
        }
        std::cout << "[Info] Game state changed to PLAY.\n";
        break;

    case 1:
        std::cout << "[Info] Button 1 (HOW TO PLAY) clicked.\n";
        currentGameState = GameState::HOWTOPLAY;
        if(!howToPlay_flag)
        {
            runHowToPlay();
        }
        std::cout << "[Info] Game state changed to HOWTOPLAY.\n";
        break;

    case 2:
        std::cout << "[Info] Button 2 (CREDITS) clicked.\n";
        currentGameState = GameState::CREDITS;
        if(!credits_flag)
        {
            runCredits();
        }
        std::cout << "[Info] Game state changed to CREDITS.\n";
        break;

    case 3:
        std::cout << "[Info] Button 3 (QUIT) clicked.\n";
        if(!credits_flag)
        {
            quitGame();
        }
        quit = true;
        std::cout << "[Info] Quit flag set to true.\n";
        break;

    default:
        std::cerr << "[Error] Unknown button ID: " << buttonId << "\n";
        break;
    }
}

CursorPosition getCurrentCursorPosition()
{
    CursorPosition pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

SDL_Texture *loadTexture(const char *path)
{
    SDL_Texture *newTexture = nullptr;
    SDL_Surface *loadedSurface = SDL_LoadBMP(path);
    if (loadedSurface == nullptr)
    {
        std::cerr << "[ERROR] Unable to load image " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr)
        {
            std::cerr << "[ERROR] Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void quitGame()
{
    SDL_Quit();
    exit(0);
}

void drawWhiteBorder(SDL_Renderer *renderer, SDL_Rect rect)
{
    if (!renderer)
    {
        std::cerr << "[ERROR] Renderer is null. Cannot draw a border." << std::endl;
        return;
    }

    const int thickness = 4;

    renderer = NULL;
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect top = {
        rect.x - thickness,
        rect.y - thickness,
        rect.w + 2 * thickness,
        thickness};
    SDL_RenderFillRect(renderer, &top);

    SDL_Rect bottom = {
        rect.x - thickness,
        rect.y + rect.h,
        rect.w + 2 * thickness,
        thickness};
    SDL_RenderFillRect(renderer, &bottom);

    SDL_Rect left = {
        rect.x - thickness,
        rect.y,
        thickness,
        rect.h};
    SDL_RenderFillRect(renderer, &left);

    SDL_Rect right = {
        rect.x + rect.w,
        rect.y,
        thickness,
        rect.h};
    SDL_RenderFillRect(renderer, &right);
}

int main(int argc, char *argv[])
{
    if (!init())
    {
        std::cerr << "[ERROR] Failed to initialize!" << std::endl;
    }
    else
    {
        if (!loadMedia())
        {
            std::cerr << "[ERROR] Failed to load media!" << std::endl;
        }
        else
        {
            while (!quit)
            {
                switch (currentGameState)
                {
                case GameState::MENU:
                    runMenu();
                    break;

                case GameState::PLAY:
                    // PlaySound(NULL, NULL, 0);
                    startGame();
                    break;

                case GameState::HOWTOPLAY:
                    runHowToPlay();
                    break;

                case GameState::CREDITS:
                    runCredits();
                    break;

                case GameState::QUIT:
                    quitGame();
                    break;
                }
            }
        }
        close();
    }

    return 0;
}