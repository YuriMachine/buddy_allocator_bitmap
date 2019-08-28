#pragma once
#include "bitmap.h"
#define MAX_LEVELS 16

typedef struct  {
  int num_levels;
  char* memory; // the memory area to be managed
  uint8_t* bitmap_buffer;
  BitMap bitmap;
  int min_bucket_size; // the minimum page of RAM that can be returned
} BuddyAllocator;


// computes the size in bytes for the buffer of the allocator
int BuddyAllocator_calcSize(int num_levels);


// initializes the buddy allocator, and checks that the buffer is large enough
void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         uint8_t* bitmap_buffer,
                         int buffer_size,
                         char* memory,
                         int min_bucket_size);

// allocates a buddy at a given level.
// returns bit_num on bitmap
int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level);

// releases an allocated buddy
void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int bit_num);

//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);
