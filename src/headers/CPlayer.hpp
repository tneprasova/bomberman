#pragma once

#include <SDL2/SDL.h>

#include "CObject.hpp"
#include "CScore.hpp"
#include "EBonusType.hpp"

/**
 * @brief Takes care of player instances
 */
class CPlayer : public CObject
{
public:
    /**
     * @brief CPlayer constructor
     * 
     * @param position the position on the screen window
     * @param tile the tile type in map
     * @param texture the sprite to render 
     * @param score the score of the player
     * @param textSource pointer to the text texture which shows the score
     * @param up the key used to move up
     * @param down the key used to move down
     * @param left the key used to move left
     * @param right the key used to move right
     * @param bomb the key used to place bombs
     */
    CPlayer(const std::pair<int,int> & position,
            const ETileType & tile,
            const std::shared_ptr<CRenderWindow::CTexture> & texture,
            int * score,
            const std::shared_ptr<CRenderWindow::CText> & textSource,
            const SDL_Scancode & up,
            const SDL_Scancode & down,
            const SDL_Scancode & left,
            const SDL_Scancode & right,
            const SDL_Scancode & bomb);

    /**
     * @brief Updates the object according to events
     * 
     * Render itself, the score and move according to the keys pressed
     * 
     * React to events:
     *  - POINTS - add points to score
     *  - GET_BONUS - gain a boost according to the bonus type
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
     * When in collision with CExplosion or CEnemy - flag itself to get removed,
     * create a POINTS and PLAYER_DEAD event
     * When a bomb gets placed, create a PLACE_BOMB event. Make sure only one
     * bomb gets placed per one key press
     * 
     * @param events the list of events
     * @param tileSet the set of tiles on the map
     * @param objects the list of existing objects
     */
    void createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects) override;

    /**
     * @brief Retuns the position on the tile map
     * 
     * Needs to be overriden, because the player can stand between two tiles while saving the map
     * This chooses the one on which it is 'standing more'
     * 
     * @return the position
     */
    std::pair<int,int> getTilePos() const override;

private:
    SDL_Scancode up;    /**< The key used to move up */
    SDL_Scancode down;  /**< The key used to move down */
    SDL_Scancode left;  /**< The key used to move left */
    SDL_Scancode right; /**< The key used to move right */
    SDL_Scancode bomb;  /**< The key used to place bombs */
    bool placingBomb;   /**< Flag that ensures only one bomb gets placed per one key press */
    int speed;          /**< The speed of the player */
    int bombSize;       /**< The size of player's explosions */
    CScore score;       /**< Player's score */

    // Scaling constants for wall collisions
    const double upWall    = 0.8;
    const double downWall  = -0.1;
    const double leftWall  = 0.1;
    const double rightWall = 0.1;

    // Scaling constants for the collision box
    const double xBox = 0.25;
    const double yBox = 0.1;
    const double wBox = 0.5;
    const double hBox = 0.2;

    /**
     * @brief Changes position according to key presses
     * 
     * @param tileSet the tiles on the map - needed for collisions
     */
    void move(const TileSet & tileSet);

    /**
     * @brief Specifies the collision box of the player
     */
    void setCollisionBox() override;
};