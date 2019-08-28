#include <stdint.h>

typedef struct {
  int num_bits;
  int buffer_size;
  uint8_t* buffer;
} BitMap;

void BitMap_init(BitMap* bit_map, int bits, int buffer_size, uint8_t* buffer);
void BitMap_set(BitMap* bit_map, int i, int status);
int BitMap_get(BitMap* bit_map, int q);
