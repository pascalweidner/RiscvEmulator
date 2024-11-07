#include "dram.h"

uint32_t dram32_load(DRAM32 *dram, uint32_t addr, uint32_t size) {
    assert(size == 8 || size == 16 || size == 32);
    uint32_t addrMapped = addr - DRAM32_BASE;
    switch(size) {
        case 8: return dram32_load_byte(dram, addrMapped); break;
        case 16: return dram32_load_halfWord(dram, addrMapped); break;
        case 32: return dram32_load_byte(dram, addrMapped); break;
    }
}

static uint32_t dram32_load_byte(DRAM32 *dram, uint32_t addr) {

}

static uint32_t dram32_load_halfWord(DRAM32 *dram, uint32_t addr) {

}

static uint32_t dram32_load_word(DRAM32 *dram, uint32_t addr) {

} 

void dram32_store(DRAM32 *dram, uint32_t addr, uint32_t size, uint32_t value) {
    uint32_t addrMapped = addr - DRAM32_BASE;

}

DRAM32 *init_dram32() {
    DRAM32 *dram32 = (DRAM32*)malloc(sizeof(DRAM32));
    dram32->x = malloc(DRAM_SIZE);
    return dram32;
}
