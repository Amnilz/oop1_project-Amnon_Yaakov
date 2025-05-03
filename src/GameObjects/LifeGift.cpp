#include "GameObjects/LifeGift.h"
#include <iostream>
#include <sfml/Graphics.hpp>
#include "consts.h"
#include "Managers/ResourceManager.h"

LifeGift::LifeGift(const sf::Texture& texture, const sf::Vector2f& position)
    : Gift(texture, position)
{
    setScale(LIFE_GIFT_SCALE, LIFE_GIFT_SCALE);
    setCenter();
    
}

void LifeGift::onCollected(Player& player)
{
  
    player.addGiftEffect(GiftEffect::AddLife);

}


