#include "dram.h"

static uint32_t dram32_load_byte(DRAM32* dram, uint32_t addr);
static uint32_t dram32_load_halfWord(DRAM32 *dram, uint32_t addr);
static uint32_t dram32_load_word(DRAM32 *dram, uint32_t addr);

uint32_t dram32_load(DRAM32 *dram, uint32_t addr, uint32_t size) {
    assert(size == 8 || size == 16 || size == 32);
    assert(addr >= DRAM32_BASE);
    assert(dram != NULL);

    uint32_t addrMapped = addr - DRAM32_BASE;
    switch(size) {
        case 8: return dram32_load_byte(dram, addrMapped); break;
        case 16: return dram32_load_halfWord(dram, addrMapped); break;
        case 32: return dram32_load_word(dram, addrMapped); break;
    }

    return 0;
}

/*
 * Returns 32 bit value, even though it only supports sizes 8 and 16.
 * This is the case, as in RISC-V there are only two instructions to load unsigned values from the dram:
 * One for bytes and one for halfwords, since unsigned word = word in a 32bit architecture
*/
uint32_t dram32_loadU(DRAM32 *dram, uint32_t addr, uint32_t size) {
    assert(size == 8 || size == 16);
    assert(addr >= DRAM32_BASE);
    assert(dram != NULL);

    uint32_t addrMapped = addr - DRAM32_BASE;
    switch(size) {
        case 8: return (uint32_t)(*((uint8_t *)(dram->mem + addrMapped))); break; // load byte
        case 16: return (uint32_t)(*((uint16_t *)(dram->mem + addrMapped))); break; // load halfword
    }

    return 0;
}


static uint32_t dram32_load_byte(DRAM32 *dram, uint32_t addr) {
    uint32_t value = *((uint8_t*)(dram->mem + addr));
    // sign extends the value coming from the addr
    if((value >> 7)) {
        uint32_t mask = ((1 << 24) - 1) << 8;
        value |= mask;
    }

    return value;
}

static uint32_t dram32_load_halfWord(DRAM32 *dram, uint32_t addr) {
    uint32_t value = *((uint16_t *)(dram->mem + addr));
    // sign extends the value coming from the addr
    if((value >> 15)) {
        uint32_t mask = ((1 << 16) - 1) << 16; 
        value |= mask;
    };

    return value;
}

static uint32_t dram32_load_word(DRAM32 *dram, uint32_t addr) {
    uint32_t value = *((uint32_t *)(dram->mem + addr));
    return value;
} 


static void dram32_store_byte(DRAM32 *dram, uint32_t addr, uint8_t value);
static void dram32_store_halfWord(DRAM32 *dram, uint32_t addr, uint16_t value);
static void dram32_store_word(DRAM32 *dram, uint32_t addr, uint32_t value);

void dram32_store(DRAM32 *dram, uint32_t addr, uint32_t size, uint32_t value) {
    assert(size == 8 || size == 16 || size == 32);
    printf("addr: %d\n", addr);
    assert(addr >= DRAM32_BASE);
    assert(dram != NULL);

    uint32_t addrMapped = addr - DRAM32_BASE;
    switch(size) {
        case 8: return dram32_store_byte(dram, addrMapped, value); break;
        case 16: return dram32_store_halfWord(dram, addrMapped, value); break;
        case 32: return dram32_store_word(dram, addrMapped, value); break;
    }
}

static void dram32_store_byte(DRAM32 *dram, uint32_t addr, uint8_t value) {
    *((uint8_t *)(dram->mem + addr)) = value;
}

static void dram32_store_halfWord(DRAM32 *dram, uint32_t addr, uint16_t value) {
    *((uint16_t *)(dram->mem + addr)) = value;
}

static void dram32_store_word(DRAM32* dram, uint32_t addr, uint32_t value) {
    *((uint32_t *)(dram->mem + addr)) = value;
}


DRAM32 *init_dram32() {
    DRAM32 *dram32 = (DRAM32*)malloc(sizeof(DRAM32));
    return dram32;
}

void free_dram32(DRAM32 *dram) {
    free(dram);
}