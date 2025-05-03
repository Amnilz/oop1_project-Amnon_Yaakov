#pragma once
#include "GameObjects/StaticObject.h"
#include "Managers/ResourceManager.h"


class Rock : public StaticObject 
{
public:
    Rock(const sf::Vector2f& pos): 
        StaticObject(ResourceManager::instance().getTexture("rock.png"),pos,true,true){}
};
