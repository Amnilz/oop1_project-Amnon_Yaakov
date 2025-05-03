
#pragma once
#include "World.h"
#include "GameObjects/Player.h"
#include "GameObjects/Enemy.h"
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>

class LevelManager 
{
public:
     LevelManager() = default;

    void init(const std::string& playlistFilename);
    bool loadNextLevel(World& world, Player& player, std::vector<std::unique_ptr<UpdatableObject>>& m_gameObjects);
    bool loadCurrentLevel(World& world, Player& player, std::vector<std::unique_ptr<UpdatableObject>>& m_gameObjects);
    int getLevelCount() const;
    bool isLastLevel() const;
    int getCurrentLevel() const;
    void reset();

private:
    
    std::vector<std::string> m_levelFiles;
    int m_currentLevel = 0;
    bool m_initialized = false;
};