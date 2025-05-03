#pragma once

#include "GameObjects/GameObject.h"



class World;

class UpdatableObject : public GameObject
{
public:
	UpdatableObject() = default;
	virtual ~UpdatableObject() = default;



   virtual void update(sf::Time dt, World& world) = 0;
   virtual void reset() = 0;

   void collide(GameObject& other) override;
   void collide(Player& p) override;
   void collide(Enemy& e) override;
   void collide(Bomb& bomb) override;
   void collide(Explosion& ex) override;
   virtual void setMovementSpeed(float speed);

};