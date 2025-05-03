#pragma once
#include "GameObjects/GameObject.h"


class StaticObject : public GameObject
{
public:
    
    StaticObject() = default;

    
    StaticObject(const sf::Texture& texture,const sf::Vector2f& pos,bool blocked,bool breakable);

    virtual ~StaticObject() = default;

    bool isBlocked() const;
    bool isBreakable() const;
    void collide(GameObject& other) override;
    void collide(Player& other) override;
    void collide(Enemy& other) override;
	void collide(Bomb& other) override;
    void collide(Explosion& other) override;
	void collide(Door& other) override;
        
	

private:
    
    bool m_blocked;
    bool m_breakable;
};
