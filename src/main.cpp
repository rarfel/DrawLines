#include "headers/window.h"

int main(int argc, char *argv[])
{
  SDLWindowState state;
  state.width = 800;
  state.height = 600;

  InitSDL();

  CreateWindowAndRenderer("Draw Lines", SDL_WINDOW_RESIZABLE, state);

  glm::vec4 backgroundColor = {0.0, 0.0, 0.0, 1.0};
  glm::vec4 pixelColor = {1.0, 1.0, 1.0, 1.0};
  glm::vec4 lineColor = {1.0, 0.0, 1.0, 1.0};

  Pixel pixel[6];
  size_t sizePixels = sizeof(pixel)/sizeof(pixel[0]);

  LoopHandler(state, backgroundColor, pixel, sizePixels, pixelColor, lineColor);
}
