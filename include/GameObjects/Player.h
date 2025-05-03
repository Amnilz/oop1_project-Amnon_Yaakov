#pragma once

#include "GameObjects/MovingObject.h"
#include "Managers/ResourceManager.h"
#include "consts.h"


class World;
class StaticObject;
class Enemy;
class GameController;
class Bomb;
class Explosion;
class Gift;
class Door;


class Player : public MovingObject
{
public:
   
    Player(GameController* gc, const sf::Vector2f& startPos);

    void update(sf::Time dt, World& world) override;
 
    void collide(GameObject& other) override;
   
    void collide(Player& other) override;
    void collide(Enemy& enemy) override;
	void collide(Bomb& bomb) override;
	void collide(Explosion& explosion) override;
    void collide(Door& door);
	void collide(Gift& gift);

    void addGiftEffect(GiftEffect eff);
    const std::vector<GiftEffect>& getGiftEffects() const;
    void clearGiftEffects();
   

    void setLife(int life);
	int getLife() const ;
    int getScore() const;
    void addScore(int points);
	

    void placeBomb();
	void setScore(int score);
private:

    float m_speed;
    int m_score;
    int m_life;
    std::vector<GiftEffect> m_giftEffects;
    GameController* m_gameController;

    void handleMovement(sf::Time deltaTime, World& world) override;
    bool canMoveTo(const sf::Vector2f& newPos, World& world);

   

};
