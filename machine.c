#include "machine.h"

VM32 *init_vm32() {
    VM32 *vm = (VM32*)malloc(sizeof(VM32));
    vm->dram = init_dram32();
    vm->cpu = init_vCPU32(vm->dram);
}

VM32 *create_vm(char *specs) {
    
}