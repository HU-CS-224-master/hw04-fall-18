#pragma once

struct Color
{
  int red;
  int green;
  int blue;

  Color() : red(255), green(255), blue(255)
  {}
  Color(int red_, int green_, int blue_) : red(red_), green(green_), blue(blue_)
  {}
};
