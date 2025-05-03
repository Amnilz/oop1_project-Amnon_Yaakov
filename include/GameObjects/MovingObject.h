#pragma once
#include "GameObjects/GameObject.h"
#include <SFML/System/Time.hpp>
#include "UpdatableObject.h"


class World;
class Player;
class Enemy;


class MovingObject : public UpdatableObject
{
public:
    MovingObject(const sf::Vector2f& startPos, float speed);
	virtual ~MovingObject() = default;
    

    
    virtual void update(sf::Time dt, World& world)=0;
	virtual void reset() override;
 
    void setMovementSpeed(float speed) override;
    float getMovementSpeed() const;

    void setStartPos(const sf::Vector2f& startPos);

    sf::Vector2f getStartPos() const;

    virtual void handleMovement(sf::Time deltaTime,World& world) = 0;


private:

    sf::Vector2f m_startPos;
    float m_speed;
};
