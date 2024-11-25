#include "includes.h"
#include "rv32i.h"
#include "dram.h"
#include "machine.h"


void read_file(DRAM32* dram, char *filename)
{
    FILE *file;

    printf("%s\n", filename);
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Unable to open file %s", filename);
    }

    printf("test4\n");

/*

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
    */
    fclose(file);
}

int main() {
    printf("test\n");
    VM32 *vm = create_vm("rv32i\0");
    printf("test2\n");

    read_file(vm->dram, "/mnt/c/Development/RiscvEmulator/RiscvEmulator/trib5.out");

    printf("test3\n");

    cpu32_run(vm->cpu);

    free(vm->cpu);
    free(vm->dram);
    free(vm);
}