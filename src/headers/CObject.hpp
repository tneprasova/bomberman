#pragma once

#include <utility>
#include <map>
#include <memory>
#include <algorithm>
#include <list>

#include "GameConstants.hpp"
#include "CRenderWindow.hpp"
#include "CTile.hpp"
#include "ETileType.hpp"
#include "EEvent.hpp"

/**
 * @brief An abstract class for all objects on map
 */
class CObject
{
public:
    /**
     * @brief CObject constructor
     * 
     * @param position the position on the screen window
     * @param tile the tile type in map
     * @param texture the sprite to render
     */
    CObject(const std::pair<int, int> & position, const ETileType & tile,
            const std::shared_ptr<CRenderWindow::CTexture> & texture);

    /**
     * @brief Default destructor 
     */
    virtual ~CObject() = default;

    /**
     * @brief Get the tile type
     * 
     * This identification is needed for loading and saving the game and
     * also for collisions between objects
     * 
     * @return tile type 
     */
    ETileType getTile() const;

    /**
     * @brief Renders the object on screen depending on its position
     */
    void render() const;

    /**
     * @brief Updates the object according to events
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    virtual void update(Events & events, const TileSet & tileSet,
                        const std::list<std::shared_ptr<CObject>> & objects) = 0;

    /**
     * @brief Creates new events
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    virtual void createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects);

    /**
     * @brief Returns the position of the object in a 2D vector of tiles
     * 
     * Needed for loading and saving the game
     * 
     * @return the position
     */
    virtual std::pair<int, int> getTilePos() const;

    friend class CObjectEventManager;

protected:
    bool toRemove;                                      /**< Utility variable for deleting objects */
    std::pair<int, int> position;                       /**< The position of the object on the screen */
    std::shared_ptr<CRenderWindow::CTexture> texture;   /**< The texture of the object */
    SDL_Rect box;                                       /**< The collision box of the object */
    ETileType tile;                                     /**< Specifies the tile type on the map */

    /**
     * @brief Sets up the collision box
     */
    virtual void setCollisionBox();

    /**
     * @brief Checks for collisions with walls
     * 
     * Parametres u, d, l, r serve for changing the collision tolerance
     * They are used by the objects, whose collision box is not tileWidth * tileWidth
     * 
     * @param tileSet the tiles on the map
     * @return true - collides with a wall
     * @return false - otherwise
     */
    bool wallCollision(const TileSet & tileSet,
                       const double & u = 0,
                       const double & d = 0,
                       const double & l = 0,
                       const double & r = 0) const;

    /**
     * @brief Checks for collision with other objects
     * 
     * Parametres u, d, l, r serve for changing the collision tolerance
     * They are used by the objects, whose collision box is not tileWidth * tileWidth
     * 
     * @param objects the list of objects
     * @param tileType the tile type of the other object we check collision for
     * @return the pointer to the colliding object
     * @return nullpointer if no such object is in collision
     */
    CObject * objectCollision(const std::list<std::shared_ptr<CObject>> & objects,
                              const ETileType & tileType,
                              const double & u = 0,
                              const double & d = 0,
                              const double & l = 0,
                              const double & r = 0) const;

    /**
     * @brief Checks for collision with another collision box
     * 
     * Parametres u, d, l, r serve for changing the collision tolerance
     * They are used by the objects, whose collision box is not tileWidth * tileWidth
     * 
     * @param otherBox the other collision box
     * @return true - the two collision boxes collide
     * @return false - otherwise
     */
    bool checkCollision(const SDL_Rect & otherBox,
                        const double & u = 0,
                        const double & d = 0,
                        const double & l = 0,
                        const double & r = 0) const;

    /**
     * @brief Changes the position of the object
     * 
     * @param x x coordinate
     * @param y y coordinate
     */
    void changePos(const int & x, const int & y);

    /**
     * @brief Utility function which scales by the tile width
     */
    static int scale(const int & x);

    /**
     * @brief Utility function which scales the position by the tile width
     */
    static std::pair<int,int> scale(const std::pair<int,int> & x);

    /**
     * @brief Utility function which descales by the tile width
     */
    static int deScale(const int & x);

    /**
     * @brief Utility function which descales the position by the tile width
     */
    static std::pair<int,int> deScale(const std::pair<int,int> & x);
};