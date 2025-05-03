#pragma once
#include "GameObjects/MovingObject.h"
#include <vector>
#include "Managers/ResourceManager.h"
#include "consts.h"


class World;
class Player;
class Bomb;
class Explosion;

class Enemy : public MovingObject
{
public:

    Enemy(Player & player);
	virtual ~Enemy() = default;

    void collide(GameObject& other) override;
    void collide(Player& p) override;
    void collide(Enemy& e) override;
	void collide(Bomb& bomb) override;
	void collide(Explosion& ex) override;
	void collide(Door& door) override;

    void update(sf::Time deltaTime, World& world) override;
	virtual void reset() override;
    
   
    void clearPath();

   
private:
    Player& m_currentPlayer;
    bool m_isLookingRight;
    std::vector<Node> m_path;
    size_t m_currentTargetIndex;
    

    bool isValid(const Node& node, const std::vector<std::vector<bool>>& visited) const;
    void handleMovement(sf::Time deltaTime, World& world) override;
    std::optional<Node> m_randomTarget;
    sf::Vector2f getCellCenter(int gridX, int gridY) const;
    sf::Vector2f getCurrentCellCenter() const;
    std::optional<Node> chooseRandomNeighbor(int gridX, int gridY, const World& world) const;
    sf::Vector2f moveTowardsTarget(const sf::Vector2f& currentPos, const sf::Vector2f& targetCenter, float step);
    void updateDirection(const sf::Vector2f& oldPos, const sf::Vector2f& newPos);
    std::vector<Node> findPath(World& world);
};
