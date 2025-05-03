#include "GameObjects/UpdatableObject.h"





void UpdatableObject::collide(GameObject& other){}
void UpdatableObject::collide(Player& p){}

void UpdatableObject::collide(Enemy& e){}

void UpdatableObject::collide(Bomb& bomb){}

void UpdatableObject::collide(Explosion& ex){}

void UpdatableObject::setMovementSpeed(float speed){}
