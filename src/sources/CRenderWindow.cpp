#include "CRenderWindow.hpp"

SDL_Renderer * CRenderWindow::renderer = nullptr;

CRenderWindow::CRenderWindow(const char * title, const int & width, const int & height)
{
    using namespace std;

    this->window = nullptr;
    this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    
    if (! this->window)
        throw runtime_error("SDL window initialization error: "s.append(SDL_GetError()));
}

CRenderWindow::~CRenderWindow()
{
    // Pre delete all textures, otherwise they would get destroyed after CRenderWindow
    // and their destruction wouldn't work properly
    for (auto & tile : this->tiles)
        SDL_DestroyTexture(tile.second->texture);

    for (auto & texture : this->UI)
        SDL_DestroyTexture(texture.second->texture);

    // Pre delete all text textures and their fonts
    for (auto & text : this->text)
    {
        SDL_DestroyTexture(text.second->textTexture);
        TTF_CloseFont(text.second->font);
    }

    if (this->renderer)
        SDL_DestroyRenderer(this->renderer);
    if (this->window)
        SDL_DestroyWindow(this->window);
    
    this->renderer = nullptr;
    this->window = nullptr;
}

void CRenderWindow::startRender()
{
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    this->loadTextures();
    this->loadTexts();
}

std::shared_ptr<CRenderWindow::CTexture> CRenderWindow::getTexture(const ETileType & tile) const
{
    return this->tiles.find(tile)->second;
}

std::shared_ptr<CRenderWindow::CText> CRenderWindow::getText(const ETextType & textType) const
{
    return this->text.find(textType)->second;
}

std::shared_ptr<CRenderWindow::CTexture> CRenderWindow::getUI(const EUIType & UIType) const
{
    return this->UI.find(UIType)->second;
}

void CRenderWindow::clear()
{
    SDL_RenderClear(this->renderer);
}

void CRenderWindow::display()
{
    // SDL_RenderDrawPoint is used as a bug fix for a bug built in SDL2, which occurs while rendering text
    // The screen would otherwise become blank, if a new text message was to render
    SDL_RenderDrawPoint(renderer, 0, 0);
    SDL_RenderPresent(this->renderer);
}

void CRenderWindow::loadTextures()
{
    using std::shared_ptr;

    // Tile textures
    this->tiles.emplace(EMPTY,      shared_ptr<CTexture>(new CTexture("assets/grass.png", 32, 32)));
    this->tiles.emplace(WALL,       shared_ptr<CTexture>(new CTexture("assets/wall.png", 32, 32)));
    this->tiles.emplace(BREAKABLE,  shared_ptr<CTexture>(new CTexture("assets/breakable-wood.png", 32, 32)));
    this->tiles.emplace(DOOR,       shared_ptr<CTexture>(new CTexture("assets/door.png", 32, 32)));
    this->tiles.emplace(BOMB,       shared_ptr<CTexture>(new CTexture("assets/bomb.png", 32, 32)));
    this->tiles.emplace(BOOM,       shared_ptr<CTexture>(new CTexture("assets/boom.png", 32, 32)));
    this->tiles.emplace(BONUS,      shared_ptr<CTexture>(new CTexture("assets/bonus.png", 32, 32)));
    this->tiles.emplace(PLAYER1,    shared_ptr<CTexture>(new CTexture("assets/player1.png", 32, 32)));
    this->tiles.emplace(PLAYER2,    shared_ptr<CTexture>(new CTexture("assets/player2.png", 32, 32)));
    this->tiles.emplace(ENEMY,      shared_ptr<CTexture>(new CTexture("assets/enemy.png", 32, 32)));

    // UI textures
    this->UI.emplace(UI_BACKGROUND, shared_ptr<CTexture>(new CTexture("assets/UI-Background.png", 128, 128)));
    this->UI.emplace(UI_NEW_GAME,   shared_ptr<CTexture>(new CTexture("assets/UI-NewGame.png", 66, 15)));
    this->UI.emplace(UI_LOAD,       shared_ptr<CTexture>(new CTexture("assets/UI-Load.png", 66, 15)));
    this->UI.emplace(UI_DUEL,       shared_ptr<CTexture>(new CTexture("assets/UI-Duel.png", 66, 15)));
    this->UI.emplace(UI_EXIT,       shared_ptr<CTexture>(new CTexture("assets/UI-Exit.png", 66, 15)));
}

void CRenderWindow::loadTexts()
{
    using std::shared_ptr;

    this->text.emplace(PLAYER1_SCORE, shared_ptr<CText>(new CText("./assets/PixelEmulator.ttf", {172,50,50,255}, 32)));
    this->text.emplace(PLAYER2_SCORE, shared_ptr<CText>(new CText("./assets/PixelEmulator.ttf", {34,175,175,255}, 32)));
    this->text.emplace(HIGH_SCORE,    shared_ptr<CText>(new CText("./assets/PixelEmulator.ttf", {255,170,0,255}, 32)));
}