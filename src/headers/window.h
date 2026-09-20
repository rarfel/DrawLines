#pragma once
#include "SDL3/SDL.h"
#include "glm/glm.hpp"
#include <cstddef>

struct SDLWindowState
{
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  float width = -1;
  float height = -1;
};

struct Pixel{
  SDL_FRect pixel = {-1,-1,5,5};
  bool active = false;
  bool lineDraw = false;
};

enum MouseButtons{
  leftClick = 1,
  middleClick = 2,
  rightClick = 3
};

int InitSDL();

int CreateWindowAndRenderer(const char *title, SDL_WindowFlags flags, SDLWindowState &state);

void LoopHandler(SDLWindowState state, glm::vec4 backgroundColor, Pixel pixel[], size_t sizePixels, glm::vec4 pixelColor, glm::vec4 lineColor);

void DrawPixelAndLine(SDLWindowState state, Pixel pixel[], size_t sizePixels, glm::vec4 pixelColor, glm::vec4 lineColor ,bool &drawAllLines);

bool EventHandler(SDLWindowState *state, SDL_Event &event, Pixel pixel[], size_t sizePixels, bool &drawAllLines);

void DrawBackground(SDLWindowState state, glm::vec4 backgroundColor);

void CleanUp(SDLWindowState &state);
