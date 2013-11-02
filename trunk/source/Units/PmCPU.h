#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMCPU_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMCPU_H__

#include "System/PmUnit.h"

// Central Processing Unit
class PmCPU : public PmUnit
{
public:
    PmCPU(PmSystem *pSystem);
    virtual ~PmCPU();

    virtual GS_VOID Reset(GS_BOOL hardReset);

    virtual GS_BYTE ReadReg(GS_BYTE /*reg*/) { return 0; }
    virtual GS_VOID WriteReg(GS_BYTE /*reg*/, GS_BYTE /*val*/) {}

public:
    GS_VOID SetStall(GS_BOOL stall);
    GS_BOOL CallIrq(GS_BYTE addr);
    GS_UINT Exec();

private:
    GS_UINT ExecXX();
    GS_UINT ExecCE();
    GS_UINT ExecCF();

private:
    GS_BYTE Read8(GS_UINT32 addr);
    GS_VOID Write8(GS_UINT32 addr, GS_BYTE val);
    GS_VOID OnIRQ();
    GS_UINT16 Read16(GS_UINT32 addr);
    GS_VOID Write16(GS_UINT32 addr, GS_UINT16 val);
    GS_BYTE Fetch8();
    GS_UINT16 Fetch16();
    GS_UINT16 S8_TO_16(GS_BYTE a);
    GS_VOID SetU(GS_BYTE val);
    GS_BYTE ADD8(GS_BYTE a, GS_BYTE b);
    GS_UINT16 ADD16(GS_UINT16 a, GS_UINT16 b);
    GS_BYTE ADC8(GS_BYTE a, GS_BYTE b);
    GS_UINT16 ADC16(GS_UINT16 a, GS_UINT16 b);
    GS_BYTE SUB8(GS_BYTE a, GS_BYTE b);
    GS_UINT16 SUB16(GS_UINT16 a, GS_UINT16 b);
    GS_BYTE SBC8(GS_BYTE a, GS_BYTE b);
    GS_UINT16 SBC16(GS_UINT16 a, GS_UINT16 b);
    GS_BYTE AND8(GS_BYTE a, GS_BYTE b);
    GS_BYTE OR8(GS_BYTE a, GS_BYTE b);
    GS_BYTE XOR8(GS_BYTE a, GS_BYTE b);
    GS_BYTE INC8(GS_BYTE a);
    GS_UINT16 INC16(GS_UINT16 a);
    GS_BYTE DEC8(GS_BYTE a);
    GS_UINT16 DEC16(GS_UINT16 a);
    GS_VOID PUSH(GS_BYTE a);
    GS_BYTE POP();
    GS_VOID CALLS(GS_UINT16 offset);
    GS_VOID JMPS(GS_UINT16 offset);
    GS_VOID CALLU(GS_UINT16 addr);
    GS_VOID JMPU(GS_UINT16 addr);
    GS_VOID JDBNZ(GS_UINT16 offset);
    GS_BYTE SWAP(GS_BYTE a);
    GS_VOID RET();
    GS_VOID RETI();
    GS_VOID CALLX(GS_UINT16 addr);
    GS_VOID CALLI(GS_UINT16 addr);
    GS_VOID JMPI(GS_UINT16 addr);
    GS_BYTE SAL(GS_BYTE a);
    GS_BYTE SHL(GS_BYTE a);
    GS_BYTE SAR(GS_BYTE a);
    GS_BYTE SHR(GS_BYTE a);
    GS_BYTE ROLC(GS_BYTE a);
    GS_BYTE ROL(GS_BYTE a);
    GS_BYTE RORC(GS_BYTE a);
    GS_BYTE ROR(GS_BYTE a);
    GS_BYTE NOT(GS_BYTE a);
    GS_BYTE NEG(GS_BYTE a);
    GS_VOID HALT();
    GS_VOID STOP();
    GS_VOID MUL();
    GS_VOID DIV();

private:
    enum
    {
        STATUS_NORMAL = 0, 
        STATUS_HALT   = 1, 
        STATUS_STOP   = 2, 
        STATUS_IRQ    = 3, 
    };

    struct State
    {
        PmReg32 ba;   // B A
        PmReg32 hl;   // I H L
        PmReg32 x;    // IX X
        PmReg32 y;    // IY Y
        PmReg32 sp;   // SP
        PmReg32 pc;   // V PC
        PmReg32 op;   // For operate
        PmReg32 n;    // To make [N + #nn] easy to get, copy 'I' here and do not use the lowest byte
        GS_BYTE u1;   // V Shadow 1
        GS_BYTE u2;   // V Shadow 2
        GS_BYTE e;
        GS_BYTE ir;   // Last Instruction Register (for open-bus)
        GS_BYTE f;    // Flag

        GS_BYTE bStallCpu;
        GS_BYTE ucShiftU;  // Shift U, set to 2 when: U modify, branch, return
        GS_BYTE eStatus;   // CPU status
        GS_BYTE ucIrqAddr; // IRQ Vector when Status is IRQ

        State() { Reset(); }
        GS_VOID Reset() { gs_zero(this, sizeof(*this)); }
    };

    State m_state;
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMCPU_H__
