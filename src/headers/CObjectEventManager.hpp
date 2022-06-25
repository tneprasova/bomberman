#pragma once

#include <list>
#include <tuple>

#include "CRenderWindow.hpp"
#include "CTile.hpp"
#include "CObject.hpp"
#include "CPlayer.hpp"
#include "CEnemy.hpp"
#include "CDoor.hpp"
#include "CExplosion.hpp"
#include "CBomb.hpp"
#include "CBonus.hpp"
#include "Utilities.hpp"
#include "EGameMode.hpp"
#include "EEvent.hpp"
#include "GameConstants.hpp"

/**
 * @brief Takes care of objects on the game field and the tiles
 * 
 * Executes events to construct new objects and determines the end of the game
 */
class CObjectEventManager
{
public:
    bool needsNewMap;                               /**< Flag to determine whether a new map needs to be loaded */
    bool endGame;                                   /**< Flag that signalizes the end of the game */

    /**
     * @brief CObjectEventManager constructor
     * 
     * @param renderer pointer to the CRenderWindow class - needed for textures
     */
    CObjectEventManager(CRenderWindow * renderer);

    /**
     * @brief Update the whole playing field
     * 
     * Updates the tile set and all of the objects on the playing field
     * Removes objects that are flagged to get removed
     */
    void update();

    /**
     * @brief Manages events created by the objects
     * 
     * This function takes care of creating new objects
     * and determining the end of the game
     */
    void manageEvents();

    /**
     * @brief Starts the game
     * 
     * @param map the map to start the game with
     */
    void startGame(const std::pair<Map, int> & map);

    /**
     * @brief Constructs tiles and objects from the map
     * 
     * @param map the map to construct from
     */
    void loadFromMap(const Map & map);

    /**
     * @brief Turns the tiles and objects back into a 2D vector of enums
     * 
     * @return the 2D vector of enums representing the game map
     */
    const std::pair<Map, int> saveIntoMap() const;

private:
    CRenderWindow * renderer;                       /**< Pointer to the renderer - we need it so we have access to the textures */
    std::list<std::shared_ptr<CObject>> objects;    /**< The objects that are currenty on the playing field */
    Events events;                                  /**< List of current events */
    TileSet tileSet;                                /**< A 2D vector of tile objects (walls, breakables and empty grass tiles) */
    std::pair<int,int> currentScore;                /**< Current score, needed for loading and saving the game */
    EGameMode mode;                                 /**< Current game mode */
    int alivePlayers;                               /**< Alive players - determines the end of the game */
    int aliveEnemies;                               /**< Alive enemies - determines when to create a door */
    int rounds;                                     /**< Number of rounds in duel mode */
    int bonusChance;                                /**< A percentual chance for a bonus to drop from a destroyed breakable */

    /**
     * @brief Adds an event to the event list
     * 
     * @param event the name of the event
     * @param position used by events, which need to construct something on a certain position
     * @param num used by events which need to carry a numerical value - ex.: POINTS
     * @param obj used by events which need to carry a pointer to an object - ex.: GET_BONUS
     */
    void addEvent(const EEvent & event, const std::pair<int, int> & position = {0,0}, const int & num = 0, CObject * obj = nullptr);

    /**
     * @brief Adds an objects to the list
     * 
     * @param obj pointer to the new object
     */
    void addObject(CObject * obj);

    /**
     * @brief Sets a tile in the tile set
     * 
     * @param x x position
     * @param y y position
     * @param tileType the type of tile to which will be set
     */
    void setTile(const int & x, const int & y, const ETileType & tileType);
};