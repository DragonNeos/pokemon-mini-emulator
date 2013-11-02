#include "PmCPU.h"
#include "Base/PmRegMap.h"

PmCPU::PmCPU(PmSystem *pSystem)
    : PmUnit(pSystem)
{
    SetReady();
}

PmCPU::~PmCPU()
{
}

GS_VOID PmCPU::Reset(GS_BOOL hardReset)
{
    m_state.Reset();
    R_PC = Read16(hardReset? 0: 2);
    R_E = 0x1F;
    R_F = I_FLAG;
    SetU(0);
}

GS_VOID PmCPU::SetStall(GS_BOOL stall)
{
    m_state.bStallCpu = stall? 1: 0;
}

GS_BOOL PmCPU::CallIrq(GS_BYTE addr)
{
    RETURN_FAIL_ON_FAIL(STATUS_IRQ != m_state.eStatus);

    m_state.eStatus = STATUS_IRQ;
    m_state.ucIrqAddr = addr;
    return GS_TRUE;
}

GS_UINT PmCPU::Exec()
{
    // Stall
    if (m_state.bStallCpu)
    {
        return 32;
    }

    // Shift U
    if (m_state.ucShiftU)
    {
        R_U1 = R_U2;
        R_U2 = R_PC_V;
        --m_state.ucShiftU;
        OnIRQ();
    }

    // Check HALT or STOP status
    if (STATUS_NORMAL != m_state.eStatus)
    {
        if (STATUS_IRQ == m_state.eStatus)
        {
            m_state.eStatus = STATUS_NORMAL; // Return to normal
            CALLI(m_state.ucIrqAddr);        // Jump to IRQ vector
            return 20;
        }
        else
        {
            return 8;                        // Cause short NOPs
        }
    }

    return ExecXX();
}

GS_BYTE PmCPU::Read8(GS_UINT32 addr)
{
    return SysRead(addr);
}

GS_VOID PmCPU::Write8(GS_UINT32 addr, GS_BYTE val)
{
    SysWrite(addr, val);
}

GS_VOID PmCPU::OnIRQ()
{
    GS_BOOL enable = (0 == m_state.ucShiftU) && (I_FLAG != (R_F & I_FLAG));
    SysSetIrq(enable);
}

GS_UINT16 PmCPU::Read16(GS_UINT32 addr)
{
    GS_BYTE L = Read8(addr);
    GS_BYTE H = Read8(addr + 1);
    return (H << 8) | L;
}

GS_VOID PmCPU::Write16(GS_UINT32 addr, GS_UINT16 val)
{
    Write8(addr,     val & 0xFF);
    Write8(addr + 1, val >> 8);
}

GS_BYTE PmCPU::Fetch8()
{
    GS_UINT32 addr = (R_PC & 0x8000) // Banked area or not
                   ? (R_PC & 0x7FFF) | (R_PC_V << 15) 
                   : R_PC;
    R_IR = Read8(addr);
    ++R_PC;
    return R_IR;
}

GS_UINT16 PmCPU::Fetch16()
{
    GS_BYTE L = Fetch8();
    GS_BYTE H = Fetch8();
    return (H << 8) | L;
}

GS_UINT16 PmCPU::S8_TO_16(GS_BYTE a)
{
    return (a & 0x80)? (0xFF00 | a): a;
}

GS_VOID PmCPU::SetU(GS_BYTE val)
{
    if (val != R_U2) { m_state.ucShiftU = 2; }
    R_U2 = R_U1 = val;
    OnIRQ();
}

GS_BYTE PmCPU::ADD8(GS_BYTE a, GS_BYTE b)
{
    switch (R_F & (B_FLAG | N_FLAG))
    {
    case 0x00:
        R_OP = a + b;
        R_OP_BL? CLEAR_F_Z: SET_F_Z;
        R_OP_BH? SET_F_C: CLEAR_F_C;
        (((a ^ b) & 0x80) == 0 && ((a ^ R_OP_BL) & 0x80) != 0)? SET_F_O: CLEAR_F_O;
        (R_OP_BL & 0x80)? SET_F_S: CLEAR_F_S;
        return R_OP_BL;
    case B_FLAG:
        R_OP = a + b;
        if ((R_OP_BL & 0x0F) >= 0x0A) { R_OP += 0x06; }
        if ((R_OP_BL & 0xF0) >= 0xA0) { R_OP += 0x60; }
        R_OP_BL? CLEAR_F_Z: SET_F_Z;
        R_OP_BH? SET_F_C: CLEAR_F_C;
        CLEAR_F_O;
        CLEAR_F_S;
        return R_OP_BL;
    case N_FLAG:
        R_OP = (a & 0x0F) + (b & 0x0F);
        (R_OP_BL & 0x0F)? CLEAR_F_Z: SET_F_Z;
        (R_OP_BL & 0xF0)? SET_F_C: CLEAR_F_C;
        (((a ^ b) & 0x08) == 0 && ((a ^ R_OP_BL) & 0x08) != 0)? SET_F_O: CLEAR_F_O;
        (R_OP_BL & 0x08)? SET_F_S: CLEAR_F_S;
        return R_OP_BL & 0x0F;
    default: // B_FLAG | N_FLAG
        R_OP = (a & 0x0F) + (b & 0x0F);
        if ((R_OP_BL & 0x0F) >= 0x0A) { R_OP += 0x06; }
        (R_OP_BL & 0x0F)? CLEAR_F_Z: SET_F_Z;
        (R_OP_BL & 0xF0)? SET_F_C: CLEAR_F_C;
        CLEAR_F_O;
        CLEAR_F_S;
        return R_OP_BL & 0x0F;
    }
}

GS_UINT16 PmCPU::ADD16(GS_UINT16 a, GS_UINT16 b)
{
    R_OP = a + b;
    R_OP_WL? CLEAR_F_Z: SET_F_Z;
    R_OP_WH? SET_F_C: CLEAR_F_C;
    (((a ^ b) & 0x8000) == 0 && ((a ^ R_OP_WL) & 0x8000) != 0)? SET_F_O: CLEAR_F_O;
    (R_OP_WL & 0x8000)? SET_F_S: CLEAR_F_S;
    return R_OP_WL;
}

GS_BYTE PmCPU::ADC8(GS_BYTE a, GS_BYTE b)
{
    switch (R_F & (B_FLAG | N_FLAG))
    {
    case 0x00:
        R_OP = a + b + R_F_C;
        R_OP_BL? CLEAR_F_Z: SET_F_Z;
        R_OP_BH? SET_F_C: CLEAR_F_C;
        (((a ^ b) & 0x80) == 0 && ((a ^ R_OP_BL) & 0x80) != 0)? SET_F_O: CLEAR_F_O;
        (R_OP_BL & 0x80)? SET_F_S: CLEAR_F_S;
        return R_OP_BL;
    case B_FLAG:
        R_OP = a + b + R_F_C;
        if ((R_OP_BL & 0x0F) >= 0x0A) { R_OP += 0x06; }
        if ((R_OP_BL & 0xF0) >= 0xA0) { R_OP += 0x60; }
        R_OP_BL? CLEAR_F_Z: SET_F_Z;
        R_OP_BH? SET_F_C: CLEAR_F_C;
        CLEAR_F_O;
        CLEAR_F_S;
        return R_OP_BL;
    case N_FLAG:
        R_OP = (a & 0x0F) + (b & 0x0F) + R_F_C;
        (R_OP_BL & 0x0F)? CLEAR_F_Z: SET_F_Z;
        (R_OP_BL & 0xF0)? SET_F_C: CLEAR_F_C;
        (((a ^ b) & 0x08) == 0 && ((a ^ R_OP_BL) & 0x08) != 0)? SET_F_O: CLEAR_F_O;
        (R_OP_BL & 0x08)? SET_F_S: CLEAR_F_S;
        return R_OP_BL & 0x0F;
    default: // B_FLAG | N_FLAG
        R_OP = (a & 0x0F) + (b & 0x0F) + R_F_C;
        if ((R_OP_BL & 0x0F) >= 0x0A) { R_OP += 0x06; }
        (R_OP_BL & 0x0F)? CLEAR_F_Z: SET_F_Z;
        (R_OP_BL & 0xF0)? SET_F_C: CLEAR_F_C;
        CLEAR_F_O;
        CLEAR_F_S;
        return R_OP_BL & 0x0F;
    }
}

GS_UINT16 PmCPU::ADC16(GS_UINT16 a, GS_UINT16 b)
{
    R_OP = a + b + R_F_C;
    R_OP_WL? CLEAR_F_Z: SET_F_Z;
    R_OP_WH? SET_F_C: CLEAR_F_C;
    (((a ^ b) & 0x8000) == 0 && ((a ^ R_OP_WL) & 0x8000) != 0)? SET_F_O: CLEAR_F_O;
    (R_OP_WL & 0x8000)? SET_F_S: CLEAR_F_S;
    return R_OP_WL;
}

GS_BYTE PmCPU::SUB8(GS_BYTE a, GS_BYTE b)
{
    switch (R_F & (B_FLAG | N_FLAG))
    {
    case 0x00:
        R_OP = a - b;
        R_OP_BL? CLEAR_F_Z: SET_F_Z;
        R_OP_BH? SET_F_C: CLEAR_F_C;
        (((a ^ b) & 0x80) != 0 && ((a ^ R_OP_BL) & 0x80) != 0)? SET_F_O: CLEAR_F_O;
        (R_OP_BL & 0x80)? SET_F_S: CLEAR_F_S;
        return R_OP_BL;
    case B_FLAG:
        R_OP = a - b;
        if ((R_OP_BL & 0x0F) >= 0x0A) { R_OP -= 0x06; }
        if ((R_OP_BL & 0xF0) >= 0xA0) { R_OP -= 0x60; }
        R_OP_BL? CLEAR_F_Z: SET_F_Z;
        R_OP_BH? SET_F_C: CLEAR_F_C;
        CLEAR_F_O;
        CLEAR_F_S;
        return R_OP_BL;
    case N_FLAG:
        R_OP = (a & 0x0F) - (b & 0x0F);
        (R_OP_BL & 0x0F)? CLEAR_F_Z: SET_F_Z;
        (R_OP_BL & 0xF0)? SET_F_C: CLEAR_F_C;
        (((a ^ b) & 0x08) != 0 && ((a ^ R_OP_BL) & 0x08) != 0)? SET_F_O: CLEAR_F_O;
        (R_OP_BL & 0x08)? SET_F_S: CLEAR_F_S;
        return R_OP_BL & 0x0F;
    default: // B_FLAG | N_FLAG
        R_OP = (a & 0x0F) - (b & 0x0F);
        if ((R_OP_BL & 0x0F) >= 0x0A) { R_OP -= 0x06; }
        (R_OP_BL & 0x0F)? CLEAR_F_Z: SET_F_Z;
        (R_OP_BL & 0xF0)? SET_F_C: CLEAR_F_C;
        CLEAR_F_O;
        CLEAR_F_S;
        return R_OP_BL & 0x0F;
    }
}

GS_UINT16 PmCPU::SUB16(GS_UINT16 a, GS_UINT16 b)
{
    R_OP = a - b;
    R_OP_WL? CLEAR_F_Z: SET_F_Z;
    R_OP_WH? SET_F_C: CLEAR_F_C;
    (((a ^ b) & 0x8000) != 0 && ((a ^ R_OP_WL) & 0x8000) != 0)? SET_F_O: CLEAR_F_O;
    (R_OP_WL & 0x8000)? SET_F_S: CLEAR_F_S;
    return R_OP_WL;
}

GS_BYTE PmCPU::SBC8(GS_BYTE a, GS_BYTE b)
{
    switch (R_F & (B_FLAG | N_FLAG))
    {
    case 0x00:
        R_OP = a - b - R_F_C;
        R_OP_BL? CLEAR_F_Z: SET_F_Z;
        R_OP_BH? SET_F_C: CLEAR_F_C;
        (((a ^ b) & 0x80) != 0 && ((a ^ R_OP_BL) & 0x80) != 0)? SET_F_O: CLEAR_F_O;
        (R_OP_BL & 0x80)? SET_F_S: CLEAR_F_S;
        return R_OP_BL;
    case B_FLAG:
        R_OP = a - b - R_F_C;
        if ((R_OP_BL & 0x0F) >= 0x0A) { R_OP -= 0x06; }
        if ((R_OP_BL & 0xF0) >= 0xA0) { R_OP -= 0x60; }
        R_OP_BL? CLEAR_F_Z: SET_F_Z;
        R_OP_BH? SET_F_C: CLEAR_F_C;
        CLEAR_F_O;
        CLEAR_F_S;
        return R_OP_BL;
    case N_FLAG:
        R_OP = (a & 0x0F) - (b & 0x0F) - R_F_C;
        (R_OP_BL & 0x0F)? CLEAR_F_Z: SET_F_Z;
        (R_OP_BL & 0xF0)? SET_F_C: CLEAR_F_C;
        (((a ^ b) & 0x08) != 0 && ((a ^ R_OP_BL) & 0x08) != 0)? SET_F_O: CLEAR_F_O;
        (R_OP_BL & 0x08)? SET_F_S: CLEAR_F_S;
        return R_OP_BL & 0x0F;
    default: // B_FLAG | N_FLAG
        R_OP = (a & 0x0F) - (b & 0x0F) - R_F_C;
        if ((R_OP_BL & 0x0F) >= 0x0A) { R_OP -= 0x06; }
        (R_OP_BL & 0x0F)? CLEAR_F_Z: SET_F_Z;
        (R_OP_BL & 0xF0)? SET_F_C: CLEAR_F_C;
        CLEAR_F_O;
        CLEAR_F_S;
        return R_OP_BL & 0x0F;
    }
}

GS_UINT16 PmCPU::SBC16(GS_UINT16 a, GS_UINT16 b)
{
    R_OP = a - b - R_F_C;
    R_OP_WL? CLEAR_F_Z: SET_F_Z;
    R_OP_WH? SET_F_C: CLEAR_F_C;
    (((a ^ b) & 0x8000) != 0 && ((a ^ R_OP_WL) & 0x8000) != 0)? SET_F_O: CLEAR_F_O;
    (R_OP_WL & 0x8000)? SET_F_S: CLEAR_F_S;
    return R_OP_WL;
}

GS_BYTE PmCPU::AND8(GS_BYTE a, GS_BYTE b)
{
    a &= b;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::OR8(GS_BYTE a, GS_BYTE b)
{
    a |= b;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::XOR8(GS_BYTE a, GS_BYTE b)
{
    a ^= b;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::INC8(GS_BYTE a)
{
    ++a;
    a? CLEAR_F_Z: SET_F_Z;
    return a;
}

GS_UINT16 PmCPU::INC16(GS_UINT16 a)
{
    ++a;
    a? CLEAR_F_Z: SET_F_Z;
    return a;
}

GS_BYTE PmCPU::DEC8(GS_BYTE a)
{
    --a;
    a? CLEAR_F_Z: SET_F_Z;
    return a;
}

GS_UINT16 PmCPU::DEC16(GS_UINT16 a)
{
    --a;
    a? CLEAR_F_Z: SET_F_Z;
    return a;
}

GS_VOID PmCPU::PUSH(GS_BYTE a)
{
    --R_SP;
    Write8(R_SP, a);
}

GS_BYTE PmCPU::POP()
{
    return Read8(R_SP++);
}

GS_VOID PmCPU::CALLS(GS_UINT16 offset)
{
    PUSH(R_PC_V);
    PUSH(R_PC_H);
    PUSH(R_PC_L);
    JMPS(offset);
}

GS_VOID PmCPU::JMPS(GS_UINT16 offset)
{
    R_PC_V = R_U2 = R_U1;
    R_PC += offset - 1;
}

GS_VOID PmCPU::CALLU(GS_UINT16 addr)
{
    PUSH(R_PC_V);
    PUSH(R_PC_H);
    PUSH(R_PC_L);
    JMPU(addr);
}

GS_VOID PmCPU::JMPU(GS_UINT16 addr)
{
    R_PC_V = R_U2 = R_U1;
    R_PC = addr;
}

GS_VOID PmCPU::JDBNZ(GS_UINT16 offset)
{
    R_B = DEC8(R_B);
    if (R_B)
    {
        JMPS(offset);
    }
}

GS_BYTE PmCPU::SWAP(GS_BYTE a)
{
    return (a << 4) | (a >> 4);
}

GS_VOID PmCPU::RET()
{
    R_PC_L = POP();
    R_PC_H = POP();
    R_PC_V = POP();
    SetU(R_PC_V);
}

GS_VOID PmCPU::RETI()
{
    R_F = POP();
    R_PC_L = POP();
    R_PC_H = POP();
    R_PC_V = POP();
    SetU(R_PC_V);
    OnIRQ();
}

GS_VOID PmCPU::CALLX(GS_UINT16 addr)
{
    PUSH(R_PC_V);
    PUSH(R_PC_H);
    PUSH(R_PC_L);
    R_PC_V = R_U2 = R_U1;
    R_PC = Read16((R_I << 16) + addr);
}

GS_VOID PmCPU::CALLI(GS_UINT16 addr)
{
    PUSH(R_PC_V);
    PUSH(R_PC_H);
    PUSH(R_PC_L);
    JMPI(addr);
}

GS_VOID PmCPU::JMPI(GS_UINT16 addr)
{
    PUSH(R_F);
    R_F |= I_FLAG;
    R_PC_V = R_U2 = R_U1;
    R_PC = Read16(addr);
    OnIRQ();
}

GS_BYTE PmCPU::SAL(GS_BYTE a)
{
    (a & 0x80)? SET_F_C: CLEAR_F_C;
    a <<= 1;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    (R_F_C ^ R_F_S)? SET_F_O: CLEAR_F_O;
    return a;
}

GS_BYTE PmCPU::SHL(GS_BYTE a)
{
    (a & 0x80)? SET_F_C: CLEAR_F_C;
    a <<= 1;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::SAR(GS_BYTE a)
{
    (a & 0x01)? SET_F_C: CLEAR_F_C;
    a = (a & 0x80) | (a >> 1);
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    CLEAR_F_O;
    return a;
}

GS_BYTE PmCPU::SHR(GS_BYTE a)
{
    (a & 0x01)? SET_F_C: CLEAR_F_C;
    a >>= 1;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::ROLC(GS_BYTE a)
{
    GS_BYTE carry = (a & 0x80)? 1: 0;
    a = (a << 1) | R_F_C;
    carry? SET_F_C: CLEAR_F_C;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::ROL(GS_BYTE a)
{
    (a & 0x80)? SET_F_C: CLEAR_F_C;
    a = (a << 1) | (a >> 7);
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::RORC(GS_BYTE a)
{
    GS_BYTE carry = (a & 0x01)? 1: 0;
    a = (a >> 1) | (R_F_C << 7);
    carry? SET_F_C: CLEAR_F_C;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::ROR(GS_BYTE a)
{
    (a & 0x01)? SET_F_C: CLEAR_F_C;
    a = (a >> 1) | (a << 7);
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_BYTE PmCPU::NOT(GS_BYTE a)
{
    a ^= 0xFF;
    a? CLEAR_F_Z: SET_F_Z;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;    
}

GS_BYTE PmCPU::NEG(GS_BYTE a)
{
    a = -a;
    a? CLEAR_F_Z: SET_F_Z;
    a? SET_F_C: CLEAR_F_C;
    (a == 0x80)? SET_F_O: CLEAR_F_O;
    (a & 0x80)? SET_F_S: CLEAR_F_S;
    return a;
}

GS_VOID PmCPU::HALT()
{
    m_state.eStatus = STATUS_HALT;
}

GS_VOID PmCPU::STOP()
{
    m_state.eStatus = STATUS_STOP;
}

GS_VOID PmCPU::MUL()
{
    R_HL = R_L * R_A;
    R_HL? CLEAR_F_Z: SET_F_Z;
    CLEAR_F_C;
    CLEAR_F_O;
    (R_HL & 0x8000)? SET_F_S: CLEAR_F_S;
}

GS_VOID PmCPU::DIV()
{
    if (0 == R_A)
    {
        GS_ASSERT(GS_FALSE);
        return;
    }

    R_OP = R_HL / R_A;
    if (R_OP_BH)
    {
        CLEAR_F_Z;
        CLEAR_F_C;
        SET_F_O;
        SET_F_S;
    }
    else
    {
        R_H = R_HL % R_A;
        R_L = R_OP_BL;
        R_L? CLEAR_F_Z: SET_F_Z;
        CLEAR_F_C;
        CLEAR_F_O;
        (R_L & 0x80)? SET_F_S: CLEAR_F_S;
    }
}
