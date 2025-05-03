#include "GameObjects/IceGift.h"
#include <iostream>
#include "Managers/GameController.h"

IceGift::IceGift(const sf::Texture& texture, const sf::Vector2f& position)
    : Gift(texture, position)
{
    setScale(ICE_GIFT_SCALE, ICE_GIFT_SCALE);
    setCenter();
}

void IceGift::onCollected(Player& player)
{
    
    player.addGiftEffect(GiftEffect::FreezeEnemies);
}


