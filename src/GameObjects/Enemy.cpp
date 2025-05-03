#include "GameObjects/Enemy.h"
#include "World.h"
#include "GameObjects/Player.h"
#include "Managers/ResourceManager.h"
#include <queue>
#include <cmath>
#include <algorithm>
#include "consts.h"


Enemy::Enemy(Player& player)
    : MovingObject(sf::Vector2f(0.f, 0.f), 0.f)  
    , m_isLookingRight(true)
    , m_path()                    
    , m_currentTargetIndex(0)
    , m_currentPlayer(player)
{
    
    auto& tex = ResourceManager::instance().getTexture("enemy.png");
    setTexture(tex);
    setCenter();
    setScale(ENEMY_DEFAULT_SCALE, ENEMY_DEFAULT_SCALE);
    setMovementSpeed(ENEMY_DEFAULT_SPEED);
    m_isLookingRight = true;
	setToRemove(false);
	setEnemy(true);
    
}


//   BFS מהאויב לשחקן במפה
std::vector<Node> Enemy::findPath(World& world)
{
    sf::Vector2f enemyPos = getPosition();
    sf::Vector2f playerPos = m_currentPlayer.getPosition();
    // המרת המיקומים  
    Node enemyNode
    {
         static_cast<int>((enemyPos.x) / TILE_SIZE),
         static_cast<int>((enemyPos.y - TOP_BAR_OFFSET) / TILE_SIZE)
    };
    Node playerNode
    {
         static_cast<int>((playerPos.x) / TILE_SIZE),
         static_cast<int>((playerPos.y - TOP_BAR_OFFSET) / TILE_SIZE)
    };
    // קבלת מספר השורות והעמודות במפה
    int rows = world.getMaxHeight();
    int cols = world.getMaxWidth();
    // מטריצה לסימון תאים שבוקרו
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
	// מטריצה לשמירת הנתיב
    std::vector<std::vector<Node>> route(rows, std::vector<Node>(cols, Node{ -1, -1 }));

    // סימון התאים הלא נגישים כ-visited
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (!world.isCellWalkable(c, r))
                visited[r][c] = true;
        }
    }
    // אם אין נתיב מחזירים רשימה ריקה
    if (visited[enemyNode.y][enemyNode.x])
    {
        return {};
    }
    // תור מהתא של האויב
    std::queue<Node> q;
    q.push(enemyNode);
    visited[enemyNode.y][enemyNode.x] = true;
    route[enemyNode.y][enemyNode.x] = enemyNode; // מתחילים מהתא הנוכחי

    // לולאה BFS
    while (!q.empty())
    {
        Node current = q.front();
        q.pop();

        // אם הגענו לתא השחקן יוצאים מהלולאה
        if (current.x == playerNode.x && current.y == playerNode.y)
        {
            break;
        }
        // רשימת השכנים 
        const Node neighbors[4] = {
            { current.x - 1, current.y },
            { current.x + 1, current.y },
            { current.x,     current.y - 1 },
            { current.x,     current.y + 1 }
        };
        // עבור כל שכן – אם הוא תקין מסמנים אותו ומוסיפים לתור
        for (auto& nbr : neighbors)
        {
            if (isValid(nbr, visited))
            {
                visited[nbr.y][nbr.x] = true;
                route[nbr.y][nbr.x] = current;
                q.push(nbr);
            }
        }
    }
    // אם  אין נתיב מחזירים רשימה ריקה
    if (route[playerNode.y][playerNode.x].x == -1)
    {
        return {};
    }
    // בניית הנתיב על ידי מעקב מהתא של השחקן חזרה לאויב
    std::vector<Node> path;
    Node step = playerNode;
    while (!(step.x == enemyNode.x && step.y == enemyNode.y))
    {
        path.push_back(step);
        step = route[step.y][step.x];
    }
    // הוספת תא האויב לתחילת הנתיב
    path.push_back(enemyNode);
    // הפיכת הנתיב כך שהתחלה תהיה מהמיקום של האויב
    std::reverse(path.begin(), path.end());
    return path;
}


// פונקציה שבודקת אם תא מסוים תקין
bool Enemy::isValid(const Node& node,
    const std::vector<std::vector<bool>>& visited) const
{
    if (node.y < 0 || node.y >= (int)visited.size() ||
        node.x < 0 || node.x >= (int)visited[0].size())
    {
        return false;
    }
    return !visited[node.y][node.x];
}


// פונקציה לעדכון האויב בכל פריים
void Enemy::update(sf::Time dt, World& world)
{
    handleMovement(dt, world);
}

void Enemy::reset()
{
    MovingObject::reset();
	clearPath();    
}



void Enemy::handleMovement(sf::Time dt, World& world)
{
    // אם מהירות האויב 0 אין צורך לעדכן
	float Speed = getMovementSpeed();
    if (Speed == 0.f)
    {
        return;
    }
    // קבלת המיקום הנוכחי והמרת המיקום  
    sf::Vector2f pos = getPosition();
    int gridX = static_cast<int>(pos.x / TILE_SIZE);
    int gridY = static_cast<int>((pos.y - TOP_BAR_OFFSET) / TILE_SIZE);
    // חישוב מרכז התא בו 
    sf::Vector2f cellCenter = getCellCenter(gridX, gridY);

    // אם האויב קרוב למרכז התא – "מצמדים" אותו למרכז ומחשבים נתיב חדש לשחקן
    if (std::abs(pos.x - cellCenter.x) < 0.1f && std::abs(pos.y - cellCenter.y) < 0.1f)
    {
        pos = cellCenter;
        setPosition(pos);

        //  נתיב חדש לשחקן
        m_path = findPath(world);

        // אם קיים נתיב תקין 
        if (!m_path.empty() && m_path.size() > 1) 
        {
            m_currentTargetIndex = 1; // בוחרים את הצעד הבא בנתיב
            m_randomTarget.reset();   // מאפסים יעד רנדומלי אם היה 
        }
        
    }

    Node targetNode;
    bool hasTarget = false;

    // אם קיים נתיב תקין לשחקן משתמשים בצעד הבא
    if (!m_path.empty() && m_currentTargetIndex < m_path.size()) {
        targetNode = m_path[m_currentTargetIndex];
        hasTarget = true;
        m_randomTarget.reset(); // ביטול יעד רנדומלי
    }
    else
    {
        // אם אין נתיב לשחקן בחירת תא רנדומלי 
        if (!m_randomTarget.has_value()) 
        {
            auto randomNeighbor = chooseRandomNeighbor(gridX, gridY, world);
            if (randomNeighbor.has_value())
            {
                m_randomTarget = randomNeighbor.value();
            }
               
        }
        if (m_randomTarget.has_value()) 
        {
            targetNode = m_randomTarget.value();
            hasTarget = true;
        }
    }

    // אין תא זמין לעבור אליו
    if (!hasTarget)
    {
        return;
    }
        

    // חישוב מרכז התא של היעד
    sf::Vector2f targetCenter = getCellCenter(targetNode.x, targetNode.y);

    // חישוב הצעד 
    float step = getMovementSpeed() * dt.asSeconds();
    sf::Vector2f newPos = moveTowardsTarget(pos, targetCenter, step);

    // עדכון כיוון 
    updateDirection(pos, newPos);

    // עדכון המיקום 
    setPosition(newPos);

    // אם האויב הגיע למרכז התא  מעדכנים את הנתיב או מאפסים 
    if (std::abs(newPos.x - targetCenter.x) < 0.1f &&
        std::abs(newPos.y - targetCenter.y) < 0.1f)
    {
        if (!m_path.empty() && m_currentTargetIndex < m_path.size() - 1)
        {
            m_currentTargetIndex++;  // ממשיכים לצעד הבא בנתיב
        }
           
        else
        {
            m_randomTarget.reset();  // מאפסים יעד רנדומלי
        }
           
    }
}


void Enemy::collide(GameObject& other) { other.collide(*this); }

// במקרה של התנגשות עם שחקן מאפסים את הנתיב
void Enemy::collide(Player& p)
{
    p.collide(*this);
    m_path.clear();
    m_currentTargetIndex = 0;
}

void Enemy::collide(Enemy& e) {}


void Enemy::collide(Bomb& bomb) {}

// במקרה של התנגשות עם פיצוץ מסמנים את האויב כמת  
void Enemy::collide(Explosion& ex) 
{
    m_currentPlayer.addScore(5);
    setToRemove(true);
}

void Enemy::collide(Door& door) {}


// ניקוי הנתיב
void Enemy::clearPath()
{
    m_path.clear();
    m_currentTargetIndex = 0;
}

// מחזירה את מרכז התא עבור מיקום ספציפי
sf::Vector2f Enemy::getCellCenter(int gridX, int gridY) const
{
    return sf::Vector2f(
        gridX * TILE_SIZE + TILE_SIZE / 2.f,
        gridY * TILE_SIZE + TILE_SIZE / 2.f + TOP_BAR_OFFSET
    );
}

// מחזיר את מרכז התא
sf::Vector2f Enemy::getCurrentCellCenter() const
{
    sf::Vector2f pos = getPosition();
    int gridX = static_cast<int>(pos.x / TILE_SIZE);
    int gridY = static_cast<int>((pos.y - TOP_BAR_OFFSET) / TILE_SIZE);
    return getCellCenter(gridX, gridY);
}

// מחשב את המיקום החדש לקראת יעד מסוים
sf::Vector2f Enemy::moveTowardsTarget(const sf::Vector2f& currentPos, const sf::Vector2f& targetCenter, float step)
{
    sf::Vector2f newPos = currentPos;
    //  x-מתקדמים רק בציר ה
    if (std::abs(currentPos.x - targetCenter.x) > 0.1f)
    {
        if (std::abs(currentPos.x - targetCenter.x) <= step)
            newPos.x = targetCenter.x;
        else
            newPos.x += (currentPos.x < targetCenter.x ? step : -step);
    }
    // y-מתקדמים רק בציר ה
    else if (std::abs(currentPos.y - targetCenter.y) > 0.1f)
    {
        if (std::abs(currentPos.y - targetCenter.y) <= step)
        {
            newPos.y = targetCenter.y;
        }
        else 
        {
            newPos.y += (currentPos.y < targetCenter.y ? step : -step);
        }
    }
    return newPos;
}

// עדכון כיוון ה sprite 
void Enemy::updateDirection(const sf::Vector2f& oldPos, const sf::Vector2f& newPos)
{
    float dx = newPos.x - oldPos.x;
    if (dx > 0 && !m_isLookingRight) 
    {
        m_isLookingRight = true;
        setScale(ENEMY_DEFAULT_SCALE, ENEMY_DEFAULT_SCALE);
    }
    else if (dx < 0 && m_isLookingRight) 
    {
        m_isLookingRight = false;
        setScale(-ENEMY_DEFAULT_SCALE, ENEMY_DEFAULT_SCALE);
    }
}


//   במקרה ואין נתיב לשחקן בוחר תא שכן רנדומלי
std::optional<Node> Enemy::chooseRandomNeighbor(int gridX, int gridY, const World& world) const 
{
    std::vector<Node> validNeighbors;
    const Node neighbors[4] = {
        { gridX - 1, gridY },
        { gridX + 1, gridY },
        { gridX,     gridY - 1 },
        { gridX,     gridY + 1 }
    };

    // בודק אם תא נגיש אם כן מוסיף לרשימה
    for (const auto& nbr : neighbors)
    {
        if (world.isCellWalkable(nbr.x, nbr.y))
        {
            validNeighbors.push_back(nbr);
        }
    }
            
    // אם אין שכנים 
    if (validNeighbors.empty())
    {
        return std::nullopt;
    }
    int randIndex = rand() % validNeighbors.size();
    return validNeighbors[randIndex];
}
