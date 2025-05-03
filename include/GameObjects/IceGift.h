#pragma once
#include "GameObjects/Gift.h"


class Player;

class IceGift : public Gift
{
public:
    IceGift(const sf::Texture& texture, const sf::Vector2f& position);

    virtual ~IceGift() = default;
    void onCollected(Player& player) override;
};
