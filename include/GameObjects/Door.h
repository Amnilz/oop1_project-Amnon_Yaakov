#pragma once
#include "GameObjects/StaticObject.h"
#include "Managers/ResourceManager.h"
#include <iostream>


class GameObject;
class Player;

class Door : public StaticObject 
{
public:
    Door(const sf::Vector2f& pos)
        : StaticObject(ResourceManager::instance().getTexture("door.png"),
            pos,
            false, 
            false) 
    { 
		setToRemove(false);
    }

    virtual void collide(GameObject& other) override;
    virtual void collide(Player& player) override;
};
