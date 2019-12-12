# GeneralStack
## Description:
A general implementation of a single type stack. The item's size must be
specified at runtime for Initialization and an item comparison function must
be supplied for Search.  

## Implementation details:
The stack is implemented via a list of tables. When a table fills,
another one is allocated and the new table's size grows linearly. The
Stack always has at least the starting table allocated.
  
## Function list:
### Initialization & Termination
- initStack  
- freeStack  
### Properties
- isStackEmpty

### Search
- itemExists

### Insertion & Removal
- push
- pop

## Dependencies:
- stdlib.h
- string.h

