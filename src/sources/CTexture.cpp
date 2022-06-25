#include "CRenderWindow.hpp"

CRenderWindow::CTexture::CTexture()
: texture(nullptr)
{}

CRenderWindow::CTexture::CTexture(const char * filePath, const int & w, const int & h)
: width(w),
  height(h),
  texture(nullptr)
{
    using namespace std;

    if (! renderer)
        throw runtime_error("SDL texture loading error: Renderer not initialized");

    this->texture = IMG_LoadTexture(renderer, filePath);

    if (! this->texture)
        throw runtime_error("SDL texture loading error: "s.append(SDL_GetError()));
}

void CRenderWindow::CTexture::render(const int & srcX, const int & srcY,
                                     const int & tarX, const int & tarY, const int & tarH, const int & tarW) const
{
    SDL_Rect source;
    source.x = srcX;
    source.y = srcY;
    source.w = this->width;
    source.h = this->height;
    
    SDL_Rect target;
    target.x = tarX;
    target.y = tarY;
    target.w = tarH;
    target.h = tarW;

    SDL_RenderCopy(renderer, this->texture, &source, &target);
}
