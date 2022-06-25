#include "CObject.hpp"

CObject::CObject(const std::pair<int, int> & position,
                 const ETileType & tile,
                 const std::shared_ptr<CRenderWindow::CTexture> & texture)
: toRemove(false),
  position(scale(position)),
  texture(texture),
  tile(tile)
{ setCollisionBox(); }

ETileType CObject::getTile() const { return this->tile; }

void CObject::render() const
{
    this->texture->render(0, 0, this->position.first, this->position.second, tileWidth, tileWidth);
}

void CObject::createEvents(Events & events, const TileSet & tileSet,
                           const std::list<std::shared_ptr<CObject>> & objects)
{}

std::pair<int, int> CObject::getTilePos() const
{
    return deScale(this->position);
}

void CObject::setCollisionBox()
{
    this->box.x = this->position.first;
    this->box.y = this->position.second;
    this->box.w = tileWidth;
    this->box.h = tileWidth;
}

bool CObject::wallCollision(const TileSet & tileSet,
                            const double & u,
                            const double & d,
                            const double & l,
                            const double & r) const
{
    auto tile = getTilePos();

    // Check adjacent tiles for collision
    for (int y = tile.second - 2; y < tile.second + 2; ++ y)
    {
        for (int x = tile.first - 2; x < tile.first + 2; ++ x)
        {
            if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
                if(tileSet[y][x]->tileType == WALL || tileSet[y][x]->tileType == BREAKABLE)
                {
                    if(checkCollision(tileSet[y][x]->box, u, d, l, r))
                    return true;
                }
        }
    }

    return false;
}

CObject * CObject::objectCollision(const std::list<std::shared_ptr<CObject>> & objects,
                                   const ETileType & tileType,
                                   const double & u,
                                   const double & d,
                                   const double & l,
                                   const double & r) const
{
    for (auto & obj : objects)
        if (obj->getTile() == tileType)
            if (checkCollision(obj->box, u, d, l, r))
                return obj.get();
    
    return nullptr;
}

bool CObject::checkCollision(const SDL_Rect & otherBox,
                             const double & u,
                             const double & d,
                             const double & l,
                             const double & r) const
{
    if (otherBox.y + otherBox.h  - (tileWidth * u) > this->box.y    // Up collision
    && this->box.y + this->box.h - (tileWidth * d) > otherBox.y     // Down collision
    && otherBox.x + otherBox.w   - (tileWidth * l) > this->box.x    // Left collision
    && this->box.x + this->box.w - (tileWidth * r) > otherBox.x)    // Right collision
        return true;

    return false;
}

void CObject::changePos(const int & x, const int & y) { this->position = std::make_pair(x, y); }

int CObject::scale(const int & x) { return x * tileWidth; }

std::pair<int,int> CObject::scale(const std::pair<int,int> & x) { return std::make_pair(x.first * tileWidth, x.second * tileWidth); }

int CObject::deScale(const int & x) { return x / tileWidth; }

std::pair<int,int> CObject::deScale(const std::pair<int,int> & x) { return std::make_pair(x.first / tileWidth, x.second / tileWidth); }