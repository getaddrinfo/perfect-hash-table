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

// for this one, we can optimise by looking at the
// smaller set
phs_comp_t phs_intersection(phs_t* left, phs_t* right) {
    phs_t* smaller = left->tab->size > right->tab->size ? right : left;
    phs_t* larger = smaller == left ? right : left;

    phs_comp_t comp = {
        .keys = NULL,
        .size = 0
    };

    char** keys = calloc(smaller->tab->size, sizeof(char*));

    if (keys == NULL) {
        return comp;
    }

    pht_iterator_t* iter = pht_iter(smaller->tab);

    int i = 0;

    while (pht_next(iter)) {
        int present = phs_has(larger, iter->key);
        
        if (present == 1) {
            PHT_DEBUG("found %s in left and right", iter->key);
            keys[i++] = iter->key;
        }
    }

    pht_iter_destroy(iter);

    comp.keys = keys;
    comp.size = i++;

    return comp;
}

// we can't look at smaller set, must look at left
phs_comp_t phs_difference(phs_t* current, phs_t* other) {
    pht_iterator_t* iterator = pht_iter(current->tab);

    phs_comp_t comp = {
        .keys = NULL,
        .size = 0
    };

    char** keys = calloc(current->tab->size, sizeof(char*));

    if (keys == NULL) {
        return comp;
    }    

    int i = 0;

    while (pht_next(iterator)) {
        int present = phs_has(other, iterator->key);

        if (present == 0) {
            PHT_DEBUG("found %s in current but not other", iterator->key);
            keys[i++] = iterator->key;
        }
    }

    pht_iter_destroy(iterator);
    
    comp.keys = keys;
    comp.size = i++;

    return comp;
}

phs_comp_t phs_union(phs_t* left, phs_t* right) {
    phs_t* smaller = left->tab->size > right->tab->size ? right : left;
    phs_t* larger = left == smaller ? right : left;

    phs_comp_t comp = {
        .keys = NULL,
        .size = 0
    };

    char** keys = calloc(
        smaller->tab->size + larger->tab->size,
        sizeof(char*)
    );

    if (keys == NULL) {
        return comp;
    }

    // we want to check in the smaller tab, because then 
    // we are doing less checks (and therefore less hashes)
    pht_iterator_t* primary = pht_iter(larger->tab);
    pht_iterator_t* secondary = pht_iter(smaller->tab);

    int i = 0;

    while (pht_next(primary)) {
        keys[i++] = primary->key;
    }

    pht_iter_destroy(primary);

    while (pht_next(secondary)) {
        int is_already_present_in_larger = phs_has(larger, secondary->key);

        if (is_already_present_in_larger == 0) {
            keys[i++] = secondary->key;
        }
    }

    pht_iter_destroy(secondary);

    comp.keys = keys;
    comp.size = i++;
    
    return comp;
}

void phs_destroy(phs_t* set) {
    pht_destroy(set->tab);
    free(set);
}

void phs_comp_free(phs_comp_t comp) {
    free(comp.keys);
}