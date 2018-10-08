#pragma once

struct Point
{
  int x;
  int y;

  Point() : x(0), y(0)
  {}
  Point(int x_, int y_) : x(x_), y(y_)
  {}
  Point(const Point& other) : x(other.x), y(other.y)
  {}
};
