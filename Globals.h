#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "GameEntities.h"
#include "GameState.h"
#include "GameUI.h"

const int SCREEN_WIDTH          = 1920;
const int SCREEN_HEIGHT         = 1080;

const int BUTTON_WIDTH          = 200;
const int BUTTON_HEIGHT         = 50;
const int TOTAL_BUTTONS         = 4;

const int MAX_ENEMIES           = 5;
const int MAX_PROJECTILES       = 4;
const int MAX_BOSS_PROJECTILES  = 4;
const int NUM_STARS             = 100;

int enemySpawnTimer         = 0;
int ENEMY_SPAWN_INTERVAL    = 60;
int ENEMY_SPEED_DIVISOR     = 1;
int ENEMY_SPEED             = 1;

int MINI_BOSS_SPEED_DIVISOR             = 1;
int MINI_BOSS_SPEED_DIVISOR_HORIZONTAL  = 1;
int playerScore = 0;

bool godmode        = false;
bool quit           = false;
bool howToPlay_flag = false;
bool credits_flag   = false;
bool quit_flag      = false;
bool newgame_flag   = false;
bool miniBoss_flag = true;
bool megaBoss_flag = true;

Player player;
Portal portal_in;
Portal portal_out;

MiniBoss miniBoss;
MegaBoss megaBoss;

SDL_Event e;

SDL_Window*     gWindow         = nullptr;
SDL_Renderer*   gRenderer       = nullptr;

SDL_Texture* gHowToPlayTexture  = nullptr;
SDL_Texture* gCredits           = nullptr;
SDL_Texture* gMenu              = nullptr;
SDL_Texture* gBackgroundTexture = nullptr;
SDL_Texture* gGameOverTexture   = nullptr;
SDL_Texture* gGameOverBackground   = nullptr;

SDL_Texture* scoreTexture       = nullptr;

SDL_Texture* planeTexture       = nullptr;
SDL_Texture* projectileTexture  = nullptr;
SDL_Texture* enemyTexture       = nullptr;
SDL_Texture* cautionTexture     = nullptr;

GameState currentGameState = GameState::MENU;
Button buttons[TOTAL_BUTTONS];
HitEffect effects[MAX_ENEMIES];

Star stars[NUM_STARS];
MiniBossProjectile miniBossProjectiles[MAX_BOSS_PROJECTILES];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes SDL, creates the window and renderer.
///
/// @note   Sets up SDL subsystems and creates the main game window
///          and renderer required for rendering graphics.
///
/// @return true if initialization succeeds, false otherwise.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool init();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Loads textures and sets button positions for the main menu.
///
/// @note   This function initializes all necessary textures for
///          the main menu and configures the clickable button areas.
///
/// @return true if all media loads successfully, false otherwise.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool loadMedia();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Cleans up SDL resources by freeing textures,
///        destroying renderer and window, and quitting SDL.
///
/// @note   Call this function before exiting the application
///         to release all allocated SDL resources.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void close();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Handles the main menu loop, processes user input,
///         and renders the menu screen.
///
/// @note   Waits for mouse clicks on buttons and processes SDL events.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void runMenu();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Displays the How To Play screen and handles input.
///
/// @note   Renders gameplay instructions and waits for user input.
///         Returns to the main menu on mouse click or ESC key press.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void runHowToPlay();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Displays the Credits screen and handles input.
///
/// @note   Shows credits information and waits for user input.
///         Returns to the main menu when the user clicks or presses ESC.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void runCredits();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes SDL, creates the game window and renderer,
///         loads game assets, and starts the main game logic loop.
///
/// @note   This function sets up the environment needed to run the game,
///         including window creation, renderer initialization, and resource loading.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void startGame();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Loads all game textures from BMP files.
///
/// @param  renderer        Pointer to SDL_Renderer for creating textures.
/// @param  window          Pointer to SDL_Window (unused but provided for context).
///
/// @note   This function loads backgrounds, sprites, and portal textures.
/// 
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loadGame(SDL_Renderer *renderer, SDL_Window *window);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Main game loop and logic handler.
///
/// @details
/// This function encapsulates the entire runtime logic of the game. It is responsible for
/// initializing and managing game entities such as enemies, projectiles, bosses (MiniBoss
/// and MegaBoss), and the player. It also handles user input, collision detection,
/// rendering, score tracking, and enemy spawning behavior.
///
/// Key features include:   - Rendering of background, player, projectiles, enemies, and bosses
///                         - Spawning logic for standard enemies and bosses based on score conditions
///                         - Collision detection between projectiles and entities
///                         - Special effects (e.g., explosion visuals)
///                         - Player shooting mechanics with score-based upgrades
///                         - Enemy and boss AI with movement and attack patterns
///                         - Score display using digit textures
///                         - Game termination handling on collision or exit
///
/// @param renderer         Pointer to the SDL_Renderer used for drawing graphics.
/// @param window           Pointer to the SDL_Window used as the game display window.
/// @param SCREEN_WIDTH     Width of the game screen in pixels.
/// @param SCREEN_HEIGHT    Height of the game screen in pixels.
/// @param miniBoss_flag    Flag to control the spawn condition of the MiniBoss.
/// @param megaBoss_flag    Flag to control the spawn condition of the MegaBoss.
///
/// @note
/// The game loop runs indefinitely until a quit event (such as window close) occurs
/// or the game is terminated due to a player collision. All game entities are updated
/// and rendered in each frame.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gameLogic(SDL_Renderer *renderer, SDL_Window *window, int WIDTH, int HEIGHT, bool miniBoss_flag, bool megaBoss_flag);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes star positions and speeds randomly.
///
/// @note   Stars are positioned within screen bounds and assigned random speeds.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initStars();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Renders moving stars and updates their positions.
///
/// @param renderer         Pointer to SDL_Renderer used for drawing.
///
/// @note   Stars move vertically and wrap to the top once they exit the bottom of the screen.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void renderStars(SDL_Renderer* renderer);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Activates and randomly positions a portal on the screen.
///
/// @param  portal           Reference to the Portal object to activate and position.
/// @param  screenWidth      Width of the screen/window.
/// @param  screenHeight     Height of the screen/window.
///
/// @note   The portal's position is constrained to be fully within screen bounds.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void spawnPortal(Portal &portal, int screenWidth, int screenHeight);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks if the player collides with an active portal and teleports the player randomly within screen bounds.
///
/// @param  playerRect          Reference to the player's SDL_Rect (position and size).
/// @param  portal              Reference to the Portal object to check collision with.
/// @param  screenWidth         Width of the screen/window.
/// @param  screenHeight        Height of the screen/window.
///
/// @note   If a collision is detected, the player is repositioned randomly inside the screen,
///         and the portal is deactivated.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handlePortalCollision(SDL_Rect &playerRect, Portal &portal, int screenWidth, int screenHeight);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Teleports the mouse cursor to a specified position within the window.
///
/// @param  window              Pointer to the SDL_Window where the cursor will be warped.
/// @param  screenWidth         Width of the window (unused, but kept for consistency).
/// @param  screenHeight        Height of the window (unused, but kept for consistency).
/// @param  newX                New X coordinate to teleport the cursor to.
/// @param  newY                New Y coordinate to teleport the cursor to.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void teleportPlayer(SDL_Window *window, int screenWidth, int screenHeight, int newX, int newY);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Teleports an enemy to a random position away from the cursor,
///         then spawns an active portal at that location.
///
/// @param  enemy               Reference to the Enemy to teleport.
/// @param  screenWidth         Width of the playable screen area.
/// @param  screenHeight        Height of the playable screen area.
/// @param  cursor              Current cursor position (to keep teleport distance).
/// @param  portal              Reference to Portal to activate at enemy's new position.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void teleportEnemy(Enemy &enemy, int screenWidth, int screenHeight, const CursorPosition &cursor, Portal &portal);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Creates a screen shake effect by randomly offsetting
///         the renderer's logical size for a short duration.
///
/// @param  renderer            Pointer to the SDL_Renderer.
/// @param  intensity           Maximum pixel offset for shake effect.
/// @param  duration            Number of shake frames (iterations).
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void screenShake(SDL_Renderer* renderer, int intensity, int duration);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Renders a health bar above the MiniBoss.
///
/// @param  renderer            Pointer to the SDL_Renderer used for drawing.
/// @param  miniBoss            Reference to the MiniBoss object containing position and health.
///
/// @note   The health bar shows remaining health as a green bar over a red background.
///         It is drawn slightly above the MiniBoss's current position.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void renderHealthBar(SDL_Renderer *renderer, const MiniBoss &miniBoss);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Initializes the MegaBoss with textures and default values.
///
/// @param  megaBoss            Reference to the MegaBoss object to initialize.
/// @param  bodyTex             Texture for the MegaBoss body.
/// @param  leftArmTex          Texture for the MegaBoss left arm.
/// @param  rightArmTex         Texture for the MegaBoss right arm.
///
/// @note   This function sets initial positions, activates components, 
///         and assigns default health and speed values.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initMegaBoss(MegaBoss &megaBoss, SDL_Texture *bodyTex, SDL_Texture *leftArmTex, SDL_Texture *rightArmTex);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Checks if two rectangles (projectile and enemy) intersect.
///
/// @param  projectileRect      SDL_Rect of the projectile.
/// @param  enemyRect           SDL_Rect of the enemy.
///
/// @return true if the rectangles intersect (collision), false otherwise.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool checkEnemiesCollision(const SDL_Rect &projectileRect, const SDL_Rect &enemyRect);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Renders a temporary explosion effect at given coordinates.
///
/// @param  x                   X position of the explosion.
/// @param  y                   Y position of the explosion.
/// @param  explosionTexture    Texture used to render the explosion.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void createExplosion(int x, int y, SDL_Texture* explosionTexture);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Deactivates all enemies and moves them off-screen.
///
/// @param  enemies             Array of enemy objects to clear.
/// @param  maxEnemies          Total number of enemies in the array.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clearEnemies(Enemy enemies[], int maxEnemies);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Renders the Game Over screen along with the final playerScore.
///
/// @param  renderer            The SDL renderer used for drawing.
/// @param  digitTextures       Array of digit textures (0–9) used to render the playerScore.
/// @param  scoreTexture        Texture for the "Score" label.
/// @param  playerScore               Final player playerScore to render as digits.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void renderGameOver(SDL_Renderer* renderer, SDL_Texture* digitTextures[],
                    SDL_Texture* scoreTexture, int playerScore);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Handles termination of the game loop, optionally
///         rendering the Game Over screen.
///
/// @param  renderer            SDL renderer used for drawing.
/// @param  digitTextures       Array of textures representing digits 0–9.
/// @param  playerScore               Final playerScore to display on the Game Over screen.
/// @param  onCollision         If true, show Game Over screen before quitting.
///                             If false, quit immediately without rendering.
///
/// @return void
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void terminateGame(SDL_Renderer* renderer, SDL_Texture* digitTextures[], int playerScore, bool onCollision);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Retrieves the current position of the mouse cursor.
///
/// @note   Uses SDL_GetMouseState to get the x and y coordinates
///         of the mouse cursor relative to the window.
///
/// @return CursorPosition struct containing the current x and y coordinates.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CursorPosition getCurrentCursorPosition();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Loads a BMP image from disk and creates an SDL_Texture.
///
/// @param  path                The file path to the BMP image to load.
///
/// @note   Attempts to load a BMP image file into an SDL_Surface,
///         then creates a texture from that surface using the global
///         renderer `gRenderer`. Handles errors if loading or texture
///         creation fails.
///
/// @return Pointer to the created SDL_Texture, or nullptr on failure.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SDL_Texture* loadTexture(const char* path);

void quitGame();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Draws a white border around a given SDL_Rect.
///
/// @param  renderer             The SDL renderer used to draw the border.
/// @param  rect                 The rectangle around which the border will be drawn.
///
/// @note The border is 4 pixels thick on each side.
/// @note The border is drawn slightly outside the original rect dimensions.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawWhiteBorder(SDL_Renderer *renderer, SDL_Rect rect);


///////////////////////////////////////////////////////////////
/// @brief  Draws a filled circle at the specified center and radius.
///
/// @param  renderer            Pointer to the SDL_Renderer to draw on.
/// @param  cx                  X-coordinate of the circle center.
/// @param  cy                  Y-coordinate of the circle center.
/// @param  radius              Radius of the circle.
///
/// @note   Uses a simple pixel-by-pixel check to fill all points
///         within the circle's radius. This is a brute-force approach
///         and may not be optimal for large circles.
///
/// @return void
///////////////////////////////////////////////////////////////
void drawCircle(SDL_Renderer *renderer, int cx, int cy, int radius);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Handles button click events in the MENU state.
///
/// @param  buttonId            The ID of the button clicked.
///                     0: PLAY, 1: HOW TO PLAY, 2: CREDITS, 3: QUIT
///
/// @note   This function only processes clicks if the current state is MENU.
///         Does NOT start the game directly; just changes the state.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handleButtonClick(int buttonId);

#endif // GLOBALS_H
