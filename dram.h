#ifndef DRAM_H
#define DRAM_H
#include "includes.h"

#define DRAM_SIZE 1024 * 1024 * 1
#define DRAM32_BASE 0x80000000

typedef struct DRAM32 {
    uint8_t mem[DRAM_SIZE];
} DRAM32;


/*
    @param dram: pointer to a global dram instance, where the data is stored
    @param addr: address, from where you want to get the data
    @param size: how many bits you want to get; must either be 8, 16, 32
    @throws program exits if the size is not 8, 16, or 32
    @returns the data that you requested stored as an 32bit value (sign extended)
*/
uint32_t dram32_load(DRAM32 *dram, uint32_t addr, uint32_t size);

/*
    @param dram: pointer to a global dram instance, where the data is stored
    @param addr: address, from where you want to get the data
    @param size: how many bits you want to get; must either be 8, 16, 32
    @throws program exits if the size is not 8, 16, or 32
    @returns the data that you requested stored as an 32bit value (zero extended)
*/
uint32_t dram32_loadU(DRAM32 *dram, uint32_t addr, uint32_t size);

/*
    @param dram: pointer to a global dram instance, where the data is stored to
    @param addr: address, to where to store the byte in the dram
    @param size: size of the value to be stored in bits; must either be 8, 16 or 32
    @param value: value that will be stored; only the size least significant bits are stored of value
    @throws program exits if size is not 8, 16 or 32
*/
void dram32_store(DRAM32 *dram, uint32_t addr, uint32_t size, uint32_t value);

DRAM32 *init_dram32();

void free_dram32(DRAM32 *dram);

#endif