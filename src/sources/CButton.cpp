#include "CButton.hpp"

CButton::CButton(const std::pair<int,int> & position, const int & width, const int & height, const EUIType & type,
                 const std::shared_ptr<CRenderWindow::CTexture> & texture)
: type(type),
  position(position),
  width(width),
  height(height),
  texture(texture)
{}

bool CButton::isPressed(const std::pair<int,int> & position) const
{
    bool res = true;

    if (position.first < this->position.first || position.first > this->position.first + this->width)
        res = false;

    if (position.second < this->position.second || position.second > this->position.second + this->height)
        res = false;

    return res;
}

void CButton::render() const
{
    this->texture->render(0, 0, this->position.first, this->position.second, this->width, this->height);
}