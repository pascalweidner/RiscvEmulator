#include "machine.h"
#include "32bit/rv32extensions/rv32i.h"
#include "32bit/rv32extensions/rv32m.h"
#include "32bit/rv32extensions/rv32f.h"
#include "cpu32.h"
#include <ctype.h>
#include <stdint.h>

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

static void register_frtypeHandler(FRTypeInstructionHandler *table, uint8_t func, FRTypeInstructionHandler handler) {
    table[func] = handler;
}

static void register_fitypeHandler(FITypeInstructionHandler *table, uint8_t func, FITypeInstructionHandler handler) {
    table[func] = handler;
}

static void register_fstypeHandler(FSTypeInstructionHandler *table, uint8_t func, FSTypeInstructionHandler handler) {
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

static void register_rv32f_instructions(InstructionHandler *table, FSTypeInstructionHandler *fstypeTable, FITypeInstructionHandler *fitypeTable, FRTypeInstructionHandler *frtypeTable) {
    register_handler(table, FRTYPE, frtype);
    register_handler(table, FMADD, fmadd_handler);
    register_handler(table, FMSUB, fmsub_handler);
    register_handler(table, FNMADD, fnmadd_handler);
    register_handler(table, FNMSUB, fnmsub_handler);
    register_handler(table, FSTYPE, fstype);
    register_handler(table, FITYPE, fitype);

    //register frtype instructions
    register_frtypeHandler(frtypeTable, FADD_INST, fadd_handler);
    register_frtypeHandler(frtypeTable, FSUB_INST, fsub_handler);
    register_frtypeHandler(frtypeTable, FMUL_INST, fmul_handler);
    register_frtypeHandler(frtypeTable, FDIV_INST, fdiv_handler);
    register_frtypeHandler(frtypeTable, FSQRT_INST, fsqrt_handler);
    register_frtypeHandler(frtypeTable, FMIN_MAX_INST, fmin_max_handler);
    register_frtypeHandler(frtypeTable, FEQ_FLT_FLE_INST, feq_lt_le_handler);
    register_frtypeHandler(frtypeTable, FSGN_INST, fsgn_handler);
    register_frtypeHandler(frtypeTable, FCVT_WS_INST, fcvt_ws_handler);
    register_frtypeHandler(frtypeTable, FMV_FCLASS_INST, fmv_fclass_handler);
    register_frtypeHandler(frtypeTable, FCVT_SW_INST, fcvt_sw_handler);
    register_frtypeHandler(frtypeTable, FMV_WX_INST, fmv_wx_handler);

    // register fitype instructions
    register_fitypeHandler(fitypeTable, FLW_INST, flw_handler);

    // register fstype instructions
    register_fstypeHandler(fstypeTable, FSW_INST, fsw_handler);

    //TODO: register fnmadd, fnmsub fnadd, fnsub
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

    int i = 5;
    while(specs[i] != '\0') {
        switch(specs[i]) {
            case 'm':
                register_rv32m_instructions(vm->rtypeTable);
                break;
            case 'f':
                vm->cpu->fcsr = 0;
                register_rv32f_instructions(vm->table, vm->fstypeTable, vm->fitypeTable, vm->frtypeTable);
                break;
            default:
                fprintf(stderr, "this extension (%c) is not supported!", specs[i]);
                break;
        }
        i++;
    }

    return vm;

}