#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"

#define FREE 1
#define OCCUPIED 0

#define firstIdx(level) ((1 << level) - 1)
#define nodesOnLevel(level) (1 << level)
#define maxIndexOnLevel(level) (firstIdx(level) + nodesOnLevel(level))

#define leftSonIdx(idx) (idx * 2) + 1
#define rightSonIdx(idx) (idx * 2 + 2)
#define parentIdx(idx) (idx & 1 ? idx / 2 : idx / 2 - 1)
#define buddyIdx(idx) (idx & 1 ? idx + 1 : idx - 1)

static int levelIdx(size_t idx) {
  return (int) floor(log2(idx));
}

void BuddyItem_init(BuddyItem* item, char* memory, int size) {
  item->memoryChunk = memory + sizeof(BuddyItem);
  item->size = size;
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
  BitMap_init(&alloc->bitmap, nodesOnLevel(num_levels) - 1, buffer_size, bitmap_buffer);
  // initialize bitmap to 1 (free)
  for (int i = 0; i < alloc->bitmap.num_bits; ++i) {
    BitMap_set(&alloc->bitmap, i, FREE);
  }

  printf("BUDDY INITIALIZING\n");
  printf("\tlevels: %d", num_levels);
  printf("\tbucket size:%d\n", min_bucket_size);
  printf("\tmanaged memory %d bytes\n", (1<<num_levels)*min_bucket_size);
};

static void setParentsStatus(BuddyAllocator* alloc, int idx, int status) {
  // need to free parents only if the buddy of the idx is free
  if (status == FREE && !BitMap_get(&alloc->bitmap, buddyIdx(idx))) return;
  for (int pIdx = parentIdx(idx); pIdx > 0; pIdx = parentIdx(pIdx)) {
    BitMap_set(&alloc->bitmap, pIdx, status);
    if (status == FREE && !BitMap_get(&alloc->bitmap, buddyIdx(pIdx))) return;
  }
  // root special case
  BitMap_set(&alloc->bitmap, 0, status);
}

static void setChildStatus(BuddyAllocator* alloc, int idx, int status) {
  if (levelIdx(idx) > alloc->num_levels) return;
  BitMap_set(&alloc->bitmap, leftSonIdx(idx), status);
  setChildStatus(alloc, leftSonIdx(idx), status);
  BitMap_set(&alloc->bitmap, rightSonIdx(idx), status);
  setChildStatus(alloc, rightSonIdx(idx), status);
}

int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level) {
  if (level < 0) return -1;
  assert(level <= alloc->num_levels);
  // we'll use the properties of the tree to check for all the nodes in the level
  int buddyIdx = firstIdx(level);
  while (buddyIdx < maxIndexOnLevel(level) && !BitMap_get(&alloc->bitmap, buddyIdx)) buddyIdx++;
  BitMap_set(&alloc->bitmap, buddyIdx, OCCUPIED);
  // we need to set both parents and childs to OCCUPIED
  setParentsStatus(alloc, buddyIdx, OCCUPIED);
  setChildStatus(alloc, buddyIdx, OCCUPIED);
  return buddyIdx;
}

// allocates memory
BuddyItem* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  if (size <= 0 || size > (1 << num_levels) * min_bucket_size) return 0;
  // we determine the level of the page
  int mem_size = (1 << alloc->num_levels) * alloc->min_bucket_size;
  int level = levelIdx(mem_size / size);
  int buddy_size = mem_size >> level;
  // if the level is too small, we pad it to max
  if (level > alloc->num_levels) level = alloc->num_levels;
  printf("requested: %d bytes, level %d \n", size, level);

  int buddyIdx = BuddyAllocator_getBuddy(alloc, level);
  if (buddyIdx < 0) return NULL;
  char* indexedMemory = alloc->memory + (buddyIdx - firstIdx(level)) * buddy_size;
  BuddyItem* newElem = (BuddyItem*) indexedMemory;
  BuddyItem_init(newElem, indexedMemory, size);
  return newElem;
}

// releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, BuddyItem* mem) {
  printf("freeing %p\n", mem);
  // get index
  int mem_size = (1 << alloc->num_levels) * alloc->min_bucket_size;
  int level = levelIdx(mem_size / mem->size);
  int buddy_size = mem_size >> level;
  int memory_offset = mem->memoryChunk - alloc->memory;
  int idx = memory_offset / buddy_size + firstIdx(level);
  // free bits
  BitMap_set(&alloc->bitmap, idx, FREE);
  setParentsStatus(alloc, idx, FREE);
  setChildStatus(alloc, idx, FREE);
}
