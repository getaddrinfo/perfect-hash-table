#ifndef _PHS_H
#define _PHS_H

#include <stdlib.h>
#include "pht.h"

typedef struct phs_t {
    pht_t* tab;
} phs_t;


phs_t* phs_create(
    char** items, 
    size_t num_items,
    int seeds_per_cycle,
    int max_attempts
);

int phs_has(phs_t* set, char* key);
void phs_destroy(phs_t* set);

#endif // _PHS_H