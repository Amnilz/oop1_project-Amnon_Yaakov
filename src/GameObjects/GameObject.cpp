#include "GameObjects/GameObject.h"

void GameObject::setPosition(const sf::Vector2f& pos){m_sprite.setPosition(pos);}

sf::Vector2f GameObject::getPosition() const{return m_sprite.getPosition();}

sf::FloatRect GameObject::getGlobalBounds() const{return m_sprite.getGlobalBounds();}

void GameObject::draw(sf::RenderWindow& window) const{window.draw(m_sprite);}

void GameObject::collide(GameObject& other){other.collide(*this);}

void GameObject::setTexture(const sf::Texture& texture)
{
	m_sprite.setTexture(texture);
	
}
void GameObject::setCenter() 
{
	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
void GameObject::setScale(float scaleX,float scaleY)
{
	m_sprite.setScale(scaleX, scaleY);
}

void GameObject::setToRemove(bool remove)
{
m_toRemove = remove;
}

bool GameObject::getToRemove() const{return m_toRemove;}

void GameObject::setEnemy(bool enemy){m_isEnemy = enemy;}

bool GameObject::isEnemyObject() const{return m_isEnemy;}
