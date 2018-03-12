// Adapted from https://lispmachine.wordpress.com/2009/05/25/a-lifo-in-c-using-singly-linked-list/
// because I'm lazy and didn't want to put all the thought into it.

#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 1025
#define LIFO_ERROR 1
#define LIFO_SUCCESS 0
#define LIFO_LENGTH_ERROR 2
#define LIFO_PTR_ERROR NULL;

typedef struct node {
  char array[MAX_NAME_LENGTH];
  struct node * next;
} node;

typedef struct lifo {
  node * head;
  node * tail;
  int num_items;
  int max_items;
} lifo;

int push(lifo* list, const char* input);
int pop(lifo* list, char * output);
int is_empty(lifo* list);
int is_full(lifo* list);

lifo * make_lifo(int size);
int destroy_lifo(lifo * list);
