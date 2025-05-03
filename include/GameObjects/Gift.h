#pragma once
#include "GameObjects/StaticObject.h"
#include "GameObjects/Player.h" 

class Gift : public StaticObject
{
public:
    
    Gift(const sf::Texture& texture, const sf::Vector2f& position);
    virtual ~Gift() = default;

    
    void collide(GameObject& other) override;
    void collide(Player& player) override;
	
    virtual void onCollected(Player& player) = 0;
 
};
