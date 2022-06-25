#pragma once

#include "CObject.hpp"

/**
 * @brief Takes care of bomb instances
 */
class CBomb : public CObject
{
public:
    /**
     * @brief CBomb constructor
     * 
     * @param position the position on the screen window
     * @param tile the tile type in map
     * @param texture the sprite to render
     * @param explosionSize sets the explosion from center to specified lenght
     */
    CBomb(const std::pair<int, int> & position, const ETileType & tile,
          const std::shared_ptr<CRenderWindow::CTexture> & texture,
          const int & explosionSize = 1);

    /**
     * @brief Updates the object according to events
     * 
     * After a certain amount of time frames, explode
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
     * When a CBomb explodes it creates PLACE_EXPLOSION events to let the object manager know,
     * where CExplosions need to be constructed
     * 
     * The explosion looks like a cross on the map, it cannot go through the undestroyable walls
     * and its size is determined by the boomSize variable
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    void createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects) override;

private:
    bool hasExploded;   /**< Specifies, whether the bomb has exploded */
    bool shown;         /**< Utility flag for bomb ticking */
    int ticks;          /**< Number of time frames for which it is ticking */
    int boomSize;       /**< Size of the explosion */
};