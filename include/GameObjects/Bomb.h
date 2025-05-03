
#pragma once

#include <SFML/System.hpp>
#include "UpdatableObject.h"
class World;
class GameController;


class Bomb : public UpdatableObject
{
public:

    Bomb(const sf::Vector2f& position, float countdown, GameController & gc);
	virtual ~Bomb() = default;

    
    virtual void update(sf::Time dt, World& world) override;
	virtual void reset() override;


	void collide(GameObject&) override;
   
    void collide(Player&) override;
    void collide(Enemy&) override;
	void collide(Bomb&) override;
	void collide(Explosion&) override;
	void collide(Door&) override;



private:
	GameController& m_gc;
    float m_timeLeft;  
    
};

