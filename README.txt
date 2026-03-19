== BomberMan Project ===================

== Info ================================
# Name: Amnon Yaakov

== Project Summary =====================
This project is a BomberMan-style game built in C++ using the SFML library.  
The game involves a player navigating through levels, placing bombs, collecting gifts, and dealing with enemies, while progressing across multiple stages.  
It follows Object-Oriented Programming (OOP) principles to maintain modularity and clarity.

== List of Files Created ==============

1) consts.h 
   - Contains constants and enums used throughout the game (e.g., window dimensions, tile size, game states, speeds, scaling factors, etc.).  
   - Declares structs like `Node` (used by BFS) and enums like `GameState`, `MenuState`, `MenuAction`, `GiftEffect`.  

2) Managers:
    - GameController.cpp/h  
     - Manages overall game flow, including initialization, the game loop, handling events, state transitions (MENU, PLAYING, PAUSED, HELP, GAME_OVER), collision checks, and updating all dynamic objects.  
   - ResourceManager.cpp/h  
     - Implements a Singleton for loading and caching textures, fonts, and sound buffers. Ensures each resource is loaded only once.  
   - UIManager.cpp/h  
     - Manages the user interface logic, including handling menu clicks, switching between menu screens, and in-game top-bar updates (lives, score, timer).  
   - GameUI.cpp/h  
     - Displays game-related information (e.g., player lives, score, time remaining). Renders a bottom bar or overlay within the game window.  
   - Menu.cpp/h  
     - Handles rendering of various menus (Main Menu, Pause Menu, Game Over screens, Help screen).  
     - Processes user input (mouse clicks) to navigate between menu options (`NewGame`, `Help`, `Exit`, etc.).

3) World & Levels 
   - World.cpp/h:
     - Represents the game map, loading level data from text files and storing static objects (walls, rocks, doors, gifts).  
     - Maintains a 2D `m_blockedMap` that marks which cells are walkable or blocked.  
     - Responsible for drawing static objects, checking collisions with them, spawning random gifts, and “breaking” rocks on explosions.  
  - LevelManager.cpp/h  
     - Handles loading levels from text files, moving to the next level, and checking if the last level has been reached.  
   - **Level files**  
     - A file such as `Playlist.txt` that lists level filenames.  
     - Individual level files (e.g., `level1.txt`, `level2.txt`) that define the map layout (walls, rocks, doors, empty floor, player spawn, enemy spawns, etc.).

4) Game Objects 
   - GameObject.cpp/h  
     - Abstract base class for all objects in the game.  
     - Stores a sprite (`sf::Sprite`), position, and a flag indicating whether the object should be removed.  
     - Provides basic `draw` and `collide` methods using a double-dispatch pattern.  
   - MovingObject.cpp/h  
     - Abstract class for any object that can move (player, enemies).  
     - Extends `UpdatableObject` (which in turn extends `GameObject`) and includes speed, start position, and a `handleMovement` method.  
   - UpdatableObject.cpp/h 
     - Abstract class for objects that require per-frame updates (bombs, explosions, enemies, player).  
     - Declares a pure virtual `update(sf::Time dt, World& world)` function.  
   - StaticObject.cpp/h  
     - Base class for non-moving objects (wall, door, rock, floor, gifts).  
     - Contains booleans for `blocked` (does it block movement?) and `breakable` (can it be destroyed by an explosion?).  
   - Player.cpp/h 
     - Represents the player. Handles keyboard input for movement, bomb placement, collision with enemies/explosions/gifts, life count, and score.  
     - Calls `GameController` methods (e.g., `setPendingReset()`, `gameOverLose()`, `nextLevel()`) when needed.  
   - Enemy.cpp/h  
     - Represents an enemy with BFS pathfinding toward the player.  
     - Computes a path to the player's location, moving cell by cell. If no path is found, chooses a random adjacent cell.  
     - Collides with bombs, explosions, and the player.  
   - Bomb.cpp/h  
     - Represents a bomb placed by the player.  
     - Has a countdown `m_timeLeft` that when it reaches zero triggers an explosion (`Explosion` object).  
     - Changes its texture over time to indicate the fuse burning down.  
   - Explosion.cpp/h  
     - Represents an explosion effect that appears after a bomb detonates.  
     - Spawns a central explosion and up to four sub-explosions (up, down, left, right).  
     - Damages or destroys enemies, rocks, and can also harm the player.  
     - Disappears after a short lifetime (`EXPLOSION_LIFETIME`).  
   - Gift.cpp/h 
     - Abstract class for collectible items that benefit the player.  
     - Colliding with the player triggers `applyGift` and sets the gift to be removed.  
   - GuardKiller.cpp/h, IceGift.cpp/h, TimeGift.cpp/h, LifeGift.cpp/h  
     - Derived from `Gift`, each providing a unique effect:  
       - GuardKiller: Kills one enemy on the field, granting some points.  
       - IceGift: Freezes all enemies for a set duration (speed=0).  
       - TimeGift: Adds extra time to the current level timer.  
       - LifeGift: Increases the player's life by one.

5) Static Objects 
   - Floor.cpp/h  
     - Renders “empty” floor tiles. Non-blocking and non-breakable.  
   - Door.cpp/h 
     - A door to progress to the next level. When the player collides with a door, the game transitions to the next level.  
   - Wall.cpp/h  
     - A solid, non-breakable, blocking tile.  
   - Rock.cpp/h  
     - A breakable, blocking tile. When hit by an explosion, it becomes empty floor (`Floor`).  

== Data Structures & Their Uses ========
- ResourceManager: Uses `std::unordered_map<std::string, sf::Texture>`, etc., for caching resources.  
- Vectors of unique_ptr: store dynamic objects (e.g., bombs, enemies, explosions).  
- 2D vector of bool: in `World` (`m_blockedMap`) tracks blocked/unblocked cells.  
- Vector of strings: initially holds level layouts read from text files.

== Algorithms ==========================
1) BFS (Breadth-First Search)  
   - Implemented in the `Enemy` class (`findPath`) to compute a path from enemy to player.  
   - If the BFS path is empty, the enemy tries random neighboring cells.  

2) Collision Checking  
   - The game checks collisions by bounding-box intersection (`getGlobalBounds().intersects(...)`).  
   - Double-dispatch (e.g., `objectA.collide(objectB) -> objectB.collide(objectA)`) ensures we call the right function for each pair of object types.

== Design =============================
1) GameController 
   - The central orchestrator: runs the main loop, handles user input/events, updates states, and manages collisions.  

2) ResourceManager & LevelManager  
   - Singletons for global resource/level handling.  

3) World  
   - Loads levels from files, instantiates `StaticObject` (wall, rock, door) or dynamic objects (enemy) as needed.  
   - Maintains blocked/unblocked cells for movement checks.  

4) Game Objects Hierarchy  
   - GameObject → UpdatableObject → MovingObject → (Player, Enemy)  
   - GameObject → UpdatableObject → (Bomb, Explosion)
   - GameObject → StaticObject → (Door, Wall, Rock, Floor, Gift)  
   - Bomb & Explosion also extend `UpdatableObject` for timing updates.  
   - Gifts all extend `Gift` but provide unique effect overrides.

5) UI System
   - UIManager controls transitions between menu screens and game UI.  
   - Menu draws various menus (Main, Pause, Game Over).  
   - GameUI displays in-game data (lives, score, timer).  



== Additional Comments ===============
-The "Playlist" file contain the amount of levels in the game 
- All levels Files must be named in increasing order (e.g., level1.txt, level2.txt, etc.
- The game uses fixed level dimensions ( 40 columns x 20 rows)
- The standard level format uses rows of characters:  
- `'#'` = Wall, `'@'` = Rock, `'D'` = Door, `' '` = Floor, `'/'` = Player, `'!'` = Enemy.  
- The game uses an integer-based BFS for enemy pathfinding and a 2D map of bool for blocking checks.  
- Music and sound effects are handled via `sf::Music` in `GameController` and `sf::Sound` in the game objects.  
- Default window size is 1280×720, tile size is 32×32, with some offsets for UI.



