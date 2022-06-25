#include "CPlayer.hpp"

CPlayer::CPlayer(const std::pair<int,int> & position,
                 const ETileType & tile,
                 const std::shared_ptr<CRenderWindow::CTexture> & texture,
                 int * score,
                 const std::shared_ptr<CRenderWindow::CText> & textSource,
                 const SDL_Scancode & up,
                 const SDL_Scancode & down,
                 const SDL_Scancode & left,
                 const SDL_Scancode & right,
                 const SDL_Scancode & bomb)
: CObject(position, tile, texture),
  up(up),
  down(down),
  left(left),
  right(right),
  bomb(bomb),
  placingBomb(false),
  speed(playerSpeed),
  bombSize(1),
  score(score, textSource)
{}

void CPlayer::update(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    using namespace std;

    render();
    move(tileSet);
    createEvents(events, tileSet, objects);

    // Render score on screen, each player on different coordinations
    if (this->tile == PLAYER1)
        this->score.render(make_pair(0,0));
    else
        this->score.render(make_pair(mapWidth * tileWidth - (tileWidth / 2), 0));

    list<Events::iterator> eventsToRemove;

    if (! this->toRemove)   // So points wouldn't get added to a dead player or when a player kills itself
        for (auto event = events.begin(); event != events.end(); ++ event)
        {
            // Add points to the score
            if (get<0>(*event) == POINTS)
            {
                this->score += get<2>(*event);
                eventsToRemove.push_back(event);
            }
            // Gain a boost from bonus events
            if (get<0>(*event) == GET_BONUS && get<3>(*event) == this)
            {
                if (get<1>(*event).first == MEGABOMBS)
                    this->bombSize = 1 + get<1>(*event).second;

                if (get<1>(*event).first == SPEED)
                    this->speed = playerSpeed + get<1>(*event).second;

                eventsToRemove.push_back(event);
            }
        }

    // Remove events that have been carried out
    for (auto event : eventsToRemove)
        events.erase(event);
}

void CPlayer::createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    using std::make_pair;

    // Create a bomb on players coordinates
    const uint8_t * currentKeyStates = SDL_GetKeyboardState(nullptr);
    if (! placingBomb && currentKeyStates[this->bomb])
    {
        this->placingBomb = true;
        events.emplace_back(PLACE_BOMB, getTilePos(), this->bombSize, this);
    }
    // Make sure to place only one bomb per key press
    else if (placingBomb && ! currentKeyStates[this->bomb])
        this->placingBomb = false;

    // Flag itself to get removed
    if (objectCollision(objects, BOOM) || objectCollision(objects, ENEMY))
    {
        this->toRemove = true;
        events.emplace_back(PLAYER_DEAD, make_pair(0,0), 0, this);
        events.emplace_back(POINTS, make_pair(0,0), 1, nullptr);
    }
}

std::pair<int,int> CPlayer::getTilePos() const
{
    int x = (this->box.x + tileWidth * (leftWall + rightWall)) / tileWidth;
    int y = (this->box.y + tileWidth * (upWall - (downWall / 2) - 0.005)) / tileWidth;

    return std::make_pair(x, y);
}

void CPlayer::move(const TileSet & tileSet)
{
    int dirX = 0;
    int dirY = 0;

    // Move according to the key pressed
    const uint8_t * currentKeyStates = SDL_GetKeyboardState(nullptr);
    if (currentKeyStates[this->up] && !currentKeyStates[this->down])
    {
        dirY = -1;
        changePos(this->position.first, this->position.second - this->speed);
    }
    if (currentKeyStates[this->down] && ! currentKeyStates[this->up])
    {
        dirY = 1;
        changePos(this->position.first, this->position.second + this->speed);
    }
    if (currentKeyStates[this->left] && ! currentKeyStates[this->right])
    {
        dirX = -1;
        changePos(this->position.first - this->speed, this->position.second);
    }
    if (currentKeyStates[this->right] && ! currentKeyStates[this->left])
    {
        dirX = 1;
        changePos(this->position.first + this->speed, this->position.second);
    }

    setCollisionBox();

    // Move back in case it collided with a wall
    if (this->box.x < 0 || this->box.x > screenWidth - tileWidth || wallCollision(tileSet, upWall, downWall, leftWall, rightWall))
        this->position.first -= this->speed * dirX;

    if (this->box.y < 0 || this->box.y > screenHeight - tileWidth || wallCollision(tileSet, upWall, downWall, leftWall, rightWall))
        this->position.second -= this->speed * dirY;
}

void CPlayer::setCollisionBox()
{
    this->box.x = this->position.first + tileWidth * xBox;
    this->box.y = this->position.second + tileWidth * yBox;
    this->box.w = tileWidth - tileWidth * wBox;
    this->box.h = tileWidth - tileWidth * hBox;
}