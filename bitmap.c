#include "bitmap.h"

void BitMap_init(BitMap* bit_map, int bits, int buffer_size, uint8_t* buffer) {
    bit_map->buffer=buffer;
    bit_map->buffer_size=buffer_size;
    bit_map->num_bits = bits;
    bit_map->num_ints = (bits + 8 - 1) / 8;
}

void BitMap_set(BitMap* bit_map, int i, int status) {
  if (status) bit_map->buffer[i/8] |= 1 << (7 - (i%8));
  else bit_map->buffer[i/8] &= ~(1 << (7 - (i%8)));
}

int BitMap_get(BitMap* bit_map, int q) {
    if (q > bit_map->num_bits) return 0;
    return (( bit_map->buffer[q/8]) >> (7 - (q%8)) & 1);
}
