#include "CBomb.hpp"

CBomb::CBomb(const std::pair<int, int> & position, const ETileType & tile,
             const std::shared_ptr<CRenderWindow::CTexture> & texture,
             const int & explosionSize)
: CObject(position, tile, texture),
  hasExploded(false),
  shown(true),
  ticks(FPS * 2),
  boomSize(explosionSize)
{}

void CBomb::update(Events & events,
                   const TileSet & tileSet,
                   const std::list<std::shared_ptr<CObject>> & objects)
{
    this->ticks --;

    if (! this->ticks)
        this->hasExploded = true;
    
    // The bomb stops rendering for one time frame - it looks like it is ticking
    if (this->ticks % (FPS / 2) == 0)
        this-> shown = false;
    else
        this->shown = true;

    if (this->shown)
        render();

    createEvents(events, tileSet, objects);
}

void CBomb::createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    using std::make_pair;

    // The bomb is still ticking, don't do anything
    if (! this->hasExploded)
    return;

    // The bomb stopped ticking and it needs to explode
    int x = deScale(this->position.first);
    int y = deScale(this->position.second);

    // The beginning and ending coordinates of the explosion
    int iX = x - boomSize;
    int maxX = x + boomSize;
    int iY = y - boomSize;
    int maxY = y + boomSize;

    // Update the coordinates when a wall is in the way
    if (tileSet[y][x - 1]->tileType == WALL)
        iX = x;
    if (tileSet[y][x + 1]->tileType == WALL)
        maxX = x;
    if (tileSet[y - 1][x]->tileType == WALL)
        iY = y;
    if (tileSet[y + 1][x]->tileType == WALL)
        maxY = y;

    // Set explosion on X axis
    for (; iX <= maxX; ++ iX)
    {
        // Ensure that it does not try to read values out of the map
        if (iX < 0 || iX > mapWidth)
            continue;

        if (tileSet[y][iX]->tileType != WALL)
            events.emplace_back(PLACE_EXPLOSION, make_pair(iX, deScale(this->position.second)), 0, nullptr);
    }
    // Set explosion on Y axis (not considering the centre of explosion)
    for (; iY <= maxY; ++ iY)
    {
        // Ensure that it does not try to read values out of the map
        if (iY < 0 || iY > mapHeight - 1)
            continue;

        if (tileSet[iY][x]->tileType != WALL)
            events.emplace_back(PLACE_EXPLOSION, make_pair(deScale(this->position.first), iY), 0, nullptr);
    }

    this->toRemove = true;
}