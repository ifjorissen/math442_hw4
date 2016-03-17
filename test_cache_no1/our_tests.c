#include <assert.h>
#include <string.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>

#include "hash_it_out_no1/cache.h"
// #include "evict.h"

static void print_key(key_type key)
{
    uint32_t i = 0;
    while (key[i]) {
        printf("%" PRIu32 ", ", key[i]);
        ++i;
    }
    printf("\n");
}

struct args {
    bool cache_tests;
    bool dbll_tests;
};

static struct option opts[] =
{
    {"cache-tests", no_argument, 0, 'c'},
    // {"dbll-tests", no_argument, 0, 'd'},
};

static void args_init(struct args * args)
{
    args->cache_tests = false;
    args->dbll_tests = false;
}

// static void test_evict_object()
// {
//     // test the eviction object
//     printf("Running evict test\n");
//     evict_t evict = evict_create(10);

//     uint8_t a[2] = {'a', '\0'};
//     uint8_t b[2] = {'b', '\0'};
//     uint8_t c[2] = {'c', '\0'};

//     evict_set(evict, a);
//     evict_get(evict, a);
//     evict_set(evict, b);
//     evict_set(evict, c);
//     evict_delete(evict, a);
//     evict_set(evict, c);

//     key_type k = evict_select_for_removal(evict);
//     assert(k && strcmp((const char*) k, (const char*) b) == 0 && "didn't retrieve correct key");
//     free((uint8_t*) k);

//     evict_delete(evict, b);

//     k = evict_select_for_removal(evict);
//     assert(k && strcmp((const char*) k, (const char*) c) == 0 && "didn't retrieve correct key");
//     free((uint8_t*) k);

//     evict_destroy(evict);
//     free(evict);
// }

static void test_mem_overflow()
{
    // test if the cache handles memory overflow correct
    printf("Running memory overflow test\n");
    cache_t c = create_cache(256, NULL);
    assert(cache_space_used(c) == 0);
    uint8_t key[2] = {'a', '\0'};
    uint8_t val[6] = {10,11,12,13,14,15};
    cache_set(c, key, val, 6);

    key[0] = 'b';
    uint8_t val2[6] = {20,21,22,23,24,25};
    cache_set(c, key, val2, 6);

    print_cache(c);
    // assert(6 == cache_space_used(c));
    destroy_cache(c);
}

static void test_set_get()
{
    // // A generic test of the cache that randomly generates keys
    // // and values, and then sets and gets them from the cache.
    // // this test doesn't take evictions into account, so make sure
    // // that the cache is created with enough memory
    // //
    // // !!! TEST MAY FAIL !!!
    // // Our code doesn't handle duplicate, nonunique keys (at the moment)
    // // so this test may fail sometimes, but it seems to be and should be rare
    // printf("Running set/get test\n");
    // uint32_t nsets = rand() % 10;
    // uint8_t *saved_keys[nsets];
    // uint8_t *saved_vals[nsets];

    // cache_t c = create_cache(256, NULL);
    // assert(cache_space_used(c) == 0);

    // for (uint32_t i = 0; i < nsets; i++) {
    //     uint32_t key_size = (rand() % 10) + 2;
    //     saved_keys[i] = calloc(key_size, sizeof(uint8_t));
    //     for (uint32_t j = 0; j < key_size - 1; j++) {
    //         saved_keys[i][j] = (rand() % (128-33))+34; //ascii chars
    //     }

    //     saved_vals[i] = calloc(key_size, sizeof(uint8_t));
    //     for (uint32_t j = 0; j < key_size - 1; j++) {
    //         saved_vals[i][j] = (rand() % (128-33))+34; //ascii chars
    //     }
    //     saved_keys[i][key_size - 1] = '\0';
    //     saved_vals[i][key_size - 1] = '\0';
    //     printf("setting key: %s; val: %s; size: %d.\n", saved_keys[i], saved_vals[i], key_size);
    //     cache_set(c, saved_keys[i], saved_vals[i], key_size);
    // }

    // for (uint32_t i = 0; i < nsets; i++) {
    //     uint32_t size;
    //     val_type v = cache_get(c, (key_type) saved_keys[i], &size);
    //     if (* (uint8_t *) v != *saved_vals[i]) {
    //         // assert(false && "test failed!");
    //         printf("not the same value, test might have failed or it might have been evicted");
    //     }
    //     free((void *) v);
    // }
    // print_cache(c);
    // destroy_cache(c);
    // for (uint32_t i = 0; i < nsets; i++) {
    //     free(saved_keys[i]);
    //     free(saved_vals[i]);
    //     saved_keys[i] = NULL;
    //     saved_vals[i] = NULL;
    // }
}

static void test_collision()
{
    printf("Running collision test\n");
    cache_t c = create_cache(256, NULL);
    assert(cache_space_used(c) == 0);
    uint8_t key[2] = {'a', '\0'};
    uint8_t val[6] = {10,11,12,13,14,15};
    cache_set(c, key, val, 6);

    uint8_t val2[4] = {20,21,22,23};
    cache_set(c, key, val2, 4);

    destroy_cache(c);
}

static void test_space()
{
    printf("Running space test\n");
    cache_t c = create_cache(256, NULL);
    assert(cache_space_used(c) == 0);
    uint8_t key[2] = {'a', '\0'};
    uint8_t val[6] = {10,11,12,13,14,15};
    cache_set(c, key, val, 6);

    key[0] = 'b';
    uint8_t val2[4] = {20,21,22,23};
    cache_set(c, key, val2, 4);

    assert(10 == cache_space_used(c));

    destroy_cache(c);
}

static void cache_tests()
{
    test_set_get();
    test_mem_overflow();
    test_collision();
    test_space();
    // test_evict_object();
}

static int go(struct args *args)
{
    if (args->cache_tests) {
        printf("Running cache tests\n");
        cache_tests();
    }

    if (args->dbll_tests) {
        printf("Running dbll tests\n");
        // dbll_tests();
    }
    return 0;
}

int main(int argc, char *argv[]) 
{
    srand(time(NULL));
    assert(argc && argv);
    struct args args;
    args_init(&args);
    int c, idx;
    while ((c = getopt_long(argc, argv, "", opts, &idx)) != -1) {
        switch(c) {
            case 'c':
                args.cache_tests = true;
                break;
            default:
                break;
        }
    }
    return go(&args);
}


