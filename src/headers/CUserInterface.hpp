#pragma once

#include <list>
#include <memory>

#include "CButton.hpp"
#include "CRenderWindow.hpp"
#include "Utilities.hpp"

/**
 * @brief Takes care of the user interface in main menu
 */
class CUserInterface
{
public:
    /**
     * @brief CUserInterface constructor
     * 
     * @param renderer pointer to the rendering logic - needed for loading the textures
     */
    CUserInterface(const CRenderWindow * renderer);

    /**
     * @brief Handles button presses
     * 
     * @param event the event induced by the player
     * @return the button that was pressed 
     */
    EUIType handleEvents(const SDL_Event * event);

    /**
     * @brief Render the UI if it is supposed to be shown
     */
    void render() const;

    /**
     * @brief Show the UI
     */
    void show();

private:
    int shown;                                              /**< Flag which controls rendering */
    int highScore;                                          /**< The current high score */
    double scalar;                                          /**< Scales the UI according to the size of the window */
    std::shared_ptr<CRenderWindow::CText> highScoreText;    /**< The text texture of the high score */
    std::list<std::unique_ptr<CButton>> buttons;            /**< The UI components */
};