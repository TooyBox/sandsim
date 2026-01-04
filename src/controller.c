#include <controller.h>
#include <cell.h>
#include <list.h>
#include <randomsand.h>

enum state {def, drawing, random};
enum state currentState = def;

/**
Used to handle multiple mouse events easily + with proper drawing
@Param event: current event to check
**/
void mouseEvent(SDL_Event event) {
  switch (event.type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      currentState = drawing;
      break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      currentState = def;
      break;
    default:
      break;
  }
  
  Point p = (Point) {event.motion.x, event.motion.y}; 
  switch (currentState) {
    case drawing:
      addSand(p);
      break;

    default:
      break;
  }
}



/**
Used to run programs control
@ Param: window -> current window context of program
@ Return: success/failure
**/
int start(SDL_Window* window) {
  bool running = true;

  init(window);

  SDL_Event event;
  
  while(running) {
    while (SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_EVENT_QUIT:
          running = false;
          freeAllCells();
          break;

        case SDL_EVENT_KEY_DOWN:
          if (event.key.key == SDLK_E) {
            if (currentState == random) {
              currentState = def;
            } 
            else {
              currentState = random;
            }
          }
          else if (event.key.key == SDLK_C) {
            clear();
          }
          break;

        default:
          break;

      }
      if (event.type >= 0x400 && event.type < 0x600) {
        mouseEvent(event);
      }
     
    }
    if (currentState == random) {
      createRandomSand();
    }

    calculatePhysics();
    drawSand();
    SDL_UpdateWindowSurface(window);
  }
  return EXIT_SUCCESS;
}
