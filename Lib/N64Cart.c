#include "n64cart.h"

inline void setAD16(uint16_t addr) {
  AD_LOW_PORT = (uint8_t) (addr & 0xff);
  AD_HI_PORT = (uint8_t) (addr>>8 & 0xff);
}

inline void tristateAD(void) {
  setAD16(0x0000);
  AD_LOW_DDR = 0x00;
  AD_HI_DDR = 0x00;
}

inline void untristateAD(void) {
  AD_LOW_DDR = 0xff;
  AD_HI_DDR = 0xff;
}

void txAD(uint32_t addr) {
	untristateAD();
	CTL_PORT = ALEL | ALEH | RD; // turn on RD and ALEH

	setAD16(addr>>16);
	CTL_PORT &= ~ALEH;          // send hi byte

	setAD16(addr & 0xffff);
	CTL_PORT &= ~ALEL;         // send lo byte

	tristateAD();
	//_delay_us(1);
}

inline uint16_t getData16(void) { 
  return ((uint16_t)AD_HI_PIN)<<8 | ((uint16_t)AD_LOW_PIN);
}

inline void rxAD(uint16_t *buf) {
  CTL_PORT &= ~RD; // read!
  CTL_PORT |= RD;
  *buf = getData16();
}

void getCartBlock(uint8_t *blk, uint32_t blockNum) {
	uint16_t c = 0, word;
	uint32_t addr = (blockNum<<ROM_BLOCK_SIZE) + 0x10000000;
  
	txAD(addr);
	ST_PORT |= ST_RD;
	while (c<1<<ROM_BLOCK_SIZE) {
		rxAD(&word);
		blk[c++] = word>>8;
		blk[c++] = word & 0xff;
	}
	ST_PORT &= ~ST_RD;
}

// works fast if non-empty, otherwise, pretty slow.
uint8_t isEmptyBlock(uint8_t *blk) {
	uint8_t b0 = blk[0], b1 = blk[1], matchesAddress=1;
	uint16_t i;
	for (i=2; i<1<<ROM_BLOCK_SIZE; i+=2) {
		if ( ((blk[i] != b0) || (blk[i+1] != b1)) &&
		     ((blk[i] != i&0xff) || (blk[i+1] != (i+1)&0xff)) ) return 0;
	}	
	return 1;
}

// works fast if non-empty, otherwise, pretty slow.
uint8_t isEmptyBlockNum(uint32_t blockNum) {
	uint8_t blk[1<<ROM_BLOCK_SIZE];

	if (blockNum & 8191) return 0;
	
	getCartBlock(blk, blockNum);
	return isEmptyBlock(blk);
}

uint32_t calculateCartSize(void) { // in blocks
	uint32_t blockNum = 0;
	while (!isEmptyBlockNum(blockNum)) blockNum += 8192;
	return blockNum;
}
