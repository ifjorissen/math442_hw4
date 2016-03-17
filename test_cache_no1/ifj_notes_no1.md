### Testing Hash it out No. 1 [Jeremy and Nathan Hw3](https://github.com/Jmcosel/442-HW-3)

#####Notes:
  * There was no makefile, so I compiled cache.c of the files with `gcc -g -O0 -Wall -Wextra -pedantic -Werror -std=gnu11 -Wno-unused-function cache.c -o cache.o `
    * Compiling cache.c:
      * 13 errors generated
        * 5 of which were formatting related (-Wformat). 
          * e.g: `cache.c:314:92: error: format specifies type 'unsigned long' but the argument has type 'uint64_t' (aka 'unsigned long long') [-Werror, Wformat]`
        * the remaining 8 were related to converting pointers to different types (-Wpointer-sign)
          * e.g: `cache.c:127:39: warning: passing 'key_type' (aka 'const unsigned char *') to parameter of type 'const char *' converts between pointers to integer`
  * Compiling according to their recommendations (`gcc -o Test cache.c main.c -lm`)
    * Produces 29 warnings, of either of the 2 aforementioned types.
    * Note: adding some more flags and running `gcc -g -O0 -Wall -Wextra -pedantic -std=gnu11 -Wno-unused-function -o Test cache.c main.c -lm` produces 30 warnings. It seems that one of their tests had an unused parameter:
      * main.c:73:39: `warning: unused parameter 'key' [-Wunused-parameter] uint64_t test_hash_collision(key_type key)`
  * Running `./Test <cache_capacity>`
    * Bug: if <cache_capacity> is less than 128, we experience a segfault when doubling the size of the cache.
      * e.g: `./Test 64`
    * If we try to create a cache with 1 byte of memory, we get an error saying that it's too small and the cache was initialized with 64 bytes of memory. 
      * e.g the second print statement after running `./Test 1` is `The given maxmem (1) is too small; set to 64 bytes.`
    * However if we run `./Test -1`, we simply get a segfault and a malloc error.

  * They're also using the "old" cache api: create_cache() takes a capacity and a hash function.
  * Not sure if this is a bug, but the capacity of the cache is the actual number of bytes the cache is, not the maximum number of items in the cache.
  * Bug: If cache_get() cannot find an item in the cache, it returns NULL instead of a NULL pointer. 
  * Bug: line 288 of cache.c: `free((val_type *)item->value)` will occasionally throw: `malloc: *** error for object 0x100200e20: pointer being freed was not allocated`
    * Used GDB to step through the destroy cache function 
    *  ```
      (gdb) backtrace
      #0  0x00007fff9c2e0002 in __pthread_kill () from /usr/lib/system/libsystem_kernel.dylib
      #1  0x00007fff9831c5c5 in pthread_kill () from /usr/lib/system/libsystem_pthread.dylib
      #2  0x00007fff94b946e7 in abort () from /usr/lib/system/libsystem_c.dylib
      #3  0x00007fff97572070 in free () from /usr/lib/system/libsystem_malloc.dylib
      #4  0x0000000100001e22 in destroy_cache (cache=0x100200290) at hash_it_out_no1/cache.c:288
      #5  0x00000001000024b5 in test_set_get () at our_tests.c:127
      #6  0x00000001000021f9 in cache_tests () at our_tests.c:169
      #7  0x00000001000021bf in go (args=0x7fff5fbff7a8) at our_tests.c:180
      #8  0x0000000100002162 in main (argc=2, argv=0x7fff5fbff7e8) at our_tests.c:206
      ```
  * Bug:  cache.c lines 144 and 145:
    ```
    item->value = calloc(1, val_size);
    memcpy((void*)item->value, val, val_size); 
    ```

  *to do: test resize, it seems wonky
