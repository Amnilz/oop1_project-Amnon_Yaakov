#include "GameObjects/StaticObject.h"
#include "GameObjects/MovingObject.h"
#include "GameObjects/Player.h"


StaticObject::StaticObject(const sf::Texture& texture,
    const sf::Vector2f& pos,
    bool blocked,
    bool breakable)
    : m_blocked(blocked), m_breakable(breakable)
{
    setTexture(texture);
    setPosition(pos);
}



bool StaticObject::isBlocked() const{return m_blocked;}
bool StaticObject::isBreakable() const{return m_breakable;}
void StaticObject::collide(GameObject& other){other.collide(*this);}
void StaticObject::collide(Player& other){}
void StaticObject::collide(Enemy& other){}
void StaticObject::collide(Bomb& other){}
void StaticObject::collide(Explosion& other){}
void StaticObject::collide(Door& other){}


