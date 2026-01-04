#include <controller.h>
#include <cell.h>
#include <list.h>

int start(SDL_Window* window) {
  bool running = true;

  init(window);

  SDL_Event event;
  
  bool pressed = false;
  while(running) {
    while (SDL_PollEvent(&event)) {
      Point p = (Point) {event.motion.x, event.motion.y};      
      switch(event.type) {

        case SDL_EVENT_QUIT:
          running = false;
          freeAllCells();
          break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          addSand(p);
          pressed = true;
          break;

        case SDL_EVENT_MOUSE_MOTION:
          if (pressed) {
            addSand(p);
          }
          break;
      
        case SDL_EVENT_MOUSE_BUTTON_UP:
          pressed = false;
          break;

        case SDL_EVENT_KEY_DOWN:
          if (event.key.key == SDLK_E) {
            test();
          }
          break;

        default:
          break;

      }
    }
    calculatePhysics();
    drawSand();
    SDL_UpdateWindowSurface(window);
    SDL_Delay(FPS);
  }
  return EXIT_SUCCESS;
}
