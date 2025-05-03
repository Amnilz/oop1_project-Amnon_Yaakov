#pragma once
#include "GameObjects/Gift.h"

class LifeGift : public Gift
{
public:
    LifeGift(const sf::Texture& texture, const sf::Vector2f& position);
    virtual ~LifeGift() = default;

    void onCollected(Player& player) override;
};
