#pragma once

#include "CRenderWindow.hpp"

/**
 * @brief Takes care of user interface buttons
 */
class CButton
{
public:
    EUIType type;                                       /**< The type of the button */

    /**
     * @brief CButton constructor
     * 
     * @param position the position on the screen window
     * @param width the width of the button
     * @param height the height of the button
     * @param type the type of the button
     * @param texture the sprite to be rendered on screen
     */
    CButton(const std::pair<int,int> & position, const int & width, const int & height, const EUIType & type,
            const std::shared_ptr<CRenderWindow::CTexture> & texture);

    /**
     * @brief Tells whether the player has pressed the button or not
     * 
     * @param position position of the mouse click on screen
     * @return true - the button was pressed
     * @return false - otherwise
     */
    bool isPressed(const std::pair<int,int> & position) const;

    /**
     * @brief Render the button on screen
     */
    void render() const;

private:
    std::pair<int,int> position;                        /**< The position of the button on the screen window */
    int width;                                          /**< The width of the button */
    int height;                                         /**< The height of the button */
    std::shared_ptr<CRenderWindow::CTexture> texture;   /**< The texture of the button */
};