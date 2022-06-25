#pragma once

#include "CObject.hpp"

/**
 * @brief Takes care of a door instance
 */
class CDoor : public CObject
{
public:
    /**
     * @brief CDoor constructor
     * 
     * @param position the position on the screen window
     * @param tile the tile type in map
     * @param texture the sprite to render
     */
    CDoor(const std::pair<int, int> & position, const ETileType & tile,
          const std::shared_ptr<CRenderWindow::CTexture> & texture);

    /**
     * @brief Updates the door according to events
     * 
     * Only renders itself and creates events
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
     * When in collision with a player, create event DOOR_REACHED, so the game could
     * continue to another level
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    void createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects) override;
};