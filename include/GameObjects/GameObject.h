#pragma once
#include <SFML/Graphics.hpp>


class StaticObject;
class MovingObject;
class Player;
class Enemy;
class Bomb;
class Explosion;
class Door;

class GameObject 
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    
     void setPosition(const sf::Vector2f& pos);
     sf::Vector2f getPosition() const;
     sf::FloatRect getGlobalBounds() const;
     void draw(sf::RenderWindow& window) const;

    virtual void collide(GameObject& other)= 0;
    virtual void collide(Player& other) = 0;
    virtual void collide(Enemy& other) = 0;
	virtual void collide(Bomb& other) = 0;
	virtual void collide(Explosion& other) = 0;
	virtual void collide(Door& other) = 0;


	void setTexture(const sf::Texture& texture);
    void setCenter();
	void setScale(float scaleX, float scaleY);

	void setToRemove(bool remove);
	bool getToRemove() const;
    
	void setEnemy(bool enemy);
	bool isEnemyObject() const;
	

private:
    sf::Sprite m_sprite;
    bool m_toRemove;
    bool m_isEnemy;
};
