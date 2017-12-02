#include "linked_list.h"
#include <stdlib.h>

void linked_list_add(LinkedNode *list, void *element) {
  LinkedNode *new, *current;

  /* Allocate new node */
  new = malloc(sizeof(LinkedNode));
  new->element = element;
  new->next = NULL;

  /* Find last node */
  current = list;
  while (current->next != NULL) {
    current = current->next;
  }

  /* Attach new node to the last node */
  current->next = new;
}

/* Only call remove on pre-freed (element) nodes */
/* Returns previous from found (for continuing iteration) */
LinkedNode *linked_list_remove(LinkedNode **list, LinkedNode *node) {
  LinkedNode *current, *found;

  if (node == *list) { /* If we need to remove the first node */
    *list = (*list)->next; /* Set the head to the second node */
  } else {
    current = *list;
    while (current->next != NULL && current->next != node) {
      current = current->next;
    }
    if (current->next != NULL) {  /* If node was found, set the previous */
      found = current->next;      /* node's pointer to the next of the node */
      current->next = found->next;
      free(found);
      return current;
    }
  }
  return NULL;
}
