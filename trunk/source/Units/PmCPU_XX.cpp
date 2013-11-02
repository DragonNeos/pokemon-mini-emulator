#include "PmCPU.h"
#include "Base/PmRegMap.h"

GS_UINT PmCPU::ExecXX()
{
    GS_BYTE b1 = 0;
    GS_BYTE b2 = 0;
    GS_UINT16 u16 = 0;

    // Read IR
    R_IR = Fetch8();

    // Process instruction
    switch(R_IR)
    {
    case 0x00: // ADD A, A
        R_A = ADD8(R_A, R_A);
        return 8;
    case 0x01: // ADD A, B
        R_A = ADD8(R_A, R_B);
        return 8;
    case 0x02: // ADD A, #nn
        b1 = Fetch8();
        R_A = ADD8(R_A, b1);
        return 8;
    case 0x03: // ADD A, [HL]
        b1 = Read8(R_IHL);
        R_A = ADD8(R_A, b1);
        return 8;
    case 0x04: // ADD A, [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        R_A = ADD8(R_A, b1);
        return 12;
    case 0x05: // ADD A, [#nnnn]
        u16 = Fetch16();
        b1 = Read8((R_I << 16) | u16);
        R_A = ADD8(R_A, b1);
        return 16;
    case 0x06: // ADD A, [X]
        b1 = Read8(R_IX);
        R_A = ADD8(R_A, b1);
        return 8;
    case 0x07: // ADD A, [Y]
        b1 = Read8(R_IY);
        R_A = ADD8(R_A, b1);
        return 8;

    case 0x08: // ADC A, A
        R_A = ADC8(R_A, R_A);
        return 8;
    case 0x09: // ADC A, B
        R_A = ADC8(R_A, R_B);
        return 8;
    case 0x0A: // ADC A, #nn
        b1 = Fetch8();
        R_A = ADC8(R_A, b1);
        return 8;
    case 0x0B: // ADC A, [HL]
        b1 = Read8(R_IHL);
        R_A = ADC8(R_A, b1);
        return 8;
    case 0x0C: // ADC A, [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        R_A = ADC8(R_A, b1);
        return 12;
    case 0x0D: // ADC A, [#nnnn]
        u16 = Fetch16();
        b1 = Read8((R_I << 16) | u16);
        R_A = ADC8(R_A, b1);
        return 16;
    case 0x0E: // ADC A, [X]
        b1 = Read8(R_IX);
        R_A = ADC8(R_A, b1);
        return 8;
    case 0x0F: // ADC A, [Y]
        b1 = Read8(R_IY);
        R_A = ADC8(R_A, b1);
        return 8;

    case 0x10: // SUB A, A
        R_A = SUB8(R_A, R_A);
        return 8;
    case 0x11: // SUB A, B
        R_A = SUB8(R_A, R_B);
        return 8;
    case 0x12: // SUB A, #nn
        b1 = Fetch8();
        R_A = SUB8(R_A, b1);
        return 8;
    case 0x13: // SUB A, [HL]
        b1 = Read8(R_IHL);
        R_A = SUB8(R_A, b1);
        return 8;
    case 0x14: // SUB A, [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        R_A = SUB8(R_A, b1);
        return 12;
    case 0x15: // SUB A, [#nnnn]
        u16 = Fetch16();
        b1 = Read8((R_I << 16) | u16);
        R_A = SUB8(R_A, b1);
        return 16;
    case 0x16: // SUB A, [X]
        b1 = Read8(R_IX);
        R_A = SUB8(R_A, b1);
        return 8;
    case 0x17: // SUB A, [Y]
        b1 = Read8(R_IY);
        R_A = SUB8(R_A, b1);
        return 8;

    case 0x18: // SBC A, A
        R_A = SBC8(R_A, R_A);
        return 8;
    case 0x19: // SBC A, B
        R_A = SBC8(R_A, R_B);
        return 8;
    case 0x1A: // SBC A, #nn
        b1 = Fetch8();
        R_A = SBC8(R_A, b1);
        return 8;
    case 0x1B: // SBC A, [HL]
        b1 = Read8(R_IHL);
        R_A = SBC8(R_A, b1);
        return 8;
    case 0x1C: // SBC A, [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        R_A = SBC8(R_A, b1);
        return 12;
    case 0x1D: // SBC A, [#nnnn]
        u16 = Fetch16();
        b1 = Read8((R_I << 16) | u16);
        R_A = SBC8(R_A, b1);
        return 16;
    case 0x1E: // SBC A, [X]
        b1 = Read8(R_IX);
        R_A = SBC8(R_A, b1);
        return 8;
    case 0x1F: // SBC A, [Y]
        b1 = Read8(R_IY);
        R_A = SBC8(R_A, b1);
        return 8;

    case 0x20: // AND A, A
        R_A = AND8(R_A, R_A);
        return 8;
    case 0x21: // AND A, B
        R_A = AND8(R_A, R_B);
        return 8;
    case 0x22: // AND A, #nn
        b1 = Fetch8();
        R_A = AND8(R_A, b1);
        return 8;
    case 0x23: // AND A, [HL]
        b1 = Read8(R_IHL);
        R_A = AND8(R_A, b1);
        return 8;
    case 0x24: // AND A, [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        R_A = AND8(R_A, b1);
        return 12;
    case 0x25: // AND A, [#nnnn]
        u16 = Fetch16();
        b1 = Read8((R_I << 16) | u16);
        R_A = AND8(R_A, b1);
        return 16;
    case 0x26: // AND A, [X]
        b1 = Read8(R_IX);
        R_A = AND8(R_A, b1);
        return 8;
    case 0x27: // AND A, [Y]
        b1 = Read8(R_IY);
        R_A = AND8(R_A, b1);
        return 8;

    case 0x28: // OR A, A
        R_A = OR8(R_A, R_A);
        return 8;
    case 0x29: // OR A, B
        R_A = OR8(R_A, R_B);
        return 8;
    case 0x2A: // OR A, #nn
        b1 = Fetch8();
        R_A = OR8(R_A, b1);
        return 8;
    case 0x2B: // OR A, [HL]
        b1 = Read8(R_IHL);
        R_A = OR8(R_A, b1);
        return 8;
    case 0x2C: // OR A, [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        R_A = OR8(R_A, b1);
        return 12;
    case 0x2D: // OR A, [#nnnn]
        u16 = Fetch16();
        b1 = Read8((R_I << 16) | u16);
        R_A = OR8(R_A, b1);
        return 16;
    case 0x2E: // OR A, [X]
        b1 = Read8(R_IX);
        R_A = OR8(R_A, b1);
        return 8;
    case 0x2F: // OR A, [Y]
        b1 = Read8(R_IY);
        R_A = OR8(R_A, b1);
        return 8;

    case 0x30: // CMP A, A
        SUB8(R_A, R_A);
        return 8;
    case 0x31: // CMP A, B
        SUB8(R_A, R_B);
        return 8;
    case 0x32: // CMP A, #nn
        b1 = Fetch8();
        SUB8(R_A, b1);
        return 8;
    case 0x33: // CMP A, [HL]
        b1 = Read8(R_IHL);
        SUB8(R_A, b1);
        return 8;
    case 0x34: // CMP A, [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        SUB8(R_A, b1);
        return 12;
    case 0x35: // CMP A, [#nnnn]
        u16 = Fetch16();
        b1 = Read8((R_I << 16) | u16);
        SUB8(R_A, b1);
        return 16;
    case 0x36: // CMP A, [X]
        b1 = Read8(R_IX);
        SUB8(R_A, b1);
        return 8;
    case 0x37: // CMP A, [Y]
        b1 = Read8(R_IY);
        SUB8(R_A, b1);
        return 8;

    case 0x38: // XOR A, A
        R_A = XOR8(R_A, R_A);
        return 8;
    case 0x39: // XOR A, B
        R_A = XOR8(R_A, R_B);
        return 8;
    case 0x3A: // XOR A, #nn
        b1 = Fetch8();
        R_A = XOR8(R_A, b1);
        return 8;
    case 0x3B: // XOR A, [HL]
        b1 = Read8(R_IHL);
        R_A = XOR8(R_A, b1);
        return 8;
    case 0x3C: // XOR A, [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        R_A = XOR8(R_A, b1);
        return 12;
    case 0x3D: // XOR A, [#nnnn]
        u16 = Fetch16();
        b1 = Read8((R_I << 16) | u16);
        R_A = XOR8(R_A, b1);
        return 16;
    case 0x3E: // XOR A, [X]
        b1 = Read8(R_IX);
        R_A = XOR8(R_A, b1);
        return 8;
    case 0x3F: // XOR A, [Y]
        b1 = Read8(R_IY);
        R_A = XOR8(R_A, b1);
        return 8;

    case 0x40: // MOV A, A
        return 4;
    case 0x41: // MOV A, B
        R_A = R_B;
        return 4;
    case 0x42: // MOV A, L
        R_A = R_L;
        return 4;
    case 0x43: // MOV A, H
        R_A = R_H;
        return 4;
    case 0x44: // MOV A, [N+#nn]
        b1 = Fetch8();
        R_A = Read8(R_IN + b1);
        return 12;
    case 0x45: // MOV A, [HL]
        R_A = Read8(R_IHL);
        return 8;
    case 0x46: // MOV A, [X]
        R_A = Read8(R_IX);
        return 8;
    case 0x47: // MOV A, [Y]
        R_A = Read8(R_IY);
        return 8;

    case 0x48: // MOV B, A
        R_B = R_A;
        return 4;
    case 0x49: // MOV B, B
        return 4;
    case 0x4A: // MOV B, L
        R_B = R_L;
        return 4;
    case 0x4B: // MOV B, H
        R_B = R_H;
        return 4;
    case 0x4C: // MOV B, [N+#nn]
        b1 = Fetch8();
        R_B = Read8(R_IN + b1);
        return 12;
    case 0x4D: // MOV B, [HL]
        R_B = Read8(R_IHL);
        return 8;
    case 0x4E: // MOV B, [X]
        R_B = Read8(R_IX);
        return 8;
    case 0x4F: // MOV B, [Y]
        R_B = Read8(R_IY);
        return 8;

    case 0x50: // MOV L, A
        R_L = R_A;
        return 4;
    case 0x51: // MOV L, B
        R_L = R_B;
        return 4;
    case 0x52: // MOV L, L
        return 4;
    case 0x53: // MOV L, H
        R_L = R_H;
        return 4;
    case 0x54: // MOV L, [N+#nn]
        b1 = Fetch8();
        R_L = Read8(R_IN + b1);
        return 12;
    case 0x55: // MOV L, [HL]
        R_L = Read8(R_IHL);
        return 8;
    case 0x56: // MOV L, [X]
        R_L = Read8(R_IX);
        return 8;
    case 0x57: // MOV L, [Y]
        R_L = Read8(R_IY);
        return 8;

    case 0x58: // MOV H, A
        R_H = R_A;
        return 4;
    case 0x59: // MOV H, B
        R_H = R_B;
        return 4;
    case 0x5A: // MOV H, L
        R_H = R_L;
        return 4;
    case 0x5B: // MOV H, H
        return 4;
    case 0x5C: // MOV H, [N+#nn]
        b1 = Fetch8();
        R_H = Read8(R_IN + b1);
        return 12;
    case 0x5D: // MOV H, [HL]
        R_H = Read8(R_IHL);
        return 8;
    case 0x5E: // MOV H, [X]
        R_H = Read8(R_IX);
        return 8;
    case 0x5F: // MOV H, [Y]
        R_H = Read8(R_IY);
        return 8;

    case 0x60: // MOV [X], A
        Write8(R_IX, R_A);
        return 8;
    case 0x61: // MOV [X], B
        Write8(R_IX, R_B);
        return 8;
    case 0x62: // MOV [X], L
        Write8(R_IX, R_L);
        return 8;
    case 0x63: // MOV [X], H
        Write8(R_IX, R_H);
        return 8;
    case 0x64: // MOV [X], [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        Write8(R_IX, b1);
        return 16;
    case 0x65: // MOV [X], [HL]
        b1 = Read8(R_IHL);
        Write8(R_IX, b1);
        return 12;
    case 0x66: // MOV [X], [X]
        b1 = Read8(R_IX);
        Write8(R_IX, b1);
        return 12;
    case 0x67: // MOV [X], [Y]
        b1 = Read8(R_IY);
        Write8(R_IX, b1);
        return 12;

    case 0x68: // MOV [HL], A
        Write8(R_IHL, R_A);
        return 8;
    case 0x69: // MOV [HL], B
        Write8(R_IHL, R_B);
        return 8;
    case 0x6A: // MOV [HL], L
        Write8(R_IHL, R_L);
        return 8;
    case 0x6B: // MOV [HL], H
        Write8(R_IHL, R_H);
        return 8;
    case 0x6C: // MOV [HL], [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        Write8(R_IHL, b1);
        return 16;
    case 0x6D: // MOV [HL], [HL]
        b1 = Read8(R_IHL);
        Write8(R_IHL, b1);
        return 12;
    case 0x6E: // MOV [HL], [X]
        b1 = Read8(R_IX);
        Write8(R_IHL, b1);
        return 12;
    case 0x6F: // MOV [HL], [Y]
        b1 = Read8(R_IY);
        Write8(R_IHL, b1);
        return 12;

    case 0x70: // MOV [Y], A
        Write8(R_IY, R_A);
        return 8;
    case 0x71: // MOV [Y], B
        Write8(R_IY, R_B);
        return 8;
    case 0x72: // MOV [Y], L
        Write8(R_IY, R_L);
        return 8;
    case 0x73: // MOV [Y], H
        Write8(R_IY, R_H);
        return 8;
    case 0x74: // MOV [Y], [N+#nn]
        b1 = Fetch8();
        b1 = Read8(R_IN + b1);
        Write8(R_IY, b1);
        return 16;
    case 0x75: // MOV [Y], [HL]
        b1 = Read8(R_IHL);
        Write8(R_IY, b1);
        return 12;
    case 0x76: // MOV [Y], [X]
        b1 = Read8(R_IX);
        Write8(R_IY, b1);
        return 12;
    case 0x77: // MOV [Y], [Y]
        b1 = Read8(R_IY);
        Write8(R_IY, b1);
        return 12;

    case 0x78: // MOV [N+#nn], A
        b1 = Fetch8();
        Write8(R_IN + b1, R_A);
        return 8;
    case 0x79: // MOV [N+#nn], B
        b1 = Fetch8();
        Write8(R_IN + b1, R_B);
        return 8;
    case 0x7A: // MOV [N+#nn], L
        b1 = Fetch8();
        Write8(R_IN + b1, R_L);
        return 8;
    case 0x7B: // MOV [N+#nn], H
        b1 = Fetch8();
        Write8(R_IN + b1, R_H);
        return 8;
    case 0x7C: // NOTHING #nn
        b1 = Fetch8();
        return 64;
    case 0x7D: // MOV [N+#nn], [HL]
        b1 = Fetch8();
        Write8(R_IN + b1, Read8(R_IHL));
        return 16;
    case 0x7E: // MOV [N+#nn], [X]
        b1 = Fetch8();
        Write8(R_IN + b1, Read8(R_IX));
        return 16;
    case 0x7F: // MOV [N+#nn], [Y]
        b1 = Fetch8();
        Write8(R_IN + b1, Read8(R_IY));
        return 16;

    case 0x80: // INC A
        R_A = INC8(R_A);
        return 8;
    case 0x81: // INC B
        R_B = INC8(R_B);
        return 8;
    case 0x82: // INC L
        R_L = INC8(R_L);
        return 8;
    case 0x83: // INC H
        R_H = INC8(R_H);
        return 8;
    case 0x84: // INC N
        R_N = INC8(R_N);
        return 8;
    case 0x85: // INC [N+#nn]
        b1 = Fetch8();
        Write8(R_IN + b1, INC8(Read8(R_IN + b1)));
        return 16;
    case 0x86: // INC [HL]
        Write8(R_IHL, INC8(Read8(R_IHL)));
        return 12;
    case 0x87: // INC SP
        R_SP = INC16(R_SP);
        return 8;

    case 0x88: // DEC A
        R_A = DEC8(R_A);
        return 8;
    case 0x89: // DEC B
        R_B = DEC8(R_B);
        return 8;
    case 0x8A: // DEC L
        R_L = DEC8(R_L);
        return 8;
    case 0x8B: // DEC H
        R_H = DEC8(R_H);
        return 8;
    case 0x8C: // DEC N
        R_N = DEC8(R_N);
        return 8;
    case 0x8D: // DEC [N+#nn]
        b1 = Fetch8();
        Write8(R_IN + b1, DEC8(Read8(R_IN + b1)));
        return 16;
    case 0x8E: // DEC [HL]
        Write8(R_IHL, DEC8(Read8(R_IHL)));
        return 12;
    case 0x8F: // DEC SP
        R_SP = DEC16(R_SP);
        return 8;

    case 0x90: // INC BA
        R_BA = INC16(R_BA);
        return 8;
    case 0x91: // INC HL
        R_HL = INC16(R_HL);
        return 8;
    case 0x92: // INC X
        R_X = INC16(R_X);
        return 8;
    case 0x93: // INC Y
        R_Y = INC16(R_Y);
        return 8;

    case 0x94: // TST A, B
        AND8(R_A, R_B);
        return 8;
    case 0x95: // TST [HL], #nn
        b1 = Fetch8();
        AND8(Read8(R_IHL), b1);
        return 12;
    case 0x96: // TST A, #nn
        b1 = Fetch8();
        AND8(R_A, b1);
        return 8;
    case 0x97: // TST B, #nn
        b1 = Fetch8();
        AND8(R_B, b1);
        return 8;

    case 0x98: // DEC BA
        R_BA = DEC16(R_BA);
        return 8;
    case 0x99: // DEC HL
        R_HL = DEC16(R_HL);
        return 8;
    case 0x9A: // DEC X
        R_X = DEC16(R_X);
        return 8;
    case 0x9B: // DEC Y
        R_Y = DEC16(R_Y);
        return 8;

    case 0x9C: // AND F, #nn
        b1 = Fetch8();
        R_F = R_F & b1;
        OnIRQ();
        return 12;
    case 0x9D: // OR F, #nn
        b1 = Fetch8();
        R_F = R_F | b1;
        OnIRQ();
        return 12;
    case 0x9E: // XOR F, #nn
        b1 = Fetch8();
        R_F = R_F ^ b1;
        OnIRQ();
        return 12;
    case 0x9F: // MOV F, #nn
        b1 = Fetch8();
        R_F = b1;
        OnIRQ();
        return 12;

    case 0xA0: // PUSH BA
        PUSH(R_B);
        PUSH(R_A);
        return 16;
    case 0xA1: // PUSH HL
        PUSH(R_H);
        PUSH(R_L);
        return 16;
    case 0xA2: // PUSH X
        PUSH(R_X_H);
        PUSH(R_X_L);
        return 16;
    case 0xA3: // PUSH Y
        PUSH(R_Y_H);
        PUSH(R_Y_L);
        return 16;
    case 0xA4: // PUSH N
        PUSH(R_N);
        return 12;
    case 0xA5: // PUSH I
        PUSH(R_I);
        return 12;
    case 0xA6: // PUSHX
        PUSH(R_XI);
        PUSH(R_YI);
        return 16;
    case 0xA7: // PUSH F
        PUSH(R_F);
        return 12;

    case 0xA8: // POP BA
        R_A = POP();
        R_B = POP();
        return 12;
    case 0xA9: // POP HL
        R_L = POP();
        R_H = POP();
        return 12;
    case 0xAA: // POP X
        R_X_L = POP();
        R_X_H = POP();
        return 12;
    case 0xAB: // POP Y
        R_Y_L = POP();
        R_Y_H = POP();
        return 12;
    case 0xAC: // POP N
        R_N = POP();
        return 8;
    case 0xAD: // POP I
        R_I = POP();
        R_N_I = R_I;
        return 8;
    case 0xAE: // POPX
        R_YI = POP();
        R_XI = POP();
        return 12;
    case 0xAF: // POP F
        R_F = POP();
        OnIRQ();
        return 8;

    case 0xB0: // MOV A, #nn
        b1 = Fetch8();
        R_A = b1;
        return 8;
    case 0xB1: // MOV B, #nn
        b1 = Fetch8();
        R_B = b1;
        return 8;
    case 0xB2: // MOV L, #nn
        b1 = Fetch8();
        R_L = b1;
        return 8;
    case 0xB3: // MOV H, #nn
        b1 = Fetch8();
        R_H = b1;
        return 8;
    case 0xB4: // MOV N, #nn
        b1 = Fetch8();
        R_N = b1;
        return 8;
    case 0xB5: // MOV [HL], #nn
        b1 = Fetch8();
        Write8(R_IHL, b1);
        return 12;
    case 0xB6: // MOV [X], #nn
        b1 = Fetch8();
        Write8(R_IX, b1);
        return 12;
    case 0xB7: // MOV [Y], #nn
        b1 = Fetch8();
        Write8(R_IY, b1);
        return 12;

    case 0xB8: // MOV BA, [#nnnn]
        u16 = Fetch16();
        R_A = Read8((R_I << 16) | u16++);
        R_B = Read8((R_I << 16) | u16);
        return 20;
    case 0xB9: // MOV HL, [#nnnn]
        u16 = Fetch16();
        R_L = Read8((R_I << 16) | u16++);
        R_H = Read8((R_I << 16) | u16);
        return 20;
    case 0xBA: // MOV X, [#nnnn]
        u16 = Fetch16();
        R_X_L = Read8((R_I << 16) | u16++);
        R_X_H = Read8((R_I << 16) | u16);
        return 20;
    case 0xBB: // MOV Y, [#nnnn]
        u16 = Fetch16();
        R_Y_L = Read8((R_I << 16) | u16++);
        R_Y_H = Read8((R_I << 16) | u16);
        return 20;

    case 0xBC: // MOV [#nnnn], BA
        u16 = Fetch16();
        Write8((R_I << 16) | u16++, R_A);
        Write8((R_I << 16) | u16, R_B);
        return 20;
    case 0xBD: // MOV [#nnnn], HL
        u16 = Fetch16();
        Write8((R_I << 16) | u16++, R_L);
        Write8((R_I << 16) | u16, R_H);
        return 20;
    case 0xBE: // MOV [#nnnn], X
        u16 = Fetch16();
        Write8((R_I << 16) | u16++, R_X_L);
        Write8((R_I << 16) | u16, R_X_H);
        return 20;
    case 0xBF: // MOV [#nnnn], Y
        u16 = Fetch16();
        Write8((R_I << 16) | u16++, R_Y_L);
        Write8((R_I << 16) | u16, R_Y_H);
        return 20;

    case 0xC0: // ADD BA, #nnnn
        u16 = Fetch16();
        R_BA = ADD16(R_BA, u16);
        return 12;
    case 0xC1: // ADD HL, #nnnn
        u16 = Fetch16();
        R_HL = ADD16(R_HL, u16);
        return 12;
    case 0xC2: // ADD X, #nnnn
        u16 = Fetch16();
        R_X = ADD16(R_X, u16);
        return 12;
    case 0xC3: // ADD Y, #nnnn
        u16 = Fetch16();
        R_Y = ADD16(R_Y, u16);
        return 12;

    case 0xC4: // MOV BA, #nnnn
        u16 = Fetch16();
        R_BA = u16;
        return 12;
    case 0xC5: // MOV HL, #nnnn
        u16 = Fetch16();
        R_HL = u16;
        return 12;
    case 0xC6: // MOV X, #nnnn
        u16 = Fetch16();
        R_X = u16;
        return 12;
    case 0xC7: // MOV Y, #nnnn
        u16 = Fetch16();
        R_Y = u16;
        return 12;

    case 0xC8: // XCHG BA, HL
        u16 = R_HL;
        R_HL = R_BA;
        R_BA = u16;
        return 12;
    case 0xC9: // XCHG BA, X
        u16 = R_X;
        R_X = R_BA;
        R_BA = u16;
        return 12;
    case 0xCA: // XCHG BA, Y
        u16 = R_Y;
        R_Y = R_BA;
        R_BA = u16;
        return 12;
    case 0xCB: // XCHG BA, SP
        u16 = R_SP;
        R_SP = R_BA;
        R_BA = u16;
        return 12;

    case 0xCC: // XCHG A, B
        b1 = R_B;
        R_B = R_A;
        R_A = b1;
        return 8;
    case 0xCD: // XCHG A, [HL]
        b1 = Read8(R_IHL);
        Write8(R_IHL, R_A);
        R_A = b1;
        return 12;

    case 0xCE: // Expand 0
        return ExecCE();

    case 0xCF: // Expand 1
        return ExecCF();

    case 0xD0: // SUB BA, #nnnn
        u16 = Fetch16();
        R_BA = SUB16(R_BA, u16);
        return 12;
    case 0xD1: // SUB HL, #nnnn
        u16 = Fetch16();
        R_HL = SUB16(R_HL, u16);
        return 12;
    case 0xD2: // SUB X, #nnnn
        u16 = Fetch16();
        R_X = SUB16(R_X, u16);
        return 12;
    case 0xD3: // SUB Y, #nnnn
        u16 = Fetch16();
        R_Y = SUB16(R_Y, u16);
        return 12;

    case 0xD4: // CMP BA, #nnnn
        u16 = Fetch16();
        SUB16(R_BA, u16);
        return 12;
    case 0xD5: // CMP HL, #nnnn
        u16 = Fetch16();
        SUB16(R_HL, u16);
        return 12;
    case 0xD6: // CMP X, #nnnn
        u16 = Fetch16();
        SUB16(R_X, u16);
        return 12;
    case 0xD7: // CMP Y, #nnnn
        u16 = Fetch16();
        SUB16(R_Y, u16);
        return 12;

    case 0xD8: // AND [N+#nn], #nn
        b1 = Fetch8();
        b2 = Fetch8();
        Write8(R_IN + b1, AND8(Read8(R_IN + b1), b2));
        return 20;
    case 0xD9: // OR [N+#nn], #nn
        b1 = Fetch8();
        b2 = Fetch8();
        Write8(R_IN + b1, OR8(Read8(R_IN + b1), b2));
        return 20;
    case 0xDA: // XOR [N+#nn], #nn
        b1 = Fetch8();
        b2 = Fetch8();
        Write8(R_IN + b1, XOR8(Read8(R_IN + b1), b2));
        return 20;
    case 0xDB: // CMP [N+#nn], #nn
        b1 = Fetch8();
        b2 = Fetch8();
        SUB8(Read8(R_IN + b1), b2);
        return 16;
    case 0xDC: // TST [N+#nn], #nn
        b1 = Fetch8();
        b2 = Fetch8();
        AND8(Read8(R_IN + b1), b2);
        return 16;
    case 0xDD: // MOV [N+#nn], #nn
        b1 = Fetch8();
        b2 = Fetch8();
        Write8(R_IN + b1, b2);
        return 16;

    case 0xDE: // PACK
        R_A = (R_A & 0x0F) | (R_B << 4);
        return 8;

    case 0xDF: // UNPACK
        R_B = (R_A >> 4);
        R_A = R_A & 0x0F;
        return 8;

    case 0xE0: // CALLC #ss
        b1 = Fetch8();
        if (R_F_C) {
            CALLS(S8_TO_16(b1));
            return 20;
        }
        return 8;
    case 0xE1: // CALLNC #ss
        b1 = Fetch8();
        if (!(R_F_C)) {
            CALLS(S8_TO_16(b1));
            return 20;
        }
        return 8;
    case 0xE2: // CALLZ #ss
        b1 = Fetch8();
        if (R_F_Z) {
            CALLS(S8_TO_16(b1));
            return 20;
        }
        return 8;
    case 0xE3: // CALLNZ #ss
        b1 = Fetch8();
        if (!(R_F_Z)) {
            CALLS(S8_TO_16(b1));
            return 20;
        }
        return 8;

    case 0xE4: // JC #ss
        b1 = Fetch8();
        if (R_F_C) {
            JMPS(S8_TO_16(b1));
        }
        return 8;
    case 0xE5: // JNC #ss
        b1 = Fetch8();
        if (!(R_F_C)) {
            JMPS(S8_TO_16(b1));
        }
        return 8;
    case 0xE6: // JZ #ss
        b1 = Fetch8();
        if (R_F_Z) {
            JMPS(S8_TO_16(b1));
        }
        return 8;
    case 0xE7: // JNZ #ss
        b1 = Fetch8();
        if (!(R_F_Z)) {
            JMPS(S8_TO_16(b1));
        }
        return 8;

    case 0xE8: // CALLC #ssss
        u16 = Fetch16();
        if (R_F_C) {
            CALLS(u16);
            return 24;
        }
        return 12;
    case 0xE9: // CALLNC #ssss
        u16 = Fetch16();
        if (!(R_F_C)) {
            CALLS(u16);
            return 24;
        }
        return 12;
    case 0xEA: // CALLZ #ssss
        u16 = Fetch16();
        if (R_F_Z) {
            CALLS(u16);
            return 24;
        }
        return 12;
    case 0xEB: // CALLNZ #ssss
        u16 = Fetch16();
        if (!(R_F_Z)) {
            CALLS(u16);
            return 24;
        }
        return 12;

    case 0xEC: // JC #ssss
        u16 = Fetch16();
        if (R_F_C) {
            JMPS(u16);
        }
        return 12;
    case 0xED: // JNC #ssss
        u16 = Fetch16();
        if (!(R_F_C)) {
            JMPS(u16);
        }
        return 12;
    case 0xEE: // JZ #ssss
        u16 = Fetch16();
        if (R_F_Z) {
            JMPS(u16);
        }
        return 12;
    case 0xEF: // JNZ #ssss
        u16 = Fetch16();
        if (!(R_F_Z)) {
            JMPS(u16);
        }
        return 12;

    case 0xF0: // CALL #ss
        b1 = Fetch8();
        CALLS(S8_TO_16(b1));
        return 20;
    case 0xF1: // JMP #ss
        b1 = Fetch8();
        JMPS(S8_TO_16(b1));
        return 8;
    case 0xF2: // CALL #ssss
        u16 = Fetch16();
        CALLS(u16);
        return 24;
    case 0xF3: // JMP #ssss
        u16 = Fetch16();
        JMPS(u16);
        return 12;

    case 0xF4: // JMP HL
        JMPU(R_HL);
        return 8;

    case 0xF5: // JDBNZ #ss
        b1 = Fetch8();
        JDBNZ(S8_TO_16(b1));
        return 16;

    case 0xF6: // SWAP A
        R_A = SWAP(R_A);
        return 8;
    case 0xF7: // SWAP [HL]
        Write8(R_IHL, SWAP(Read8(R_IHL)));
        return 12;

    case 0xF8: // RET
        RET();
        return 16;
    case 0xF9: // RETI
        RETI();
        return 16;
    case 0xFA: // RETSKIP
        RET();
        R_PC = R_PC + 2;
        return 16;

    case 0xFB: // CALL [#nnnn]
        u16 = Fetch16();
        CALLX(u16);
        return 20;
    case 0xFC: // CINT #nn
        u16 = Fetch8();
        CALLI(u16);
        return 20;
    case 0xFD: // JINT #nn
        u16 = Fetch8();
        JMPI(u16);
        return 8;

    case 0xFE: // CRASH
        GS_ASSERT(GS_FALSE);
        return 4;

    case 0xFF: // NOP
        return 8;

    default:
        GS_ASSERT(GS_FALSE);
        return 4;
    }
}
