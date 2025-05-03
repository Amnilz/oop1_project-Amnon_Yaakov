#include "GameObjects/Door.h"
#include "GameObjects/Player.h"

void Door::collide(GameObject& other){}


void Door::collide(Player& player) {player.collide(*this);}
