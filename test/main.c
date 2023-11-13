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

int main() {
    test_pht();
    test_phs();
    test_pht_exhaustion();
    test_pht_always_collide_exhausts_primes();
    test_pht_iterator();
    
    return 0;
}