#include "headers/window.h"

struct SDLWindowState;

int InitSDL()
{
  if(!SDL_Init(SDL_INIT_VIDEO))
    {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error when initializing SDL3", nullptr);
      return 1;
    }
  return 0;
}

int CreateWindowAndRenderer(const char *title, SDL_WindowFlags flags, SDLWindowState &state)
{
  if (!SDL_CreateWindowAndRenderer(title, state.width, state.height, flags, &state.window, &state.renderer))
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
      return 1;
    }
  return 0;
}

void LoopHandler(SDLWindowState state, glm::vec4 backgroundColor, Pixel pixel[], size_t sizePixels, glm::vec4 pixelColor, glm::vec4 lineColor)
{
  bool running = true;
  bool tesselation = false;
  while(running)
  {
    SDL_Event event { 0 };
    while (SDL_PollEvent(&event))
    {
      running = EventHandler(&state, event, pixel, sizePixels, tesselation);
    }
    DrawBackground(state, backgroundColor);

    DrawPixelAndLine(state, pixel, sizePixels, pixelColor, lineColor, tesselation);

    //swap buffers and show to screen
    SDL_RenderPresent(state.renderer);
  }
  CleanUp(state);
}

void DrawLine(SDL_Renderer *renderer, Pixel &pixel1, Pixel &pixel2, glm::vec4 lineColor)
{
  float x1 = pixel1.pixel.x + pixel1.pixel.w/2;
  float x2 = pixel2.pixel.x + pixel2.pixel.w/2;

  float y1 = pixel1.pixel.y + pixel1.pixel.h/2;
  float y2 = pixel2.pixel.y + pixel2.pixel.h/2;

  SDL_SetRenderDrawColorFloat(renderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);
  SDL_RenderLine(renderer, x1, y1, x2, y2);
  pixel1.lineDraw = true;
  pixel2.lineDraw = true;
}

void DrawPixel(SDL_Renderer *renderer, Pixel &pixel, glm::vec4 pixelColor)
{
  SDL_SetRenderDrawColorFloat(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
  SDL_RenderRect(renderer, &pixel.pixel);
}

void DrawPixelAndLine(SDLWindowState state, Pixel pixel[], size_t sizePixels, glm::vec4 pixelColor, glm::vec4 lineColor ,bool &drawAllLines)
{
  for (int i = 0; i < sizePixels; i++)
  {
    if(pixel[i].active)
    {
      DrawPixel(state.renderer, pixel[i], pixelColor);

      if(pixel[i+1].active)
      {
        DrawLine(state.renderer, pixel[i], pixel[i+1], lineColor);
      }else if(i+1 >= sizePixels)
      {
        DrawLine(state.renderer, pixel[0], pixel[sizePixels - 1], lineColor);
      }
    }
    if(drawAllLines && pixel[sizePixels - 1].active)
    {
      DrawLine(state.renderer, pixel[0], pixel[i], lineColor);
    }
  }
}

void TogglePixel(SDL_Event &event, Pixel &pixel, bool toggle)
{
  if(toggle){
    pixel.pixel.x = event.button.x - pixel.pixel.w / 2;
    pixel.pixel.y = event.button.y - pixel.pixel.h / 2;
    pixel.lineDraw = !toggle;
  }
  pixel.active = toggle;
}

void MouseInputHandler(SDL_Event &event, Pixel pixel[], size_t sizePixels, bool &drawAllLines)
{
  switch (event.button.button)
  {
    case MouseButtons::leftClick:
      for(int i = 0; i < sizePixels; i++)
        if(!pixel[i].active)
        {
          TogglePixel(event, pixel[i], true);
          break;
        }
    break;

    case MouseButtons::middleClick:
      drawAllLines = drawAllLines ? false : true;
    break;

    case MouseButtons::rightClick:
      for(int i = sizePixels; i >= 0; i--)
        if(pixel[i].active)
        {
          TogglePixel(event, pixel[i], false);
          break;
        }
    break;
  }
}

bool EventHandler(SDLWindowState *state, SDL_Event &event, Pixel pixel[], size_t sizePixels, bool &drawAllLines)
{
  switch (event.type)
  {
    case SDL_EVENT_QUIT:
    {
      return false;
      break;
    }
    case SDL_EVENT_WINDOW_RESIZED:
    {
      float nWidht = 0;
      float nHeight = 0;
      float newWindowWidth = event.window.data1;
      float newWindowHeight = event.window.data2;

      nWidht = newWindowWidth/state->width;
      nHeight = newWindowHeight/state->height;

      for(int i = 0; i < sizePixels; i++)
      {
        pixel[i].pixel.x = (pixel[i].pixel.x * nWidht);
        pixel[i].pixel.y = (pixel[i].pixel.y * nHeight);
      }
      state->width = newWindowWidth;
      state->height = newWindowHeight;
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
      MouseInputHandler(event, pixel, sizePixels, drawAllLines);
    }
  }
  return true;
}

void DrawBackground(SDLWindowState state, glm::vec4 backgroundColor)
{
  SDL_SetRenderDrawColorFloat(state.renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
  SDL_RenderClear(state.renderer);
 }

void CleanUp(SDLWindowState &state)
{
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);
  SDL_Quit();
}
