
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "UpdatableObject.h"


class World;
class GameController;

class Explosion : public UpdatableObject
{
public:

    Explosion(const sf::Vector2f& centerPos,
        const std::string& type,
        World& world,
        GameController& gc);

    Explosion(const sf::Vector2f& pos, const std::string& type);

	virtual ~Explosion() = default;

    virtual void update(sf::Time dt, World& world);
	virtual void reset() override;

   
    void collide(GameObject& other) override;
    void collide(Player& other) override;
    void collide(Enemy& other) override;
    void collide(Bomb& other) override;
    void collide(Explosion& other) override;
	void collide(Door& other) override;

private:
    void performExplosion(const sf::Vector2f& centerPos, World& world, GameController& gc);
    float m_timeToLive;   
};

