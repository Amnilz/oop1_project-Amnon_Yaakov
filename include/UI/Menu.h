#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "consts.h"



class Menu 
{
public:
    Menu(float width, float height);
	~Menu() = default;

    void setMenuState(MenuState state);

    void draw(sf::RenderWindow& window);
    int  handleClick(const sf::RenderWindow& window, sf::Vector2i mousePos);
    void updateHoverState(const sf::RenderWindow& window);
    void setGameOverScore(int score);
    void setupMainMenuItems(float width, float height);
    void setupPauseMenuItems(int lives, int score, float timeRemaining);
    MenuAction processClick(const sf::RenderWindow& window, sf::Vector2i mousePos);
private:
    void configureMenuItem(sf::Text& item, const std::string& text, unsigned int charSize,
        const sf::Color& color, const sf::Vector2f& position);
    
    void setupGameOverWinItems(float width, float height);
    void setupGameOverLoseItems(float width, float height);
    void setupTopBarItems(float width, float barHeight);
   
    std::vector<sf::Sprite> m_icons;
    int m_lastGameScore;
    MenuState m_currentState;
    sf::Font m_font;
    std::vector<sf::Text> m_menuItems;
    sf::Sprite m_backgroundSprite;
    int m_hoveredItemIndex;
};
