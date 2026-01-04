#ifndef LIST_H
#define LIST_H

#define MAXPARTICLES (sp_width * sp_height)

#include <cell.h>

typedef struct Node {
  struct Node* next;
  struct Node* previous;
  Cell  data;
} Node;

typedef struct List {
  Node* start;
  Node* end;
  size_t count;
} List;

void listInit();

void freeList(List*);

void listClear(List*);

void listAppend(List*, Cell);

void listPrependOrder(List*, Cell);

void listRemove(List*, Node*);

Node* listGet(List*, Cell);

#endif
