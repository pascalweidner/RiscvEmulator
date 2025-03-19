#include "bus.h"

uint32_t bus32_load_dram(BUS32 *bus, uint32_t addr, uint32_t size)
{
    return dram32_load(bus->dram, addr, size);
}

uint32_t bus32_loadU_dram(BUS32 *bus, uint32_t addr, uint32_t size)
{
    return dram32_loadU(bus->dram, addr, size);
}

void bus32_store_dram(BUS32 *bus, uint32_t addr, uint32_t size, uint32_t value)
{
    dram32_store(bus->dram, addr, size, value);
}

void init_bus32(BUS32 *bus, DRAM32 *dram)
{
    assert(bus != NULL);
    assert(dram != NULL);

    bus->dram = dram;
}
