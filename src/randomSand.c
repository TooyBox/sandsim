#include <cell.h>
#include <randomsand.h>
#include <stdlib.h>

/**
creates sand randomly at top of screen for fun!
**/

/**
Creates sand along the top of the screen
**/
void createRandomSand() {
  double x = 1 + (rand() % (sp_width - 1));
  double y = 1;

  Point p = (Point) {x, y};

  addSand(p);
}
