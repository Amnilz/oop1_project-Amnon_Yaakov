#include "GameObjects/Player.h"
#include "World.h"
#include "GameObjects/StaticObject.h"
#include "GameObjects/Enemy.h"
#include "Managers/ResourceManager.h"
#include "Managers/GameController.h"

#include <iostream>
#include "consts.h"


Player::Player(GameController* gc, const sf::Vector2f& startPos)
    : MovingObject(startPos, PLAYER_DEFAULT_SPEED), 
    m_gameController(gc),                         
    m_life(INITIAL_LIVES),                        
    m_score(0),                                   
    m_speed(PLAYER_DEFAULT_SPEED)                 
{
    
    auto& tex = ResourceManager::instance().getTexture("player.png");
    setTexture(tex);
    setCenter();
    setScale(PLAYER_DEFAULT_SCALE, PLAYER_DEFAULT_SCALE);
    
}

// עדכון השחקן 
void Player::update(sf::Time dt, World& world)
{
    handleMovement(dt, world);
}

//  תנועת השחקן  
void Player::handleMovement(sf::Time deltaTime, World& world)
{
    float deltaSec = deltaTime.asSeconds();
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        movement.x -= m_speed * deltaSec;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        movement.x += m_speed * deltaSec;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        movement.y -= m_speed * deltaSec;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        movement.y += m_speed * deltaSec;
    }

    // חישוב המיקום החדש 
    sf::Vector2f oldPos = getPosition();
    sf::Vector2f newPos = oldPos + movement;

    // בדיקה האם השחקן יכול לעבור למיקום החדש 
    if (!canMoveTo(newPos, world))
    {
        return;
    }
    // עדכון המיקום בפועל
    setPosition(newPos);
}

// בדיקה אם השחקן יכול לנוע
bool Player::canMoveTo(const sf::Vector2f& newPos, World& world)
{
    // שמירת המיקום הישן
    sf::Vector2f oldPos = getPosition();
    // שינוי זמני למיקום החדש
    setPosition(newPos);
    sf::FloatRect newBounds = getGlobalBounds();

    setPosition(oldPos);
    return world.isCellBlocked(newBounds);
}

// יצירת פצצה במיקום השחקן
void Player::placeBomb()
{
    sf::Vector2f playerPos = getPosition();
    sf::Vector2f bombPos;
    int col = static_cast<int>(playerPos.x / TILE_SIZE);
    int row = static_cast<int>((playerPos.y - TOP_BAR_OFFSET) / TILE_SIZE);

    // חישוב מרכז התא של השחקן 
    bombPos.x = col * TILE_SIZE + TILE_SIZE / 2.f;
    bombPos.y = row * TILE_SIZE + TILE_SIZE / 2.f + TOP_BAR_OFFSET;

    // הוספת פצצה
    if (m_gameController)
    {
        m_gameController->addBomb(bombPos);
    }
}

// עדכון ניקוד
void Player::setScore(int score){m_score = score;}

void Player::collide(GameObject& other){}

void Player::collide(Player& other){}

//התנגשות עם אויב  
void Player::collide(Enemy& enemy)
{
    if (m_life > 0)
    {
        m_life--;

        if (m_gameController)
        {
            m_gameController->setPendingReset();
        }
    }
    if (m_life == 0 && m_gameController)
    {
        m_gameController->gameOverLose();
    }
}

//  התנגשות עם דלת 
void Player::collide(Door& door)
{
    if (m_gameController)
    {
        static sf::Sound doorSound;
        doorSound.setBuffer(ResourceManager::instance().getSound("door.wav"));
        doorSound.setVolume(EFFECT_VOLUME);
        doorSound.play();
        m_gameController->nextLevel();
    }
}


void Player::collide(Bomb& bomb) {}

//  התנגשות עם פיצוץ
void Player::collide(Explosion& explosion) 
{
    if (m_life <= 0 || m_gameController->getPendingReset()) 
    {
        return;
    }
    m_life--;
    if (m_life <= 0) 
    {
        if (m_gameController)
        {
            m_gameController->gameOverLose();
        }  
        return;
    }
    m_gameController->setPendingReset();
    explosion.setToRemove(true);
}


// התנגשות עם מתנה
void Player::collide(Gift& gift){gift.collide(*this);}

void Player::addGiftEffect(GiftEffect eff){m_giftEffects.push_back(eff);}

const std::vector<GiftEffect>& Player::getGiftEffects() const {return m_giftEffects;}

void Player::clearGiftEffects(){m_giftEffects.clear();}

void Player::setLife(int life) {m_life = life;}

int Player::getLife() const {return m_life;}

int Player::getScore() const {return m_score;}

void Player::addScore(int points) {m_score += points;}

