#include "cpu32.h"

vCPU32 *init_vCPU32(DRAM32 *dram, InstructionHandler *table, RTypeInstructionHandler *rtypeTable) {
    vCPU32 *cpu = (vCPU32*)malloc(sizeof(vCPU32));
    cpu->pc = DRAM32_BASE;
    cpu->x[0] = 0;
    cpu->x[2] = DRAM32_BASE + DRAM_SIZE;
    init_bus32(&(cpu->bus), dram);
    cpu->table = table;
    cpu->rtypeTable = rtypeTable;
    return cpu;
}

static int cpu32_execute(vCPU32 *cpu);

static const char* register_names[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0/fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

static void register_dump(vCPU32 *cpu) {
    for(int i = 1; i < 32; i++) {
        printf("x%d/%s: %d; ", i, register_names[i], cpu->x[i]);
    }
    printf("\n");
}

void cpu32_run(vCPU32 *cpu) {
    int c = 0;

    while(true) {
        int t = cpu32_execute(cpu);
        cpu->x[0] = 0;
        register_dump(cpu);
        c++;
    }
}

static int cpu32_execute(vCPU32 *cpu) {
    uint32_t inst = bus32_load_dram(&cpu->bus, cpu->pc, 32);

    uint8_t op = inst & 0b01111111;
    inst >>= 7;
    uint8_t rd = inst & 0b00011111;
    inst >>= 5;

    printf("opcode %d\n", op);
    cpu->table[op](cpu, inst, rd);

    return 0;
}

void rtype(vCPU32* cpu, uint32_t inst, uint8_t rd)
{
    uint16_t funct = inst & 0b00000111;
    inst >>= 3;
    uint8_t rs1 = inst &0b00011111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b00011111;
    inst >>= 5;
    funct = (funct << 7) | inst;

    ((cpu->rtypeTable)[funct])(cpu, rd, rs1, rs2);

    cpu->pc += 4;
}

#define SB_INST 0b000
#define SH_INST 0b001
#define SW_INST 0b010

void stype(vCPU32 *cpu, uint32_t inst, uint8_t imm1)
{
    uint8_t funct3 = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 5;

    uint16_t imm = imm1 | (inst << 5);

    printf("register: %d\n", rs1);

    /*TODO: check for misalignment (Regardless of EEI, loads and stores whose effective addresses are naturally aligned shall not raise an
address-misaligned exception. Loads and stores whose effective address is not naturally aligned to the
referenced datatype (i.e., the effective address is not divisible by the size of the access in bytes) have
behavior dependent on the EEI.)*/
    switch(funct3) {
        case SB_INST:
            bus32_store_dram(&(cpu->bus), cpu->x[rs1] + imm, 8, cpu->x[rs2]);
            break;
        case SH_INST:
            bus32_store_dram(&(cpu->bus), cpu->x[rs1] + imm, 16, cpu->x[rs2]);
            break;
        case SW_INST:
            bus32_store_dram(&(cpu->bus), cpu->x[rs1] + imm, 32, cpu->x[rs2]);
            break;
    }

    cpu->pc += 4;
}

#define BEQ_INST 0b000
#define BNE_INST 0b001
#define BLT_INST 0b100
#define BGE_INST 0b101
#define BLTU_INST 0b110
#define BGEU_INST 0b111

void btype(vCPU32* cpu, uint32_t inst, uint8_t imm1)
{
    uint8_t funct3 = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 5;

    int32_t imm12 = (inst >> 6);
    if(imm12 == 1) imm12 = -1;
    printf("test: %d\n", imm12);
    uint8_t imm10_5 = inst & 0x3F;
    uint16_t imm11 = (imm1 & 0x1) << 11;
    uint8_t imm4_1 = (imm1 >> 1) & 0xF;

    // FEA2 C2E3
    int32_t imm = (imm12 << 12) | imm11 | (imm10_5 << 5) | (imm4_1 << 1);
    printf("%d\n", imm);

    switch(funct3) {
        case BEQ_INST:
            if(cpu->x[rs1] == cpu->x[rs2]) cpu->pc += imm;
            cpu->pc += 4;
            break;
        case BNE_INST:
            if(cpu->x[rs1] != cpu->x[rs2]) cpu->pc += imm;
            else cpu->pc += 4;
            break;
        case BLT_INST:
            if((int32_t)(cpu->x[rs1]) < (int32_t)(cpu->x[rs2])) cpu->pc += imm;
            else cpu->pc += 4;
            break;
        case BGE_INST:
            if((int32_t)(cpu->x[rs1]) >= (int32_t)(cpu->x[rs2])) cpu->pc += imm;
            cpu->pc += 4;
            break;
        case BLTU_INST:
            if(cpu->x[rs1] < cpu->x[rs2]) cpu->pc += imm;
            cpu->pc += 4;
            break;
        case BGEU_INST:
            if(rs1 >= rs2) cpu->pc += imm;
            cpu->pc += 4;
            break;
    }
}

void utype(vCPU32 *cpu, uint32_t inst, uint8_t rd)
{
    //TODO: implement auipc
    fprintf(stderr, "not implemented");
    exit(-1);
}

void utype2(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint32_t imm = inst << 12;
    cpu->x[rd] = imm;

    cpu->pc += 4;
}

#define LB_INST 0b000
#define LH_INST 0b001
#define LW_INST 0b010
#define LBU_INST 0b100 
#define LHU_INST 0b101

void itype(vCPU32 *cpu, uint32_t inst, uint8_t rd)
{
    uint8_t funct3 = inst & 0b00000111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b00011111;
    inst >>= 5;

    switch(funct3) {
        case LB_INST:
            cpu->x[rd] = bus32_load_dram(&(cpu->bus), cpu->x[rs1] + inst, 8);
            break;
        case LH_INST:
            cpu->x[rd] = bus32_load_dram(&(cpu->bus), cpu->x[rs1] + inst, 16);
            break;
        case LW_INST:
            cpu->x[rd] = bus32_load_dram(&(cpu->bus), cpu->x[rs1] + inst, 32);
            break;
        case LBU_INST:
            cpu->x[rd] = bus32_loadU_dram(&(cpu->bus), cpu->x[rs1] + inst, 8);
            break;
        case LHU_INST:
            cpu->x[rd] = bus32_loadU_dram(&(cpu->bus), cpu->x[rs1] + inst, 16);
            break;
    }

    cpu->pc += 4;
}

#define ADDI_INST 0b000
#define SLLI_INST 0b001
#define SLTI_INST 0b010
#define SLTIU_INST 0b011
#define XORI_INST 0b100
#define SR_INST 0b101
#define ORI_INST 0b110
#define ANDI_INST 0b111
#define SRLI_INST 0
#define SRAI_INST 0b0100000

void itype2(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint8_t funct3 = inst & 0b00000111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b00011111;
    inst >>= 5;

    switch(funct3) {
        case ADDI_INST:
            if((inst >> 10)) inst |= 0xFFFFF000;
            cpu->x[rd] = (int32_t)(cpu->x[rs1]) + (int32_t)(inst);
            break;
        case SLLI_INST:
            cpu->x[rd] = (cpu->x[rs1]) << inst;
            break;
        case SLTI_INST:
            if((inst >> 10)) inst |= 0xFFFFF000;
            cpu->x[rd] = (int32_t)(cpu->x[rs1]) < (int32_t)(inst);
            break;
        case SLTIU_INST:
            if((inst >> 10)) inst |= 0xFFFFF000;
            cpu->x[rd] = cpu->x[rs1] < (uint32_t)(inst);
            break;
        case XORI_INST:
            if((inst >> 10)) inst |= 0xFFFFF000;
            cpu->x[rd] = cpu->x[rs1] ^ (uint32_t)(inst);
            break;
        case SR_INST:
            uint8_t imm = inst & 00011111;
            inst >>= 5;
            switch(inst) {
                case SRLI_INST:
                    cpu->x[rd] = cpu->x[rs1] >> imm;
                    break;
                case SRAI_INST:
                    cpu->x[rd] = (int32_t)(cpu->x[rs1]) >> imm;
                    break;
            }
            break;
        case ORI_INST:
            if((inst >> 10)) inst |= 0xFFFFF000;
            cpu->x[rd] = cpu->x[rs1] | inst;
            break;
        case ANDI_INST:
            if((inst >> 10)) inst |= 0xFFFFF000;
            cpu->x[rd] = cpu->x[rs1] & inst;
            break;
    }

    cpu->pc += 4;
}

void itype3(vCPU32* cpu, uint32_t inst, uint8_t rd) {
    assert((inst & 0b111) == 0);
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;


    if((inst >> 10)) inst |= 0xFFFFF000;
    cpu->x[rd] = cpu->pc + 4;
    cpu->pc = inst + cpu->x[rs1];
}

void jtype(vCPU32* cpu, uint32_t inst, uint8_t rd) {
    uint8_t imm19_12 = inst & 0xFF;
    inst >>= 8;
    uint8_t imm11 = inst & 0x1;
    inst >>= 1; 
    uint16_t imm10_1 = inst & 0b1111111111;
    inst >>= 1;
    int8_t imm20 = inst & 0x1;
    if (imm20 == 1) imm20 = -1;
 
    int32_t imm = (imm10_1 << 1) | (imm11 << 11) | (imm19_12 << 12) | (imm20 << 20);
    cpu->x[rd] = cpu->pc + 4;
    cpu->pc += (imm);
}

void ebreak(vCPU32* cpu, uint32_t inst, uint8_t rd) {
    exit(0);
}

void fitype(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint8_t funct3 = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint16_t imm = inst;

    cpu->fitypeTable[funct3](cpu, imm, rd, rs1);
}

void frtype(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint8_t rm = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 5;
    uint8_t funct7 = inst;

    cpu->frtypeTable[funct7](cpu, rd, rs1, rs2, rm);
}

void fstype(vCPU32 *cpu, uint32_t inst, uint8_t rd) {
    uint8_t funct3 = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 5;
    uint16_t imm = inst << 5;
    imm |= rd;

    cpu->fstypeTable[funct3](cpu, imm, rs1, rs2);
}