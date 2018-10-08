#include"Rect.h"

Rect::Rect(SDL_Rect* rect)
{
    this->m_fill_rect = rect;
}


/** Draws the rectangle **/
void Rect::draw(SDL_Renderer* renderer, SDL_Rect* fill_rect)
{
    SDL_RenderFillRect(renderer, fill_rect);
}

