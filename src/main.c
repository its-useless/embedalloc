#include <stdio.h>

#include "embedalloc.h"

static char heap[0x100000];

void addr(void* p) {
    printf("%p\n", p);
}

int main() {
    Allocator ator = ealloc_new(heap, sizeof heap);
    void* a = ealloc(&ator, 8);
    addr(a);
    void* b = ealloc(&ator, 8);
    addr(b);
    efree(&ator, b);
    efree(&ator, a);
    printf("0x%llx\n", ator.head->size);
}
