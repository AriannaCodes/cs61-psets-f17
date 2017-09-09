#define M61_DISABLE 1
#include "m61.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

unsigned long long malloc_num = 0;
unsigned long long malloc_size = 0;
unsigned long long free_num = 0;
unsigned long long free_size = 0;
unsigned long long fail_num = 0;
unsigned long long fail_size = 0;
char* heap_min = NULL;
char* heap_max = NULL;

/// m61_malloc(sz, file, line)
///    Return a pointer to `sz` bytes of newly-allocated dynamic memory.
///    The memory is not initialized. If `sz == 0`, then m61_malloc may
///    either return NULL or a unique, newly-allocated pointer value.
///    The allocation request was at location `file`:`line`.

void* m61_malloc(size_t sz, const char* file, int line) {
    // avoid uninitialized variable warnings
    (void) file, (void) line;   
    // return NULL is sz is 0
    if (sz == 0) {
        return NULL;
    }
    // create struct for holding metadata and value
    struct m61_allocation* allocation;
    allocation = base_malloc(sizeof(size_t) + sz);
    // ensure that base_malloc worked; if it didn't, fail
    if (allocation == NULL) {
        fail_num++;
        fail_size += sz;
        return NULL;
    }
    else {
        // fill struct for holding metadata and value
        allocation->size = sz;
        malloc_num++;
        malloc_size += sz;
        // update statistics
        if (heap_min == NULL || (char*) (allocation + sizeof(size_t)) < heap_min) {
            //printf("setting heap min\n");
            heap_min = (char *) (allocation + sizeof(size_t));
        }
        if (heap_max == NULL || (char*) (allocation + sizeof(size_t) + sz) > heap_max) {
            //printf("setting heap max\n");
            heap_max = (char *) (allocation + sizeof(size_t)) + sz;
        }
    }
    // return pointer to value
    //m61_printstatistics();
    //printf("---\n");
    return allocation + sizeof(size_t);
}


/// m61_free(ptr, file, line)
///    Free the memory space pointed to by `ptr`, which must have been
///    returned by a previous call to m61_malloc and friends. If
///    `ptr == NULL`, does nothing. The free was called at location
///    `file`:`line`.

void m61_free(void *ptr, const char *file, int line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // free null
    if (ptr == NULL) {
        return;
    }
    // get size of memory
    unsigned long long* size_loc = (ptr - 128);
    unsigned long long size = *size_loc;
    // call base_free
    base_free(ptr - sizeof(size_t));
    // update statistics
    free_num++;
    //printf("size is %llu \n", size);
    free_size += size;
    //m61_printstatistics();
    //printf("---\n");
    return;
}


/// m61_realloc(ptr, sz, file, line)
///    Reallocate the dynamic memory pointed to by `ptr` to hold at least
///    `sz` bytes, returning a pointer to the new block. If `ptr` is NULL,
///    behaves like `m61_malloc(sz, file, line)`. If `sz` is 0, behaves
///    like `m61_free(ptr, file, line)`. The allocation request was at
///    location `file`:`line`.

void* m61_realloc(void* ptr, size_t sz, const char* file, int line) {
    void* new_ptr = NULL;
    if (sz) {
        new_ptr = m61_malloc(sz, file, line);
    }
    if (ptr && new_ptr) {
        // Copy the data from `ptr` into `new_ptr`.
        // To do that, we must figure out the size of allocation `ptr`.
        // Your code here (to fix test014).
    }
    m61_free(ptr, file, line);
    return new_ptr;
}


/// m61_calloc(nmemb, sz, file, line)
///    Return a pointer to newly-allocated dynamic memory big enough to
///    hold an array of `nmemb` elements of `sz` bytes each. The memory
///    is initialized to zero. If `sz == 0`, then m61_malloc may
///    either return NULL or a unique, newly-allocated pointer value.
///    The allocation request was at location `file`:`line`.

void* m61_calloc(size_t nmemb, size_t sz, const char* file, int line) {
    // Your code here (to fix test016).
    void* ptr = m61_malloc(nmemb * sz, file, line);
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}


/// m61_getstatistics(stats)
///    Store the current memory statistics in `*stats`.

void m61_getstatistics(struct m61_statistics* stats) {
    // Stub: set all statistics to enormous numbers
    memset(stats, 255, sizeof(struct m61_statistics));
    // set statistics within struct
    stats->nactive = malloc_num - free_num;
    stats->active_size = malloc_size - free_size;
    stats->ntotal = malloc_num;
    stats->total_size = malloc_size;
    stats->nfail = fail_num;
    stats->fail_size = fail_size;
    stats->heap_min = heap_min;
    stats->heap_max = heap_max;
    return;
}


/// m61_printstatistics()
///    Print the current memory statistics.

void m61_printstatistics(void) {
    struct m61_statistics stats;
    m61_getstatistics(&stats);

    printf("malloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("malloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}


/// m61_printleakreport()
///    Print a report of all currently-active allocated blocks of dynamic
///    memory.

void m61_printleakreport(void) {
    // Your code here.
}
