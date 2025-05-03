#include "GameObjects/Gift.h"
#include "GameObjects/Player.h"




Gift::Gift(const sf::Texture& texture, const sf::Vector2f& position)
    : StaticObject(texture, position, false, false)
{
    setEnemy(false);
	setToRemove(false);

}

void Gift::collide(GameObject& other)
{
  other.collide(*this);
}

void Gift::collide(Player& player)
{
    onCollected(player);
    setToRemove(true);
}



