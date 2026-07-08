#include <heracles/heracles.h>
#include <kronklab/kronklab.h>
#include "kronknet/utils/hashmap/hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static int g_deleter_call_count = 0;

static void mock_deleter(void *data)
{
    (void)data;
    g_deleter_call_count++;
}

static void mock_foreach_counter(uint64_t key, void *value, void *param)
{
    (void)key;
    (void)value;
    int *count = (int *)param;
    (*count)++;
}

Test(hashmap, basic_hash_deterministic)
{
    uint64_t hash1 = knMap_basicHash(12345);
    uint64_t hash2 = knMap_basicHash(12345);
    uint64_t hash3 = knMap_basicHash(54321);

    AssertEq(hash1, hash2, "Le hash d'une même clé doit toujours être identique");
    Assert(hash1 != hash3, "Des clés différentes doivent (idéalement) produire des hash différents");
}

Test(hashmap, create_destroy_empty)
{
    knMap *map = knMap_create(knMap_basicHash, 32);
    AssertNotNull(map, "La création de la map ne doit pas renvoyer NULL");
    knMap_destroy(map); 
}

Test(hashmap, insert_and_search_success)
{
    knMap *map = knMap_create(knMap_basicHash, 16);
    uint64_t key = 42;
    char *value = "kronknet_data";

    int res = knMap_insert(map, key, value, NULL);
    AssertEq(res, 0, "L'insertion doit retourner 0 en cas de succès");

    void *found = knMap_search(map, key);
    AssertNotNull(found, "La donnée insérée doit être trouvée");
    AssertEq((char*)found, value, "Le pointeur retourné doit correspondre à la valeur insérée");

    knMap_destroy(map);
}

Test(hashmap, search_not_found)
{
    knMap *map = knMap_create(knMap_basicHash, 16);
    
    knMap_insert(map, 10, "A", NULL);
    
    void *found = knMap_search(map, 999);
    Assert(found == NULL, "La recherche d'une clé inexistante doit renvoyer NULL");

    knMap_destroy(map);
}

Test(hashmap, overwrite_calls_deleter)
{
    knMap *map = knMap_create(knMap_basicHash, 16);
    g_deleter_call_count = 0;
    
    uint64_t key = 100;
    char *val1 = "old_data";
    char *val2 = "new_data";

    knMap_insert(map, key, val1, mock_deleter);
    knMap_insert(map, key, val2, mock_deleter);

    AssertEq(g_deleter_call_count, 1, "Le deleter aurait dû être appelé 1 fois lors de l'écrasement");

    void *found = knMap_search(map, key);
    AssertEq((char*)found, val2, "La valeur doit être mise à jour après écrasement");

    knMap_destroy(map);
    AssertEq(g_deleter_call_count, 2, "Le deleter aurait dû être appelé à la destruction de la map");
}

Test(hashmap, explicit_delete_bypasse_deleter)
{
    knMap *map = knMap_create(knMap_basicHash, 16);
    g_deleter_call_count = 0;
    
    uint64_t key = 50;
    char *val = "temp_data";

    knMap_insert(map, key, val, mock_deleter);
    
    int res = knMap_delete(map, key);
    AssertEq(res, 0, "La suppression doit renvoyer 0");

    AssertEq(g_deleter_call_count, 0, "knMap_delete NE DOIT PAS appeler le deleter interne");

    void *found = knMap_search(map, key);
    Assert(found == NULL, "L'élément supprimé ne doit plus être trouvable");

    knMap_destroy(map);
    AssertEq(g_deleter_call_count, 0, "L'élément étant déjà supprimé, la destruction ne doit pas trigger de deleter");
}

Test(hashmap, delete_non_existent)
{
    knMap *map = knMap_create(knMap_basicHash, 16);

    int res = knMap_delete(map, 404);
    AssertEq(res, -1, "La suppression d'un élément inexistant doit renvoyer -1");

    knMap_destroy(map);
}

Test(hashmap, foreach_traversal)
{
    knMap *map = knMap_create(knMap_basicHash, 16);
    
    knMap_insert(map, 1, "A", NULL);
    knMap_insert(map, 2, "B", NULL);
    knMap_insert(map, 3, "C", NULL);

    int count = 0;
    knMap_foreach(map, mock_foreach_counter, &count);

    AssertEq(count, 3, "knMap_foreach doit itérer exactement 3 fois");

    knMap_destroy(map);
}

Test(hashmap, stress_and_collisions)
{
    knMap *map = knMap_create(knMap_basicHash, 8);
    int iterations = 1000;

    for (uint64_t i = 0; i < (uint64_t)iterations; i++) {
        knMap_insert(map, i, (void*)(uintptr_t)i, NULL);
    }

    int success = 1;
    for (uint64_t i = 0; i < (uint64_t)iterations; i++) {
        void *found = knMap_search(map, i);
        if (found != (void*)(uintptr_t)i) {
            success = 0;
            break;
        }
    }

    AssertEq(success, 1, "Tous les 1000 éléments doivent être retrouvés malgré les collisions");

    knMap_destroy(map);
}
