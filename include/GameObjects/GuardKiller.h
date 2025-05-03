#pragma once
#include "GameObjects/Gift.h"


class GuardKiller : public Gift
{
public:
    GuardKiller(const sf::Texture& texture, const sf::Vector2f& position);
    virtual ~GuardKiller() = default;

    void onCollected(Player& player) override;
};
