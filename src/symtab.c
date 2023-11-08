#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// structure for each element of hash table
typedef struct Node 
{
  char *name;
  void *info;
  struct Node *next;
} Node;

// control structure that holds the handle and the initial table size
typedef struct Tbl
{
  int tbl_size;
  struct Node **handle;
} Tbl;

typedef struct iterator
{
  Node **handle;
  int len;
  int lvl;
  Node *cur;
} Iterator;

// hash function that returns location to hash object to 
static unsigned int hash(const char *str)
{
 const unsigned int p = 16777619;
 unsigned int hash = 2166136261u;
 while (*str)
 {
 hash = (hash ^ *str) * p;
 str += 1;
 }
 hash += hash << 13;
 hash ^= hash >> 7;
 hash += hash << 3;
 hash ^= hash >> 17;
 hash += hash << 5;
 return hash;
}

// returns index of an object if present otherwise returns -1
static Node* lookup(Tbl *sym_tbl, const char *str)
{
  unsigned int lvl = hash(str) % sym_tbl->tbl_size;
  Node *temp_node = sym_tbl->handle[lvl];
  while (temp_node != NULL)
  {
    if (strcmp(temp_node->name, str) == 0)
      return temp_node;
    else
      temp_node = temp_node->next;
  }

  return NULL;
}

  // Creates a symbol table.
  // If successful, returns a handle for the new table.
  // If memory cannot be allocated for the table, returns NULL.
  // The parameter is a hint as to the expected number of (symbol, data)
  //   pairs to be stored in the table.
void *symtabCreate(int sizeHint)
{
  Tbl *hash_tbl = malloc(sizeof(Tbl));
  if (hash_tbl == NULL)
    return NULL;
  hash_tbl->tbl_size = sizeHint;
  hash_tbl->handle = malloc(sizeof(Node) * sizeHint);
  if (hash_tbl->handle == NULL)
  {
    free(hash_tbl);
    return NULL;
  }

  for (int i = 0; i < sizeHint; i++)
    hash_tbl->handle[i] = NULL;

  return hash_tbl;
}

  // Deletes a symbol table.
  // Reclaims all memory used by the table.
  // Note that the memory associate with data items is not reclaimed since
  //   the symbol table does not know the actual type of the data. It only
  //   manipulates pointers to the data.
  // Also note that no validation is made of the symbol table handle passed
  //   in. If not a valid handle, then the behavior is undefined (but
  //   probably bad).
void symtabDelete(void *symtabHandle)
{
  Tbl *sym_tbl = symtabHandle;
  for (int i = 0; i < sym_tbl->tbl_size; i++)
  {
    Node *n = sym_tbl->handle[i];
    while (n != NULL)
    {
      Node *next_n = n->next;
      free(n->name);
      free(n);
      n = next_n;
    }
  }
  free (sym_tbl->handle);
  free(sym_tbl);
}

  // Install a (symbol, data) pair in the table.
  // If the symbol is already installed in the table, then the data is
  //   overwritten.
  // If the symbol is not already installed, then space is allocated and
  //   a copy is made of the symbol, and the (symbol, data) pair is then
  //   installed in the table.
  // If successful, returns 1.
  // If memory cannot be allocated for a new symbol, then returns 0.
  // Note that no validation is made of the symbol table handle passed
  //   in. If not a valid handle, then the behavior is undefined (but
  //   probably bad).
int symtabInstall(void *symtabHandle, const char *symbol, void *data)
{
  Tbl *temp_handle = (Tbl*) symtabHandle;
  Node *temp_node = lookup(temp_handle, symbol);
  if (temp_node != NULL)
  {
    temp_node->info = data;
    return 1;
  }
  temp_node = malloc(sizeof(Node));
  if (temp_node == NULL)
    return 0;
  temp_node->name = malloc(strlen(symbol) + 1);
  strcpy(temp_node->name, symbol);
  if (temp_node->name == NULL)
    return 0;
  temp_node->info = data;
  unsigned int lvl = hash(symbol) % temp_handle->tbl_size;
  temp_node->next = temp_handle->handle[lvl];
  temp_handle->handle[lvl] = temp_node;

  return 1;
}

  // Return the data item stored with the given symbol.
  // If the symbol is found, return the associated data item.
  // If the symbol is not found, returns NULL.
  // Note that no validation is made of the symbol table handle passed
  //   in. If not a valid handle, then the behavior is undefined (but
  //   probably bad).
void *symtabLookup(void *symtabHandle, const char *symbol)
{
  Tbl *sym_tbl = (Tbl*) symtabHandle;
  Node *n = lookup(sym_tbl, symbol);
  if (n != NULL)
    return n->info;
  return NULL;
}
  
  // Create an iterator for the contents of the symbol table.
  // If successful, a handle to the iterator is returned which can be
  // repeatedly passed to symtabNext to iterate over the contents
  // of the table.
  // If memory cannot be allocated for the iterator, returns NULL.
  // Note that no validation is made of the symbol table handle passed
  //   in. If not a valid handle, then the behavior is undefined (but
  //   probably bad).
void *symtabCreateIterator(void *symtabHandle)
{
  Tbl *tbl = symtabHandle;
  Iterator *it = malloc(sizeof(Iterator));
  if (it == NULL)
    return NULL;

  it->handle = tbl->handle;
  it->len = tbl->tbl_size;
  it->lvl = 0;
  while (it->len > it->lvl)
  {
    if (it->handle[it->lvl] != NULL)
    {
      it->cur = it->handle[it->lvl];
      return it;
    }
    it->lvl++;
  }
  free(it);
  return NULL;
}

//   // Returns the next (symbol, data) pair for the iterator.
//   // The symbol is returned as the return value and the data item
//   // is placed in the location indicated by the second parameter.
//   // If the whole table has already been traversed then NULL is
//   //   returned and the location indicated by the second paramter
//   //   is not modified.
//   // Note that no validation is made of the iterator table handle passed
//   //   in. If not a valid handle, then the behavior is undefined (but
//   //   probably bad).
//   // Also note that if there has been a symbtabInstall call since the
//   //   iterator was created, the behavior is undefined (but probably
//   //   benign).
const char *symtabNext(void *iteratorHandle, void **returnData)
{
    Iterator *it = iteratorHandle;
    if (it->lvl == it->len)
      return NULL;
    const char * sym = it->cur->name;
    *returnData = it->cur->info;
    if (it->cur->next != NULL)
    {
      it->cur = it->cur->next;
      return sym;
    }
      
    it->lvl++;
    while (it->len > it->lvl)
    {
      if (it->handle[it->lvl] != NULL)
      {
        it->cur = it->handle[it->lvl];
        return sym;
      }
      it->lvl++;
    }

    return sym;
}
  
//   // Delete the iterator indicated by the only parameter.
//   // Reclaims all memory used by the iterator.
//   // Note that no validation is made of the iterator table handle passed
//   //   in. If not a valid handle, then the behavior is undefined (but
//   //   probably bad).
void symtabDeleteIterator(void *iteratorHandle) {free(iteratorHandle);}


