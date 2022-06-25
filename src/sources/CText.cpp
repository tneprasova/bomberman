#include "CRenderWindow.hpp"

CRenderWindow::CText::CText()
: font(nullptr),
  color({0,0,0,0}),
  textTexture(nullptr)
{}

CRenderWindow::CText::CText(const char * filePath, const SDL_Color & color, const int & size)
: font(nullptr),
  color(color),
  textTexture(nullptr),
  oldText("")
{
    using namespace std;

    this->font = TTF_OpenFont(filePath, size);
    if (! this->font)
        throw runtime_error("TTF load error: "s.append(TTF_GetError()));
}

void CRenderWindow::CText::renderNew(const std::string & text)
{
    using namespace std;

    if (this->textTexture)
        SDL_DestroyTexture(this->textTexture);

    SDL_Surface * surface = TTF_RenderText_Shaded(this->font, text.c_str(), this->color, {0,0,0,100});
    if (! surface)
        throw runtime_error("TTF text rendering from surface error: "s.append(TTF_GetError()));

    this->textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    if (! this->textTexture)
        throw runtime_error("TTF texture creation from a surface error: "s.append(TTF_GetError()));

    SDL_FreeSurface(surface);
    this->oldText = text;
}

void CRenderWindow::CText::render(const std::string & text, const std::pair<int,int> & position,
                                  const int & width, const int & height)
{
    // Creates a new texture, when the text changes
    if (this->oldText != text)
        renderNew(text);

    // Sets up the position on screen where the text will render
    SDL_Rect dest;
    dest.x = position.first;
    dest.y = position.second;
    dest.w = text.size() * width;
    dest.h = height;

    SDL_RenderCopy(renderer, textTexture, NULL, &dest);
}