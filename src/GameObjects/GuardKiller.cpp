#include "GameObjects/GuardKiller.h"
#include <iostream>
#include "Managers/GameController.h"

GuardKiller::GuardKiller(const sf::Texture& texture, const sf::Vector2f& position)
    : Gift(texture, position)
{
    setScale(GUARD_KILLER_SCALE, GUARD_KILLER_SCALE);
    setCenter();
}

void GuardKiller::onCollected(Player& player)
{
    player.addGiftEffect(GiftEffect::KillOneEnemy);
}


