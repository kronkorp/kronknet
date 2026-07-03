#include <stddef.h>
#include <kronknet/utils/hashmap/hashmap.h>
#include <stdint.h>
#include <string.h>
#include <heracles/heracles.h>

static uint64_t dummy_collision_hash(uint64_t key) {
    (void)key;
    return 42; 
}

static uint64_t stable_hash(uint64_t key) {
    key ^= key >> 30;
    key *= 0xbf58476d1ce4e5b9ULL;
    key ^= key >> 27;
    key *= 0x94d049bb133111ebULL;
    key ^= key >> 31;
    return key;
}

static int g_delete_calls = 0;
static void spy_deleter(void *data) {
    g_delete_calls++;
    free(data);
}

Test(hashmap, insert_and_search_nominal)
{
    knMap *map = knMap_create(stable_hash, 8);
    AssertExists(map, "Map is null");

    char *val1 = strdup("Client_UDP_1");
    char *val2 = strdup("Client_UDP_2");

    AssertEq(knMap_insert(map, 111122223333ULL, val1, free), 0, "not equal");
    AssertEq(knMap_insert(map, 444455556666ULL, val2, free), 0, "not equal");

    AssertEq(knMap_search(map, 111122223333ULL), val1, "not equal");
    AssertEq(knMap_search(map, 444455556666ULL), val2, "not equal");
    Assert(knMap_search(map, 999999999999ULL) == NULL, "not equal");

    knMap_destroy(map);
}

Test(hashmap, overwrite_and_deleter)
{
    knMap *map = knMap_create(stable_hash, 8);
    AssertExists(map, "not equal");
    g_delete_calls = 0;

    char *val1 = strdup("Premier_Passage");
    char *val2 = strdup("Deuxieme_Passage");

    knMap_insert(map, 42ULL, val1, spy_deleter);
    AssertEq(g_delete_calls, 0, "not equal");

    knMap_insert(map, 42ULL, val2, spy_deleter);
    AssertEq(g_delete_calls, 1, "not equal");
    AssertEq(knMap_search(map, 42ULL), val2, "not equal");

    knMap_destroy(map);
}

Test(hashmap, hash_collision_handling)
{
    knMap *map = knMap_create(dummy_collision_hash, 8);
    AssertExists(map, "Map is null");

    char *val1 = strdup("Data_1");
    char *val2 = strdup("Data_2");

    knMap_insert(map, 10ULL, val1, free);
    knMap_insert(map, 20ULL, val2, free);
    
    AssertEq(knMap_search(map, 10ULL), val1, "not equal");
    AssertEq(knMap_search(map, 20ULL), val2, "not equal");

    knMap_destroy(map);
}

Test(hashmap, auto_rehash)
{
    knMap *map = knMap_create(stable_hash, 4);
    AssertExists(map, "Map is null");

    knMap_insert(map, 1ULL, strdup("A"), free);
    knMap_insert(map, 2ULL, strdup("B"), free);
    knMap_insert(map, 3ULL, strdup("C"), free);
    knMap_insert(map, 4ULL, strdup("D"), free);

    AssertExists(knMap_search(map, 1ULL), "Should exists");
    AssertExists(knMap_search(map, 2ULL), "Should exists");
    AssertExists(knMap_search(map, 3ULL), "Should exists");
    AssertExists(knMap_search(map, 4ULL), "Should exists");

    knMap_destroy(map);
}
