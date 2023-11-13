#ifndef _PHS_H
#define _PHS_H

#include <stdlib.h>
#include "pht.h"

typedef struct phs_t {
    pht_t* tab;
} phs_t;

typedef struct phs_comp_t {
    char** keys;
    size_t size;
} phs_comp_t;


phs_t* phs_create(
    char** items, 
    size_t num_items,
    int seeds_per_cycle,
    int max_attempts
);

int phs_has(phs_t* set, char* key);
void phs_destroy(phs_t* set);
phs_comp_t phs_intersection(phs_t* left, phs_t* right);
phs_comp_t phs_difference(phs_t* current, phs_t* other);
phs_comp_t phs_union(phs_t* left, phs_t* right);

void phs_comp_free(phs_comp_t comp);

#endif // _PHS_H