#include "PmCPU.h"
#include "Base/PmRegMap.h"

GS_UINT PmCPU::ExecCE()
{
    GS_BYTE I8A;
    GS_UINT16 I16;

    // Read IR
    R_IR = Fetch8();

    // Process instruction
    switch(R_IR)
    {
    case 0x00: // ADD A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = ADD8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x01: // ADD A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_A = ADD8(R_A, Read8((R_YI << 16) | I16));
        return 16;
    case 0x02: // ADD A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = ADD8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x03: // ADD A, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_A = ADD8(R_A, Read8((R_YI << 16) | I16));
        return 16;

    case 0x04: // ADD [HL], A
        Write8(R_IHL, ADD8(Read8(R_IHL), R_A));
        return 16;
    case 0x05: // ADD [HL], #nn
        I8A = Fetch8();
        Write8(R_IHL, ADD8(Read8(R_IHL), I8A));
        return 20;
    case 0x06: // ADD [HL], [X]
        Write8(R_IHL, ADD8(Read8(R_IHL), Read8(R_IX)));
        return 20;
    case 0x07: // ADD [HL], [Y]
        Write8(R_IHL, ADD8(Read8(R_IHL), Read8(R_IY)));
        return 20;

    case 0x08: // ADC A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = ADC8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x09: // ADC A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_A = ADC8(R_A, Read8((R_YI << 16) | I16));
        return 16;
    case 0x0A: // ADC A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = ADC8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x0B: // ADC A, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_A = ADC8(R_A, Read8((R_YI << 16) | I16));
        return 16;

    case 0x0C: // ADC [HL], A
        Write8(R_IHL, ADC8(Read8(R_IHL), R_A));
        return 16;
    case 0x0D: // ADC [HL], #nn
        I8A = Fetch8();
        Write8(R_IHL, ADC8(Read8(R_IHL), I8A));
        return 20;
    case 0x0E: // ADC [HL], [X]
        Write8(R_IHL, ADC8(Read8(R_IHL), Read8(R_IX)));
        return 20;
    case 0x0F: // ADC [HL], [Y]
        Write8(R_IHL, ADC8(Read8(R_IHL), Read8(R_IY)));
        return 20;

    case 0x10: // SUB A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = SUB8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x11: // SUB A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_A = SUB8(R_A, Read8((R_YI << 16) | I16));
        return 16;
    case 0x12: // SUB A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = SUB8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x13: // SUB A, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_A = SUB8(R_A, Read8((R_YI << 16) | I16));
        return 16;

    case 0x14: // SUB [HL], A
        Write8(R_IHL, SUB8(Read8(R_IHL), R_A));
        return 16;
    case 0x15: // SUB [HL], #nn
        I8A = Fetch8();
        Write8(R_IHL, SUB8(Read8(R_IHL), I8A));
        return 20;
    case 0x16: // SUB [HL], [X]
        Write8(R_IHL, SUB8(Read8(R_IHL), Read8(R_IX)));
        return 20;
    case 0x17: // SUB [HL], [Y]
        Write8(R_IHL, SUB8(Read8(R_IHL), Read8(R_IY)));
        return 20;

    case 0x18: // SBC A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = SBC8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x19: // SBC A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_A = SBC8(R_A, Read8((R_YI << 16) | I16));
        return 16;
    case 0x1A: // SBC A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = SBC8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x1B: // SBC A, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_A = SBC8(R_A, Read8((R_YI << 16) | I16));
        return 16;

    case 0x1C: // SBC [HL], A
        Write8(R_IHL, SBC8(Read8(R_IHL), R_A));
        return 16;
    case 0x1D: // SBC [HL], #nn
        I8A = Fetch8();
        Write8(R_IHL, SBC8(Read8(R_IHL), I8A));
        return 20;
    case 0x1E: // SBC [HL], [X]
        Write8(R_IHL, SBC8(Read8(R_IHL), Read8(R_IX)));
        return 20;
    case 0x1F: // SBC [HL], [Y]
        Write8(R_IHL, SBC8(Read8(R_IHL), Read8(R_IY)));
        return 20;

    case 0x20: // AND A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = AND8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x21: // AND A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_A = AND8(R_A, Read8((R_YI << 16) | I16));
        return 16;
    case 0x22: // AND A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = AND8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x23: // AND A, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_A = AND8(R_A, Read8((R_YI << 16) | I16));
        return 16;

    case 0x24: // AND [HL], A
        Write8(R_IHL, AND8(Read8(R_IHL), R_A));
        return 16;
    case 0x25: // AND [HL], #nn
        I8A = Fetch8();
        Write8(R_IHL, AND8(Read8(R_IHL), I8A));
        return 20;
    case 0x26: // AND [HL], [X]
        Write8(R_IHL, AND8(Read8(R_IHL), Read8(R_IX)));
        return 20;
    case 0x27: // AND [HL], [Y]
        Write8(R_IHL, AND8(Read8(R_IHL), Read8(R_IY)));
        return 20;

    case 0x28: // OR A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = OR8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x29: // OR A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_A = OR8(R_A, Read8((R_YI << 16) | I16));
        return 16;
    case 0x2A: // OR A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = OR8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x2B: // OR A, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_A = OR8(R_A, Read8((R_YI << 16) | I16));
        return 16;

    case 0x2C: // OR [HL], A
        Write8(R_IHL, OR8(Read8(R_IHL), R_A));
        return 16;
    case 0x2D: // OR [HL], #nn
        I8A = Fetch8();
        Write8(R_IHL, OR8(Read8(R_IHL), I8A));
        return 20;
    case 0x2E: // OR [HL], [X]
        Write8(R_IHL, OR8(Read8(R_IHL), Read8(R_IX)));
        return 20;
    case 0x2F: // OR [HL], [Y]
        Write8(R_IHL, OR8(Read8(R_IHL), Read8(R_IY)));
        return 20;

    case 0x30: // CMP A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        SUB8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x31: // CMP A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        SUB8(R_A, Read8((R_YI << 16) | I16));
        return 16;
    case 0x32: // CMP A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        SUB8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x33: // CMP A, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        SUB8(R_A, Read8((R_YI << 16) | I16));
        return 16;

    case 0x34: // CMP [HL], A
        SUB8(Read8(R_IHL), R_A);
        return 16;
    case 0x35: // CMP [HL], #nn
        I8A = Fetch8();
        SUB8(Read8(R_IHL), I8A);
        return 20;
    case 0x36: // CMP [HL], [X]
        SUB8(Read8(R_IHL), Read8(R_IX));
        return 20;
    case 0x37: // CMP [HL], [Y]
        SUB8(Read8(R_IHL), Read8(R_IY));
        return 20;

    case 0x38: // XOR A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = XOR8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x39: // XOR A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_A = XOR8(R_A, Read8((R_YI << 16) | I16));
        return 16;
    case 0x3A: // XOR A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = XOR8(R_A, Read8((R_XI << 16) | I16));
        return 16;
    case 0x3B: // XOR A, [Y+L]
        I16 = R_Y+ S8_TO_16(R_L);
        R_A = XOR8(R_A, Read8((R_YI << 16) | I16));
        return 16;

    case 0x3C: // XOR [HL], A
        Write8(R_IHL, XOR8(Read8(R_IHL), R_A));
        return 16;
    case 0x3D: // XOR [HL], #nn
        I8A = Fetch8();
        Write8(R_IHL, XOR8(Read8(R_IHL), I8A));
        return 20;
    case 0x3E: // XOR [HL], [X]
        Write8(R_IHL, XOR8(Read8(R_IHL), Read8(R_IX)));
        return 20;
    case 0x3F: // XOR [HL], [Y]
        Write8(R_IHL, XOR8(Read8(R_IHL), Read8(R_IY)));
        return 20;

    case 0x40: // MOV A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = Read8((R_XI << 16) | I16);
        return 16;
    case 0x41: // MOV A, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_A = Read8((R_YI << 16) | I16);
        return 16;
    case 0x42: // MOV A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = Read8((R_XI << 16) | I16);
        return 16;
    case 0x43: // MOV A, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_A = Read8((R_YI << 16) | I16);
        return 16;

    case 0x44: // MOV [X+#ss], A
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        Write8((R_XI << 16) | I16, R_A);
        return 16;
    case 0x45: // MOV [Y+#ss], A
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        Write8((R_YI << 16) | I16, R_A);
        return 16;
    case 0x46: // MOV [X+L], A
        I16 = R_X + S8_TO_16(R_L);
        Write8((R_XI << 16) | I16, R_A);
        return 16;
    case 0x47: // MOV [Y+L], A
        I16 = R_Y + S8_TO_16(R_L);
        Write8((R_YI << 16) | I16, R_A);
        return 16;

    case 0x48: // MOV B, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_B = Read8((R_XI << 16) | I16);
        return 16;
    case 0x49: // MOV B, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_B = Read8((R_YI << 16) | I16);
        return 16;
    case 0x4A: // MOV B, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_B = Read8((R_XI << 16) | I16);
        return 16;
    case 0x4B: // MOV B, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_B = Read8((R_YI << 16) | I16);
        return 16;

    case 0x4C: // MOV [X+#ss], B
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        Write8((R_XI << 16) | I16, R_B);
        return 16;
    case 0x4D: // MOV [Y+#ss], B
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        Write8((R_YI << 16) | I16, R_B);
        return 16;
    case 0x4E: // MOV [X+L], B
        I16 = R_X + S8_TO_16(R_L);
        Write8((R_XI << 16) | I16, R_B);
        return 16;
    case 0x4F: // MOV [Y+L], B
        I16 = R_Y + S8_TO_16(R_L);
        Write8((R_YI << 16) | I16, R_B);
        return 16;

    case 0x50: // MOV L, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_L = Read8((R_XI << 16) | I16);
        return 16;
    case 0x51: // MOV L, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_L = Read8((R_YI << 16) | I16);
        return 16;
    case 0x52: // MOV L, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_L = Read8((R_XI << 16) | I16);
        return 16;
    case 0x53: // MOV L, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_L = Read8((R_YI << 16) | I16);
        return 16;

    case 0x54: // MOV [X+#ss], L
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        Write8((R_XI << 16) | I16, R_L);
        return 16;
    case 0x55: // MOV [Y+#ss], L
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        Write8((R_YI << 16) | I16, R_L);
        return 16;
    case 0x56: // MOV [X+L], L
        I16 = R_X + S8_TO_16(R_L);
        Write8((R_XI << 16) | I16, R_L);
        return 16;
    case 0x57: // MOV [Y+L], L
        I16 = R_Y + S8_TO_16(R_L);
        Write8((R_YI << 16) | I16, R_L);
        return 16;

    case 0x58: // MOV H, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_H = Read8((R_XI << 16) | I16);
        return 16;
    case 0x59: // MOV H, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_H = Read8((R_YI << 16) | I16);
        return 16;
    case 0x5A: // MOV H, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_H = Read8((R_XI << 16) | I16);
        return 16;
    case 0x5B: // MOV H, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_H = Read8((R_YI << 16) | I16);
        return 16;

    case 0x5C: // MOV [X+#ss], H
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        Write8((R_XI << 16) | I16, R_H);
        return 16;
    case 0x5D: // MOV [Y+#ss], H
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        Write8((R_YI << 16) | I16, R_H);
        return 16;
    case 0x5E: // MOV [X+L], H
        I16 = R_X + S8_TO_16(R_L);
        Write8((R_XI << 16) | I16, R_H);
        return 16;
    case 0x5F: // MOV [Y+L], H
        I16 = R_Y + S8_TO_16(R_L);
        Write8((R_YI << 16) | I16, R_H);
        return 16;

    case 0x60: // MOV [HL], [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        Write8(R_IHL, Read8((R_XI << 16) | I16));
        return 20;
    case 0x61: // MOV [HL], [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        Write8(R_IHL, Read8((R_YI << 16) | I16));
        return 20;
    case 0x62: // MOV [HL], [X+L]
        I16 = R_X + S8_TO_16(R_L);
        Write8(R_IHL, Read8((R_XI << 16) | I16));
        return 20;
    case 0x63: // MOV [HL], [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        Write8(R_IHL, Read8((R_YI << 16) | I16));
        return 20;

    case 0x64: // *ADC BA, #nnnn
        I16 = Fetch16();
        R_BA = ADC16(R_BA, I16);
        return 16;
    case 0x65: // *ADC HL, #nnnn
        I16 = Fetch16();
        R_HL = ADC16(R_HL, I16);
        return 16;

    case 0x66: // ??? BA, #nn00+L
        I8A = Fetch8();
        R_BA = ADC16(R_BA, (I8A << 8) | R_L);
        return 24;
    case 0x67: // ??? HL, #nn00+L
        I8A = Fetch8();
        R_HL = ADC16(R_HL, (I8A << 8) | R_L);
        return 24;

    case 0x68: // MOV [X], [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        Write8(R_IX, Read8((R_XI << 16) | I16));
        return 20;
    case 0x69: // MOV [X], [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        Write8(R_IX, Read8((R_YI << 16) | I16));
        return 20;
    case 0x6A: // MOV [X], [X+L]
        I16 = R_X + S8_TO_16(R_L);
        Write8(R_IX, Read8((R_XI << 16) | I16));
        return 20;
    case 0x6B: // MOV [X], [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        Write8(R_IX, Read8((R_YI << 16) | I16));
        return 20;

    case 0x6C: // *ADD SP, #nnnn
        I16 = Fetch8();
        R_SP = ADD16(R_SP, I16);

    case 0x6D: // ??? HL, #nn
        I8A = Fetch8();
        R_HL = ADD16(R_X, ((I8A << 4) * 3) + ((I8A & 0x08) >> 3));
        CLEAR_F_C; // It seems that carry gets clear?
        return 40;
    case 0x6E: // ??? SP, #nn00+L
        I8A = Fetch8();
        R_SP = ADD16(R_SP, (I8A << 8) | R_L);
        return 16;
    case 0x6F: // ??? HL, L
        R_HL = ADD16(R_X, ((R_L << 4) * 3) + ((R_L & 0x08) >> 3));
        CLEAR_F_C; // It seems that carry gets clear?
        return 40;

    case 0x70: // NOTHING
        R_PC++;
        return 64;
    case 0x71: // NOTHING
        R_PC++;
        return 64;
    case 0x72: // NOTHING
        return 64;
    case 0x73: // NOTHING
        return 64;

    case 0x74: // *MOV A, [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        R_A = Read8((R_XI << 16) | I16);
        return 64;
    case 0x75: // *MOV L, [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        R_L = Read8((R_YI << 16) | I16);
        return 64;
    case 0x76: // *MOV A, [X+L]
        I16 = R_X + S8_TO_16(R_L);
        R_A = Read8((R_XI << 16) | I16);
        return 64;
    case 0x77: // *MOV L, [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        R_L = Read8((R_YI << 16) | I16);
        return 64;

    case 0x78: // MOV [Y], [X+#ss]
        I8A = Fetch8();
        I16 = R_X + S8_TO_16(I8A);
        Write8(R_IY, Read8((R_XI << 16) | I16));
        return 20;
    case 0x79: // MOV [Y], [Y+#ss]
        I8A = Fetch8();
        I16 = R_Y + S8_TO_16(I8A);
        Write8(R_IY, Read8((R_YI << 16) | I16));
        return 20;
    case 0x7A: // MOV [Y], [X+L]
        I16 = R_X + S8_TO_16(R_L);
        Write8(R_IY, Read8((R_XI << 16) | I16));
        return 20;
    case 0x7B: // MOV [Y], [Y+L]
        I16 = R_Y + S8_TO_16(R_L);
        Write8(R_IY, Read8((R_YI << 16) | I16));
        return 20;

    case 0x7C: // NOTHING #nn
        R_PC++;
        return 20;
    case 0x7D: // NOTHING #nn
        R_PC++;
        return 16;
    case 0x7E: // NOTHING
        return 20;
    case 0x7F: // NOTHING
        return 16;

    case 0x80: // SAL A
        R_A = SAL(R_A);
        return 12;
    case 0x81: // SAL B
        R_B = SAL(R_B);
        return 12;
    case 0x82: // SAL [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, SAL(Read8(R_IN | I8A)));
        return 20;
    case 0x83: // SAL [HL]
        Write8(R_IHL, SAL(Read8(R_IHL)));
        return 16;

    case 0x84: // SHL A
        R_A = SHL(R_A);
        return 12;
    case 0x85: // SHL B
        R_B = SHL(R_B);
        return 12;
    case 0x86: // SHL [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, SHL(Read8(R_IN | I8A)));
        return 20;
    case 0x87: // SHL [HL]
        Write8(R_IHL, SHL(Read8(R_IHL)));
        return 16;

    case 0x88: // SAR A
        R_A = SAR(R_A);
        return 12;
    case 0x89: // SAR B
        R_B = SAR(R_B);
        return 12;
    case 0x8A: // SAR [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, SAR(Read8(R_IN | I8A)));
        return 20;
    case 0x8B: // SAR [HL]
        Write8(R_IHL, SAR(Read8(R_IHL)));
        return 16;

    case 0x8C: // SHR A
        R_A = SHR(R_A);
        return 12;
    case 0x8D: // SHR B
        R_B = SHR(R_B);
        return 12;
    case 0x8E: // SHR [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, SHR(Read8(R_IN | I8A)));
        return 20;
    case 0x8F: // SHR [HL]
        Write8(R_IHL, SHR(Read8(R_IHL)));
        return 16;

    case 0x90: // ROLC A
        R_A = ROLC(R_A);
        return 12;
    case 0x91: // ROLC B
        R_B = ROLC(R_B);
        return 12;
    case 0x92: // ROLC [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, ROLC(Read8(R_IN | I8A)));
        return 20;
    case 0x93: // ROLC [HL]
        Write8(R_IHL, ROLC(Read8(R_IHL)));
        return 16;

    case 0x94: // ROL A
        R_A = ROL(R_A);
        return 12;
    case 0x95: // ROL B
        R_B = ROL(R_B);
        return 12;
    case 0x96: // ROL [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, ROL(Read8(R_IN | I8A)));
        return 20;
    case 0x97: // ROL [HL]
        Write8(R_IHL, ROL(Read8(R_IHL)));
        return 16;

    case 0x98: // RORC A
        R_A = RORC(R_A);
        return 12;
    case 0x99: // RORC B
        R_B = RORC(R_B);
        return 12;
    case 0x9A: // RORC [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, RORC(Read8(R_IN | I8A)));
        return 20;
    case 0x9B: // RORC [HL]
        Write8(R_IHL, RORC(Read8(R_IHL)));
        return 16;

    case 0x9C: // ROR A
        R_A = ROR(R_A);
        return 12;
    case 0x9D: // ROR B
        R_B = ROR(R_B);
        return 12;
    case 0x9E: // ROR [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, ROR(Read8(R_IN | I8A)));
        return 20;
    case 0x9F: // ROR [HL]
        Write8(R_IHL, ROR(Read8(R_IHL)));
        return 16;

    case 0xA0: // NOT A
        R_A = NOT(R_A);
        return 12;
    case 0xA1: // NOT B
        R_B = NOT(R_B);
        return 12;
    case 0xA2: // NOT [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, NOT(Read8(R_IN | I8A)));
        return 20;
    case 0xA3: // NOT [HL]
        Write8(R_IHL, NOT(Read8(R_IHL)));
        return 16;

    case 0xA4: // NEG A
        R_A = NEG(R_A);
        return 12;
    case 0xA5: // NEG B
        R_B = NEG(R_B);
        return 12;
    case 0xA6: // NEG [N+#nn]
        I8A = Fetch8();
        Write8(R_IN | I8A, NEG(Read8(R_IN | I8A)));
        return 20;
    case 0xA7: // NEG [HL]
        Write8(R_IHL, NEG(Read8(R_IHL)));
        return 16;

    case 0xA8: // EX BA, A
        R_BA = S8_TO_16(R_A);
        return 12;

    case 0xA9: // NOTHING
        return 8;
    case 0xAA: // NOTHING
        return 12;

    case 0xAB: // CRASH
        GS_ASSERT(GS_FALSE);
        return 64;
    case 0xAC: // CRASH
        GS_ASSERT(GS_FALSE);
        return 64;
    case 0xAD: // CRASH
        GS_ASSERT(GS_FALSE);
        return 64;

    case 0xAE: // HALT
        HALT();
        return 8;
    case 0xAF: // STOP
        STOP();
        return 8;

    case 0xB0: // AND B, #nn
        I8A = Fetch8();
        R_B = AND8(R_B, I8A);
        return 12;
    case 0xB1: // AND L, #nn
        I8A = Fetch8();
        R_L = AND8(R_L, I8A);
        return 12;
    case 0xB2: // AND H, #nn
        I8A = Fetch8();
        R_H = AND8(R_H, I8A);
        return 12;
    case 0xB3: // MOV H, V
        R_H = R_PC_V;
        return 12;

    case 0xB4: // OR B, #nn
        I8A = Fetch8();
        R_B = OR8(R_B, I8A);
        return 12;
    case 0xB5: // OR L, #nn
        I8A = Fetch8();
        R_L = OR8(R_L, I8A);
        return 12;
    case 0xB6: // OR H, #nn
        I8A = Fetch8();
        R_H = OR8(R_H, I8A);
        return 12;
    case 0xB7: // ??? X
        R_X_H = R_PC_V;
        return 12;

    case 0xB8: // XOR B, #nn
        I8A = Fetch8();
        R_B = XOR8(R_B, I8A);
        return 12;
    case 0xB9: // XOR L, #nn
        I8A = Fetch8();
        R_L = XOR8(R_L, I8A);
        return 12;
    case 0xBA: // XOR H, #nn
        I8A = Fetch8();
        R_H = XOR8(R_H, I8A);
        return 12;
    case 0xBB: // ??? Y
        R_Y_H = R_PC_V;
        return 12;

    case 0xBC: // CMP B, #nn
        I8A = Fetch8();
        SUB8(R_B, I8A);
        return 12;
    case 0xBD: // CMP L, #nn
        I8A = Fetch8();
        SUB8(R_L, I8A);
        return 12;
    case 0xBE: // CMP H, #nn
        I8A = Fetch8();
        SUB8(R_H, I8A);
        return 12;
    case 0xBF: // CMP N, #nn
        I8A = Fetch8();
        SUB8(R_N, I8A);
        return 12;

    case 0xC0: // MOV A, N
        R_A = R_N;
        return 8;
    case 0xC1: // MOV A, F
        R_A = R_F;
        return 8;
    case 0xC2: // MOV N, A
        R_N = R_A;
        return 8;
    case 0xC3: // MOV F, A
        R_F = R_A;
        OnIRQ();
        return 8;

    case 0xC4: // MOV U, #nn
        I8A = Fetch8();
        SetU(I8A);
        return 16;
    case 0xC5: // MOV I, #nn
        I8A = Fetch8();
        R_I = I8A;
        R_N_I = R_I;
        return 12;
    case 0xC6: // MOV XI, #nn
        I8A = Fetch8();
        R_XI = I8A;
        return 12;
    case 0xC7: // MOV YI, #nn
        I8A = Fetch8();
        R_YI = I8A;
        return 12;

    case 0xC8: // MOV A, V
        R_A = R_PC_V;
        return 8;
    case 0xC9: // MOV A, I
        R_A = R_I;
        return 8;
    case 0xCA: // MOV A, XI
        R_A = R_XI;
        return 8;
    case 0xCB: // MOV A, YI
        R_A = R_YI;
        return 8;

    case 0xCC: // MOV U, A
        SetU(R_A);
        return 8;
    case 0xCD: // MOV I, A
        R_I = R_A;
        R_N_I = R_I;
        return 8;
    case 0xCE: // MOV XI, A
        R_XI = R_A;
        return 8;
    case 0xCF: // MOV YI, A
        R_YI = R_A;
        return 8;

    case 0xD0: // MOV A, [#nnnn]
        I16 = Fetch16();
        R_A = Read8(I16);
        return 20;
    case 0xD1: // MOV B, [#nnnn]
        I16 = Fetch16();
        R_B = Read8(I16);
        return 20;
    case 0xD2: // MOV L, [#nnnn]
        I16 = Fetch16();
        R_L = Read8(I16);
        return 20;
    case 0xD3: // MOV H, [#nnnn]
        I16 = Fetch16();
        R_H = Read8(I16);
        return 20;

    case 0xD4: // MOV [#nnnn], A
        I16 = Fetch16();
        Write8(I16, R_A);
        return 20;
    case 0xD5: // MOV [#nnnn], B
        I16 = Fetch16();
        Write8(I16, R_B);
        return 20;
    case 0xD6: // MOV [#nnnn], L
        I16 = Fetch16();
        Write8(I16, R_L);
        return 20;
    case 0xD7: // MOV [#nnnn], H
        I16 = Fetch16();
        Write8(I16, R_H);
        return 20;

    case 0xD8: // MUL L, A
        MUL();
        return 48;

    case 0xD9: // DIV HL, A
        DIV();
        return 52;

    case 0xDA: // ??? #nn
    case 0xDB: // ??? #nn
        GS_ASSERT(GS_FALSE);
        //return MinxCPU_ExecSPCE();
        return 64;
    case 0xDC: // CRASH
        GS_ASSERT(GS_FALSE);
        return 64;
    case 0xDD: // NOTHING
        return 16;
    case 0xDE: // ??? #nn
    case 0xDF: // ??? #nn
        GS_ASSERT(GS_FALSE);
        //return MinxCPU_ExecSPCE();
        return 64;

    case 0xE0: // JL #ss
        I8A = Fetch8();
        if ( ((R_F_O)!=0) != ((R_F_S)!=0) ) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xE1: // JLE #ss
        I8A = Fetch8();
        if ( (((R_F_O)==0) != ((R_F_S)==0)) || ((R_F_Z)!=0) ) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xE2: // JG #ss
        I8A = Fetch8();
        if ( (((R_F_O)!=0) == ((R_F_S)!=0)) && ((R_F_Z)==0) ) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xE3: // JGE #ss
        I8A = Fetch8();
        if ( ((R_F_O)==0) == ((R_F_S)==0) ) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;

    case 0xE4: // JO #ss
        I8A = Fetch8();
        if (R_F_O) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xE5: // JNO #ss
        I8A = Fetch8();
        if (!(R_F_O)) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xE6: // JP #ss
        I8A = Fetch8();
        if (!(R_F_S)) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xE7: // JNP #ss
        I8A = Fetch8();
        if (R_F_S) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;

    case 0xE8: // JNX0 #ss
        I8A = Fetch8();
        if (!(R_E & 0x01)) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xE9: // JNX1 #ss
        I8A = Fetch8();
        if (!(R_E & 0x02)) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xEA: // JNX2 #ss
        I8A = Fetch8();
        if (!(R_E & 0x04)) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xEB: // JNX3 #ss
        I8A = Fetch8();
        if (!(R_E & 0x08)) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;

    case 0xEC: // JX0 #ss
        I8A = Fetch8();
        if (R_E & 0x01) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xED: // JX1 #ss
        I8A = Fetch8();
        if (R_E & 0x02) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xEE: // JX2 #ss
        I8A = Fetch8();
        if (R_E & 0x04) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;
    case 0xEF: // JX3 #ss
        I8A = Fetch8();
        if (R_E & 0x08) {
            JMPS(S8_TO_16(I8A));
        }
        return 12;

    case 0xF0: // CALLL #ss
        I8A = Fetch8();
        if ( ((R_F_O)!=0) != ((R_F_S)!=0) ) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xF1: // CALLLE #ss
        I8A = Fetch8();
        if ( (((R_F_O)==0) != ((R_F_S)==0)) || ((R_F_Z)!=0) ) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xF2: // CALLG #ss
        I8A = Fetch8();
        if ( (((R_F_O)!=0) == ((R_F_S)!=0)) && ((R_F_Z)==0) ) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xF3: // CALLGE #ss
        I8A = Fetch8();
        if ( ((R_F_O)==0) == ((R_F_S)==0) ) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;

    case 0xF4: // CALLO #ss
        I8A = Fetch8();
        if (R_F_O) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xF5: // CALLNO #ss
        I8A = Fetch8();
        if (!(R_F_O)) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xF6: // CALLNS #ss
        I8A = Fetch8();
        if (!(R_F_S)) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xF7: // CALLS #ss
        I8A = Fetch8();
        if (R_F_S) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;

    case 0xF8: // CALLNX0 #ss
        I8A = Fetch8();
        if (!(R_E & 0x01)) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xF9: // CALLNX1 #ss
        I8A = Fetch8();
        if (!(R_E & 0x02)) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xFA: // CALLNX2 #ss
        I8A = Fetch8();
        if (!(R_E & 0x04)) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xFB: // CALLNX3 #ss
        I8A = Fetch8();
        if (!(R_E & 0x08)) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;

    case 0xFC: // CALLX0 #ss
        I8A = Fetch8();
        if (R_E & 0x01) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xFD: // CALLX1 #ss
        I8A = Fetch8();
        if (R_E & 0x02) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xFE: // CALLX2 #ss
        I8A = Fetch8();
        if (R_E & 0x04) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;
    case 0xFF: // CALLX3 #ss
        I8A = Fetch8();
        if (R_E & 0x08) {
            CALLS(S8_TO_16(I8A));
        }
        return 12;

    default:
        GS_ASSERT(GS_FALSE);
        return 4;
    }
}
