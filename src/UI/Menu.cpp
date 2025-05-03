#include "UI/Menu.h"
#include "Managers/ResourceManager.h"
#include <iostream>
#include "consts.h"

Menu::Menu(float width, float height)
    : m_hoveredItemIndex(-1)
    , m_font()
    , m_backgroundSprite()
    , m_menuItems()
    , m_currentState(MenuState::MainMenu)  
{
    m_font = ResourceManager::instance().getFont("WaffleCake.otf");
    m_backgroundSprite.setTexture(ResourceManager::instance().getTexture("menuBG.png"));
    setupMainMenuItems(width, height);
}

// שינוי מצב התפריט ועדכון פריטי התפריט בהתאם
void Menu::setMenuState(MenuState state)
{
    m_currentState = state;
    m_menuItems.clear();

    switch (m_currentState)
    {
    case MenuState::MainMenu:
        setupMainMenuItems(WINDOW_WIDTH, WINDOW_HEIGHT);
        break;

    case MenuState::GameOverWin:
        setupGameOverWinItems(WINDOW_WIDTH, WINDOW_HEIGHT);
        break;

    case MenuState::GameOverLose:
        setupGameOverLoseItems(WINDOW_WIDTH, WINDOW_HEIGHT);
        break;

    case MenuState::TopBar:
        setupTopBarItems(WINDOW_WIDTH, TOP_BAR_OFFSET);
        break;
    case MenuState::Pause:
        setupPauseMenuItems(3, 0, 150.f);  
        break;
    }
}

// ציור התפריט
void Menu::draw(sf::RenderWindow& window)
{
    if (m_currentState == MenuState::TopBar)
    {
        sf::RectangleShape barBg(sf::Vector2f(1280.f, 15.f));
        barBg.setFillColor(sf::Color(200, 200, 200)); 
        barBg.setPosition(0.f, 0.f);
        window.draw(barBg);
    }
    else
    {
        window.draw(m_backgroundSprite);
    }
    for (int i = 0; i < (int)m_menuItems.size(); i++)
    {
        window.draw(m_menuItems[i]);
    }

    if (m_currentState == MenuState::Pause) 
    {
        for (const auto& icon : m_icons) 
        {
			
            window.draw(icon);
        }
    }
}
// טיפול בלחיצה
int Menu::handleClick(const sf::RenderWindow& window, sf::Vector2i mousePos)
{
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    for (int i = 0; i < (int)m_menuItems.size(); ++i)
    {
        if (m_menuItems[i].getGlobalBounds().contains(worldPos))
        {
            return i;
        }
    }
    return -1;
}

// צביעת פריט התפריט שנמצא מעליו
void Menu::updateHoverState(const sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    int newHoveredIndex = -1;
    for (int i = 0; i < (int)m_menuItems.size(); ++i)
    {
        if (m_menuItems[i].getGlobalBounds().contains(worldPos))
        {
            newHoveredIndex = i;
            break;
        }
    }

    if (newHoveredIndex != m_hoveredItemIndex)
    {
        if (m_hoveredItemIndex != -1)
        {
            sf::Color originalColor =
                (m_currentState == MenuState::TopBar) ? sf::Color::Black : sf::Color::White;

            
            if (m_currentState != MenuState::GameOverWin && m_currentState != MenuState::GameOverLose ||
                m_hoveredItemIndex > 1)
            {
                m_menuItems[m_hoveredItemIndex].setFillColor(originalColor);
            }
        }

        if (newHoveredIndex != -1)
        {
           
            if (m_currentState != MenuState::GameOverWin && m_currentState != MenuState::GameOverLose ||
                newHoveredIndex > 1)
            {
                m_menuItems[newHoveredIndex].setFillColor(sf::Color::Yellow);
            }
        }

        m_hoveredItemIndex = newHoveredIndex;
    }
}

// יצירת פריטי תפריט ראשי 
void Menu::setupMainMenuItems(float width, float height)
{
    m_menuItems.resize(3);

    configureMenuItem(m_menuItems[0], "Play", 40, sf::Color::White, { width / 2.f, height / 2.f - 50.f });
    configureMenuItem(m_menuItems[1], "Help", 40, sf::Color::White, { width / 2.f, height / 2.f + 10.f });
    configureMenuItem(m_menuItems[2], "Exit", 40, sf::Color::White, { width / 2.f, height / 2.f + 70.f });

}
// יצירת פריטי תפריט במצב Pause 
void Menu::setupPauseMenuItems(int lives, int score, float timeRemaining) 
{
    m_menuItems.resize(5);

    m_menuItems.resize(5);
    m_icons.clear();

  
    configureMenuItem(m_menuItems[0], "Game Paused", 50, sf::Color::White,
        { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 120.f });

   
    sf::Sprite heartIcon, clockIcon;
    heartIcon.setTexture(ResourceManager::instance().getTexture("heart.png"));
    clockIcon.setTexture(ResourceManager::instance().getTexture("clock.png"));

    float iconX = WINDOW_WIDTH / 2.f - 170.f;
    heartIcon.setPosition(iconX, WINDOW_HEIGHT / 2.f - 50.f);
    heartIcon.setScale(1.2f, 1.2f);

    clockIcon.setPosition(iconX, WINDOW_HEIGHT / 2.f );
    clockIcon.setScale(1.2f, 1.2f);

    m_icons.push_back(heartIcon);
    m_icons.push_back(clockIcon);

    
    configureMenuItem(m_menuItems[1], std::to_string(lives), 40, sf::Color::White,
        { iconX + 100.f, WINDOW_HEIGHT / 2.f - 40.f });
    
    configureMenuItem(m_menuItems[2], std::to_string(static_cast<int>(timeRemaining)), 40, sf::Color::White,
        { iconX + 100.f, WINDOW_HEIGHT / 2.f + 10.f });
    
    configureMenuItem(m_menuItems[3], "Score: " + std::to_string(score), 40, sf::Color::White,
        { WINDOW_WIDTH / 2.f -50.f, WINDOW_HEIGHT / 2.f + 60.f });
    
    configureMenuItem(m_menuItems[4], "Press ESC to Return", 30, sf::Color::Yellow,
        { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 140.f });
}
//  לחיצה על התפריט והמרת האינדקס לפעולה מתאימה
MenuAction Menu::processClick(const sf::RenderWindow& window, sf::Vector2i mousePos)
{
   
        int clickedIndex = handleClick(window, mousePos);
        if (clickedIndex == -1) 
        {
            return MenuAction::None;
        }

       
        switch (m_currentState) 
        {
        case MenuState::MainMenu:
            if (clickedIndex == 0) 
            {
                return MenuAction::NewGame;
            }
            else if (clickedIndex == 1) 
            {
                return MenuAction::Help;
            }
            else if (clickedIndex == 2) 
            {
                return MenuAction::Exit;
            }
            break;
        case MenuState::GameOverWin:
        case MenuState::GameOverLose:
            
            if (clickedIndex == 2) 
            {
                return MenuAction::ReturnToMenu;
            }
            else if (clickedIndex == 3) 
            {
                return MenuAction::Exit;
            }
            break;
        case MenuState::TopBar:
            if (clickedIndex == 0)
            {
                return MenuAction::ReturnToMenu;
            }
            else if (clickedIndex == 1) 
            {
                return MenuAction::Help;
            }
            else if (clickedIndex == 2) 
            {
                return MenuAction::Exit;
            }
            break;
        case MenuState::Pause:
            
            break;
        default:
            break;
        }
        return MenuAction::None;
    
}
// הגדרת פריט תפריט 
void Menu::configureMenuItem(sf::Text& item, const std::string& text, unsigned int charSize,
    const sf::Color& color, const sf::Vector2f& position)
{
    item.setFont(m_font);
    item.setString(text);
    item.setCharacterSize(charSize);
    item.setFillColor(color);
    auto bounds = item.getLocalBounds();
    item.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    item.setPosition(position);
}

// יצירת תפריט ניצחון
void Menu::setupGameOverWinItems(float width, float height)
{
    m_menuItems.resize(4);

    configureMenuItem(m_menuItems[0], "Victory!", 50, sf::Color::Blue,{ width / 2.f, height / 2.f - 100.f });
    std::string scoreStr = "Score: " + std::to_string(m_lastGameScore);
    configureMenuItem(m_menuItems[1], scoreStr, 40, sf::Color::White,{ width / 2.f, height / 2.f - 20.f });
    configureMenuItem(m_menuItems[2], "Main Menu", 40, sf::Color::White,{ width / 2.f, height / 2.f + 60.f });
    configureMenuItem(m_menuItems[3], "Exit", 40, sf::Color::White,{ width / 2.f, height / 2.f + 130.f });
}

void Menu::setupGameOverLoseItems(float width, float height)
{
    m_menuItems.resize(4);


    configureMenuItem(m_menuItems[0], "Defeat!", 50, sf::Color::Red, { width / 2.f, height / 2.f - 100.f });
    std::string scoreStr = "Score: " + std::to_string(m_lastGameScore);
    configureMenuItem(m_menuItems[1], scoreStr, 40, sf::Color::White, { width / 2.f, height / 2.f - 20.f });
    configureMenuItem(m_menuItems[2], "Main Menu", 40, sf::Color::White, { width / 2.f, height / 2.f + 60.f });
    configureMenuItem(m_menuItems[3], "Exit", 40, sf::Color::White, { width / 2.f, height / 2.f + 130.f });

}

void Menu::setGameOverScore(int score) {m_lastGameScore = score;}

// יצירת TopBar 
void Menu::setupTopBarItems(float width, float barHeight)
{
    std::vector<std::string> labels = { "Main Menu", "Help", "Exit" };
    m_menuItems.resize(labels.size());

    float startX = 10.f;  
    float spacing = 20.f;  
    unsigned int fontSize = 15;  

    for (size_t i = 0; i < labels.size(); i++)
    {
        m_menuItems[i].setFont(m_font);
        m_menuItems[i].setString(labels[i]);
        m_menuItems[i].setCharacterSize(fontSize);
        m_menuItems[i].setFillColor(sf::Color::Black);
        auto localBounds = m_menuItems[i].getLocalBounds();
        float posY = (barHeight * 0.5f) - (localBounds.height * 0.5f) - localBounds.top;
        m_menuItems[i].setPosition(startX, posY);
        startX += (localBounds.width + spacing);
    }
}


