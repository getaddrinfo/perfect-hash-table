#define _PHT_DEBUG

#include <pht.h>
#include <phs.h>

#include <stdio.h>
#include <assert.h>

// we wrap these in do { ... } while(0) to not change semantic meaning of code
#define DEBUG_MSG(ty, msg) do { printf("test(%s): %s\n", ty, msg); } while(0)
#define DEBUG(ty, fmt, ...) do { printf("test(%s): " fmt "\n", ty, __VA_ARGS__); } while(0)
#define DEBUG_FINISH(ty) DEBUG_MSG(ty, "OK\n\n")

#define ARRAY_SIZE(it) sizeof(it)/sizeof(it[0])

char* items[] = {
    "test",
    "test2",
    "asdf",
    "fiojdsaajfjk",
    "sdoifjadsjkjfjsdaf",
    "wghatareyoua"
};

char* not_present_items[] = {
    "notreal",
    "alsonotreal",
    "wawawa",
    "jfjfjfj",
    "what is your jkfalk",
    "HahAHAHA"
};

char* mixed_items[] = {
    "test", 
    "notreal",
    "alsonotreal",
    "asdf",
    "wghatareyoua"
};

// this is a nice hack to make sure we always collide
// to text exhaustion
pht_item_t pht_item_dupes[] = {
    { .key = "test", .item = NULL },
    { .key = "test", .item = NULL }
};

pht_item_t pht_items[] = {
    { .key = "test", .item = (void*)"test"},
    { .key = "test2", .item = (void*)"test2"},
    { .key = "asdf", .item = (void*)"asdf"},
    { .key = "fiojdsaajfjk", .item = (void*)"fiojdsaajfjk"},
    { .key = "sdoifjadsjkjfjsdaf", .item = (void*)"sdoifjadsjkjfjsdaf"},
    { .key = "wghatareyoua", .item = (void*)"wghatareyoua" }
};

void test_pht() {
    DEBUG_MSG("pht", "creating");
    pht_t* tab = pht_create(
        pht_items,
        ARRAY_SIZE(pht_items),
        100000,
        20
    );

    if (tab == NULL) {
        DEBUG_MSG("pht", "tab alloc is null: fault");
        assert(0);
    }

    DEBUG_MSG("pht", "created");

    DEBUG_MSG("pht", "testing expected");

    for(int i = 0; i < ARRAY_SIZE(pht_items); i++) {
        void* item = pht_get(tab, pht_items[i].key);

        if (item == NULL) {
            DEBUG("pht", "get %s == NULL (expected != NULL)", pht_items[i].key);
            assert(0);
        } else {
            DEBUG("pht", "get %s != NULL (expected != NULL)", pht_items[i].key);
        }
    }

    for(int i = 0; i < ARRAY_SIZE(not_present_items); i++) {
        void* item = pht_get(tab, not_present_items[i]);

        if (item == NULL) {
            DEBUG("pht", "get %s == NULL (expected == NULL)", not_present_items[i]);
        } else {
            DEBUG("pht", "get %s != NULL (expected == NULL)", not_present_items[i]);
            assert(0);
        }
    }

    pht_destroy(tab);
    DEBUG_FINISH("pht");
}

void test_phs() {
    DEBUG_MSG("phs", "begin");
    phs_t* set = phs_create(items, ARRAY_SIZE(items), 100000, 2);

    if (set == NULL) {
        DEBUG_MSG("phs", "create failed");
        assert(0);
    }

    DEBUG_MSG("phs", "assert all expected items are present");
    for(int i = 0; i < ARRAY_SIZE(items); i++) {
        int exists = phs_has(set, items[i]);

        if (exists == 0) {
            DEBUG("phs", "get %s == 0 (expected 1)", items[i]);
            assert(0);
        } else {
            DEBUG("phs", "get %s == 1 (expected 1)", items[i]);
        }
    }
    
    DEBUG_MSG("phs", "assert non-expected items are not present");
    for(int i = 0; i < ARRAY_SIZE(not_present_items); i++) {
        int exists = phs_has(set, not_present_items[i]);

        if (exists == 1) {
            DEBUG("phs", "get %s == 1 (expected 0)", not_present_items[i]);
            assert(0);
        } else {
            DEBUG("phs", "get %s == 0 (expected 0)", not_present_items[i]);
        }
    }

    phs_destroy(set);
    DEBUG_FINISH("phs");
}

void test_phs_intersection() {
    DEBUG_MSG("phs/intersection", "begin");

    phs_t* left = phs_create(items, ARRAY_SIZE(items), 100000, 2);
    if (left == NULL) {
        DEBUG_MSG("phs/intersection", "left: create failed");
        assert(0);
    }

    phs_t* right = phs_create(items, ARRAY_SIZE(items), 100000, 2);
    if (right == NULL) {
        DEBUG_MSG("phs/intersection", "right: create failed");
        assert(0);
    }

    phs_comp_t diff = phs_intersection(left, right);
    for(int i = 0; i < diff.size; i++) {
        DEBUG("phs/intersection", "found %s in both", diff.keys[i]);
    }

    phs_comp_free(diff);
    phs_destroy(right);
    phs_destroy(left);

    DEBUG_FINISH("phs/intersection");
}

void test_phs_difference() {
    DEBUG_MSG("phs/difference", "begin");

    phs_t* current = phs_create(items, ARRAY_SIZE(items), 100000, 2);
    if (current == NULL) {
        DEBUG_MSG("phs/difference", "current: create failed");
        assert(0);
    }

    phs_t* other = phs_create(mixed_items, ARRAY_SIZE(mixed_items), 100000, 2);
    if (other == NULL) {
        DEBUG_MSG("phs/difference", "other: create failed");
        assert(0);
    }

    phs_comp_t diff = phs_difference(current, other);

    for(int i = 0; i < diff.size; i++) {
        DEBUG("phs/intersection", "found %s in current but not other", diff.keys[i]);
    }

    phs_comp_free(diff);
    phs_destroy(other);
    phs_destroy(current);

    DEBUG_FINISH("phs/difference");
}

void test_pht_exhaustion() {
    pht_t* tab = pht_create(
        pht_items,
        ARRAY_SIZE(pht_items),
        1,
        1
    );

    if (tab != NULL) {
        pht_destroy(tab);
        DEBUG_MSG("pht/exhaustion", "expected cannot find suitable perfect table, actually got a created table");
        assert(0);
        return;
    }

    DEBUG_MSG("pht/exhaustion", "suitable NULL return val");
    DEBUG_FINISH("pht/exhaustion");
}

void test_pht_always_collide_exhausts_primes() {
    pht_t* tab = pht_create(
        pht_item_dupes,
        ARRAY_SIZE(pht_item_dupes),
        20,
        100000 // make sure >= PRIME_LENGTH
    );

    if (tab != NULL) {
        pht_destroy(tab);
        DEBUG_MSG("pht/primes/exhaustion", "primes not properly exhausted");
        assert(0);
    }

    DEBUG_FINISH("pht/primes/exhaustion");
}

void test_pht_iterator() {
    pht_t* tab = pht_create(
        pht_items,
        ARRAY_SIZE(pht_items),
        100000,
        2
    );

    if (tab == NULL) {
        DEBUG_MSG("pht/iterator", "null tab (fail)");
        assert(0);
    }

    pht_iterator_t* iter = pht_iter(tab);

    if (iter == NULL) {
        DEBUG_MSG("pht/iter", "malloc fail (iter)");
        assert(0);
    }

    DEBUG_MSG("pht/iterator", "begin");

    while (pht_next(iter)) {
        DEBUG("pht/iterator", "key = %s, value = %p", iter->key, iter->item);
    }
    
    pht_iter_destroy(iter);
    pht_destroy(tab);

    DEBUG_FINISH("pht/iterator");
}

void test_phs_union_perfectly_different() {
    phs_t* left = phs_create(items, ARRAY_SIZE(items), 100000, 2);
    
    if (left == NULL) {
        DEBUG_MSG("phs/union/perfect", "left: null phs_create");
        assert(0);
    }

    phs_t* right = phs_create(not_present_items, ARRAY_SIZE(items), 100000, 2);

    if (right == NULL) {
        DEBUG_MSG("phs/union/perfect", "right: null phs_create");
        assert(0);
    }

    phs_comp_t union_ = phs_union(left, right);

    for(int i = 0; i < union_.size; i++) {
        DEBUG("phs/union/perfect", "found %s", union_.keys[i]);
    }

    DEBUG_FINISH("phs/union/perfect");
}

void test_phs_union_some_commonality() {
   phs_t* left = phs_create(items, ARRAY_SIZE(items), 100000, 2);
    
    if (left == NULL) {
        DEBUG_MSG("phs/union/common", "left: null phs_create");
        assert(0);
    }

    phs_t* right = phs_create(mixed_items, ARRAY_SIZE(mixed_items), 100000, 2);

    if (right == NULL) {
        DEBUG_MSG("phs/union/common", "right: null phs_create");
        assert(0);
    }

    phs_comp_t union_ = phs_union(left, right);

    for(int i = 0; i < union_.size; i++) {
        DEBUG("phs/union/common", "found %s", union_.keys[i]);
    }

    DEBUG_FINISH("phs/union/common");
}

int main() {
    test_pht();
    test_pht_exhaustion();
    test_pht_always_collide_exhausts_primes();
    test_pht_iterator();

    test_phs();
    test_phs_difference();
    test_phs_intersection();
    test_phs_union_perfectly_different();
    test_phs_union_some_commonality();

    return 0;
}