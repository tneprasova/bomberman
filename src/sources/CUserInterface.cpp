#include "CUserInterface.hpp"

CUserInterface::CUserInterface(const CRenderWindow * renderer)
: shown(true),
  highScore(0),
  scalar(1)
{
    using std::make_pair;

    // Change the scalar according to the window size
    if (screenWidth < 1200 || screenHeight < 1200)
        this->scalar = 0.5;

    if (screenWidth < 600 || screenHeight < 600)
        this->scalar = 0.25;

    // Load the highscore
    this->highScore = loadData(config, "High score");
    this->highScoreText = renderer->getText(HIGH_SCORE);

    // Get the dimensions of the background, scale them
    int width = renderer->getUI(UI_BACKGROUND)->width * 8 * this->scalar;
    int height = renderer->getUI(UI_BACKGROUND)->height * 8 * this->scalar;

    this->buttons.emplace_back(new CButton(make_pair(screenWidth / 2 - width / 2, screenHeight / 2 - height / 2), width, height, UI_BACKGROUND, renderer->getUI(UI_BACKGROUND)));

    // Scalars for button dimensions and coordinations were choosed experimentally
    // They should scale the UI to fit a window of any given size
    width = renderer->getUI(UI_NEW_GAME)->width * 10 * this->scalar;
    height = renderer->getUI(UI_NEW_GAME)->height * 10 * this->scalar;

    this->buttons.emplace_back(new CButton(make_pair(screenWidth / 2 - width / 2, screenHeight / 2 - height * 2.75), width, height, UI_NEW_GAME, renderer->getUI(UI_NEW_GAME)));
    this->buttons.emplace_back(new CButton(make_pair(screenWidth / 2 - width / 2, screenHeight / 2 - height * 1.25), width, height, UI_LOAD, renderer->getUI(UI_LOAD)));
    this->buttons.emplace_back(new CButton(make_pair(screenWidth / 2 - width / 2, screenHeight / 2 + height * 0.25), width, height, UI_DUEL, renderer->getUI(UI_DUEL)));
    this->buttons.emplace_back(new CButton(make_pair(screenWidth / 2 - width / 2, screenHeight / 2 + height * 1.75), width, height, UI_EXIT, renderer->getUI(UI_EXIT)));
}

EUIType CUserInterface::handleEvents(const SDL_Event * event)
{
    if (this->shown)
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            std::pair<int,int> position;
            SDL_GetMouseState(&position.first, &position.second);

            for (auto & button : this->buttons)
            {
                if (button->isPressed(position) && button->type != UI_BACKGROUND)
                {
                    this->shown = false;
                    return button->type;
                }
            }
        }
    return NONE;
}

void CUserInterface::render() const
{
    using namespace std;

    // Render everything when the UI is shown
    if (this->shown)
    {
        this->highScoreText->render("HIGH SCORE: "s.append(to_string(this->highScore)), make_pair(tileWidth / 2, 0), 40 * this->scalar, 80 * this->scalar);
        for (auto & button : this->buttons)
            button->render();
    }
}

void CUserInterface::show()
{
    this->shown = true;
    // Update the high score
    this->highScore = loadData(config, "High score");
}