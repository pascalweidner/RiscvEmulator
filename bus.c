#include "bus.h"

uint32_t bus32_load_dram(BUS32 *bus, uint32_t addr, uint32_t size) {
    return dram32_load(bus->dram, addr, size);
}

void bus32_store_dram(BUS32 *bus, uint32_t addr, uint32_t size) {
    dram32_store(bus->dram, addr, size);
}

BUS32 *init_bus32(BUS32 *bus, DRAM32 *dram) {
    bus->dram = dram;

    return bus;
}
