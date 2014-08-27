#ifndef N64CART_H
#define N64CART_H

#include <Board/Dataflash.h>
#include "string.h"

/** Page size of the ROM. This is kept at 512, 
*  because N64 carts can be read in 256 word-chunks
*  max (512 bytes). Do not change this value.
*/
#define ROM_BLOCK_SIZE	9 // 2**ROM_BLOCK_SIZE bytes/block

typedef struct {
	char name[21];
	uint8_t nameLen;
	uint32_t size;
	uint8_t header[1<<ROM_BLOCK_SIZE];
} CartInfo;

CartInfo currentCartInfo;

void Cart_Init(void);
void getCartBlock(uint8_t *blk, uint32_t blockNum); //as uint8[512]
uint32_t calculateCartSize(void); // in blocks
#endif
