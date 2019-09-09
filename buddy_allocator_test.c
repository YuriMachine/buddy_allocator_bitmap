#include "buddy_allocator.h"
#include <stdio.h>

#define BUDDY_LEVELS 9
#define BUFFER_SIZE (1 << BUDDY_LEVELS) - 1
#define MEMORY_SIZE (1024*1024)
#define MIN_BUCKET_SIZE (MEMORY_SIZE>>(BUDDY_LEVELS))

uint8_t buffer[BUFFER_SIZE];
char memory[MEMORY_SIZE];

BuddyAllocator alloc;
int main(int argc, char** argv) {
  // we initialize the allocator
  printf("init... ");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BUFFER_SIZE,
                      memory,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");
  test(&alloc);
  BuddyItem* p1=BuddyAllocator_malloc(&alloc, 100);
  test(&alloc);
  BuddyItem* p2=BuddyAllocator_malloc(&alloc, 100);
  test(&alloc);
  BuddyItem* p3=BuddyAllocator_malloc(&alloc, 100000);
  test(&alloc);
  BuddyAllocator_free(&alloc, p3);
  test(&alloc);
  BuddyAllocator_free(&alloc, p2);
  test(&alloc);
  BuddyAllocator_free(&alloc, p1);
  test(&alloc);
}
