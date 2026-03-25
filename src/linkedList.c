#include <cell.h>
#include <list.h>

#define isStructEqual(one, two) (one.Color == two.Color && one.location.x == two.location.x && one.location.y == two.location.y && one.oldLocation.x == two.oldLocation.x && one.oldLocation.y == two.oldLocation.y)

// very shit list :D

bool isOff = false;

/** Can optimize with commented code below **/
Node *nodePool = NULL;
bool *nodeUsed = NULL;

/**
Node freeStart = {0};
Node freeEnd = {0};
**/

//If called more than one time
bool initPool = false;


/** Potential other solution
void initNodePool() {
  nodePool = (Node*) malloc(sizeof(Node) * MAXPARTICLES);
  if (nodePool == NULL) {
    printf("Failed to malloc; too large\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < MAXPARTICLES; i++) {
    nodePool[i].next = &nodePool[i+1];
  }
}


void allocFreeNode() {
  
}
**/

/**
Initalizes a given list
@ Param l: List to initialize
**/
void listInit(List* l) {
  l->start = NULL;
  l->end = NULL;
  l->count = 0;
  if (!initPool) {
    nodePool = (Node*) malloc(sizeof(Node) * MAXPARTICLES);
    if (nodePool == NULL) {
      printf("Failed to malloc; too large\n");
      exit(EXIT_FAILURE);
    }
    nodeUsed = (bool*) malloc(sizeof(bool) * MAXPARTICLES);
     if (nodeUsed == NULL) {
      printf("Failed to malloc; too large\n");
      exit(EXIT_FAILURE);
    }
    initPool = true;
  }
}


void setOff() {
  isOff = true;
}


/**
Clears the given list [l] and nodePool.
@Param l: List to clear
**/
void listClear(List *l) {
  memset(nodePool, 0, sizeof(Node) * MAXPARTICLES);
  memset(nodeUsed, 0, sizeof(bool) * MAXPARTICLES);

  l->start = NULL;
  l->end = NULL;
  l->count = 0;
}


/**
Used to free a list, but currently just frees the node pool and nodeUsed
**/
void freeList(List *l) {
  if (initPool) {
    free(nodePool);
    free(nodeUsed);
  }
  l->start = NULL;
  l->end = NULL;
  l->count=0;
}

/**
Used to grab some free node from the nodePool
@Return: Returns address of node from nodePool to use
**/
Node* allocNode(List *l) {
  if (isOff) {
    return NULL;
  }
  size_t i = l->count;
  size_t checks = 0;
  while (checks < MAXPARTICLES) {
    i = i % MAXPARTICLES;
    
    if (!nodeUsed[i]) {
      nodeUsed[i] = true;
      memset(&nodePool[i], 0, sizeof(Node));
      nodePool[i].next = NULL;
      nodePool[i].previous = NULL;
      return &nodePool[i];
    }
    i++;
    checks+=1;
  }
  return NULL;
}


/**
Frees node n, frees up item in nodePool
@Param n: node to remove
**/
void freeNode(Node* n) {
  if (isOff) {
    return;
  }
  int index = n - nodePool;
  nodeUsed[index] = false;
  memset(n, 0, sizeof(Node));
  n->next = NULL;
  n->previous = NULL;
}


/**
Appends item d to list l
@Param l: List to add to
@Param d: item to add to list
**/
void listAppend(List* l, Cell d) {
  if (isOff) {
    return;
  }

  Node* node = allocNode(l);
  if (node == NULL) {
    printf("Failed: Malloc[append]\n");
    exit(EXIT_FAILURE);
  }
 
  if (l->count >= MAXPARTICLES) {
    node = NULL;
    return;
  }

  node->next = NULL;
  node->previous = NULL;
  node->data = d;

  if (l->start == NULL || l->end == NULL) {
    l->start = node;
    l->end = node;
    l->count = 1;
  }
  else {
    l->end->next = node;
    node->previous = l->end;
    l->end = node;
    l->count++;
  }
}


/**
Adds item from from of list to back
Adds them in order of largest y -> smallest (also from largest x -> smallest)
@Param l: list to add to
@Param d: Cell to add to list
void listPrependOrder(List* l, Cell d) {
  if (isOff) {
    return;
  }
  Node* node = allocNode(l);
  if (node == NULL) {
    printf("Failed: Malloc[append]\n");
    exit(EXIT_FAILURE);
  }
  if (l->count >= MAXPARTICLES) {
    node = NULL;
    return;
  }
  node->data = d;
  node->next = NULL;
  node->previous = NULL;

  if (l->start == NULL) {
    l->start = node;
    l->end = node;
    l->count = 1;
    return;
  }


  Node* current = l->start;
  while (current != NULL) {
      if (d.location.y >= current->data.location.y) {
        if (d.location.x >= current->data.location.x) {
          node->next = current;
          node->previous = current->previous;
          if (current->previous) {
            current->previous->next = node;
          }
          else {
            l->start = node;
          }
          current->previous = node;
          l->count++;
          return;
        }
      }
      current = current->next; 
    }

  node->previous = l->end;
  l->end->next = node;
  l->end = node;
  l->count++;
}
**/

/**
Removes item n from list l
@Param l: list to remove from
@Param n: node to remove from list
**/
void listRemove(List *l, Node* n) {
  if (isOff) {
    return;
  }
  for (Node* current = l->start; current != NULL; current = current->next) {
    if (current == n) {
      if (l->start == l->end && l->start == n) {
        l->start = NULL;
        l->end = NULL;
      }
      else if (n == l->start) {
        l->start = n->next;
        if (l->start) {
          l->start->previous = NULL;
        }
      }
      else if (n == l->end) {
        l->end = n->previous;
        if (l->end) {
          l->end->next = NULL;
        }
      }
      else {
        if (n->previous) {
          n->previous->next = n->next;
        }
        if (n->next) {
          n->next->previous = n->previous;
        }
      }

      freeNode(n);  // return to pool
      l->count--;
      break;
    }
  }
}

/**
Returns node that contains item c
@Param l: list to search
@Param c: item to find
@Return: Node that contains item c
**/
Node* listGet(List* l, Cell c) {
  if (isOff) {
    return NULL;
  }
  for (Node* current = l->start; current != NULL; current = current->next) {
    if (isStructEqual(current->data, c)) {
      return current;
    }
  } 
  return NULL;
}
