#include "CDoor.hpp"

CDoor::CDoor(const std::pair<int, int> & position, const ETileType & tile,
             const std::shared_ptr<CRenderWindow::CTexture> & texture)
: CObject(position, tile, texture)
{}

void CDoor::update(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    render();
    createEvents(events, tileSet, objects);
}

void CDoor::createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    if (objectCollision(objects, PLAYER1))
        events.emplace_back(DOOR_REACHED, std::make_pair(0,0), 0, nullptr);
}