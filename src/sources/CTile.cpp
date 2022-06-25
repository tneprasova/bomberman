#include "CTile.hpp"

CTile::CTile(const int & x, const int & y, const ETileType & tileType, std::shared_ptr<CRenderWindow::CTexture> texture)
: tileType(tileType),
  position(std::make_pair(x * tileWidth, y * tileWidth)),
  texture(texture)
{
    this->box.x = x * tileWidth;
    this->box.y = y * tileWidth;
    this->box.w = tileWidth;
    this->box.h = tileWidth;
}

void CTile::update()
{
    this->texture->render(0, 0, this->position.first, this->position.second, tileWidth, tileWidth);
}