####HW4: Testy Cache
-----
@ifjorissen, @aled1027

#####HW4 Goal: Test everyone else's cache implementations and report back on bugs and API violations. 

----
#####Step 1: freeze cache repo. Done, our cache can be found [here](https://github.com/aled1027/hash_it_out/)

----

#####Step 2: Our tests can be found in the `alex_work` branch of our repository in the file `c_code/cache_tests.c`

Summary of our tests:

| test name | test function | description |
|---|---|---|
| t_sg | `test_set_get()` | tests that the cache can set and get values correctly |
| t_mem | `test_mem_overflow()` | tests that the cache handles memory overflow correctly |
| t_dup | `test_duplicate_key()` | tests that the cache handles duplicate keys correctly |
| t_space | `test_space()` | tests that the cache is aware of the space (in bytes) used by its values |
| t_del | `test_delete()` | tests the deletion of items from the cache |
| t_hash | `test_hash_function()` | tests whether or not the hash function is deterministic |


----

#####Step 3: Test other implementations of the cache

I (Isabella) did hws 1,3,4 and Alex did 5,6, and 7. The table summarizing our results:

| Code Number| No. Tests Failed | Names of Failed Tests | Comments |
|---|---|---|---|
| 1 | 4 | t_hash, t_del, t_sg, t_mem|see [Code Notes No. 1][]|
| 2 | 0 | - | All our tests pass on our implementation |
| 3 | 6 | t_hash, t_del, t_sg, t_mem, t_space, t_dup|see [Code Notes No. 3][]|
| 4 | 1 | t_hash |see [Code Notes No. 4][]|
| 5 | 1 | t_hash |see [Code Notes No. 5][]|
| 6 | 4 | t_hash, t_del, t_sg, t_mem |see [Code Notes No. 6][]|
| 7 | 4 | t_hash, t_del, t_sg, t_mem |see [Code Notes No. 7][]|

##### Code Notes No. 1
1. Lots of warnings when compiling with -Wall, -pedantic, etc. 
2. Broke cache api (using old cache.h)
3. t_sg fails with this error: `our_test(25397,0x7fff79b21000) malloc: *** error for object 0x7fcda0c036b0: pointer being freed was not allocated`. After looking in gdb, seems to be an issue with destroy_cache?
4. Cache's max mem argument throws an error if set to <64, and automatically sets capacity to 64 if this is the case. Without even running it, we can see that this automatically causes our t_mem test to fail, since we initalize the cache with 10 as the max.
5. After changing SIZE_STANDARD variable from 64 to 8 in their code.. (no longer black box testing...) I see that t_mem is still failing: `!!!!FAILURE!!!!! cache space used after a mem overflow is incorrect`
6. Segfault on the t_del test.
7. t_hash fails. `!!!!FAILURE!!!!! hash function not deterministic: value fail`

##### Code Notes No. 3
1. The `create_meta` function doesn't return anything and isn't a void function. 
2. There are some extra functions defined in cache.h. Also using _key_t and val_t types.
3. When trying to run their own tests (`make` and then `./out`) I get a seg fault:
   ```
   Created a new cache with size 2097152
  Adding data to cache.
  Trying to add a value of size 32, with available memory 2097152
  done
  Added data to cache.

  Testing ability to return value
  [1]    26489 segmentation fault  ./out
  ```
4. Fixed the line in cache_set according to moodle thread but still experiencing segfault. 
5. When running agains our suite of tests, we segfault before we can even test ability to detect return value. 
6. t_sg fails with a segfault.
7. t_mem failed once with:
 ```
   ***Running cache tests***
  Running cache memory overflow test
  Created a new cache with size 10
  Trying to add a value of size 6, with available memory 10
  Couldn't find a value to evict.
  Couldn't add a 32-64 byte value because there are no slabs of that range, and no free slabs to be allocated
  Trying to add a value of size 6, with available memory 10
  Couldn't find a value to evict.
  Couldn't add a 32-64 byte value because there are no slabs of that range, and no free slabs to be allocated
  our_test(27035,0x7fff79b21000) malloc: *** error for object 0x7fb65ac005b0: pointer being freed was not allocated
 ```
 and then consistently with:
 ```
  ***Running cache tests***
  Running cache memory overflow test
  Created a new cache with size 10
  Trying to add a value of size 6, with available memory 10
  [1]    27154 segmentation fault  ./our_test --cache-tests
```
8. All other tests fail similarly....

##### Code Notes No. 4
1. Lots of warnings when compiling with -Wall, -pedantic, etc.
2. Uses the old api, including val_type, key_type
3. Broke the API: cache_get() no longer takes `val_size` parameter.
4. All tests pass except for t_hash. 

##### Code Notes No. 5 
1. broke `create_cache` API 
2. Use `key_t/val_t` instead of `key_type` and `val_type`
3. Turn off warnings in makefile
4. Added #include <inttypes.h> to lru.h
5. our hash function test failed, not because the hash function is bad, but because `cache_get` does not copy the
   pointer that it is returning. If I call `cache_get` on the same key, then the pointers gotten point to the same piece
   of memory.
6. The makefile isn't working properly. When a file is updated, object files are not recompiled/used. |

##### Code Notes No. 6
1. broke `create_cache` API
2. delete test segfaults. `item_ptr` is NULL on line cache.c:82
3. `hash_function` test failed for same reason as Alec
4. `test_set_get` fails on `cache_get`. On line LRU.c:52, `node_ptr` is NULL
5. `test_memory_overflow` fails. Memory returned is simply wrong.

##### Code Notes No. 7
1. Couldn't compile because missing function. lru_replacement.c:138 `place_items_in_array(num_del_ids,policy->bottom)`
2. `test_delete` fails. Could not access the memory of the key in `cache_delete`
3. hash function -- same problem with memory
4. `set_get` test failing on destroy cache -- problem because freeing memory after getting. see delete.
5. `test_mem_overflow` fails. Seems to be a problem with dynamic resize.
