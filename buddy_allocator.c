#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"

// these are trivial helpers to support you in case you want
// to do a bitmap implementation
static int levelIdx(size_t idx) {
  return (int) floor(log2(idx));
}

static int buddyIdx(int idx) {
  if (idx&0x1) return idx - 1;
  return idx + 1;
}

static int parentIdx(int idx) {
  return idx/2;
}

static int startIdx(int idx) {
  return (idx - (1 << levelIdx(idx)));
}

void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         uint8_t* bitmap_buffer,
                         int buffer_size,
                         char* memory,
                         int min_bucket_size) {
  alloc->memory = memory;
  alloc->num_levels = num_levels;
  alloc->min_bucket_size = min_bucket_size;
  assert(num_levels < MAX_LEVELS);

  BitMap_init(&alloc->bitmap, buffer_size, bitmap_buffer);
  // initialize bitmap to 1 (free)
  for (int i = 0; i < buffer_size; ++i) {
    BitMap_setBit(&alloc->bitmap, i, 1);
  }

  printf("BUDDY INITIALIZING\n");
  printf("\tlevels: %d", num_levels);
  printf("\tbucket size:%d\n", min_bucket_size);
  printf("\tmanaged memory %d bytes\n", (1<<num_levels)*min_bucket_size);
};

static int getMinBuddy(BuddyAllocator* alloc, int level) {
  // we start searching from the last pos
  for (int i = pow(2, level + 1) - 1; i > 0; i--) {
    if (BitMap_bit(&alloc->bitmap, i)) return i;
  }
  printf("no buddy found :(\n");
  return 0;
}

int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level) {
  if (level < 0) return 0;
  assert(level <= alloc->num_levels);
  // need to find smallest buddy
  int idx = getMinBuddy(alloc, level);
  if (idx == 0) return 0;
  // set 0 (occupied) to self and parents
  for (int i = idx; i > 0; i = i / 2) {
    BitMap_setBit(&alloc->bitmap, idx, 0);
  }
  // todo
  return 0;
}

void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int bit_num) {
  // todo
}

// allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  // we determine the level of the page
  int mem_size = (1 << alloc->num_levels) * alloc->min_bucket_size;
  int level = floor(log2(mem_size / (size + 8)));

  // if the level is too small, we pad it to max
  if (level > alloc->num_levels) level = alloc->num_levels;

  printf("requested: %d bytes, level %d \n", size, level);

  // todo

  return 0;
}

// releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem) {
  printf("freeing %p", mem);
  // we retrieve the buddy from the system
  char* p = (char*) mem;
  p = p - 8;
  // todo
}
