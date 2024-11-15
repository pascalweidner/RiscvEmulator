#include "includes.h"
#include "cpu.h"
#include "dram.h"


void read_file(DRAM32* dram, char *filename)
{
    FILE *file;
    uint32_t *buffer;
    unsigned long fileLen;

    //Open file
    file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s", filename);
    }


    uint32_t *pc = (uint32_t *)(dram->mem);
    uint32_t b;
    //1FE4
    while(fread(&b, sizeof(uint32_t), 1, file) == 1) {
        *pc = ((b >> 24) & 0x000000FF) |  // Move byte 3 to byte 0
           ((b >> 8)  & 0x0000FF00) |  // Move byte 2 to byte 1
           ((b << 8)  & 0x00FF0000) |  // Move byte 1 to byte 2
           ((b << 24) & 0xFF000000);
        pc++;
    }

    fclose(file);
}

int main() {
    DRAM32 *dram = init_dram32();
    vCPU32 *cpu = init_vCPU32(dram);

    read_file(dram, "trib5.out");

    cpu32_run(cpu);

    free_dram32(dram);
    free(cpu);
}