#pragma once
#include <SDL2/SDL.h>
#include"Point.h"

/** Rect class **/

class Rect
{
private:
    SDL_Rect* m_fill_rect;
    //Color color;
    Point m_top_left;
    Point m_bottom_right;
public:
    Rect(SDL_Rect*);
    //Rect(const Point& topLeft, const Point& bottomRight, const Color&);
    void draw(SDL_Renderer*, SDL_Rect*);
};

