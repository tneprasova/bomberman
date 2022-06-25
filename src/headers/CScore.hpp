#pragma once

#include "CObject.hpp"
#include "CRenderWindow.hpp"

/**
 * @brief Takes care of the player's score
 */
class CScore
{
public:
    /**
     * @brief CScore constructor
     * 
     * @param num the score number
     * @param textSource the source of the text to render
     */
    CScore(int * num, const std::shared_ptr<CRenderWindow::CText> & textSource);

    /**
     * @brief Add to score
     * 
     * @param num the number to add
     */
    void operator += (const int & num);

    /**
     * @brief Render the score on screen
     * 
     * @param position the position to render it on
     */
    void render(const std::pair<int,int> & position) const;

private:
    int * score;                                /**< The score number */
    std::shared_ptr<CRenderWindow::CText> text; /**< The text texture that gets rendered */
};