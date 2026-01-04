#include <cell.h>
#include <list.h>

#define isStructEqual(one, two) (one.Color == two.Color && \
     one.location.x == two.location.x && \
     one.location.y == two.location.y)

// very shit list :D

Node *nodePool = {0};
bool *nodeUsed = {0};

bool initPool = false;

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


void freeList(List *l) {
  if (initPool) {
    free(nodePool);
    free(nodeUsed);
  }
}


Node* allocNode(List *l) {
  size_t i = l->count;
  while (true) {
    if (i == MAXPARTICLES) {
      i = 0;
    }
    
    if (!nodeUsed[i]) {
      nodeUsed[i] = true;
      nodePool[i].next = NULL;
      nodePool[i].previous = NULL;
      return &nodePool[i];
    }
    i++;
  }
  return NULL;
}


void freeNode(Node* n) {
  int index = n - nodePool;
  nodeUsed[index] = false;
  n->next = NULL;
  n->previous = NULL;
}


void listAppend(List* l, Cell d) {
  Node* node = allocNode(l);
  if (node == NULL) {
    printf("Failed: Malloc[append]\n");
    exit(EXIT_FAILURE);
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
    l->start->previous = node;
    node->next = l->start;
    l->start = node;
    l->count++;
  }
}


void listPrependOrder(List* l, Cell d) {
  Node* node = allocNode(l);
  if (node == NULL) {
    printf("Failed: Malloc[append]\n");
    exit(EXIT_FAILURE);
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


void listRemove(List *l, Node* n) {
  for (Node* current = l->start; current != NULL; current = current->next) {
    if (current == n) {
      if(l->start == l->end && l->start == n) {
        freeNode(n);
        l->start = NULL;
        l->end = NULL;
        l->count = 0;
        break;
      }
      else if (current == l->start) {
        Node *s = l->start->next;
        s->previous = NULL;
        freeNode(l->start);
        l->start = s;
        l->count--;
        break;
      }
      else if (current == l->end) {
         Node* p = l->end->previous; 
        p->next = NULL;
        freeNode(l->end);
        l->end = p;
        l->count--;
        break;
     }
      else {
        current->previous->next = current->next;
        current->next->previous = current->previous;
        freeNode(current);
        l->count--;  
        break;
      }
    }
  }
}


Node* listGet(List* l, Cell c) {
  for (Node* current = l->start; current != NULL; current = current->next) {
    if (isStructEqual(current->data, c)) {
      return current;
    }
  } 
  return NULL;
}
