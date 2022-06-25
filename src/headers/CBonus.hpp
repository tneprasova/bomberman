#pragma once

#include <map>

#include "CObject.hpp"
#include "Utilities.hpp"
#include "EBonusType.hpp"

/**
 * @brief Takes care of bonus instances
 */
class CBonus : public CObject
{
public:
    /**
     * @brief CBonus constructor
     * 
     * @param position the position on the screen window
     * @param tile the tile type in map
     * @param texture the sprite to render
     */
    CBonus(const std::pair<int, int> & position, const ETileType & tile,
           const std::shared_ptr<CRenderWindow::CTexture> & texture);

    /**
     * @brief Updates the object according to events
     * 
     * When in collision with a player, create events
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    void update(Events & events,
                const TileSet & tileSet,
                const std::list<std::shared_ptr<CObject>> & objects) override;

    /**
     * @brief Create events
     * 
     * The bonus flags itself to be removed and creates an event GET_BONUS,
     * which carries the type of the bonus, its strenght and a reference
     * to the player who picked it up.
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    void createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects) override;

    /**
     * @brief Retuns the position on the tile map
     * 
     * Needs to be overriden, because the bonus has a custom collision box
     * 
     * @return the position
     */
    std::pair<int,int> getTilePos() const override;

private:
    inline static std::map<EBonusType, int> bonuses;     /**< A map of all the existing events */

    // Scaling constants for the collision box
    const double xBox = 0.25;
    const double yBox = 0.25;
    const double wBox = 0.5;
    const double hBox = 0.5;

    /**
     * @brief Specifies the collision box of the bonus
     */
    void setCollisionBox() override;
};