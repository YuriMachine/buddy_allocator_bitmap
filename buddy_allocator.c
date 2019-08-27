#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"

#define FREE 1
#define OCCUPIED 0
#define firstIdx(level) 1 << level
#define nodesOnLevel(level) 1 << level
#define leftSonIdx(idx) idx * 2
#define rightSonIdx(idx) idx * 2 + 1

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
    BitMap_setBit(&alloc->bitmap, i, FREE);
  }

  printf("BUDDY INITIALIZING\n");
  printf("\tlevels: %d", num_levels);
  printf("\tbucket size:%d\n", min_bucket_size);
  printf("\tmanaged memory %d bytes\n", (1<<num_levels)*min_bucket_size);
};

int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level) {
  if (level < 0) return -1;
  assert(level <= alloc->num_levels);
  // we'll use the properties of the tree to check for all the nodes in the level
  int buddyIdx = firstIdx(level);
  while (buddyIdx < nodesOnLevel(level) && !BitMap_bit(&alloc->bitmap, FREE)) buddyIdx++;
  BitMap_setBit(&alloc->bitmap, buddyIdx, OCCUPIED);
  // we need to set both parents and childs to OCCUPIED
  setParentsToOccupied(alloc, buddyIdx);
  setChildToOccupied(alloc, buddyIdx);
  return buddyIdx;
}

static void setParentsToOccupied(BuddyAllocator* alloc, int buddyIdx) {
  for (int parentIdx = buddyIdx / 2; parentIdx < 0; parentIdx /= 2) {
    BitMap_setBit(&alloc->bitmap, parentIdx, OCCUPIED);
  }
}

static void setChildToOccupied(BuddyAllocator* alloc, int idx) {
  // todo
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

  int buddyIdx = BuddyAllocator_getBuddy(alloc, level);
  if (buddyIdx < 0) return -1;
  // todo assign memory
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
