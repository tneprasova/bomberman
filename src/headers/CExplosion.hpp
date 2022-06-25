#pragma once

#include "CObject.hpp"

/**
 * @brief Takes care of explosion instances
 */
class CExplosion : public CObject
{
public:
    /**
     * @brief CExplosion constructor
     * 
     * @param position the position on the screen window
     * @param tile the tile type in map
     * @param texture the sprite to render
     */
    CExplosion(const std::pair<int, int> & position, const ETileType & tile,
          const std::shared_ptr<CRenderWindow::CTexture> & texture);

    /**
     * @brief Updates the object according to events
     * 
     * Render itself, after a certain amount of time frames, flag itself to get removed
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    void update(Events & events,
                const TileSet & tileSet,
                const std::list<std::shared_ptr<CObject>> & objects) override;

private:
    int duration;   /**< The amount of time frames for which the instance lives */
};