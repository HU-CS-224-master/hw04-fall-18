/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
  and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>

#include <SDL2_image/SDL_image.h>

#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include"Rect.h"

struct MouseState {
  int x, y;
  bool mouse_clicked;
  MouseState() : x(0), y(0), mouse_clicked(false)
  {}
};

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//The window to render to and it renderer.
SDL_Window* WINDOW = NULL;
SDL_Renderer* RENDERER = NULL;

//Starts up SDL and creates window
bool init();

//Loads media
bool load_media();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* load_texture( std::string path );

bool init()
{
  // Fail if cannot Initialize SDL.
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    std::cout << "SDL could not initialize! SDL Error: "
	      << SDL_GetError() << "\n";
    return false;
  }
  // Warn if cannot set texture filtering to linear.
  if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    std::cout << "Warning: Linear texture filtering not enabled!\n";
  }
  // Fail if cannot create window.
  WINDOW = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
			     SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
			     SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
  if (WINDOW == NULL) {
    std::cout << "Window could not be created! SDL Error: "
	      << SDL_GetError() << "\n";
    return false;
  }
  // Fail if cannot create renderer for window.
  RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
  if (RENDERER == NULL) {
    std::cout << "Renderer could not be created! SDL Error: "
	      << SDL_GetError() << "\n";
    return false;
  }
  // Initialize renderer color
  SDL_SetRenderDrawColor(RENDERER, 0xFF, 0xFF, 0xFF, 0xFF);
  // Fail it cannot initialize PNG loading.
  int img_flags = IMG_INIT_PNG;
  if (!(IMG_Init(img_flags) & img_flags)) {
    std::cout << "SDL_image could not initialize! SDL_image Error: "
	      << IMG_GetError() << "\n";
    return false;
  }
  // All tests passed.
  return true;
}

bool load_media()
{
  // Loading success.
  bool success = true;

  // Nothing to load.
  return success;
}

void close()
{
  //Destroy window
  SDL_DestroyRenderer(RENDERER);
  SDL_DestroyWindow(WINDOW);
  WINDOW = NULL;
  RENDERER = NULL;
  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

SDL_Texture* load_texture(const std::string& path)
{
  //The final texture
  SDL_Texture* new_texture = NULL;
  //Load image at specified path
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (loaded_surface == NULL) {
    std::cout << "Unable to load image " << path.c_str()
	      << "! SDL_image Error: " << IMG_GetError() << "\n";
  }
  else {
    //Create texture from surface pixels
    new_texture = SDL_CreateTextureFromSurface(RENDERER, loaded_surface);
    if (new_texture == NULL) {
    std::cout << "Unable to create texture from " << path.c_str()
	      << "! SDL Error: " << SDL_GetError() << "\n";
    }
    //Get rid of old loaded surface
    SDL_FreeSurface(loaded_surface);
  }
  return new_texture;
}

void close_with_message(const std::string& message = "") {
  if (message != "") {
    std::cout << message << "\n";
  }
  close();
}

bool is_mouse_event(const SDL_Event& e)
{
  return e.type == SDL_MOUSEMOTION or e.type == SDL_MOUSEBUTTONDOWN or e.type == SDL_MOUSEBUTTONUP;
}

Rect* handle_mouse_event(const SDL_Event& e, MouseState& mouse_state,
			 SDL_Rect& fill_rect)
{
  int x, y;
  SDL_GetMouseState(&x, &y);
  Rect* rect_to_draw = NULL;
  if (e.type == SDL_MOUSEMOTION and mouse_state.mouse_clicked) {
    std::cout << "Mouse drag.\n";
    fill_rect.x = mouse_state.x;			    
    fill_rect.y = mouse_state.y;
    fill_rect.w = x - mouse_state.x;
    fill_rect.h = y - mouse_state.y;
  }
  else if (e.type == SDL_MOUSEBUTTONDOWN and not mouse_state.mouse_clicked) {
    std::cout << "Mouse button down.\n";
    mouse_state.mouse_clicked = true;
    mouse_state.x = x;
    mouse_state.y = y;
  }
  else if (e.type == SDL_MOUSEBUTTONUP) {
    std::cout << "Mouse button up.\n";
    mouse_state.mouse_clicked = false;
    rect_to_draw = new Rect(&fill_rect);
  }
  return rect_to_draw;
}

int main(int argc, char* args[])
{
  //Start up SDL and create window
  if (!init()) {
    close_with_message("Failed to initialize!");
    return 0;
  }
  //Load media
  if (!load_media()) {
    close_with_message("Failed to load media!");
    return 0;
  }
  // Set up main loop.
  bool quit = false;  //Main loop controller
  SDL_Event e;        //Event handler that takes care of all events
  MouseState mouse_state;
  SDL_Rect fill_rect;
  Rect* rect = NULL;
  // Enter main loop.
  while (!quit) {
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
      //User requests quit
      if (e.type == SDL_QUIT) {
	quit = true;
      }
      else if (is_mouse_event(e)) {
	rect = handle_mouse_event(e, mouse_state, fill_rect);
      }
    }
    //Clear screen, set up input rectangle if any, andd render.
    SDL_SetRenderDrawColor(RENDERER, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(RENDERER);
    if (rect) {
      SDL_SetRenderDrawColor(RENDERER, 0xFF, 0x00, 0x00, 0xFF);
      rect->draw(RENDERER, &fill_rect);
    }
    SDL_RenderPresent(RENDERER);
  }
  //Free resources, close SDL, adn exit.
  close();
  return 0;
}
