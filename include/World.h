#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include "consts.h"
#include "GameObjects/Player.h"
#include "GameObjects/StaticObject.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/Gift.h"
#include "GameObjects/TimeGift.h"
#include "GameObjects/IceGift.h"
#include "GameObjects/GuardKiller.h"
#include "GameObjects/LifeGift.h"
#include "GameObjects/Wall.h"
#include "GameObjects/Rock.h"
#include "GameObjects/Door.h"
#include "GameObjects/Floor.h"


class World 
{
public:
    World() = default;

    
    void draw(sf::RenderWindow& window);
    bool isCellWalkable(int col, int row) const;
    int getMaxWidth() const;
    int getMaxHeight() const;
    void setCellBlocked(int col, int row, bool blocked);
    
    void spawnRandomGift();
  
    void clearStatics();

    void loadLevelFromFile(const std::string& filename, Player& player, std::vector<std::unique_ptr<UpdatableObject>>& updatableObjects);
	bool breakRocks(const sf::Vector2f& position);
	void checkCollisionWithStaticObjects(Player& player);
	bool isCellBlocked(sf::FloatRect);
	


private:

    void parseLine(const std::string& line, int row, Player& player, std::vector<std::unique_ptr<UpdatableObject>>& updatableObjects);
    std::vector<std::unique_ptr<StaticObject>> m_statics;
    std::vector<std::vector<bool>> m_blockedMap;
   
    int m_maxWidth = 0;
    int m_maxHeight = 0;
   
};