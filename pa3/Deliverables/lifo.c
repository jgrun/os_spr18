#include "lifo.h"

#include <stdio.h>

int push(lifo* list, const char* input) {
  if(is_full(list)) {
    return LIFO_ERROR;
  }
  node * n = (node*) malloc(sizeof(node));
  if(n == NULL) {
    return LIFO_ERROR;
  }
  int length = strlen(input);
  if(length >= MAX_NAME_LENGTH) {
    return LIFO_LENGTH_ERROR;
  }
  int i;
  for(i = 0; i < length; i++) {
    n->array[i] = input[i];
  }
  n->next = NULL;
  if(is_empty(list)) {
    list->head = n;
    list->tail = n;
    list->num_items++;
  }
  else {
    list->tail->next = n;
    list->tail = n;
    list->num_items++;
  }
  return LIFO_SUCCESS;
}

int pop(lifo* list, char * output) {
  if(is_empty(list)) {
    return LIFO_ERROR;
  }
  int length = strlen(list->head->array);
  int i;
  for(i = 0; i < length; i++) {
    output[i] = list->head->array[i];
  }
  node * n = list->head;
  list->head = n->next;
  free(n);
  list->num_items--;
  return i;
}

int is_empty(lifo* list) {
  if(list->num_items == 0) {
    return LIFO_ERROR;
  }
  return LIFO_SUCCESS;
}

int is_full(lifo* list) {
  if(list->num_items == list->max_items) {
    return LIFO_ERROR;
  }
  return LIFO_SUCCESS;
}

lifo * make_lifo(int size) {
  lifo * list = (lifo*) malloc(sizeof(lifo));
  if(list == NULL) {
    return LIFO_PTR_ERROR;
  }
  list->max_items = size;
  list->num_items = 0;
  list->head = NULL;
  list->tail = NULL;
  return list;
}

int destroy_lifo(lifo * list) {
  char output[MAX_NAME_LENGTH];
  while(!(is_empty(list))) {
    pop(list, output);
  }
  free(list);
  return LIFO_SUCCESS;
}
