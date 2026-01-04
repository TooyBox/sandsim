#ifndef CELL_H
#define CELL_H

#include <main.h>

typedef struct Cell {
  Uint32 Color;
  Point location;
  Point oldLocation;
} Cell;

int init(SDL_Window*);

void clear();

void test();

int addSand(Point);

void freeAllCells();

void calculatePhysics();

void drawSand();

#endif
