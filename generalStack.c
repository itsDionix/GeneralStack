/******************************************************************************
 *  Copyright (C) 2019 - Haohua Dong e Diogo Antunes
 *
 *  This file is a part of GeneralStack.
 *
 *  GeneralStack is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GeneralStack is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * DESCRIPTION
 *  A general implementation of a single type stack. The item's size must be
 *  specified at runtime for Initialization and an item comparison function must
 *  be supplied for Search.
 *
 *  Implementation details:
 *      The stack is implemented via a list of tables. When a table fills,
 *      another one is allocated and the new table's size grows linearly. The
 *      Stack always has at least the starting table allocated.
 *
 *****************************************************************************/

#include "generalStack.h"

#include <stdlib.h>
#include <string.h>

struct node {
  void *Items;       // Pointer to table of items
  struct node *next; // Pointer to next node.
};
struct _stack {
  struct node *head;        // List of tables
  unsigned int i;           // First empty space index
  unsigned int n;           // Current table size
  unsigned int itemSize;    // Size of each item to be stored.
  unsigned int initialSize; // Initial size of the Stack.
};
/* Description: Allocates a Stack object and initializes it with a table of the
 * specified size.
 * Arguments: The initial size of the stack in items, and the
 * size of each item in bytes.
 * Return: Pointer to the created Stack.
 * */
Stack *initStack(unsigned int initial_size, unsigned int item_size) {
  Stack *newSt;
  newSt = (Stack *)malloc(sizeof(Stack));
  if (newSt == NULL)
    exit(0);

  newSt->head = (struct node *)malloc(sizeof(struct node));
  if (newSt->head == NULL)
    exit(0);

  newSt->head->Items = malloc(initial_size * item_size);
  if (newSt->head->Items == NULL)
    exit(0);

  newSt->head->next = NULL;
  newSt->n = initial_size;
  newSt->initialSize = initial_size;
  newSt->itemSize = item_size;
  newSt->i = 0;
  return newSt;
};
/* Description: Returns 1 if the Stack is empty, 0 otherwise.
 * */
int isStackEmpty(Stack *stack) {
  return stack->i == 0 && stack->head->next == NULL;
}
/* Description: Returns 1 if the item already exists in the Stack, 0 otherwise.
 * Arguments:
 *  Stack *     - Pointer to Stack
 *  item        - Pointer to item to search for.
 *  max_depth   - Maximum number of items to check. -1 for no limit.
 *  equal       - Function used to compare the items, must be 0 for different
 *                items.
 * */
int itemExists(Stack *stack, void *item, int max_depth,
               int equal(void *, void *)) {
  struct node *node_ptr;
  int i, n;

  node_ptr = stack->head;
  n = stack->n;
  i = stack->i - 1; // i - 1 is the first occupied index
  while (node_ptr != NULL) {
    for (; i >= 0; i--) {
      if (max_depth == 0)
        return 0;
      if (max_depth > 0)
        max_depth--;
      if (equal(item, node_ptr->Items + i * stack->itemSize))
        return 1;
    }
    // Move to next table
    node_ptr = node_ptr->next;
    n = n - stack->initialSize;
    i = n - 1;
  }
  return 0;
}
/* Description: Frees a Stack object and its contents.
 * */
void freeStack(Stack *stack) {
  struct node *old;

  if (stack == NULL)
    exit(0);
  while (stack->head != NULL) {
    old = stack->head;
    stack->head = stack->head->next;
    free(old->Items);
    free(old);
  }
  free(stack);
}
/* Description: Copies an item to the top of the Stack. If the current table is
 * full, allocates one more with a linearly increasing size.
 * Arguments: Pointer to the Stack and pointer to the item to be copied.
 * */
void push(Stack *stack, void *item) {
  struct node *head, *new_node;
  unsigned int i, n, itemSize;

  if (stack == NULL)
    exit(0);

  // Using local variables for readability only, let the compiler micromanage
  head = stack->head;
  i = stack->i;
  n = stack->n;
  itemSize = stack->itemSize;

  if (i == n) {
    // Current table is full, allocate one more.
    n = n + stack->initialSize;
    // Check for overflow, since i == old n :
    if (i >= n)
      exit(0);

    new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node == NULL)
      exit(0);

    new_node->Items = malloc(n * itemSize);
    if (new_node->Items == NULL)
      exit(0);

    new_node->next = head;
    head = new_node;
    i = 0;
    // Update values since using local variables
    stack->head = head;
    stack->n = n;
  }

  memcpy(head->Items + i * itemSize, item, itemSize);
  i++;

  stack->i = i;
}
/* Description: Copies an item from the top of the Stack and deletes it from the
 * Stack. Only frees a table if a pop is called while it is empty. As a
 * consequence, the Stack always keeps at least the starting table in memory,
 * until it is freed.
 * Arguments: Pointer to the Stack and pointer with the destination address.
 * */
void pop(Stack *stack, void *dest) {
  struct node *old, *head;
  unsigned int i, n, itemSize;

  if (stack == NULL || isStackEmpty(stack))
    exit(0);

  // Using local variables for readability only, let the compiler micromanage
  head = stack->head;
  i = stack->i;
  n = stack->n;
  itemSize = stack->itemSize;

  if (i == 0) {
    // Current table is empty, free it.
    n = n - stack->initialSize;
    i = n;
    old = head;
    // Since Stack is not empty, if current table is empty then head->next is
    // the next table and is full
    head = head->next;
    free(old->Items);
    free(old);

    // Update values since using local variables
    stack->head = head;
    stack->n = n;
  }

  i--;
  memcpy(dest, head->Items + i * itemSize, itemSize);

  stack->i = i;
}
