
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "UI/GameUI.h"
#include "UI/Menu.h"
#include "consts.h"

class UIManager
{
public:
    UIManager(sf::RenderWindow& window);
    ~UIManager() = default;

   
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    void initWithMapHeight(float windowWidth, float windowHeight, float mapHeight);
    
    void handleInput(const sf::Event& event, GameState currentState);
    void resetEventFlags();
    void update(int life, int score, float timeRemaining, GameState currentState, int currentLevel, int totalLevels);
    void render(sf::RenderWindow& window);

    bool shouldReturnToMenu() const;
    bool shouldShowHelp() const;

    void resetMenuFlags();
    void renderHelp(sf::RenderWindow& window);
    void setMenuState(MenuState state);
    void setGameOverScore(int score);
    bool shouldStartNewGame() const;
    bool shouldExitGame() const;
    bool shouldPauseGame() const;
    bool shouldReturnFromHelp() const;
    

private:
   
    GameUI m_gameUI;
    Menu m_menu;
    GameState m_currentState;
    GameState m_previousState;
    bool m_pauseRequested;
    bool m_returnFromHelpRequested;
    bool m_newGameRequested;
    bool m_exitRequested;
    bool m_returnToMenuRequested;
    bool m_helpRequested;
    
    sf::RenderWindow& m_window;
};