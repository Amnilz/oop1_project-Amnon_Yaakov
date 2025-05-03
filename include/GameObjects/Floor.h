#pragma once
#include "GameObjects/StaticObject.h"
#include "Managers/ResourceManager.h"


class Floor : public StaticObject
{
public:
    Floor(const sf::Vector2f& pos)
        : StaticObject(ResourceManager::instance().getTexture("empty.png"), pos, false, false)
    {
        setToRemove(false);
        setEnemy(false);
    }
};