#include "cpu.h"

// R-type Instructions
#define RTYPE 0b0110011
#define STYPE 0b0100011
#define ITYPE 0b0000011
#define ITYPE2 0b0010011
#define ITYPE3 0b1100111
#define JTYPE 0b1101111
#define UTYPE 0b0010111
#define UTYPE2 0b0110111
#define BTYPE 1100011
 
vCPU32 *init_vCPU32(DRAM32 *dram) {
    vCPU32 *cpu = (vCPU32*)malloc(sizeof(vCPU32));
    cpu->pc = DRAM32_BASE;
    cpu->x[0] = 0;
    init_bus32(cpu->bus, dram);
}

void cpu32_run(vCPU32 *cpu) {

}

static void rtype(uint32_t inst, uint8_t rd);
static void stype(uint32_t inst, uint8_t imm1);
static void btype(uint32_t inst, uint8_t imm1);
static void utype(uint32_t inst, uint8_t rd);
static void utype2(uint32_t inst, uint8_t rd);
static void itype(uint32_t inst, uint8_t rd);
static void itype2(uint32_t inst, uint8_t rd);
static void itype3(uint32_t inst, uint8_t rd);
static void jtype(uint32_t inst, uint8_t rd);

static int cpu32_execute(vCPU32 *cpu) {
    uint32_t inst = bus32_load_dram(&cpu->bus, cpu->pc, 32);

    uint8_t op = inst & 0b01111111;
    inst >>= 7;
    uint8_t rd = inst & 0b00011111;
    inst >>= 5;
    switch (op)
    {
        case ITYPE:
            itype(inst, rd);
            break;
        case ITYPE2:
            itype2(inst, rd);
            break;
        case UTYPE:
            utype(inst, rd);
            break;
        case STYPE:
            stype(inst, rd);
            break;
        case RTYPE: 
            rtype(inst, rd);
            break;
        case UTYPE2:
            utype2(inst, rd);
            break;
        case BTYPE:
            btype(inst, rd);
            break;
        case ITYPE3:
            itype3(inst, rd);
            break;
        case JTYPE:
            jtype(inst, rd);
            break;
        default:
            return -1;
            break;
    }

    return 0;
}

#define ADD_INST 0
#define SUB_INST 0b0000100000
#define SLL_INST 0b0010000000
#define SLT_INST 0b0100000000
#define SLTU_INST 0b0110000000
#define XOR_INST 0b1000000000
#define SRL_INST 0b1010000000
#define SRA_INST 0b1010100000
#define OR_INST 0b1100000000
#define AND_INST 0b1110000000

void rtype(vCPU32* cpu, uint32_t inst, uint8_t rd)
{
    uint16_t funct = inst & 0b00000111;
    inst >>= 3;
    uint8_t rs1 = inst &0b00011111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b00011111;
    inst >>= 5;
    funct = (funct << 7) | inst;

    switch(funct) {
        case ADD_INST:
            cpu->x[rd] = (int32_t)(cpu->x[rs1]) + (int32_t)(cpu->x[rs2]);
            break;
        case SUB_INST:
            cpu->x[rd] = (int32_t)(cpu->x[rs1]) - (int32_t)(cpu->x[rs2]);
            break;
        case SLL_INST:
            cpu->x[rd] = cpu->x[rs1] << (cpu->x[rs2] & 0b1111);
            break;
        case SLT_INST:
            cpu->x[rd] = (int32_t)(cpu->x[rs1]) < (int32_t)(cpu->x[rs2]);
            break;
        case SLTU_INST:
            cpu->x[rd] = (uint32_t)(cpu->x[rs1]) < (uint32_t)(cpu->x[rs2]);
            break;
        case XOR_INST:
            cpu->x[rd] = cpu->x[rs1] ^ cpu->x[rs2];
            break;
        case SRL_INST:
            cpu->x[rd] = cpu->x[rs1] >> (cpu->x[rs2] & 0b1111);
            break;
        case SRA_INST:
            cpu->x[rd] = (int32_t)(cpu->x[rs1]) >> (cpu->x[rs2] & 0b1111);
        case OR_INST:
            cpu->x[rd] = cpu->x[rs1] | cpu->x[rs2];
            break;
        case AND_INST:
            cpu->x[rd] = cpu->x[rs1] & cpu->x[rs2];
            break;
        default:
            break;
    }

}

#define SB_INST 0b000
#define SH_INST 0b001
#define SW_INST 0b010

void stype(uint32_t inst, uint8_t imm1)
{
    uint8_t funct3 = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 5;

    switch(funct3) {
        case SB_INST:
            //TODO: store byte
            break;
        case SH_INST:
            //TODO: store half word
            break;
        case SW_INST:
            //TODO: store word
            break;
    }
}

#define BEQ_INST 0b000
#define BNE_INST 0b001
#define BLT_INST 0b100
#define BGE_INST 0b101
#define BLTU_INST 0b110
#define BGEU_INST 0b111

void btype(uint32_t inst, uint8_t imm1)
{
    uint8_t funct3 = inst & 0b111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b11111;
    inst >>= 5;
    uint8_t rs2 = inst & 0b11111;
    inst >>= 5;

    switch(funct3) {
        case BEQ_INST:
            //TODO: branch if equals
            break;
        case BNE_INST:
            //TODO: branch if not equals
            break;
        case BLT_INST:
            //TODO: branch if less than
            break;
        case BGE_INST:
            //TODO: branch if greater or equals
            break;
        case BLTU_INST:
            //TODO: branch if less than unsigned
            break;
        case BGEU_INST:
            //TODO: branch if greater or equals unsigned
            break
    }
}

void utype(uint32_t inst, uint8_t rd)
{
    //TODO: implement auipc
}

void utype2(uint32_t inst, uint8_t rd) {
    // TODO: implement load upper immediate
}

#define LB_INST 0b000
#define LH_INST 0b001
#define LW_INST 0b010
#define LBU_INST 0b100 
#define LHU_INST 0b101

void itype(uint32_t inst, uint8_t rd)
{
    uint8_t funct3 = inst & 0b00000111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b00011111;
    inst >>= 5;

    switch(funct3) {
        case LB_INST:
            //TODO: implement load byte instruction
            break;
        case LH_INST:
            //TODO: implement load half word instruction
            break;
        case LW_INST:
            //TODO: implement load word instruction
            break;
        case LBU_INST:
            //TODO: implement load byte unsigned
            break;
        case LHU_INST:
            //TODO: implement load half word unsigned
            break;
    }
}

void itype2(uint32_t inst, uint8_t rd) {
    uint8_t funct3 = inst & 0b00000111;
    inst >>= 3;
    uint8_t rs1 = inst & 0b00011111;
    inst >>= 5;

    switch(funct3) {
        case ADDI_INST:
            //TODO: implement addi inst
            break;
        case SLLI_INST:
            //TODO: implement set less than immediate inst
            break;
        case SLTIU_INST:
            //TODO: implement set less than immediate unsigned inst
            break;
        case XORI_INST:
            //TODO: implement xor immediate inst
            break;
        case SR_INST:
            uint8_t imm = inst & 00011111;
            inst >>= 5;
            switch(inst) {
                case SRLI_INST:
                    //TODO: implement shift right logical immediate
                    break;
                case SRAI_INST:
                    //TODO: implement shift right arithmetic immediate
                    break;
            }
            break;
        case ORI_INST:
            //TODO: implement or immediate inst
            break;
        case ANDI_INST:
            //TODO: implement and immediate inst
            break;
    }
}

void itype3(uint32_t inst, uint8_t rd) {
    assert((inst & 0b111) == 0);
    //TODO: implement jump and link register
}

void jtype(uint32_t inst, uint8_t rd) {
    //TODO: implement jump and link instruction
}