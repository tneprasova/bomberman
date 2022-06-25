#include "CExplosion.hpp"

CExplosion::CExplosion(const std::pair<int, int> & position, const ETileType & tile,
                       const std::shared_ptr<CRenderWindow::CTexture> & texture)
: CObject(position, tile, texture),
  duration(FPS / 2)
{}

void CExplosion::update(Events & events, const TileSet & tileSet,
                        const std::list<std::shared_ptr<CObject>> & objects)
{
    render();
    -- this->duration;

    if (! this->duration)
        this->toRemove = true;
}