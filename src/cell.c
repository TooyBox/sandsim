#include <main.h>
#include <cell.h>
#include <list.h>


List activeCells;

// occupied -> permanent location, where particles currently resting
bool occupied[sp_width][sp_height] = {0};
// tempOcc -> temp positions of moving particles
bool tempOcc[sp_width][sp_height] = {0};


#define gravity 1.0


#define numDirections 30
/* All directions to check, easy to change if wanted :D */
Point directions[numDirections] = {
    {2, 2}, {2, 1}, 
    {2, 0}, {2, -1}, {2, -2}, 
    {1, 2}, {1, 1}, 
    {1, 0}, {1, -1}, {1, -2}, 
    {0, 2}, {0, 1}, 
    {0, 0}, {0, -1}, {0, -2}, 
    {-1, 2}, {-1, 1}, 
    {-1, 0}, {-1, -1}, {-1, -2}, 
    {-2, 2}, {-2, 1}, 
    {-2, 0}, {-2, -1}, {-2, -2}
};


/* Sand can only move in three locations[P]:
   [ ] [S] [ ]
   [P] [P] [P]
*/
#define NUMCHECKS 3
Point check[NUMCHECKS] = {{0, gravity}, {gravity, gravity}, {gravity*-1, gravity}};


SDL_Window* window;

Uint8 colors[3] = {255, 125, 125};
int pos = 1; // index of colors
int sign = 1; // if we are adding or removing [125..255]

/**
Clears current state + screen
**/
void clear() {
  SDL_Surface* surface = SDL_GetWindowSurface(window); 

  colors[0] = 255;
  colors[1] = 125;
  colors[2] = 125;
  pos = 1;
  sign = 1;

  memset(occupied, 0, sizeof(occupied));
  memset(tempOcc, 0, sizeof(tempOcc));

  SDL_FillSurfaceRect(surface, NULL, 0x0);

  listClear(&activeCells);

  SDL_DestroySurface(surface);
}


/**
Initalizes the current cell context
@ Param: w: current window context
@ Return: success
**/
int init(SDL_Window* w) {
  window = w;
  listInit(&activeCells);
  return EXIT_SUCCESS;
}

/**
Used to free all cells
**/
void freeAllCells() {
  freeList(&activeCells);
}

/** 
Used to print, can press [e] to activate
**/
void test() {
  printf("size ActiveCells: %ld\n", activeCells.count);
}


/**
Creates [numDirections] particles of sand
@ Param p: central location which particles are created around
@ Return: success.
**/
int addSand(Point p) {
  #ifdef SPDEBUG
  printf("Starting to add sand around point (%f, %f)\n", p.x, p.y);
  #endif

  int i;
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  for (i = 0; i < numDirections; i++) {
    double w = p.x + directions[i].x;
    double h = p.y + directions[i].y;
    
    if (w >= sp_width || w < 0 || h >= sp_height || h < 0) {
      #ifdef SPDEBUG
      printf("Trying to place outside of bounds\n");
      #endif
      continue;
    }

    // Both cells have to be non-occupuied to create a sand particle
    if (!occupied[(int)w][(int)h] && !tempOcc[(int)w][(int)h]) {
        Uint32 color = SDL_MapSurfaceRGB(
            surface, 
            colors[0], 
            colors[1], 
            colors[2]);

        Cell newCell = (Cell) {color, (Point) {w, h}, (Point) {-1, -1}};
        listPrependOrder(&activeCells, newCell);
        tempOcc[(int)w][(int)h] = true;

        #ifdef SPDEBUG
        printf("list count: %ld\n", activeCells.count);
        #endif

        colors[pos] = colors[pos] + (sign * 1); 
      
        // Just changes values of colors according to a typical movement of a 
        // HSV slider
        if (colors[pos] == 255 && sign == 1) {
          switch(pos) {
            case 0:
              pos = 2;
              break;
            case 2:
              pos = 1;
              break;
            case 1:
              pos = 0;
              break;
          }
          sign = -1;
        }
        else if (colors[pos] == 125 && sign == -1) {
          switch(pos) {
            case 0:
              pos = 2;
              break;
            case 2:
              pos = 1;
              break;
            case 1:
              pos = 0;
              break;
          }
         sign = 1;
       }
    }
  }
  SDL_DestroySurface(surface);
  return EXIT_SUCCESS;
}

/**
Calculates next position of sand to be drawn
**/
void calculatePhysics() {
  if (activeCells.start == NULL) {
    return;
  }

  #ifdef SPDEBUG
  printf("Starting to calcuate Physics\n");
  #endif
  if (activeCells.count > 0) {
   Node* current = activeCells.start;
   
   while (current != NULL) {
      Node* next = current->next;

      double cx = current->data.location.x;
      double cy = current->data.location.y;

      bool canMove = false;

      for (int i = 0; i < NUMCHECKS; i++) {
        double w = cx + check[i].x;
        double h = cy + check[i].y;
        if (w >= sp_width || w < 0 || h >= sp_height || h < 0) {
          continue;
        }
        
        if (tempOcc[(int)w][(int)h]) { //If occupied, we cant move there, but we could next cycle
            // probably causes particles to fall in odd ways down slopes
            canMove = true;
            continue;
        }
        else if (!occupied[(int)w][(int)h] && !tempOcc[(int)w][(int)h]) {
            // if unoccupied and no falling sand in that location
            canMove = true;
            tempOcc[(int)cx][(int)cy] = false;

            current->data.oldLocation.x = cx;
            current->data.oldLocation.y = cy;

            tempOcc[(int)w][(int)h] = true;
            current->data.location.x = w;
            current->data.location.y = h;
            #ifdef SPDEBUG
            printf("Creating sand at (%f, %f)\n",
                 current->data.location.x,
                 current->data.location.y);
            #endif
            break;
        }
      }
      if (canMove == false) {
        listRemove(&activeCells, current);
        tempOcc[(int)cx][(int)cy] = false;
        occupied[(int)cx][(int)cy] = true;
      }
      current = next;
    }
  }
}


/**
Draws sand particles to screen, only draws active particles
Also causes previous position to be cleared
**/
void drawSand() {
  Node* current = NULL;
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  if (activeCells.start == NULL) {
    return;
  }
  for (current = activeCells.start; current != NULL; current = current->next) {
    SDL_Rect rect = {0};
    if (current->data.oldLocation.x > -1) { //fills old location with black
      rect = (SDL_Rect) {
          current->data.oldLocation.x,
          current->data.oldLocation.y,
          1, 1
      };
      SDL_FillSurfaceRect(surface, &rect, 0x0);
    }
   
    rect = (SDL_Rect) {
        current->data.location.x,
        current->data.location.y,
        1, 1
    };
    SDL_FillSurfaceRect(surface, &rect, current->data.Color);
  }
  SDL_DestroySurface(surface);
}
