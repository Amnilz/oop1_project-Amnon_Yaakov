#include "GameObjects/MovingObject.h"
#include <cmath> 
#include <iostream>


MovingObject::MovingObject(const sf::Vector2f& startPos, float speed) : m_startPos(startPos)
, m_speed(speed){}







void MovingObject::setMovementSpeed(float speed){m_speed = speed;}

float MovingObject::getMovementSpeed() const{return m_speed;}

void MovingObject::setStartPos(const sf::Vector2f& startPos){m_startPos = startPos;}

sf::Vector2f MovingObject::getStartPos() const{return m_startPos;}

void MovingObject::reset(){setPosition(m_startPos);}





