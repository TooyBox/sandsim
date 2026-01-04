#include <main.h>
#include <controller.h>

int main() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("Test", sp_width, sp_height, 0);

  int val = start(window);

  return val;
}
