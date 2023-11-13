#include <phs.h>

#include <stdio.h>

phs_t* phs_create(
    char** items,
    size_t num_items,
    int seeds_per_cycle,
    int max_attempts
) {
    pht_item_t* hm_items = calloc(num_items, sizeof(pht_item_t));

    if (hm_items == NULL) {
        return NULL;
    }

    int i;
    for(i = 0; i < num_items; i++) {
        // safety: pht_create does strcpy on this
        hm_items[i].key = items[i];
        hm_items[i].item = items[i];
    }

    pht_t* real_tab = pht_create(
        hm_items,
        num_items,
        seeds_per_cycle,
        max_attempts
    );

    if (real_tab == NULL) {
        free(hm_items);
        return NULL;
    }

    phs_t* set = malloc(sizeof(phs_t));
    if (set == NULL) {
        pht_destroy(real_tab);
        free(hm_items);
        return NULL;
    }
    
    set->tab = real_tab;
    return set;
}

int phs_has(phs_t* set, char* key) {
    if(pht_get(set->tab, key) != NULL) return 1;
    return 0;
}

void phs_destroy(phs_t* set) {
    pht_destroy(set->tab);
    free(set);
}