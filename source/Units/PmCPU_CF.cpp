#include "PmCPU.h"
#include "Base/PmRegMap.h"

GS_UINT PmCPU::ExecCF(void)
{
    GS_BYTE I8A;
    GS_UINT16 I16;

    // Read IR
    R_IR = Fetch8();

    // Process instruction
    switch(R_IR) {
    case 0x00: // ADD BA, BA
        R_BA = ADD16(R_BA, R_BA);
        return 16;
    case 0x01: // ADD BA, HL
        R_BA = ADD16(R_BA, R_HL);
        return 16;
    case 0x02: // ADD BA, X
        R_BA = ADD16(R_BA, R_X);
        return 16;
    case 0x03: // ADD BA, Y
        R_BA = ADD16(R_BA, R_Y);
        return 16;

    case 0x04: // ADC BA, BA
        R_BA = ADC16(R_BA, R_BA);
        return 16;
    case 0x05: // ADC BA, HL
        R_BA = ADC16(R_BA, R_HL);
        return 16;
    case 0x06: // ADC BA, X
        R_BA = ADC16(R_BA, R_X);
        return 16;
    case 0x07: // ADC BA, Y
        R_BA = ADC16(R_BA, R_Y);
        return 16;

    case 0x08: // SUB BA, BA
        R_BA = SUB16(R_BA, R_BA);
        return 16;
    case 0x09: // SUB BA, HL
        R_BA = SUB16(R_BA, R_HL);
        return 16;
    case 0x0A: // SUB BA, X
        R_BA = SUB16(R_BA, R_X);
        return 16;
    case 0x0B: // SUB BA, Y
        R_BA = SUB16(R_BA, R_Y);
        return 16;

    case 0x0C: // SBC BA, BA
        R_BA = SBC16(R_BA, R_BA);
        return 16;
    case 0x0D: // SBC BA, HL
        R_BA = SBC16(R_BA, R_HL);
        return 16;
    case 0x0E: // SBC BA, X
        R_BA = SBC16(R_BA, R_X);
        return 16;
    case 0x0F: // SBC BA, Y
        R_BA = SBC16(R_BA, R_Y);
        return 16;

    case 0x10: // *ADD BA, BA
        R_BA = ADD16(R_BA, R_BA);
        return 16;
    case 0x11: // *ADD BA, HL
        R_BA = ADD16(R_BA, R_HL);
        return 16;
    case 0x12: // *ADD BA, X
        R_BA = ADD16(R_BA, R_X);
        return 16;
    case 0x13: // *ADD BA, Y
        R_BA = ADD16(R_BA, R_Y);
        return 16;

    case 0x14: // *ADC BA, BA
        R_BA = ADC16(R_BA, R_BA);
        return 16;
    case 0x15: // *ADC BA, HL
        R_BA = ADC16(R_BA, R_HL);
        return 16;
    case 0x16: // *ADC BA, X
        R_BA = ADC16(R_BA, R_X);
        return 16;
    case 0x17: // *ADC BA, Y
        R_BA = ADC16(R_BA, R_Y);
        return 16;

    case 0x18: // CMP BA, BA
        SUB16(R_BA, R_BA);
        return 16;
    case 0x19: // CMP BA, HL
        SUB16(R_BA, R_HL);
        return 16;
    case 0x1A: // CMP BA, X
        SUB16(R_BA, R_X);
        return 16;
    case 0x1B: // CMP BA, Y
        SUB16(R_BA, R_Y);
        return 16;

    case 0x1C: // *SBC BA, BA
        R_BA = SBC16(R_BA, R_BA);
        return 16;
    case 0x1D: // *SBC BA, HL
        R_BA = SBC16(R_BA, R_HL);
        return 16;
    case 0x1E: // *SBC BA, X
        R_BA = SBC16(R_BA, R_X);
        return 16;
    case 0x1F: // *SBC BA, Y
        R_BA = SBC16(R_BA, R_Y);
        return 16;

    case 0x20: // ADD HL, BA
        R_HL = ADD16(R_HL, R_BA);
        return 16;
    case 0x21: // ADD HL, HL
        R_HL = ADD16(R_HL, R_HL);
        return 16;
    case 0x22: // ADD HL, X
        R_HL = ADD16(R_HL, R_X);
        return 16;
    case 0x23: // ADD HL, Y
        R_HL = ADD16(R_HL, R_Y);
        return 16;

    case 0x24: // ADC HL, BA
        R_HL = ADC16(R_HL, R_BA);
        return 16;
    case 0x25: // ADC HL, HL
        R_HL = ADC16(R_HL, R_HL);
        return 16;
    case 0x26: // ADC HL, X
        R_HL = ADC16(R_HL, R_X);
        return 16;
    case 0x27: // ADC HL, Y
        R_HL = ADC16(R_HL, R_Y);
        return 16;

    case 0x28: // SUB HL, BA
        R_HL = SUB16(R_HL, R_BA);
        return 16;
    case 0x29: // SUB HL, HL
        R_HL = SUB16(R_HL, R_HL);
        return 16;
    case 0x2A: // SUB HL, X
        R_HL = SUB16(R_HL, R_X);
        return 16;
    case 0x2B: // SUB HL, Y
        R_HL = SUB16(R_HL, R_Y);
        return 16;

    case 0x2C: // SBC HL, BA
        R_HL = SBC16(R_HL, R_BA);
        return 16;
    case 0x2D: // SBC HL, HL
        R_HL = SBC16(R_HL, R_HL);
        return 16;
    case 0x2E: // SBC HL, X
        R_HL = SBC16(R_HL, R_X);
        return 16;
    case 0x2F: // SBC HL, Y
        R_HL = SBC16(R_HL, R_Y);
        return 16;

    case 0x30: // *ADD HL, BA
        R_HL = ADD16(R_HL, R_BA);
        return 16;
    case 0x31: // *ADD BA, HL
        R_HL = ADD16(R_HL, R_HL);
        return 16;
    case 0x32: // *ADD BA, X
        R_HL = ADD16(R_HL, R_X);
        return 16;
    case 0x33: // *ADD BA, Y
        R_HL = ADD16(R_HL, R_Y);
        return 16;

    case 0x34: // *ADC HL, BA
        R_HL = ADC16(R_HL, R_BA);
        return 16;
    case 0x35: // *ADC HL, HL
        R_HL = ADC16(R_HL, R_HL);
        return 16;
    case 0x36: // *ADC HL, X
        R_HL = ADC16(R_HL, R_X);
        return 16;
    case 0x37: // *ADC HL, Y
        R_HL = ADC16(R_HL, R_Y);
        return 16;

    case 0x38: // CMP HL, BA
        SUB16(R_HL, R_BA);
        return 16;
    case 0x39: // CMP HL, HL
        SUB16(R_HL, R_HL);
        return 16;
    case 0x3A: // CMP HL, X
        SUB16(R_HL, R_X);
        return 16;
    case 0x3B: // CMP HL, Y
        SUB16(R_HL, R_Y);
        return 16;

    case 0x3C: // *SBC BA, BA
        R_BA = SBC16(R_BA, R_BA);
        return 16;
    case 0x3D: // *SBC BA, HL
        R_BA = SBC16(R_BA, R_HL);
        return 16;
    case 0x3E: // *SBC BA, X
        R_BA = SBC16(R_BA, R_X);
        return 16;
    case 0x3F: // *SBC BA, Y
        R_BA = SBC16(R_BA, R_Y);
        return 16;

    case 0x40: // ADD X, BA
        R_X = ADD16(R_X, R_BA);
        return 16;
    case 0x41: // ADD X, HL
        R_X = ADD16(R_X, R_HL);
        return 16;
    case 0x42: // ADD Y, BA
        R_Y = ADD16(R_Y, R_BA);
        return 16;
    case 0x43: // ADD Y, HL
        R_Y = ADD16(R_Y, R_HL);
        return 16;
    case 0x44: // ADD SP, BA
        R_SP = ADD16(R_SP, R_BA);
        return 16;
    case 0x45: // ADD SP, HL
        R_SP = ADD16(R_SP, R_HL);
        return 16;
    case 0x46: // CRASH
        GS_ASSERT(GS_FALSE);
        return 16;
    case 0x47: // CRASH
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x48: // SUB X, BA
        R_X = SUB16(R_X, R_BA);
        return 16;
    case 0x49: // SUB X, HL
        R_X = SUB16(R_X, R_HL);
        return 16;
    case 0x4A: // SUB Y, BA
        R_Y = SUB16(R_Y, R_BA);
        return 16;
    case 0x4B: // SUB Y, HL
        R_Y = SUB16(R_Y, R_HL);
        return 16;
    case 0x4C: // SUB SP, BA
        R_SP = SUB16(R_SP, R_BA);
        return 16;
    case 0x4D: // SUB SP, HL
        R_SP = SUB16(R_SP, R_HL);
        return 16;
    case 0x4E: // CRASH
        GS_ASSERT(GS_FALSE);
        return 16;
    case 0x4F: // CRASH
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x50: // *ADD X, BA
        R_X = ADD16(R_X, R_BA);
        return 16;
    case 0x51: // *ADD X, HL
        R_X = ADD16(R_X, R_HL);
        return 16;
    case 0x52: // *ADD Y, BA
        R_Y = ADD16(R_Y, R_BA);
        return 16;
    case 0x53: // *ADD Y, HL
        R_Y = ADD16(R_Y, R_HL);
        return 16;
    case 0x54: // *ADD SP, BA
        R_SP = ADD16(R_SP, R_BA);
        return 16;
    case 0x55: // *ADD SP, HL
        R_SP = ADD16(R_SP, R_HL);
        return 16;
    case 0x56: // CRASH
        GS_ASSERT(GS_FALSE);
        return 16;
    case 0x57: // CRASH
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x58: // *SUB X, BA
        R_X = SUB16(R_X, R_BA);
        return 16;
    case 0x59: // *SUB X, HL
        R_X = SUB16(R_X, R_HL);
        return 16;
    case 0x5A: // *SUB Y, BA
        R_Y = SUB16(R_Y, R_BA);
        return 16;
    case 0x5B: // *SUB Y, HL
        R_Y = SUB16(R_Y, R_HL);
        return 16;
    case 0x5C: // CMP SP, BA
        SUB16(R_SP, R_BA);
        return 16;
    case 0x5D: // CMP SP, HL
        SUB16(R_SP, R_HL);
        return 16;
    case 0x5E: // CRASH
        GS_ASSERT(GS_FALSE);
        return 16;
    case 0x5F: // CRASH
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x60: // ADC BA, #nnnn
        I16 = Fetch16();
        R_BA = ADC16(R_BA, I16);
        return 16;
    case 0x61: // ADC HL, #nnnn
        I16 = Fetch16();
        R_HL = ADC16(R_HL, I16);
        return 16;
    case 0x62: // SBC BA, #nnnn
        I16 = Fetch16();
        R_BA = SBC16(R_BA, I16);
        return 16;
    case 0x63: // SBC HL, #nnnn
        I16 = Fetch16();
        R_HL = SBC16(R_HL, I16);
        return 16;

    case 0x64: // UNSTABLE
        GS_ASSERT(GS_FALSE);
        return 16;
    case 0x65: // UNSTABLE
        GS_ASSERT(GS_FALSE);
        return 16;
    case 0x66: // UNSTABLE
        GS_ASSERT(GS_FALSE);
        return 16;
    case 0x67: // UNSTABLE
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x68: // ADD SP, #nnnn
        I16 = Fetch16();
        R_SP = ADD16(R_SP, I16);
        return 16;
    case 0x69: // UNSTABLE
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x6A: // SUB SP, #nnnn
        I16 = Fetch16();
        R_SP = SUB16(R_SP, I16);
        return 16;
    case 0x6B: // UNSTABLE
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x6C: // CMP SP, #nnnn
        I16 = Fetch16();
        SUB16(R_SP, I16);
        return 16;
    case 0x6D: // UNSTABLE
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x6E: // MOV SP, #nnnn
        I16 = Fetch16();
        R_SP = I16;
        return 16;
    case 0x6F: // UNSTABLE
        GS_ASSERT(GS_FALSE);
        return 16;

    case 0x70: // MOV BA, [SP+#ss]
        I8A = Fetch8();
        I16 = R_SP + S8_TO_16(I8A);
        R_A = Read8(I16++);
        R_B = Read8(I16);
        return 24;
    case 0x71: // MOV HL, [SP+#ss]
        I8A = Fetch8();
        I16 = R_SP + S8_TO_16(I8A);
        R_L = Read8(I16++);
        R_H = Read8(I16);
        return 24;
    case 0x72: // MOV X, [SP+#ss]
        I8A = Fetch8();
        I16 = R_SP + S8_TO_16(I8A);
        R_X_L = Read8(I16++);
        R_X_H = Read8(I16);
        return 24;
    case 0x73: // MOV Y, [SP+#ss]
        I8A = Fetch8();
        I16 = R_SP + S8_TO_16(I8A);
        R_Y_L = Read8(I16++);
        R_Y_H = Read8(I16);
        return 24;

    case 0x74: // MOV [SP+#ss], BA
        I8A = Fetch8();
        I16 = R_SP + S8_TO_16(I8A);
        Write8(I16++, R_A);
        Write8(I16, R_B);
        return 24;
    case 0x75: // MOV [SP+#ss], HL
        I8A = Fetch8();
        I16 = R_SP + S8_TO_16(I8A);
        Write8(I16++, R_L);
        Write8(I16, R_H);
        return 24;
    case 0x76: // MOV [SP+#ss], X
        I8A = Fetch8();
        I16 = R_SP + S8_TO_16(I8A);
        Write8(I16++, R_X_L);
        Write8(I16, R_X_H);
        return 24;
    case 0x77: // MOV [SP+#ss], Y
        I8A = Fetch8();
        I16 = R_SP + S8_TO_16(I8A);
        Write8(I16++, R_Y_L);
        Write8(I16, R_Y_H);
        return 24;

    case 0x78: // MOV SP, [#nnnn]
        I16 = Fetch16();
        R_SP_L = Read8((R_I << 16) | I16++);
        R_SP_H = Read8((R_I << 16) | I16);
        return 24;

    case 0x79: // ??? #nn
    case 0x7A: // ??? #nn
    case 0x7B: // ??? #nn
        GS_ASSERT(GS_FALSE);
        //return MinxCPU_ExecSPCF();
        return 64;

    case 0x7C: // MOV [#nnnn], SP
        I16 = Fetch16();
        Write8((R_I << 16) | I16++, R_SP_L);
        Write8((R_I << 16) | I16, R_SP_H);
        return 24;

    case 0x7D: case 0x7E: case 0x7F: // ??? #nn
    case 0x80: case 0x81: case 0x82: case 0x83:
    case 0x84: case 0x85: case 0x86: case 0x87:
    case 0x88: case 0x89: case 0x8A: case 0x8B:
    case 0x8C: case 0x8D: case 0x8E: case 0x8F:
    case 0x90: case 0x91: case 0x92: case 0x93:
    case 0x94: case 0x95: case 0x96: case 0x97:
    case 0x98: case 0x99: case 0x9A: case 0x9B:
    case 0x9C: case 0x9D: case 0x9E: case 0x9F:
    case 0xA0: case 0xA1: case 0xA2: case 0xA3:
    case 0xA4: case 0xA5: case 0xA6: case 0xA7:
    case 0xA8: case 0xA9: case 0xAA: case 0xAB:
    case 0xAC: case 0xAD: case 0xAE: case 0xAF:
        GS_ASSERT(GS_FALSE);
        //return MinxCPU_ExecSPCF();
        return 64;

    case 0xB0: // PUSH A
        PUSH(R_A);
        return 12;
    case 0xB1: // PUSH B
        PUSH(R_B);
        return 12;
    case 0xB2: // PUSH L
        PUSH(R_L);
        return 12;
    case 0xB3: // PUSH H
        PUSH(R_H);
        return 12;

    case 0xB4: // POP A
        R_A = POP();
        return 12;
    case 0xB5: // POP B
        R_B = POP();
        return 12;
    case 0xB6: // POP L
        R_L = POP();
        return 12;
    case 0xB7: // POP H
        R_H = POP();
        return 12;
    case 0xB8: // PUSHA
        PUSH(R_B);
        PUSH(R_A);
        PUSH(R_H);
        PUSH(R_L);
        PUSH(R_X_H);
        PUSH(R_X_L);
        PUSH(R_Y_H);
        PUSH(R_Y_L);
        PUSH(R_N);
        return 48;
    case 0xB9: // PUSHAX
        PUSH(R_B);
        PUSH(R_A);
        PUSH(R_H);
        PUSH(R_L);
        PUSH(R_X_H);
        PUSH(R_X_L);
        PUSH(R_Y_H);
        PUSH(R_Y_L);
        PUSH(R_N);
        PUSH(R_I);
        PUSH(R_XI);
        PUSH(R_YI);
        return 60;

    case 0xBA: case 0xBB: // ??? #n
        GS_ASSERT(GS_FALSE);
        //return MinxCPU_ExecSPCF();
        return 64;

    case 0xBC: // POPA
        R_N = POP();
        R_Y_L = POP();
        R_Y_H = POP();
        R_X_L = POP();
        R_X_H = POP();
        R_L = POP();
        R_H = POP();
        R_A = POP();
        R_B = POP();
        return 44;

    case 0xBD: // POPAX
        R_YI = POP();
        R_XI = POP();
        R_I = POP();
        R_N_I = R_I;
        R_N = POP();
        R_Y_L = POP();
        R_Y_H = POP();
        R_X_L = POP();
        R_X_H = POP();
        R_L = POP();
        R_H = POP();
        R_A = POP();
        R_B = POP();
        return 56;

    case 0xBE: case 0xBF: // ??? #n
        GS_ASSERT(GS_FALSE);
        //return MinxCPU_ExecSPCF();
        return 64;

    case 0xC0: // MOV BA, [HL]
        I16 = R_HL;
        R_A = Read8((R_I << 16) | I16++);
        R_B = Read8((R_I << 16) | I16);
        return 20;
    case 0xC1: // MOV HL, [HL]
        I16 = R_HL;
        R_L = Read8((R_I << 16) | I16++);
        R_H = Read8((R_I << 16) | I16);
        return 20;
    case 0xC2: // MOV X, [HL]
        I16 = R_HL;
        R_X_L = Read8((R_I << 16) | I16++);
        R_X_H = Read8((R_I << 16) | I16);
        return 20;
    case 0xC3: // MOV Y, [HL]
        I16 = R_HL;
        R_Y_L = Read8((R_I << 16) | I16++);
        R_Y_H = Read8((R_I << 16) | I16);
        return 20;

    case 0xC4: // MOV [HL], BA
        I16 = R_HL;
        Write8((R_I << 16) | I16++, R_A);
        Write8((R_I << 16) | I16, R_B);
        return 20;
    case 0xC5: // MOV [HL], HL
        I16 = R_HL;
        Write8((R_I << 16) | I16++, R_L);
        Write8((R_I << 16) | I16, R_H);
        return 20;
    case 0xC6: // MOV [HL], X
        I16 = R_HL;
        Write8((R_I << 16) | I16++, R_X_L);
        Write8((R_I << 16) | I16, R_X_H);
        return 20;
    case 0xC7: // MOV [HL], Y
        I16 = R_HL;
        Write8((R_I << 16) | I16++, R_Y_L);
        Write8((R_I << 16) | I16, R_Y_H);
        return 20;

    case 0xC8: case 0xC9: case 0xCA: case 0xCB: // MOV B, V
    case 0xCC: case 0xCD: case 0xCE: case 0xCF:
        R_B = R_PC_V;
        return 12;

    case 0xD0: // MOV BA, [X]
        I16 = R_X;
        R_A = Read8((R_XI << 16) | I16++);
        R_B = Read8((R_XI << 16) | I16);
        return 20;
    case 0xD1: // MOV HL, [X]
        I16 = R_X;
        R_L = Read8((R_XI << 16) | I16++);
        R_H = Read8((R_XI << 16) | I16);
        return 20;
    case 0xD2: // MOV X, [X]
        I16 = R_X;
        R_X_L = Read8((R_XI << 16) | I16++);
        R_X_H = Read8((R_XI << 16) | I16);
        return 20;
    case 0xD3: // MOV Y, [X]
        I16 = R_X;
        R_Y_L = Read8((R_XI << 16) | I16++);
        R_Y_H = Read8((R_XI << 16) | I16);
        return 20;

    case 0xD4: // MOV [X], BA
        I16 = R_X;
        Write8((R_XI << 16) | I16++, R_A);
        Write8((R_XI << 16) | I16, R_B);
        return 20;
    case 0xD5: // MOV [X], HL
        I16 = R_X;
        Write8((R_XI << 16) | I16++, R_L);
        Write8((R_XI << 16) | I16, R_H);
        return 20;
    case 0xD6: // MOV [X], X
        I16 = R_X;
        Write8((R_XI << 16) | I16++, R_X_L);
        Write8((R_XI << 16) | I16, R_X_H);
        return 20;
    case 0xD7: // MOV [X], Y
        I16 = R_X;
        Write8((R_XI << 16) | I16++, R_Y_L);
        Write8((R_XI << 16) | I16, R_Y_H);
        return 20;

    case 0xD8: // MOV BA, [Y]
        I16 = R_Y;
        R_A = Read8((R_YI << 16) | I16++);
        R_B = Read8((R_YI << 16) | I16);
        return 20;
    case 0xD9: // MOV HL, [Y]
        I16 = R_Y;
        R_L = Read8((R_YI << 16) | I16++);
        R_H = Read8((R_YI << 16) | I16);
        return 20;
    case 0xDA: // MOV X, [Y]
        I16 = R_Y;
        R_X_L = Read8((R_YI << 16) | I16++);
        R_X_H = Read8((R_YI << 16) | I16);
        return 20;
    case 0xDB: // MOV Y, [Y]
        I16 = R_Y;
        R_Y_L = Read8((R_YI << 16) | I16++);
        R_Y_H = Read8((R_YI << 16) | I16);
        return 20;

    case 0xDC: // MOV [Y], BA
        I16 = R_Y;
        Write8((R_YI << 16) | I16++, R_A);
        Write8((R_YI << 16) | I16, R_B);
        return 20;
    case 0xDD: // MOV [Y], HL
        I16 = R_Y;
        Write8((R_YI << 16) | I16++, R_L);
        Write8((R_YI << 16) | I16, R_H);
        return 20;
    case 0xDE: // MOV [Y], X
        I16 = R_Y;
        Write8((R_YI << 16) | I16++, R_X_L);
        Write8((R_YI << 16) | I16, R_X_H);
        return 20;
    case 0xDF: // MOV [Y], Y
        I16 = R_Y;
        Write8((R_YI << 16) | I16++, R_Y_L);
        Write8((R_YI << 16) | I16, R_Y_H);
        return 20;

    case 0xE0: // MOV BA, BA
        return 8;
    case 0xE1: // MOV BA, HL
        R_BA = R_HL;
        return 8;
    case 0xE2: // MOV BA, X
        R_BA = R_X;
        return 8;
    case 0xE3: // MOV BA, Y
        R_BA = R_Y;
        return 8;

    case 0xE4: // MOV HL, BA
        R_HL = R_BA;
        return 8;
    case 0xE5: // MOV HL, HL
        return 8;
    case 0xE6: // MOV HL, X
        R_HL = R_X;
        return 8;
    case 0xE7: // MOV HL, Y
        R_HL = R_Y;
        return 8;

    case 0xE8: // MOV X, BA
        R_X = R_BA;
        return 8;
    case 0xE9: // MOV X, HL
        R_X = R_HL;
        return 8;
    case 0xEA: // MOV X, X
        return 8;
    case 0xEB: // MOV X, Y
        R_X = R_Y;
        return 8;

    case 0xEC: // MOV Y, BA
        R_Y = R_BA;
        return 8;
    case 0xED: // MOV Y, HL
        R_Y = R_HL;
        return 8;
    case 0xEE: // MOV Y, X
        R_Y = R_X;
        return 8;
    case 0xEF: // MOV Y, Y
        return 8;

    case 0xF0: // MOV SP, BA
        R_SP = R_BA;
        return 8;
    case 0xF1: // MOV SP, HL
        R_SP = R_HL;
        return 8;
    case 0xF2: // MOV SP, X
        R_SP = R_X;
        return 8;
    case 0xF3: // MOV SP, Y
        R_SP = R_Y;
        return 8;

    case 0xF4: // MOV HL, SP
        R_HL = R_SP;
        return 8;
    case 0xF5: // MOV HL, PC
        R_HL = R_PC;
        return 8;
    case 0xF6: // ??? X
        R_X_H = R_PC_V;
        return 12;
    case 0xF7: // ??? Y
        R_Y_H = R_PC_V;
        return 12;

    case 0xF8: // MOV BA, SP
        R_BA = R_SP;
        return 8;
    case 0xF9: // MOV BA, PC
        R_BA = R_PC;
        return 8;

    case 0xFA: // MOV X, SP
        R_X = R_SP;
        return 8;

    case 0xFB: // NOTHING
    case 0xFC: // NOTHING
        return 12;
    case 0xFD: // MOV A, E
        R_A = R_E;
        return 12;

    case 0xFE: // MOV Y, SP
        R_Y = R_SP;
        return 8;

    case 0xFF: // NOTHING
        return 64;

    default:
        GS_ASSERT(GS_FALSE);
        return 4;
    }
}
