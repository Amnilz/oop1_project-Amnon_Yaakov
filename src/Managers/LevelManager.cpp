#include "Managers/LevelManager.h"



void LevelManager::init(const std::string& playlistFilename)
{
    if (!m_initialized)
    {
        std::ifstream file(playlistFilename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + playlistFilename);
        }

        int numLevels = 0;
        file >> numLevels;
        file.close();

        m_levelFiles.clear();
        for (int i = 1; i <= numLevels; i++)
        {
            m_levelFiles.push_back("level" + std::to_string(i) + ".txt");
        }

        m_initialized = true;
    }
}




bool LevelManager::loadNextLevel(World& world, Player& player, std::vector<std::unique_ptr<UpdatableObject>>& updatableObjects)
{
    if (m_currentLevel  >= (int)m_levelFiles.size()-1)
    {
        return false;
    }
    world.clearStatics(); 
    player.reset();
	updatableObjects.clear();
    m_currentLevel++;
    return loadCurrentLevel(world, player, updatableObjects);
}

bool LevelManager::loadCurrentLevel(World& world, Player& player, std::vector<std::unique_ptr<UpdatableObject>>& updatableObjects)
{
		world.loadLevelFromFile(m_levelFiles[m_currentLevel], player, updatableObjects);
        return true;
}

int LevelManager::getLevelCount() const 
{
    return static_cast<int>(m_levelFiles.size());
}
bool LevelManager::isLastLevel() const {return m_currentLevel == m_levelFiles.size() - 1;}


int LevelManager::getCurrentLevel() const{return m_currentLevel;}

void LevelManager::reset(){m_currentLevel = 0;}