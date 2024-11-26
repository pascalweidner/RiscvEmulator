#include "machine.h"
#include "rv32i.h"
#include "rv32m.h"
#include "includes.h"
#include <ctype.h>

static VM32 *init_vm32() {
    VM32 *vm = (VM32*)calloc(1,sizeof(VM32));
    vm->dram = init_dram32();
    vm->cpu = init_vCPU32(vm->dram, vm->table, vm->rtypeTable);
    memset(vm->table, 0, sizeof(vm->table));
    memset(vm->rtypeTable, 0, sizeof(vm->rtypeTable));

    return vm;
}

static void register_handler(InstructionHandler *table, uint8_t opcode, InstructionHandler handler) {
    table[opcode] = handler;
}

static void register_rtypeHandler(RTypeInstructionHandler *table, uint16_t func, RTypeInstructionHandler handler) {
    table[func] = handler;
}

static void register_rv32i_instructions(InstructionHandler *table, RTypeInstructionHandler *rtypeTable) {
    register_handler(table, RTYPE, rtype);
    register_handler(table, STYPE, stype);
    register_handler(table, BTYPE, btype);
    register_handler(table, ITYPE, itype);
    register_handler(table, ITYPE2, itype2);
    register_handler(table, ITYPE3, itype3);
    register_handler(table, JTYPE, jtype);
    register_handler(table, UTYPE, utype);
    register_handler(table, UTYPE2, utype2);
    register_handler(table, BTYPE, btype);
    register_handler(table, EBREAK, ebreak);

    register_rtypeHandler(rtypeTable, ADD_INST, add_handler);
    register_rtypeHandler(rtypeTable, SUB_INST, sub_handler);
    register_rtypeHandler(rtypeTable, SLL_INST, sll_handler);
    register_rtypeHandler(rtypeTable, SLT_INST, slt_handler);
    register_rtypeHandler(rtypeTable, SLTU_INST, sltu_handler);
    register_rtypeHandler(rtypeTable, XOR_INST, xor_handler);
    register_rtypeHandler(rtypeTable, SRL_INST, srl_handler);
    register_rtypeHandler(rtypeTable, SRA_INST, sra_handler);
    register_rtypeHandler(rtypeTable, OR_INST, or_handler);
    register_rtypeHandler(rtypeTable, AND_INST, and_handler);
}

static void register_rv32m_instructions(RTypeInstructionHandler *rtypeTable) {
    register_rtypeHandler(rtypeTable, MUL_INST, mul_handler);
    register_rtypeHandler(rtypeTable, MUL_H_INST, mulh_handler);
    register_rtypeHandler(rtypeTable, MULHSU_INST, mulhsu_handler);
    register_rtypeHandler(rtypeTable, MULHU_INST, mulhu_handler);
    register_rtypeHandler(rtypeTable, DIV_INST, div_handler);
    register_rtypeHandler(rtypeTable, DIVU_INST, divu_handler);
    register_rtypeHandler(rtypeTable, REM_INST, rem_handler);
    register_rtypeHandler(rtypeTable, REMU_INST, remu_handler);

}

void toLowercase(char *str) {
    if (str == NULL) return; // Check for null pointer
    int i = 0;
    while (str[i] != '\0') {
        str[i] = (char)toupper(str[i]);
        printf("%c\n", *str);

        i++;
    }
    printf("end\n");
}

VM32 *create_vm(char *specs) {
    if(strncmp(specs, "rv32i", 5) != 0) exit(-1);


    VM32 *vm = init_vm32();

    register_rv32i_instructions(vm->table, vm->rtypeTable);

    printf("test\n");

    int i = 5;
    while(specs[i] != '\0') {
        switch(specs[i]) {
            case 'm':
                register_rv32m_instructions(vm->rtypeTable);
                break;
            default:
                fprintf(stderr, "this extension (%c) is not supported!", specs[i]);
                break;
        }
        i++;
    }

    return vm;

}