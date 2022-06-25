#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <map>
#include <memory>

#include "GameConstants.hpp"
#include "ETileType.hpp"
#include "ETextType.hpp"
#include "EUIType.hpp"

/**
 * @brief Class that takes care of all the rendering logic 
 */
class CRenderWindow
{
public:
    /**
     * @brief CRenderWindow constructor
     * 
     * @param title the title shown in the window
     * @param width width of the window
     * @param height height of the window
     * @warning Throws an error if the window could not be loaded
     */
    CRenderWindow(const char * title, const int & width, const int & height);

    CRenderWindow(const CRenderWindow & orig) = delete;
    CRenderWindow & operator = (const CRenderWindow & orig) = delete;

    /**
     * @brief CRenderWindow destructor
     */
    ~CRenderWindow();

    /**
     * @brief Opens a window on screen and starts rendering
     */
    void startRender();

    /**
     * @brief Clears the buffer of the renderer
     */
    void clear();

    /**
     * @brief Displays the rendered content on screen
     */
    void display();

    /**
     * @brief A class that holds renderable textures for the objects
     * 
     * It belongs to the CRenderWindow class, because it needs
     * access to the renderer
     */
    class CTexture
    {
    public:
        int width;      /**< Width of the texture */
        int height;     /**< Height of the texture */

        /**
         * @brief CTexture default constructor
         * @warning Throws an error if the texture could not be loaded
         */
        CTexture();

        /**
         * @brief CTexture constructor
         * 
         * @param filePath the file path to the image
         * @param w width of the image
         * @param h height of the image
         */
        CTexture(const char * filePath, const int & w, const int & h);

        CTexture(const CTexture & original) = delete;
        CTexture operator = (const CTexture & original) = delete;

        /**
         * @brief Renders the texture on screen
         * 
         * @param srcX x position of the source image
         * @param srcY y position of the source image
         * @param tarX x position of the target image
         * @param tarY y position of the target image
         * @param tarH width in pixels of the target image
         * @param tarW height in pixels of the target image
         */
        void render(const int & srcX, const int & srcY,
                    const int & tarX, const int & tarY, const int & tarH, const int & tarW) const;
    
    friend class CRenderWindow;

    private:
        SDL_Texture * texture;      /**< The loaded texture */
    };

    /**
     * @brief A class that manages renderable text textures
     */
    class CText
    {
    public:
        /**
         * @brief CText default constructor
         */
        CText();

        /**
         * @brief CText constructor
         * 
         * @param filePath the file path to the font
         * @param color color of the rendered text
         * @param size size of the font
         */
        CText(const char * filePath, const SDL_Color & color, const int & size);

        CText(const CText & original) = delete;
        CText operator = (const CText & original) = delete;

        /**
         * @brief Create a new text texture in case the rendered text needs to change
         * 
         * @param text the text to be rendered
         */
        void renderNew(const std::string & text);

        /**
         * @brief Render the text texture
         * 
         * @param text the text to be rendered
         * @param position the position on the screen
         * @param width the width of a letter
         * @param height the height of the text
         */
        void render(const std::string & text, const std::pair<int,int> & position,
                    const int & width = tileWidth / 2, const int & height = tileWidth);

        friend class CRenderWindow;

    private:
        TTF_Font * font;            /**< The font of the text */
        SDL_Color color;            /**< The color of the text */
        SDL_Texture * textTexture;  /**< The texture of the text */
        std::string oldText;        /**< The message that is currently rendered */
    };

    /**
     * @brief Gets a texture of a specified tile
     * 
     * @param tile the tile type
     * @return the texture
     */
    std::shared_ptr<CRenderWindow::CTexture> getTexture(const ETileType & tile) const;

    /**
     * @brief Gets a texture of a specified text
     * 
     * @param textType the text type
     * @return the texture
     */
    std::shared_ptr<CRenderWindow::CText> getText(const ETextType & textType) const;

    /**
     * @brief Gets a texture of a specified UI component
     * 
     * @param UIType the text type
     * @return the text texture
     */
    std::shared_ptr<CRenderWindow::CTexture> getUI(const EUIType & UIType) const;

    friend class CObjectEventManager;

private:
    SDL_Window * window;                                  /**< Stores the created window */
    static SDL_Renderer * renderer;                       /**< Stores the rendering context for the window*/
    std::map<ETileType, std::shared_ptr<CTexture>> tiles; /**< Stores tile textures */
    std::map<ETextType, std::shared_ptr<CText>> text;     /**< Stores text textures */
    std::map<EUIType, std::shared_ptr<CTexture>> UI;      /**< Stores UI textures */

    /**
     * @brief Loads in the needed textures
     */
    void loadTextures();

    /**
     * @brief Loads in the needed text textures
     */
    void loadTexts();
};