#include "CBonus.hpp"

CBonus::CBonus(const std::pair<int, int> & position, const ETileType & tile,
               const std::shared_ptr<CRenderWindow::CTexture> & texture)
: CObject(position, tile, texture)
{
    if (this->bonuses.empty())
    {
        this->bonuses.emplace(MEGABOMBS, loadData(config, "Bonus mega bombs"));
        this->bonuses.emplace(SPEED, loadData(config, "Bonus speed"));
    }
    setCollisionBox();
}

void CBonus::update(Events & events,
                    const TileSet & tileSet,
                    const std::list<std::shared_ptr<CObject>> & objects)
{
    render();

    if (objectCollision(objects, PLAYER1) || objectCollision(objects, PLAYER2))
    {
        this->toRemove = true;
        createEvents(events, tileSet, objects);
    }
}

void CBonus::createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    using std::make_pair;

    // Randomly chooses from the bonus types
    auto it = this->bonuses.begin();
    advance(it, randomInt(0, this->bonuses.size() - 1));

    auto collidingObj = objectCollision(objects, PLAYER1);
    if (collidingObj)
        events.emplace_back(GET_BONUS, make_pair(it->first, it->second), 0, collidingObj);

    else
        events.emplace_back(GET_BONUS, make_pair(it->first, it->second), 0, objectCollision(objects, PLAYER2));
}

std::pair<int,int> CBonus::getTilePos() const
{
    int x = (this->position.first + tileWidth * (xBox + wBox)) / tileWidth;
    int y = (this->position.second + tileWidth * (yBox + hBox)) / tileWidth;

    return std::make_pair(x, y);
}

void CBonus::setCollisionBox()
{
    this->box.x = this->position.first + tileWidth * xBox;
    this->box.y = this->position.second + tileWidth * yBox;
    this->box.w = tileWidth - tileWidth * wBox;
    this->box.h = tileWidth - tileWidth * hBox;
}