#include "UI/GameUI.h"
#include "Managers/ResourceManager.h"
#include <string>
#include <iostream>

GameUI::GameUI()
{
}

void GameUI::setupBackground()
{
    float bottomY = m_windowHeight - m_bottomBarHeight;
    m_bgSprite.setPosition(0.f, bottomY);
    sf::FloatRect texBounds(0.f, 0.f, (float)m_bgSprite.getTexture()->getSize().x, (float)m_bgSprite.getTexture()->getSize().y);
    float scaleX = m_windowWidth / texBounds.width;
    float scaleY = m_bottomBarHeight / texBounds.height;
    m_bgSprite.setScale(scaleX, scaleY);
}

void GameUI::update(int life, int score, float timeRemaining, int currentLevel, int totalLevels)
{
   
    m_life = life;
    m_score = score;
    m_timeRemaining = timeRemaining;
    
    m_lifeText.setString(std::to_string(m_life));
    m_scoreText.setString("Score: " + std::to_string(m_score));
    int t = static_cast<int>(m_timeRemaining);
    m_timeText.setString(std::to_string(t));
    m_levelText.setString(std::string("Level: ") + std::to_string(currentLevel+1) + "-" + std::to_string(totalLevels));

}

void GameUI::draw(sf::RenderWindow& window)
{
    
    window.draw(m_bgSprite);
    window.draw(m_heartIcon);
    window.draw(m_clockIcon);
    window.draw(m_lifeText);
    window.draw(m_timeText);
    window.draw(m_scoreText);
    window.draw(m_levelText);
}

void GameUI::setup(float windowWidth, float windowHeight,
    float topBarHeight, float middleHeight,
    float bottomBarHeight)
{
    
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    m_topBarHeight = topBarHeight;
    m_middleHeight = middleHeight;
    m_bottomBarHeight = bottomBarHeight;

    m_font = ResourceManager::instance().getFont("WaffleCake.otf");
    m_bgSprite.setTexture(ResourceManager::instance().getTexture("menuBG.png"));
    m_heartIcon.setTexture(ResourceManager::instance().getTexture("heart.png"));
    m_clockIcon.setTexture(ResourceManager::instance().getTexture("clock.png"));

    
    setupTexts();
    setupBackground();
    setupPositions();
}
void GameUI::setupTexts()
{
    m_lifeText = createText(m_font, 20, sf::Color::White);
    m_timeText = createText(m_font, 20, sf::Color::White);
    m_scoreText = createText(m_font, 20, sf::Color::White);
    m_levelText = createText(m_font, 20, sf::Color::White);
}
void GameUI::setupPositions()
{
    
    float bottomStart = m_windowHeight - m_bottomBarHeight;
    float offset = 10.f; 

    m_heartIcon.setPosition(20.f, bottomStart + offset);
    m_clockIcon.setPosition(180.f, bottomStart + offset);
    m_lifeText.setPosition(60.f, bottomStart + offset);
    m_timeText.setPosition(220.f, bottomStart + 15.f);
    m_scoreText.setPosition(360.f, bottomStart + 15.f);
    m_levelText.setPosition(500.f, bottomStart + 15.f);
}

sf::Text GameUI::createText(const sf::Font& font, unsigned int charSize, const sf::Color& color) 
{

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(charSize);
    text.setFillColor(color);
    return text;
}