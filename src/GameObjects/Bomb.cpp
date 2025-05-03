#include "GameObjects/Bomb.h"
#include "Managers/ResourceManager.h"
#include "World.h"
#include <iostream>
#include "consts.h"
#include <vector>
#include "GameObjects/Explosion.h"
#include "Managers/GameController.h"



Bomb::Bomb(const sf::Vector2f& position, float countdown,GameController& gc)
	: m_timeLeft(countdown), m_gc(gc)
{

    auto& tex = ResourceManager::instance().getTexture("bomb4.png");
    setTexture(tex); 
    setCenter();
    setPosition(position); 
    setScale(BOMB_SCALE, BOMB_SCALE);
    setToRemove(false);
    setEnemy(false);
}


//  עדכון זמן, שינוי טקסטורה ובדיקת התפוצצות
void Bomb::update(sf::Time dt, World& world)
{
    if (getToRemove())
    {
        return;
    }
    m_timeLeft -= dt.asSeconds(); // הפחתת הזמן שנותר
    if (m_timeLeft > 3.f)
    {
        setTexture(ResourceManager::instance().getTexture("bomb4.png"));
    }
    else if (m_timeLeft > 2.f)
    {
        setTexture(ResourceManager::instance().getTexture("bomb3.png"));
    }
    else if (m_timeLeft > 1.f)
    {
        setTexture(ResourceManager::instance().getTexture("bomb2.png"));
    }
    else
    {
        setTexture(ResourceManager::instance().getTexture("bomb1.png"));
    }
    if (!getToRemove() && m_timeLeft <= 0.f)
    {
        m_gc.addExplosion(getPosition(), "center.png");
        setToRemove(true);
    }
}

void Bomb::reset()
{
    setToRemove(true);
}

void Bomb::collide(GameObject&) {}
void Bomb::collide(Player& other) {}
void Bomb::collide(Enemy& other) {}
void Bomb::collide(Bomb&) {}
void Bomb::collide(Explosion&) {}
void Bomb::collide(Door&) {}
