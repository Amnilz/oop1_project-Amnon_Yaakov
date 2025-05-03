#pragma once
#include "GameObjects/Gift.h"

class TimeGift : public Gift
{
public:
    TimeGift(const sf::Texture& texture, const sf::Vector2f& position);
    virtual ~TimeGift() = default;

    void onCollected(Player& player) override;
};
