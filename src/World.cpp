#include "World.h"
#include "GameObjects/Wall.h"
#include "GameObjects/Rock.h"
#include "GameObjects/Door.h"
#include <iostream> 
#include "consts.h"
#include <random>
#include "GameObjects/Floor.h"


// טוען שלב מקובץ, מאפס רשימות וממלא את המפה
void World::loadLevelFromFile(const std::string& filename, Player& player, std::vector<std::unique_ptr<UpdatableObject>>& updatableObjects)
{
    m_statics.clear();
    m_blockedMap.clear();
    updatableObjects.clear();
    m_maxWidth = 0;
    m_maxHeight = 0;

    std::ifstream ifs(filename);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open level file: " + filename);
    } 
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(ifs, line))
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }   
    ifs.close();
    m_maxHeight = (int)lines.size();
    for (auto& ln : lines)
    {
        m_maxWidth = std::max(m_maxWidth, (int)ln.size());
    }
    m_blockedMap.assign(m_maxHeight, std::vector<bool>(m_maxWidth, false));

    for (int row = 0; row < m_maxHeight; ++row)
    {
        parseLine(lines[row], row, player, updatableObjects);
    }
    if (m_maxHeight == 0)
    {
        throw std::runtime_error("Loaded level is empty: " + filename);
    }
}

bool World::breakRocks(const sf::Vector2f& position)
{
    // הגדרת התחום של התא
    sf::FloatRect tileRect(
        position.x - TILE_SIZE / 2.f,
        position.y - TILE_SIZE / 2.f,
        TILE_SIZE, TILE_SIZE
    );

    // נחפש אובייקט שממוקם באותו תא
    for (auto it = m_statics.begin(); it != m_statics.end(); ++it)
    {
        if (tileRect.intersects((*it)->getGlobalBounds()))
        {
            // אם האובייקט שביר => לשבור (לשים "empty") ולהחזיר true
            if ((*it)->isBreakable())
            {
                sf::Vector2f breakPos = (*it)->getPosition();
                it = m_statics.erase(it);
                m_statics.push_back(std::make_unique<Floor>(breakPos));
                // עדכון מפת חסימות
                int row = static_cast<int>(breakPos.y / TILE_SIZE);
                int col = static_cast<int>(breakPos.x / TILE_SIZE);
                setCellBlocked(col, row, false);

                return true;  // שבירת אבן => מציירים פיצוץ
            }
            else
            {
                // אם זה אובייקט חסום (קיר) ולא שביר => לא מציירים פיצוץ
                if ((*it)->isBlocked())
                {
                 return false;
                }
                // אחרת (אובייקט לא חסום, כמו רצפה/מתנה) => מציירים פיצוץ
                return true;
            }
        }
    }
    return true;
}


void World::checkCollisionWithStaticObjects(Player& player)
{
    sf::FloatRect playerBounds = player.getGlobalBounds();

    std::vector<size_t> toRemove;

    for (size_t i = 0; i < m_statics.size(); ++i)
    {
        if (playerBounds.intersects(m_statics[i]->getGlobalBounds()))
        {
            player.collide(*m_statics[i]);
            m_statics[i]->collide(player);

            if (m_statics[i]->getToRemove())
            {
                toRemove.push_back(i);
            }
        }
    }

    for (size_t i = toRemove.size(); i > 0; --i)
    {
        m_statics.erase(m_statics.begin() + toRemove[i - 1]);
    }
}

bool World::isCellBlocked(sf::FloatRect newBounds)
{
    // בדיקה האם יש התנגשויות עם אובייקטים חסומים
    for (auto& so : m_statics)
    {
        if (so->isBlocked() &&
            newBounds.intersects(so->getGlobalBounds()))
        {
            return false;
        }
    }
	return true;
}

// מפרש שורה ומוסיף אובייקטים לפי תו
void World::parseLine(const std::string& line, int row, Player& player, std::vector<std::unique_ptr<UpdatableObject>>& updatableObjects)
{
    for (int col = 0; col < (int)line.size(); ++col)
    {
        sf::Vector2f pos(col * TILE_SIZE, row * TILE_SIZE + TOP_BAR_OFFSET);
        char c = line[col];
        switch (c)
        {
        case '#': // קיר
            m_statics.push_back(std::make_unique<Wall>(pos));
            m_blockedMap[row][col] = true;
            break;
        case '@': // סלע
			
            m_statics.push_back(std::make_unique<Rock>(pos));
            m_blockedMap[row][col] = true;
            break;
        case 'D': // דלת
            m_statics.push_back(std::make_unique<Floor>(pos));
            m_statics.push_back(std::make_unique<Door>(pos));
            m_blockedMap[row][col] = false;
            break;
        case ' ': // רצפה ריקה
            m_statics.push_back(std::make_unique<Floor>(pos));
			m_blockedMap[row][col] = false;
            break;

        case '/': // מיקום התחלה של השחקן
        {
            m_statics.push_back(std::make_unique<Floor>(pos));
            sf::Vector2f centerPos(pos.x + TILE_SIZE / 2.f, pos.y + TILE_SIZE / 2.f);
            player.setPosition(centerPos);
            player.setStartPos(centerPos);
            break;
        }
        case '!': // אויב
        {
			m_statics.push_back(std::make_unique<Floor>(pos));
            sf::Vector2f centerPos(pos.x + TILE_SIZE / 2.f, pos.y + TILE_SIZE / 2.f);
            std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy> (player);
            newEnemy->setPosition(centerPos);
            newEnemy->setStartPos(centerPos);
			updatableObjects.push_back(std::move(newEnemy));
           
            break;
        }
        default:
            break;
        }
    }
}

//סימון תא כחסום
void World::setCellBlocked(int col, int row, bool blocked)
{
    if (row < 0 || row >= m_maxHeight || col < 0 || col >= m_maxWidth)
        return;
    m_blockedMap[row][col] = blocked;
}

// מצייר את כל האובייקטים סטטיים 
void World::draw(sf::RenderWindow& window)
{

    for (auto& obj : m_statics)
    {
        obj->draw(window);
    }
       
}

// בודק האם ניתן ללכת לתא
bool World::isCellWalkable(int col, int row) const
{
    if (row < 0 || row >= m_maxHeight || col < 0 || col >= m_maxWidth)
    {
        return false;
    }
        
    return !m_blockedMap[row][col];
}

int World::getMaxWidth() const {return m_maxWidth;}
int World::getMaxHeight() const {return m_maxHeight;}



// יוצר מתנה אקראית בתא פנוי במפה
void World::spawnRandomGift()
{
    std::vector<sf::Vector2i> freeCells;
    for (int row = 0; row < m_maxHeight; ++row)
    {
        for (int col = 0; col < m_maxWidth; ++col)
        {
            if (isCellWalkable(col, row))
            {
                freeCells.emplace_back(col, row);
            }  
        }   
    }
        

    if (freeCells.empty())
    {
        return;
    }
       

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, (int)freeCells.size() - 1);
    sf::Vector2i cell = freeCells[dist(rng)];

    sf::Vector2f position(
        cell.x * TILE_SIZE + TILE_SIZE / 2.f,
        cell.y * TILE_SIZE + TILE_SIZE / 2.f + TOP_BAR_OFFSET
    );

    int giftType = std::rand() % 4;
    std::unique_ptr<StaticObject> gift;
    switch (giftType)
    {
    case 0:
        gift = std::make_unique<IceGift>(
            ResourceManager::instance().getTexture("IceGift.png"), position);
        break;
    case 1:
        gift = std::make_unique<GuardKiller>(
            ResourceManager::instance().getTexture("guardKiller.png"), position);
        break;
    case 2:
        gift = std::make_unique<TimeGift>(
            ResourceManager::instance().getTexture("clock.png"), position);
        break;
    case 3:
        gift = std::make_unique<LifeGift>(
            ResourceManager::instance().getTexture("heart.png"), position);
        break;
    }
    m_statics.push_back(std::move(gift));
}

// מסיר אובייקטים סטטיים
void World::clearStatics() {m_statics.clear();}
