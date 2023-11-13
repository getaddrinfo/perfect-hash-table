# pht

A perfect hash table implementation in C. Not recommended for real-world usage.

Uses the FNV-32a algorithm on keys to calculate the hash. Uses prime numbers for the table size.

## TODO

- [ ] Maybe move to a `dst` pointer style api to allow returning error codes - there are multiple reasons it could fail (malloc, exhaustion, etc).
- [x] Iterators

## Usage
```c
#include <pht.h> // or however you do it

int main() {
    int a, b, c;
    
    a = 1;
    b = 2;
    c = 3;


    pht_items items[] = {
        { .key = "a key", .item = (void*)&a },
        { .key = "another key", .item = (void*)&b },
        { .key = "another another key", .item = (void*)&c }
    };

    // 3rd argument = number of cycles of primes to try
    // 4th argument = how many resizes to test before giving up
    //
    // the keys can be freed if you desire as pht_create strcpys them.
    pht_t* tab = pht_create(items, 3, 100000, 20);

    if (tab == NULL) {
        // failed to create (malloc, found no suitable table size + seed combo)
        return 1;
    }

    int* a_from_tab = (int*)pht_get(tab, "a key");
    void* unknown = pht_get(tab, "unknown key");

    printf("a_from_tab = %i\n", *a_from_tab); // a_from_tab = 1;
    printf("unknown = %p\n", unknown); // unknown = 0x0 (aka NULL)

    // cleanup memory
    pht_destroy(tab);

    // it is YOUR responsibility to clean up the items in the table

    return 0;
}
```

### Set

We also include a set built on top of `pht_t`. Currently only supports presence checks.

```c
#include <phs.h>

int main() {
    char* items[] = {
        "a key",
        "another key",
        "another another key"
    };

    phs_t* set = phs_create(items, 3, 100000, 20);

    if (set == NULL) {
        // failed to create (malloc, found no suitable table size + seed combo)
        return 1;
    }

    int a_key_exists = pht_has(set, "a key");
    int unknown_key_exists = pht_has(set, "unknown key");

    printf("a_key_exists = %i\n", exists); // a_key_exists = 1
    printf("unknown_key_exists = %i\n", unknown_key_exists); // unknown_key_exists = 0

    phs_destroy(set);

    return 0;
}
```