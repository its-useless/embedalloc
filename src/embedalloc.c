#include "embedalloc.h"
#define NULL ((void*)0)
#define true ((char)1)
#define false ((char)0)

Allocator ealloc_new(void* start, eaull size) {
  Allocator inst;
  inst.start = start;
  inst.end = (char*)start + size;

  block_t* block = (block_t*)start;
  block->size = size;

  inst.head = (block_t*)block;
  inst.head->next = NULL;
  inst.head->prev = NULL;
  return inst;
}

void* ealloc(Allocator* ator, eaull size) {
  // align size to 8 bytes
  size = (size + 7) & ~7;

  eaull total_needed = sizeof(block_t) + size;

  // find a block by first-fit
  block_t* block = ator->head;
  for (;;) {
    if (!block)
      break;
    if (block->size >= total_needed && !block->in_use) {
      // found a block
      break;
    }
    block = block->next;
  }

  if (!block)
    return NULL;

  if (block->size > total_needed) {
    // split the block
    block_t* next = (block_t*)((char*)block + total_needed);
    next->size = block->size - total_needed;
    next->prev = block;
    next->next = NULL;
    next->in_use = false;

    block->size = total_needed;
    block->next = next;
  }
  block->in_use = true;

  // ator->head = block;

  void* ptr = block;
  ptr += sizeof(block_t);

  return ptr;
}

void efree(Allocator* ator, void* ptr) {
  if (!ptr)
    return;

  block_t* block = (block_t*)(ptr - sizeof(block_t));
  block->in_use = false;

  // coalesce with next free blocks
  {
    block_t* next = block->next;
    while (next && !next->in_use) {
      block->size += next->size;
      block->next = next->next;
      if (next->next) {
        next->next->prev = block;
      }
      next = block->next;
    }
  }

  // coalesce with previous free blocks
  {
    block_t* prev = block->prev;
    while (prev && !prev->in_use) {
      prev->size += block->size;
      prev->next = block->next;
      if (block->next) {
        block->next->prev = prev;
      }
      block = prev;
      prev = block->prev;
    }
  }
}
