#ifndef BUS_H
#define BUS_H
#include "../includes.h"
#include "dram.h"

typedef struct BUS32 {
    DRAM32 *dram;
} BUS32;

/*
    @param bus: takes the BUS32 instance of the vCPU32
    @param addr: address to where to store the data; must be greater than DRAM32_BASE
    @param size: size of the data to store in bits; must be either 8, 16, 32
    @throws exits program if size is not 8, 16, or 32
*/
void bus32_store_dram(BUS32 *bus, uint32_t addr, uint32_t size, uint32_t value); 

/*
    @param bus: instance of BUS32 of the vCPU32
    @param addr: address to where to store the data; must be greater than DRAM32_BASE
    @param size: size of the data to store in bits; must be either 8, 16, 32
    @throws exits program if size is not 8, 16, or 32
    @returns value at address (sign extended)
*/
uint32_t bus32_load_dram(BUS32 *bus, uint32_t addr, uint32_t size);


/*
    @param bus: instance of BUS32 of the vCPU32
    @param addr: address to where to store the data; must be greater than DRAM32_BASE
    @param size: size of the data to store in bits; must be either 8, 16, 32
    @throws exits program if size is not 8, 16, or 32
    @returns value at address (zero extended)
*/
uint32_t bus32_loadU_dram(BUS32 *bus, uint32_t addr, uint32_t size);

/*
    @param bus BUS32 instance of the vCPU32
    @param dram global DRAM32 instance of the machine
    @throws exits program if bus or dram are NULL
*/
void init_bus32(BUS32 *bus, DRAM32 *dram);

#endif