#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "CRenderWindow.hpp"
#include "CUserInterface.hpp"
#include "CObjectEventManager.hpp"
#include "CMap.hpp"
#include "GameConstants.hpp"

/**
 * @brief Merges all of the game logic together
 */
class CGame
{
public:
    /**
     * @brief Start the game
     * 
     * It catches all possible errors made during the game initialization.
     * If an error occured, the game won't start and an error message will be printed
     * 
     * @param title the title shown in the game window
     */
    CGame(const char * title);

    CGame(const CGame & orig) = delete;
    CGame & operator = (const CGame & orig) = delete;

    /**
     * @brief Destroys the game when the player closes the window
     */
    ~CGame();

    /**
     * @brief Starts the game loop
     */
    void run();

    /**
     * @brief Handles user induced events
     * 
     * Events:  - Clicking the X button on the game window - quit the game
     *          - Pressing F5 in singleplayer mode - save the game
     *          - Pressing ESC during a game - jump to the main menu and discard the current game
     *          - Pressing a button in the main menu - does something according to the button pressed
     */
    void handleEvents();

    /**
     * @brief Returns the game state
     * 
     * @return true - game is still running
     * @return false - game is not running - either due to an error or by quitting the game
     */
    bool isRunning() const;

    /**
     * @brief Creates a new map for the game
     */
    void newMap();

    /**
     * @brief Starts a new game according to the specified game mode
     */
    void newGame();

private:
    bool gameOn;                                    /**< Flag that keeps the game loop going */
    bool startGame;                                 /**< Flag that starts the game */
    EGameMode mode;                                 /**< The current game mode */
    CRenderWindow * window;                         /**< Holds rendering and it's logic */
    std::shared_ptr<CMap> map;                      /**< Holds the map manager */
    std::shared_ptr<CObjectEventManager> manager;   /**< Holds the game field manager */
    std::shared_ptr<CUserInterface> UI;             /**< Holds the main menu user interface */

    /**
     * @brief Initialize SDL
     * 
     * Initializes needed subsystems and libraries for SDL
     * @warning Throws an error when something goes wrong
     */
    static void initSDL();

    /**
     * @brief Checks, whether the data in the configuration file is valid
     * 
     * @warning Throws an error when it isn't
     */
    static void checkConfig();
};