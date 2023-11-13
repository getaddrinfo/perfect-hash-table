#include <stdlib.h>
#include <string.h>
#include <pht.h>
#include <phtfnv.h>

#include <stdio.h>

// only valid for non pointer arrays (need to know the size of the array itself)
#define ARRAY_SIZE(it) sizeof(it) / sizeof(it[0])

const int PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
    547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
    877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,
    1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583,
    1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
    1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129, 2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357,
    2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741,
    2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903, 2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181,
    3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413, 3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571,
    3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643, 3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907, 3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989,
    4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057, 4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139, 4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231, 4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297, 4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409,
    4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493, 4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583, 4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657, 4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751, 4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831,
    4861, 4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937, 4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003, 5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087, 5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179, 5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279,
    5281, 5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443, 5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521, 5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639, 5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693,
    5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791, 5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857, 5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953, 5981, 5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133,
    6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301, 6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473, 6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571,
    6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673, 6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761, 6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833, 6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997,
    7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103, 7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207, 7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411, 7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499,
    7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723, 7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829, 7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919,
    7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009, 8011, 8017, 8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111, 8117, 8123, 8147, 8161, 8167, 8171, 8179, 8191, 8209, 8219, 8221, 8231, 8233, 8237, 8243, 8263, 8269, 8273, 8287, 8291, 8293, 8297, 8311, 8317, 8329, 8353, 8363, 8369, 8377, 8387,
    8389, 8419, 8423, 8429, 8431, 8443, 8447, 8461, 8467, 8501, 8513, 8521, 8527, 8537, 8539, 8543, 8563, 8573, 8581, 8597, 8599, 8609, 8623, 8627, 8629, 8641, 8647, 8663, 8669, 8677, 8681, 8689, 8693, 8699, 8707, 8713, 8719, 8731, 8737, 8741, 8747, 8753, 8761, 8779, 8783, 8803, 8807, 8819, 8821, 8831,
    8837, 8839, 8849, 8861, 8863, 8867, 8887, 8893, 8923, 8929, 8933, 8941, 8951, 8963, 8969, 8971, 8999, 9001, 9007, 9011, 9013, 9029, 9041, 9043, 9049, 9059, 9067, 9091, 9103, 9109, 9127, 9133, 9137, 9151, 9157, 9161, 9173, 9181, 9187, 9199, 9203, 9209, 9221, 9227, 9239, 9241, 9257, 9277, 9281, 9283,
    9293, 9311, 9319, 9323, 9337, 9341, 9343, 9349, 9371, 9377, 9391, 9397, 9403, 9413, 9419, 9421, 9431, 9433, 9437, 9439, 9461, 9463, 9467, 9473, 9479, 9491, 9497, 9511, 9521, 9533, 9539, 9547, 9551, 9587, 9601, 9613, 9619, 9623, 9629, 9631, 9643, 9649, 9661, 9677, 9679, 9689, 9697, 9719, 9721, 9733,
    9739, 9743, 9749, 9767, 9769, 9781, 9787, 9791, 9803, 9811, 9817, 9829, 9833, 9839, 9851, 9857, 9859, 9871, 9883, 9887, 9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973
};

const int PRIME_LENGTH = ARRAY_SIZE(PRIMES);

int _find_next_prime_gt_naive(int current);
void _zero_array(int *arr, size_t size);

typedef struct {
    int offset;
    int flag;
} flag_access;

flag_access flag_array_offset(int cycle) {
    flag_access loc;
    
    loc.offset = cycle / sizeof(int);
    loc.flag = cycle & (sizeof(int) - 1); // assuming sizeof(int) is a power of 2

    return loc;
}

// TODO: fix mem leak in calloc slots (int*)
pht_t *pht_create(
    pht_item_t items[],
    size_t num_elems,
    int seeds_per_cycle,
    int max_attempts)
{
    // curr_prime is the current prime number we are using;
    // attempt is the number of resizes of the table we have tried, if it exceeds max_attempts we return NULL
    // seed_cycle is the current seed being tested
    // perfect (0 or 1) represents if we have found a perfect seed + size combo
    //
    // higher seeds_per_cycle -> more likely to find a smaller table, but it takes more time to compute
    // higher attempts (given smaller seeds_per_cycle) -> more likely to find a table faster, but more likely to use more memory than necessary

    int curr_prime, max_prime, attempt, seed_cycle, perfect;

    // at first, set curr_prime to the array size
    // it is then set to the actual curr_prime in first iter.
    curr_prime = num_elems; // will be set to next gt prime
    max_prime = PRIMES[PRIME_LENGTH - 1];

    // most number of resizes of slots = max_attempts
    for (attempt = 0; attempt < max_attempts; attempt++)
    {
        curr_prime = _find_next_prime_gt_naive(curr_prime);

        // we use bitflags to represent the current state
        // of the slots - this way we can save memory
        // for large slots (also this is on the stack
        // so we don't have to worry about freeing it)
        int slot_flags[(curr_prime / sizeof(int)) + 1];

        // cycle seeds from 0 -> seeds_per_cycle - 1
        for (seed_cycle = 0; seed_cycle < seeds_per_cycle; seed_cycle++)
        {
            PHT_DEBUG("seed cycle: %i", seed_cycle);
            perfect = 1;
        
            // zero the array for this cycle
            _zero_array(slot_flags, ARRAY_SIZE(slot_flags));

            // check each element to see if it already exists
            // if it does, set perfect = 0 and break
            for (int curr_elem_index = 0; curr_elem_index < num_elems; curr_elem_index++)
            {
                pht_item_t item = items[curr_elem_index];

                int loc = fnv_32a_str(item.key, seed_cycle) % curr_prime;
                // since we are using potentially >sizeof(int) flags,
                // we need to find an offset for the 
                flag_access offset = flag_array_offset(loc);

                // if the flag is set, we have not got a perfect array...
                if ((slot_flags[offset.offset] & offset.flag) == offset.flag)
                {
                    perfect = 0;
                    break;
                }

                slot_flags[offset.offset] |= offset.flag;
            }

            if (perfect == 0)
            {
                PHT_DEBUG_MSG("non-perfect, trying again");
                continue;
            };

            break;
        }

        if (perfect == 1)
        {
            break;
        }

        // we can't continue from here...
        if (curr_prime == max_prime)
        {
            perfect = 0;
            break;
        }
    }

    if (perfect == 0)
        return NULL;

    PHT_DEBUG(
        "perfect after %i resize(s), %i seed cycle(s) in final attempt (total cycles = %i)",
        attempt,
        seed_cycle + 1,
        attempt * seeds_per_cycle + (seed_cycle + 1) // seed_cycle starts at 0
    );

    pht_t *tab = malloc(sizeof(pht_t));

    // mallocing failed
    if (tab == NULL)
    {
        PHT_DEBUG_MSG("malloc failed");
        return NULL;
    }

    pht_item_t *arr = (pht_item_t *)calloc(curr_prime, sizeof(pht_item_t));

    // mallocing failed, free the table
    if (arr == NULL)
    {
        PHT_DEBUG_MSG("calloc fail");
        free(tab);
        return NULL;
    }

    for (int i = 0; i < num_elems; i++)
    {
        int loc = fnv_32a_str(items[i].key, seed_cycle) % curr_prime;
        int len = strlen(items[i].key);

        char *dst = (char *)malloc(sizeof(char) * len);
        strcpy(dst, items[i].key);

        arr[loc].key = dst;
        arr[loc].item = items[i].item;
    }

    // set up the table with hash table info
    tab->items = arr;
    tab->size = num_elems;
    tab->cap = curr_prime;
    tab->_seed = seed_cycle;

    PHT_DEBUG_MSG("finished");

    return tab;
}

void *pht_get(pht_t *table, char *key)
{
    int loc = fnv_32a_str(key, table->_seed) % table->cap;
    pht_item_t item = table->items[loc];

    // key == NULL -> no item set here
    if (item.key == NULL)
        return NULL;

    // if the keys do not match, we have a collision
    // from a non-defined key - dereferencing the front
    // of the char array may give us a quicker result (if 
    // the first chars don't match) in comparison to strcmp.
    if (
        *item.key != *key ||
        strcmp(item.key, key) != 0)
        return NULL;

    return item.item;
}

void pht_destroy(pht_t *table)
{
    for (int i = 0; i < table->cap; i++)
    {
        // if there is no key, there is no entry there
        if (table->items[i].key == NULL)
            continue;

        // free the key and the value (we copied the key)
        // we assume that they are maintaining the lifetime of
        // the reference to the table item when they pass it in
        free(table->items[i].key);
    }

    // next, free the table
    free(table->items);
    free(table);
}

int _bin_search_primes(int curr)
{
    int low = 0;
    int high = PRIME_LENGTH - 1;

    while (low <= high)
    {
        int mid = low + ((high - low) / 2);

        if (PRIMES[mid] < curr)
            low = mid + 1;
        else if (PRIMES[mid] > curr)
            high = mid - 1;
        else
            return mid + 1;
    }

    if (high < 0)
        return 0;
    if (low > (PRIME_LENGTH - 1))
        return PRIME_LENGTH;
    else
        return (low < high) ? low + 1 : high + 1;
}

pht_iterator_t* pht_iter(pht_t* tab) {
    pht_iterator_t* iter = malloc(sizeof(pht_iterator_t));

    if (iter == NULL) {
        return NULL;
    }

    iter->_table = tab;
    iter->_curr = -1;

    return iter;
}

int pht_next(pht_iterator_t* iter) {
    int i, found;
    pht_t* tab = iter->_table;

    found = 0;

    for(i = iter->_curr + 1; i < tab->cap; i++) {
        pht_item_t item = tab->items[i];

        PHT_DEBUG("found %p", item.key);

        if (item.key == NULL) continue;

        found = 1;

        iter->item = item.item;
        iter->key = item.key;
        iter->_curr = i;

        break;
    }

    if (found == 0) {
        iter->key = NULL;
        iter->item = NULL;
    }

    return found;
}

void pht_iter_destroy(pht_iterator_t* iter) {
    free(iter);
}

// 0 denotes we are done.
int _find_next_prime_gt_naive(int current)
{
    int index = _bin_search_primes(current);
    return PRIMES[index];
}

void _zero_array(int *arr, size_t size)
{
    for (int i = 0; i < size; i++)
        arr[i] = 0;
}