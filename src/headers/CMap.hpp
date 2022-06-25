#pragma once

#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <memory>

#include "ETileType.hpp"
#include "EGameMode.hpp"
#include "CObject.hpp"
#include "GameConstants.hpp"
#include "Utilities.hpp"

/**
 * @brief Manages the game map
 * 
 * Takes care of generating the game map, loading it and saving it
 * into a file
 */
class CMap
{
public:
    /**
     * @brief Create a new map
     * 
     * @param mode the game mode
     * @param saveFile the path to the save file
     */
    CMap(const EGameMode & mode, const std::string & saveFile = loadData(config, "Save file", true).str());

    /**
     * @brief Saves the map and the score into a file
     */
    void save() const;

    /**
     * @brief Loads a map and the score from a file
     */
    void load();

    /**
     * @brief Sets the map and the score
     * 
     * @param data a pair containing the map and the score
     */
    void setMap(const std::pair<Map, int> & data);

    /**
     * @brief Get a map and the score
     * 
     * @return a pair of map and score
     */
    std::pair<Map, int> getMap() const;

private:
    Map map;                                        /**< The game map */
    std::set<std::pair<int, int>> availableTiles;   /**< The tiles on which the entities can step on */
    static int numberOfBreakables;                  /**< The nuber of breakables to be generated */
    static int numberOfEnemies;                     /**< The number of enemies to be generated in singleplayer mode */
    int score;                                      /**< The score in singleplayer mode */
    std::string saveFile;                           /**< Path to the save file */

    /**
     * @brief Set a tile to a specified type
     * 
     * @param x the x position in the map
     * @param y the y position in the map
     * @param tile the type of the tile
     */
    void setTile(const int & x, const int & y, const ETileType & tile);

    /**
     * @brief Randomly sets the player while generating the map
     * 
     * @param player type of the player
     */
    void setPlayer(const ETileType & player);

    /**
     * @brief Randomly sets the enemies while generating the map
     * 
     * When there aren't enough available tiles for the specified number
     * of enemies, set only as many as possible
     */
    void setEnemies();

    /**
     * @brief Randomly sets breakable walls while generating the map
     * 
     * When there aren't enough available tiles for the specified number
     * of breakables, set only as many as possible
     */
    void setBreakables();

    /**
     * @brief Takes a random tile from the available tiles pool and sets it to a specified type
     * 
     * The tile is then removed from the pool of available tiles
     * 
     * @param tileType the type of the tile
     * @return the chosen tile
     */
    std::pair<int, int> takeRandomTile(const ETileType & tileType);

    /**
     * @brief Utility function for setPlayer()
     * 
     * Takes the adjacent tiles of the input center tile an if they are in
     * the available tiles pool, removes them.
     * That way the player can always start the game properly
     * 
     * By adjacent tiles we mean this:
     * 
     * & & &
     * 
     * & C &
     * 
     * & & &
     * 
     * C - the input center tile
     * 
     * & - the adjacent tiles 
     * 
     * @param tile the center tile
     */
    void takeAdjacentTiles(const std::pair<int, int> & tile);

    /**
     * @brief Sets up the map from a map loaded from a file
     * 
     * Utility function for load()
     * 
     * @param newMap the map previously loaded from a file
     * @warning throws an exception when the map layout is incorrect
     */
    void setUp(const Map & newMap);

    /**
     * @brief Utility function for map loading
     * 
     * Throws an error
     */
    static void mapLoadError();
};