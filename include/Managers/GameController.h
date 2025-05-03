#pragma once

#include <SFML/Graphics.hpp>
#include "World.h"
#include "GameObjects/Player.h"
#include "GameObjects/Bomb.h"   
#include "LevelManager.h"
#include "GameObjects/Explosion.h"   
#include "UI/Menu.h"
#include "UI/GameUI.h"
#include "UI/UIManager.h"

class GameController
{
public:
    GameController();

   
    void run();

    void resetLevel();
    void setPendingReset();
    bool getPendingReset() const;
   
    void gameOverLose();
    void nextLevel();
    void scoreEnemies();

    void addBomb(sf::Vector2f bombPos);
    void addExplosion(sf::Vector2f explosionPos, const std::string& exploTexture);
    void addSubExplosion(sf::Vector2f pos, const std::string& texture);
 
    void queueObjectToAdd(std::unique_ptr<UpdatableObject> obj);

private:

   
    void initializeResources();
    void initializeUI();
    void initializeLevel();
    World m_world;
    Player m_player;
    int m_initialEnemyCount;
    
	std::vector<std::unique_ptr<UpdatableObject>> m_gameObjects;
    std::vector<std::unique_ptr<UpdatableObject>> m_objectsToAdd;
   
    void initGame();
    void resetGame();

    // --- אירועים ---
    void handleEvents();
    void processSystemEvent(const sf::Event& event);
    void processUIFlags();
    void handleKeyPressedEvent(const sf::Event& event);
   
    void handleKeyPress(sf::Keyboard::Key key);
   

    // --- עדכון הלוגיקה ---
    void update(sf::Time deltaTime);
    void updateTime(sf::Time deltaTime);
    void processCollisions();
    void updateGameObjects(sf::Time deltaTime);
   
    void processEnemyFreeze();
    void processGiftSpawn();
    void killEnemy();
    void freezeEnemies();
    void addTime(sf::Time extraTime);
    void startFreezeTimer();
   

    // ---  לעדכון  UI ---
    void updateUIGameplay();
    void updateUIMenu();
    void updateUIGameOver();

    void finishGameVictory();
    void playSound(std::string soundName);
    void render();
    void renderGameObjects();

    // משתני סאונד ומוסיקה
    sf::Sound m_sound;
    sf::Music m_music;


    // מצבי המשחק
    GameState m_state;
    GameState m_previousState;

    bool m_isGameOver;
    bool m_enemiesFrozen;
    bool m_pendingReset;

    // חלון המשחק וממשק המשתמש
    sf::RenderWindow m_window;
    UIManager m_uiManager;

    // משתני זמן
    sf::Time m_timeRemaining;
    sf::Time m_initialLevelTime;
    sf::Time m_giftSpawnInterval;
    sf::Clock m_freezeClock;
    sf::Clock m_giftSpawnClock;
    sf::Clock m_levelClock;

    
    LevelManager m_levelManager;
};
