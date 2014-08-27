#ifndef N64CART_H
#define N64CART_H

#include <util/delay.h>
#include "ports.h"

/** Page size of the ROM. This is kept at 512, 
*  because N64 carts can be read in 256 word-chunks
*  max (512 bytes). Do not change this value.
*/
#define ROM_BLOCK_SIZE	9 // 2**ROM_BLOCK_SIZE bytes/block

void getCartBlock(uint8_t *blk, uint32_t blockNum); //as uint8[512]
uint32_t calculateCartSize(void); // in blocks
#endif
