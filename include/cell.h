#ifndef CELL_H
#define CELL_H

#include <main.h>

typedef struct Cell {
  Uint32 Color;
  Point location;
} Cell;

int init(SDL_Window*);

int addSand(Point);

void freeAllCells();

void calculatePhysics();

void drawInactiveSand();

void drawSand();

#endif
