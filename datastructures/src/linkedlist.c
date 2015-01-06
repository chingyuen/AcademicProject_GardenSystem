#include <stdio.h>

enum {
  CHAR,
  INT,
  FLOAT,
  DOUBLE,
} TYPE;

typedef struct node {
  void *item;
} node_t;

typedef struct list {
  void *head;
  void *next;
} linked_list_t;

