#pragma once
#include <string>

//==================================================
// Enums
//==================================================

enum class GameState
{
    MENU,
    PLAYING,
    PAUSED,
    HELP,
    GAME_OVER
};

enum class MenuState
{
    MainMenu,
    GameOverWin,
    GameOverLose,
    TopBar,
    Pause
};

enum class MenuAction
{
    None,
    NewGame,
    Help,
    Exit,
    ReturnToMenu
};

enum class GiftEffect 
{
    FreezeEnemies,
    KillOneEnemy,
    AddTime,
    AddLife,
    None
};
//==================================================
// Structs
//==================================================

struct Node
{
    int x = 0;
    int y = 0;
};

//==================================================
// Global Constants: General Settings
//==================================================

constexpr float TILE_SIZE = 32.f;
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
const float TOP_BAR_OFFSET = 15.f;
constexpr int INITIAL_LIVES = 3;
const float MAP_HEIGHT_PX = 20 * TILE_SIZE;



//==================================================
// Global Constants: Timings & Durations
//==================================================

const float EXPLOSION_LIFETIME = 0.5f;
constexpr float INITIAL_LEVEL_TIME = 100.f;
constexpr float GIFT_SPAWN_INTERVAL = 10.f;
constexpr float FREEZE_DURATION = 3.f;
constexpr float BOMB_TIMER = 4.f;
constexpr float TIME_GIFT_DURATION = 15.f;

//==================================================
// Global Constants: Speeds & Scales
//==================================================

constexpr float ENEMY_DEFAULT_SPEED = 50.f;
constexpr float PLAYER_DEFAULT_SPEED = 110.f;

const float ENEMY_DEFAULT_SCALE = 0.60f;
constexpr float PLAYER_DEFAULT_SCALE = 0.85f;

constexpr float ICE_GIFT_SCALE = 0.9f;
constexpr float LIFE_GIFT_SCALE = 0.8f;
constexpr float GUARD_KILLER_SCALE = 1.f;
constexpr float TIME_GIFT_SCALE = 0.9f;
constexpr float BOMB_SCALE = 1.7f;

//==================================================
// Global Constants: Audio Volumes
//==================================================

constexpr float EFFECT_VOLUME = 3.f;
constexpr float MUSIC_VOLUME = 2.f;
