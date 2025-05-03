#include "GameObjects/TimeGift.h"
#include <iostream>
#include "Managers/GameController.h"


TimeGift::TimeGift(const sf::Texture& texture, const sf::Vector2f& position)
    : Gift(texture, position)
{
    setScale(TIME_GIFT_SCALE, TIME_GIFT_SCALE);
    setCenter();
}

void TimeGift::onCollected(Player& player)
{
    player.addGiftEffect(GiftEffect::AddTime);
}

