#include "GameObjects/Explosion.h"
#include "Managers/ResourceManager.h"
#include "consts.h"
#include "GameObjects/Bomb.h"
#include "GameObjects/Player.h"
#include "GameObjects/Enemy.h"
#include "Managers/GameController.h"






// זהו הבנאי "הראשי" שיוצר את כל הפיצוצים סביבו
Explosion::Explosion(const sf::Vector2f& centerPos,
    const std::string& textureForCenter,
    World& world,
    GameController& gc)
    : m_timeToLive(EXPLOSION_LIFETIME)
{
    // 1) קובע ספרייט לפיצוץ המרכזי
    setTexture(ResourceManager::instance().getTexture("center.png"));
    setCenter();
    setPosition(centerPos);
    setToRemove(false);
    setEnemy(false);

    // 2) מפעיל את הלוגיקה עצמה: שבירת אבנים, יצירת פיצוצים משניים וכו'
    performExplosion(centerPos, world, gc);
}

Explosion::Explosion(const sf::Vector2f& pos, const std::string& textureForExplosion)
    : m_timeToLive(EXPLOSION_LIFETIME)
{
    // זהו בנאי "משני" (Sub-Explosion): לא מבצע performExplosion
    setTexture(ResourceManager::instance().getTexture(textureForExplosion));
    setCenter();
    setPosition(pos);
    setToRemove(false);
    setEnemy(false);
}



// פונקציה לעדכון מצב הפיצוץ בכל פריים
void Explosion::update(sf::Time dt, World & world)
{
    // אם הפיצוץ כבר הסתיים  אין צורך בעדכון
    if (getToRemove())
    {
        return;
    }
    // הפחתת הזמן שנותר 
    m_timeToLive -= dt.asSeconds();
    //  נסמן שהפיצוץ הסתיים
    if (m_timeToLive <= 0.f)
    {
        setToRemove(true);
    }
}

void Explosion::reset(){setToRemove(true);}


void Explosion::performExplosion(const sf::Vector2f& centerPos,
    World& world,
    GameController& gc)
{
   
    if (gc.getPendingReset())
    {
        return;
    }
      
    
    if (world.breakRocks(centerPos))
    {
        //  בתא מרכז
        gc.addSubExplosion(centerPos, "center.png");
        // בודקים למעלה
        sf::Vector2f upPos = centerPos + sf::Vector2f(0, -TILE_SIZE);
        if (world.breakRocks(upPos))
        {
            gc.addSubExplosion(upPos, "up.png");
        }
        // למטה
        sf::Vector2f downPos = centerPos + sf::Vector2f(0, TILE_SIZE);
        if (world.breakRocks(downPos))
        {
            gc.addSubExplosion(downPos, "down.png");
        }
        // שמאלה
        sf::Vector2f leftPos = centerPos + sf::Vector2f(-TILE_SIZE, 0);
        if (world.breakRocks(leftPos))
        {
            gc.addSubExplosion(leftPos, "left.png");
        }
        // ימינה
        sf::Vector2f rightPos = centerPos + sf::Vector2f(TILE_SIZE, 0);
        if (world.breakRocks(rightPos))
        {
            gc.addSubExplosion(rightPos, "right.png");
        }
    }
    // השמעת סאונד 
    static sf::Sound explosionSound;
    static bool loaded = false;
    if (!loaded)
    {
        explosionSound.setBuffer(ResourceManager::instance().getSound("explosion.wav"));
        explosionSound.setVolume(EFFECT_VOLUME);
        loaded = true;
    }
    explosionSound.play();
}

void Explosion::collide(Bomb& other) {}
void Explosion::collide(Explosion& other) {}
void Explosion::collide(Door& other) {}
void Explosion::collide(GameObject& other) {other.collide(*this);}
void Explosion::collide(Player& other) 
{ 
    if (getToRemove()) 
    {
        return;
    }
    other.collide(*this); 
}

void Explosion::collide(Enemy& other) {other.collide(*this);}
