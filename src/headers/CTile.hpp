#pragma once

#include <SDL2/SDL.h>

#include "CRenderWindow.hpp"
#include "ETileType.hpp"
#include "GameConstants.hpp"

/**
 * @brief Class for tiles on the game map
 * 
 * They don't have a reason to be CObjects, because they don't have any special behaviour
 */
class CTile
{
public:
    /**
     * @brief CTile constructor
     * 
     * @param x x position
     * @param y y position
     * @param tileType the type of the tile
     * @param texture pointer to the texture to render
     */
    CTile(const int & x, const int & y, const ETileType & tileType,
          std::shared_ptr<CRenderWindow::CTexture> texture);

    /**
     * @brief Renders the tile on screen
     */
    void update();

    friend class CObjectEventManager;
    friend class CObject;
    friend class CBomb;

private:
    ETileType tileType;                                 /**< The type of the tile */
    SDL_Rect box;                                       /**< The collision box */
    std::pair<int, int> position;                       /**< The position on the screen window */
    std::shared_ptr<CRenderWindow::CTexture> texture;   /**< The texture of the tile */
};