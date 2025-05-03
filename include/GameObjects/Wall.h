#pragma once
#include "GameObjects/StaticObject.h"
#include "Managers/ResourceManager.h"


class Wall : public StaticObject 
{
public:
    Wall(const sf::Vector2f& pos)
        : StaticObject(ResourceManager::instance().getTexture("wall.png"),pos,true,false)
    {
        setToRemove(false);
        setEnemy(false);
    }
};
