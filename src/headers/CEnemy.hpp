#pragma once

#include <set>

#include "GameConstants.hpp"
#include "CObject.hpp"
#include "Utilities.hpp"

/**
 * @brief The available directions of movement
 */
enum EDirection
{
    STAY,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @brief Takes care of enemy instances
 */
class CEnemy : public CObject
{
public:
    /**
     * @brief CEnemy constructor
     * 
     * @param position the position on the screen window
     * @param tile the tile type in map
     * @param texture the sprite to render
     */
    CEnemy(const std::pair<int,int> & position, const ETileType & tile,
           const std::shared_ptr<CRenderWindow::CTexture> & texture);

    /**
     * @brief Updates the object according to events
     * 
     * Move and render itself each time frame
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    void update(Events & events, const TileSet & tileSet,
                const std::list<std::shared_ptr<CObject>> & objects) override;

    /**
     * @brief Create events
     * 
     * When in collision with an explosion, flag itself to be removed.
     * Create an event POINTS, which carries the amount of points, that
     * the player is going to gain from killing the enemy.
     * In case it was the last enemy standing, create an event ENEMIES_DEAD,
     * so the object manager knows to create a door to another level.
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    void createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects) override;

    /**
     * @brief Retuns the position on the tile map
     * 
     * Needs to be overriden, because the enemy can stand between two tiles while saving the map
     * This chooses the one on which it is 'standing more'
     * 
     * @return the position
     */
    std::pair<int,int> getTilePos() const override;

private:
    int frameNumber;                            /**< The number of frames for which the enemy moves in a certain direction */
    std::set<EDirection> availableDirections;   /**< The directions which haven't been tried yet */
    EDirection currentDirection;                /**< The current direction of movement */

    // Constants to specify the collision box, chosen based on the dimensions
    // of the enemies sprite
    const double xBox = 0.25;
    const double yBox = 0.15;
    const double wBox = 0.5;
    const double hBox = 0.2;

    /**
     * @brief Simple AI for the enemy's movement
     * 
     * Chooses a direction of movement and the number of frames of moving in that direction at random
     * Chooses only the directions, in which it can move - meaning it recognizes walls
     * It does not voluntarily step into an ongoing explosion, the enemy must be caught into it
     * 
     * @param tileSet the tiles on the map - needed for collisions
     * @param objects the objects on the map
     */
    void move(const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects);

    /**
     * @brief Set a random direction and random duration of moving that way
     */
    void setDirection();

    /**
     * @brief Specifies the collision box of the enemy
     */
    void setCollisionBox() override;
};