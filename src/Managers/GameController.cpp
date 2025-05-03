#include "Managers/GameController.h"
#include <SFML/System/Clock.hpp>
#include "consts.h"
#include <iostream>
#include <vector>


GameController::GameController()
    : m_player(this, sf::Vector2f(0.f, 0.f))
    , m_world()
	, m_initialEnemyCount(0)
    , m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "BomberMan")
    , m_state(GameState::MENU)
    , m_previousState(GameState::MENU)
    , m_isGameOver(false)
    , m_enemiesFrozen(false)
    , m_pendingReset(false)
    , m_uiManager(m_window)
    , m_timeRemaining(sf::seconds(INITIAL_LEVEL_TIME))
    , m_initialLevelTime(sf::seconds(INITIAL_LEVEL_TIME))
    , m_giftSpawnInterval(sf::seconds(GIFT_SPAWN_INTERVAL))
    , m_freezeClock()
    , m_giftSpawnClock()
    , m_levelClock()
    , m_levelManager()
{
    m_window.setFramerateLimit(60);
    initializeResources();
    initializeUI();
    initializeLevel();
}


// טעינת משאבים
void GameController::initializeResources()
{
    auto& resourceManager = ResourceManager::instance();
    resourceManager.getTexture("menuBG.png");
    resourceManager.getFont("WaffleCake.otf");
    if (!m_music.openFromFile("background.wav"))
    {
        std::cout << "Error loading music" << std::endl;
    }
    m_music.setLoop(true);
    m_music.setVolume(MUSIC_VOLUME);
}
// אתחול UI לפי גובה המפה
void GameController::initializeUI()
{
    
    m_uiManager.initWithMapHeight(WINDOW_WIDTH, WINDOW_HEIGHT, MAP_HEIGHT_PX);
}
// אתחול השלבים
void GameController::initializeLevel() 
{
    m_levelManager.init("Playlist.txt");
}


// לולאת המשחק 
void GameController::run()
{
    initGame();
    sf::Clock clock;
    while (m_window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        // עיבוד קלט ואירועים
        handleEvents();

        switch (m_state)
        {
        case GameState::MENU:
            m_music.stop();
            updateUIMenu();
            render();
            break;

        case GameState::PLAYING:
            m_uiManager.setMenuState(MenuState::TopBar);
            update(deltaTime);
            render();
            if (m_isGameOver)
            {
                gameOverLose();
            }
            break;

        case GameState::PAUSED:
            render();
            break;

        case GameState::HELP:
            m_uiManager.renderHelp(m_window);
            break;

        case GameState::GAME_OVER:
            updateUIGameOver();
            m_music.stop();
            if (m_uiManager.shouldReturnToMenu())
            {
                resetGame();
                m_state = GameState::MENU;
                m_uiManager.setMenuState(MenuState::MainMenu);
            }
            render();
            break;
        }
    }
}
// אתחול המשחק טעינת הרמה הנוכחית
void GameController::initGame()
{
    m_levelManager.loadCurrentLevel(m_world, m_player, m_gameObjects);
    m_initialEnemyCount = 0;
    for (const auto& obj : m_gameObjects)
    {
        if (obj->isEnemyObject())  // בודק אם האובייקט הוא אויב
        {
            m_initialEnemyCount++;
        }
	}
   
    m_uiManager.initWithMapHeight(WINDOW_WIDTH, WINDOW_HEIGHT, MAP_HEIGHT_PX);
    m_timeRemaining = m_initialLevelTime;
}

// עיבוד אירועים 
void GameController::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_uiManager.handleInput(event, m_state);
        processSystemEvent(event);
        processUIFlags();
        m_uiManager.resetMenuFlags();
    }
}
// עיבוד אירועי מערכת - סגירת חלון, לחיצות מקלדת
void GameController::processSystemEvent(const sf::Event& event)
{
    if (event.type == sf::Event::Closed)
    {
        m_music.stop();
		m_window.close();
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        handleKeyPressedEvent(event);
    }
}
// עיבוד דגלי UI
void GameController::processUIFlags()
{
    if (m_uiManager.shouldReturnFromHelp())
    {
        m_state = m_previousState;
    }
    if (m_uiManager.shouldPauseGame())
    {
        m_state = (m_state == GameState::PLAYING) ? GameState::PAUSED : GameState::PLAYING;
    }
    if (m_uiManager.shouldExitGame())
    {
        m_window.close();
    }
    if (m_uiManager.shouldStartNewGame())
    {
        resetGame();
        m_state = GameState::PLAYING;
        m_uiManager.setMenuState(MenuState::TopBar);
        m_music.play();
        m_levelClock.restart();
    }
    if (m_uiManager.shouldReturnToMenu())
    {
        m_state = GameState::MENU;
        m_uiManager.setMenuState(MenuState::MainMenu);
    }
    if (m_uiManager.shouldShowHelp())
    {
        m_previousState = m_state;
        m_state = GameState::HELP;
    }
}
// טיפול באירועי מקלדת לפי מצב המשחק
void GameController::handleKeyPressedEvent(const sf::Event& event)
{
    switch (m_state)
    {
    case GameState::HELP:
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_state = m_previousState;
        }
        break;

    case GameState::PLAYING:
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_state = GameState::PAUSED;
            updateUIGameplay();
        }
        else
        {
            handleKeyPress(event.key.code);
        }
        break;

    case GameState::PAUSED:
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_state = GameState::PLAYING;
        }
        break;

    case GameState::GAME_OVER:
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_window.close();
        }
        break;

    default:
        break;
    }
}



void GameController::handleKeyPress(sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::B:
        m_player.placeBomb();
        break;
    default:
        break;
    }
}

// עדכון לוגיקת משחק 
void GameController::update(sf::Time deltaTime)
{
    updateTime(deltaTime);
    if (m_timeRemaining <= sf::Time::Zero)
    {
        gameOverLose();
        return;
    }

    if (m_pendingReset)
    {
        sf::sleep(sf::milliseconds(10));
        resetLevel();
        m_pendingReset = false;
        return;
    }

    processCollisions();
    updateGameObjects(deltaTime);
    processEnemyFreeze();
    processGiftSpawn();
    updateUIGameplay();
}

// עדכון הזמן שנותר 
void GameController::updateTime(sf::Time deltaTime)
{
    m_timeRemaining -= deltaTime;
}

// בדיקת התנגשויות בין האובייקטים
void GameController::processCollisions()
{

    if (m_pendingReset)
    {
        return;
    }
    for (auto& obj : m_gameObjects)
    {
        //בדיקה שחקן מול אובייקטים
        if (m_player.getGlobalBounds().intersects(obj->getGlobalBounds()))
        {
            m_player.collide(*obj);    
            obj->collide(m_player);    
        }

    }

    for (int i = 0; i < (int)m_gameObjects.size(); ++i)
    {
        for (int j = i + 1; j < (int)m_gameObjects.size(); ++j)
        {
            //בדיקה אוייבקט בין אובייקט
            if (m_gameObjects[i]->getGlobalBounds().intersects(m_gameObjects[j]->getGlobalBounds()))
            {
                m_gameObjects[i]->collide(*m_gameObjects[j]); 
                m_gameObjects[j]->collide(*m_gameObjects[i]); 
            }
        }
    }
    m_world.checkCollisionWithStaticObjects(m_player);
}
    

// עדכון אובייקטים
void GameController::updateGameObjects(sf::Time deltaTime)
{
    m_player.update(deltaTime, m_world);

    for (auto& obj : m_gameObjects)
    {
       obj->update(deltaTime, m_world);
    }

    for (auto& newObj : m_objectsToAdd)
    {
        m_gameObjects.push_back(std::move(newObj));
    }
    m_objectsToAdd.clear();

    m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
        [](const std::unique_ptr<UpdatableObject>& obj)
        {return obj->getToRemove();}),m_gameObjects.end());

    auto& effects = m_player.getGiftEffects();
    for (auto effect : effects)
    {
        switch (effect)
        {
        case GiftEffect::FreezeEnemies:
            freezeEnemies();
			
            break;
        case GiftEffect::KillOneEnemy:
            killEnemy();
			
            break;
        case GiftEffect::AddTime:
            addTime(sf::seconds(TIME_GIFT_DURATION));
            playSound("clock.ogg");
            break;
        case GiftEffect::AddLife:
            m_player.setLife(m_player.getLife() + 1);
			playSound("lifePickup.wav");
            break;
        default:
            break;
        }
    }
    // מנקים את הרשימה בפריים הזה, כדי שבפריים הבא אין כפילויות
    m_player.clearGiftEffects();
}
void GameController::queueObjectToAdd(std::unique_ptr<UpdatableObject> obj)
{
    m_objectsToAdd.push_back(std::move(obj));
}

void GameController::processEnemyFreeze()
{
    if (m_enemiesFrozen && m_freezeClock.getElapsedTime() >= sf::seconds(FREEZE_DURATION))
    {
        m_enemiesFrozen = false;
        for (auto& obj : m_gameObjects)
        {
            if (obj->isEnemyObject() && !obj->getToRemove())
            {
                obj->setMovementSpeed(ENEMY_DEFAULT_SPEED);
            }
        }
    }
}


// יצירת מתנה אקראית 
void GameController::processGiftSpawn()
{
    if (m_giftSpawnClock.getElapsedTime() >= m_giftSpawnInterval)
    {
        m_world.spawnRandomGift();
        m_giftSpawnClock.restart();
    }
}


// עדכון UI במהלך המשחק 
void GameController::updateUIGameplay()
{
    m_uiManager.update(m_player.getLife(), m_player.getScore(), m_timeRemaining.asSeconds(),m_state,
        m_levelManager.getCurrentLevel(), m_levelManager.getLevelCount());
}
// עדכון ממשק התפריט
void GameController::updateUIMenu()
{
    m_uiManager.update(0, 0, 0, m_state,m_levelManager.getCurrentLevel(),m_levelManager.getLevelCount());
}
// עדכון מסך סיום המשחק
void GameController::updateUIGameOver()
{
    m_uiManager.update(0, m_player.getScore(), 0, m_state, m_levelManager.getCurrentLevel(), m_levelManager.getLevelCount());
}


// הוספת פצצה 
void GameController::addBomb(sf::Vector2f bombPos) 
{
    m_gameObjects.push_back(std::make_unique<Bomb>(bombPos,BOMB_TIMER,*this));
}
// הוספת פיצוץ
void GameController::addExplosion(sf::Vector2f explosionPos, const std::string& texture)
{
   
    queueObjectToAdd(std::make_unique<Explosion>(explosionPos, texture, m_world, *this));
}
void GameController::addSubExplosion(sf::Vector2f pos, const std::string& texture)
{
    queueObjectToAdd(std::make_unique<Explosion>(pos, texture));
}

void GameController::killEnemy()
{
    
    for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
    {
           // בודק אם האובייקט הוא אויב ושעוד לא סומן להסרה
       if ((*it)->isEnemyObject() && !(*it)->getToRemove())
        {
                m_player.addScore(5);       // מוסיף ניקוד לפי הצורך
                (*it)->setToRemove(true);
                playSound("enemyDie.wav"); 
                break;                      
        }
    }

     // מסיר את כל האובייקטים שסומנו להסרה מהרשימה
     m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
     [](const std::unique_ptr<UpdatableObject>& obj)
     {return obj->getToRemove();}),m_gameObjects.end());
    

}
void GameController::freezeEnemies()
{
    // מעבר על כל האובייקטים ובדיקה אם הם אויבים
    for (auto& obj : m_gameObjects)
    {
        if (obj->isEnemyObject() && !obj->getToRemove())
        {
			playSound("freeze.wav");
            obj->setMovementSpeed(0.f);
        }
    }
    // אתחול הטיימר וסימון אויבים קפואים"
    m_freezeClock.restart();
    m_enemiesFrozen = true;
}



// איפוס השלב 
void GameController::resetLevel()
{
    for (auto& object : m_gameObjects)
    {
       object->reset();
    }


    m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
        [](const std::unique_ptr<UpdatableObject>& obj)
        {
            return obj->getToRemove();
        }),
        m_gameObjects.end());

    m_player.reset();
}
// סימון איפוס 
void GameController::setPendingReset() 
{
    m_pendingReset = true;
}
// בדיקה אם יש איפוס ממתין
bool GameController::getPendingReset() const 
{
    return m_pendingReset;
}
// הוספת זמן
void GameController::addTime(sf::Time extraTime) 
{
    m_timeRemaining += extraTime;
}
// התחלת טיימר הקפאת אויבים
void GameController::startFreezeTimer()
{

    m_freezeClock.restart();
    m_enemiesFrozen = true;
}
// מעבר לשלב הבא – אם אין רמה נוספת, סיום משחק ניצחון
void GameController::nextLevel()
{
    if (m_levelManager.isLastLevel())
    {
        scoreEnemies();
        finishGameVictory();
        return;
    }
    scoreEnemies();
    if (m_levelManager.loadNextLevel(m_world, m_player, m_gameObjects))
    {
        m_timeRemaining = m_initialLevelTime;
     
    }
}
// חישוב בונוס על מספר האויבים שנותרו
void GameController::scoreEnemies()
{
    m_player.addScore(m_initialEnemyCount*3 +25);
}
// סיום המשחק בניצחון
void GameController::finishGameVictory()
{
    playSound("gameVictory.wav");
    m_uiManager.setGameOverScore(m_player.getScore());
    m_uiManager.setMenuState(MenuState::GameOverWin);
    m_state = GameState::GAME_OVER;
}
// הפעלת קול
void GameController::playSound(std::string soundName)
{
    m_sound.setBuffer(ResourceManager::instance().getSound(soundName));
    m_sound.setVolume(EFFECT_VOLUME);
    m_sound.play();
}
// סיום המשחק בהפסד
void GameController::gameOverLose()
{
 
    m_isGameOver = true;
    playSound("gameOver.ogg");
    m_state = GameState::GAME_OVER;
    m_uiManager.setGameOverScore(m_player.getScore());
    m_uiManager.setMenuState(MenuState::GameOverLose);
}

// איפוס המשחק
void GameController::resetGame()
{
    m_isGameOver = false;
    m_pendingReset = false;
    m_timeRemaining = m_initialLevelTime;
    m_levelManager.reset();
    m_world.clearStatics();
    m_player.setScore(0);
    m_player.setLife(INITIAL_LIVES);
    m_levelManager.loadCurrentLevel(m_world, m_player, m_gameObjects);
}

// עדכון המסך
void GameController::render()
{
    m_window.clear();

    if (m_state == GameState::PLAYING)
    {
        renderGameObjects();
        m_uiManager.render(m_window);
    }
    else if (m_state == GameState::PAUSED)
    {
        m_uiManager.render(m_window);
    }
    else
    {
        m_uiManager.render(m_window);
    }

    m_window.display();
}

// ציור אובייקטים במשחק
void GameController::renderGameObjects()
{
    
    m_world.draw(m_window);
    m_player.draw(m_window);

	for (auto& object : m_gameObjects)
	{
		object->draw(m_window);
	}
}
