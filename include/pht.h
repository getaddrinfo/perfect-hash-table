#ifndef _PHT_H
#define _PHT_H

#ifndef _PHT_DEBUG
    #define PHT_DEBUG(msg, ...)
#else
    #define PHT_DEBUG(fmt, ...) do { printf("pht: " fmt "\n", __VA_ARGS__); } while(0)
#endif
#define PHT_DEBUG_MSG(msg) PHT_DEBUG("%s", msg)


#include <stdlib.h>

typedef struct {
    char* key;
    void* item;
} pht_item_t;

typedef struct {
    pht_item_t* items;
    int _seed;

    int size;
    int cap;
} pht_t;

typedef struct {
    char* key;
    void* item;

    pht_t* _table;
    int _curr;
} pht_iterator_t;

pht_t* pht_create(pht_item_t* items, size_t num_items, int cycles, int max_attempts);

// gets an item from the perfect hash table
void* pht_get(pht_t* table, char* key);

// destroys the table and cleans up memory
void pht_destroy(pht_t* table);

// destroys an iterator
void pht_iter_destroy(pht_iterator_t* iter);


// creates an iterator over the table
pht_iterator_t* pht_iter(pht_t* table);

// returns true if there is a next item,
// the passed iterator is updated to contain
// the necessary values
int pht_next(pht_iterator_t* iter);

#endif // _PHT_H