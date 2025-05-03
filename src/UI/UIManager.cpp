#include "UI/UIManager.h"
#include "Managers/ResourceManager.h"


UIManager::UIManager(sf::RenderWindow& window)
    : m_window(window)
    , m_newGameRequested(false)
    , m_exitRequested(false)
    , m_returnToMenuRequested(false)
    , m_helpRequested(false)
    , m_pauseRequested(false)
    , m_returnFromHelpRequested(false)
    , m_currentState(GameState::MENU),
    m_gameUI(), 
    m_menu(WINDOW_WIDTH,WINDOW_HEIGHT)
{
       
     
}

// אתחול UI עם גובה המפה
void UIManager::initWithMapHeight(float windowWidth, float windowHeight, float mapHeight)
{
    
    float bottomBarHeight = windowHeight - (TOP_BAR_OFFSET + mapHeight);
    if (bottomBarHeight < 0.f)
    {
        bottomBarHeight = 0.f;
    }
       

    m_gameUI.setup(windowWidth, windowHeight, TOP_BAR_OFFSET, mapHeight, bottomBarHeight);
    setMenuState(MenuState::MainMenu);
}

// עדכון UI בהתאם למצב המשחק 
void UIManager::update(int life, int score, float timeRemaining, GameState currentState, int currentLevel, int totalLevels)
{
    m_currentState = currentState;

    if (currentState == GameState::PLAYING) 
    {
        m_gameUI.update(life, score, timeRemaining,currentLevel,totalLevels);
        m_menu.setMenuState(MenuState::TopBar);
    }
    else if (currentState == GameState::PAUSED) 
    {
        m_menu.setMenuState(MenuState::Pause);
        m_menu.setupPauseMenuItems(life, score, timeRemaining);
    }

    if (currentState == GameState::MENU ||
        currentState == GameState::GAME_OVER ||
        currentState == GameState::PLAYING ||
        currentState == GameState::PAUSED) 
    {
        m_menu.updateHoverState(m_window);
    }
}

// ציור ה-UI בהתאם למצב המשחק
void UIManager::render(sf::RenderWindow& window)
{
    if (m_currentState == GameState::PLAYING) 
    {
        m_menu.draw(window);
        m_gameUI.draw(window);
    }
    else if (m_currentState != GameState::HELP) 
    {
        m_menu.draw(window);
    }
}

// בדיקה אם נדרש חזרה לתפריט
bool UIManager::shouldReturnToMenu() const {return m_returnToMenuRequested;}

// בדיקה אם יש בקשה להראות את המסך עזרה
bool UIManager::shouldShowHelp() const {return m_helpRequested;}

// עיבוד קלט ושינוי דגלי אירוע
void UIManager::handleInput(const sf::Event& event, GameState currentState)
{
    m_currentState = currentState;

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape) 
        {
            switch (m_currentState) 
            {
            case GameState::HELP:
                m_returnFromHelpRequested = true;
                break;
            case GameState::PLAYING:
                m_pauseRequested = true;
                break;
            case GameState::PAUSED:
                m_pauseRequested = false;
                break;
            default:
                break;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
        MenuAction action = m_menu.processClick(m_window, mousePos);
        switch (action) 
        {
        case MenuAction::NewGame:
            m_newGameRequested = true;
            break;
        case MenuAction::Help:
            m_helpRequested = true;
            break;
        case MenuAction::Exit:
            m_exitRequested = true;
            break;
        case MenuAction::ReturnToMenu:
            m_returnToMenuRequested = true;
            break;
        default:
            break;
        }
    }

    resetEventFlags(); // איפוס דגלי אירוע 
}

// איפוס דגלי אירוע
void UIManager::resetEventFlags()
{
    m_pauseRequested = false;
    m_returnFromHelpRequested = false;
}

// שינוי מצב התפריט
void UIManager::setMenuState(MenuState state){m_menu.setMenuState(state);}

// עדכון ניקוד במסך סיום משחק
void UIManager::setGameOverScore(int score){m_menu.setGameOverScore(score);}

// בדיקה אם נדרש התחלת משחק חדש
bool UIManager::shouldStartNewGame() const {return m_newGameRequested;}

// בדיקה אם נדרש לצאת
bool UIManager::shouldExitGame() const {return m_exitRequested;}

// בדיקה אם נדרש להשהות 
bool UIManager::shouldPauseGame() const {return m_pauseRequested;}

bool UIManager::shouldReturnFromHelp() const{return m_returnFromHelpRequested;}

// איפוס דגלי תפריט
void UIManager::resetMenuFlags()
{
    m_newGameRequested = false;
    m_exitRequested = false;
    m_returnToMenuRequested = false;
    m_helpRequested = false;
}

// ציור מסך עזרה
void UIManager::renderHelp(sf::RenderWindow& window)
{
    window.clear();
    sf::Texture& helpTexture = ResourceManager::instance().getTexture("helpMenu.png");
    sf::Sprite helpSprite(helpTexture);
    window.draw(helpSprite);
    window.display();
}
