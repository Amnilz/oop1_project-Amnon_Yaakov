#pragma once
#include <SFML/Graphics.hpp>

class GameUI {
public:
    
    GameUI();

    
    void update(int life, int score, float timeRemaining , int currentLevel, int totalLevels);
    void draw(sf::RenderWindow& window);
    void setup(float windowWidth, float windowHeight, float topBarHeight, float middleHeight, float bottomBarHeight);
    
    void setupBackground();


private:
   
    int   m_life;
    int   m_score;
    float m_timeRemaining;
    void setupTexts();

    void setupPositions();

    sf::Text createText(const sf::Font& font, unsigned int charSize, const sf::Color& color);
    
    float m_windowWidth;
    float m_windowHeight;
    float m_topBarHeight;
    float m_middleHeight;
    float m_bottomBarHeight;

   
    sf::Sprite m_bgSprite;

    sf::Sprite m_heartIcon;
    sf::Sprite m_clockIcon;

    
    sf::Font   m_font;
    sf::Text   m_lifeText;
    sf::Text   m_timeText;
    sf::Text   m_scoreText;
    sf::Text   m_levelText;
};
