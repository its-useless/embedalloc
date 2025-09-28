#ifndef EMBEDALLOC_H
#define EMBEDALLOC_H

typedef unsigned long long eaull;

typedef struct block {
    eaull size;
    char in_use;
    struct block* next;
    struct block* prev;
} block_t;

typedef struct Allocator {
    block_t* head;
    void* start;
    void* end;
} Allocator;

#ifdef __cplusplus
extern "C" {
#endif
Allocator ealloc_new(void* start, eaull size);
void* ealloc(Allocator* ator, eaull size);
void efree(Allocator* ator, void* ptr);
#ifdef __cplusplus
}
#endif

#endif
