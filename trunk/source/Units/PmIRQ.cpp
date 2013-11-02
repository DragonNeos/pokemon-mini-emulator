#include "PmIRQ.h"
#include "Base/PmHardRegMap.h"

PmIRQ::PmIRQ(PmSystem *pSystem, GS_BYTE *pReg)
    : PmUnit(pSystem)
    , m_pReg(pReg)
{
    SetReady();
}

PmIRQ::~PmIRQ()
{
}

GS_VOID PmIRQ::Reset(GS_BOOL /*hardReset*/)
{
    m_state.Reset();

    m_state.bEnable = 1;
    PMR_IRQ_ACT1 = 0x00;
    PMR_IRQ_ACT2 = 0x00;
    PMR_IRQ_ACT3 = 0x00;
    PMR_IRQ_ACT4 = 0x00;
}

GS_BYTE PmIRQ::ReadReg(GS_BYTE reg)
{
    // 0x20 to 0x2A
    switch (reg)
    {
    case 0x20: // IRQ Priority 1
        return PMR_IRQ_PRI1;
    case 0x21: // IRQ Priority 2
        return PMR_IRQ_PRI2;
    case 0x22: // IRQ Priority 3
        return PMR_IRQ_PRI3 & 0x03;
    case 0x23: // IRQ Enable 1
        return PMR_IRQ_ENA1;
    case 0x24: // IRQ Enable 2
        return PMR_IRQ_ENA2 & 0x3F;
    case 0x25: // IRQ Enable 3
        return PMR_IRQ_ENA3;
    case 0x26: // IRQ Enable 4
        return PMR_IRQ_ENA4 & 0xF7;
    case 0x27: // IRQ Active 1
        return PMR_IRQ_ACT1;
    case 0x28: // IRQ Active 2
        return PMR_IRQ_ACT2 & 0x3F;
    case 0x29: // IRQ Active 3
        return PMR_IRQ_ACT3;
    case 0x2A: // IRQ Active 4
        return PMR_IRQ_ACT4 & 0xF7;
    default:   // Unused
        return 0;
    }
}

GS_VOID PmIRQ::WriteReg(GS_BYTE reg, GS_BYTE val)
{
    // 0x20 to 0x2A
    switch (reg)
    {
    case 0x20: // IRQ Priority 1
        PMR_IRQ_PRI1 = val;
        Process();
        return;
    case 0x21: // IRQ Priority 2
        PMR_IRQ_PRI2 = val;
        Process();
        return;
    case 0x22: // IRQ Priority 3
        PMR_IRQ_PRI3 = val;
        Process();
        return;
    case 0x23: // IRQ Enable 1
        PMR_IRQ_ENA1 = val;
        Process();
        return;
    case 0x24: // IRQ Enable 2
        PMR_IRQ_ENA2 = val & 0x3F;
        Process();
        return;
    case 0x25: // IRQ Enable 3
        PMR_IRQ_ENA3 = val;
        Process();
        return;
    case 0x26: // IRQ Enable 4
        PMR_IRQ_ENA4 = val & 0xF7;
        Process();
        return;
    case 0x27: // IRQ Active 1
        PMR_IRQ_ACT1 &= ~val;
        return;
    case 0x28: // IRQ Active 2
        PMR_IRQ_ACT2 &= ~val;
        return;
    case 0x29: // IRQ Active 3
        PMR_IRQ_ACT3 &= ~val;
        return;
    case 0x2A: // IRQ Active 4
        PMR_IRQ_ACT4 &= ~val;
        return;
    default:   // Unused
        return;
    }
}

GS_VOID PmIRQ::SetEnable(GS_BOOL enable)
{
    m_state.bEnable = enable? 1: 0;
}

GS_VOID PmIRQ::Set(GS_BYTE intr)
{
    switch (intr)
    {
    case 0x03: // PRC Copy Complete
        PMR_IRQ_ACT1 |= 0x80;
        break;
    case 0x04: // PRC Frame Divider Overflow
        PMR_IRQ_ACT1 |= 0x40;
        break;
    case 0x05: // Timer 2-B Underflow
        PMR_IRQ_ACT1 |= 0x20;
        break;
    case 0x06: // Timer 2-A Underflow (8-Bits only)
        PMR_IRQ_ACT1 |= 0x10;
        break;
    case 0x07: // Timer 1-B Underflow
        PMR_IRQ_ACT1 |= 0x08;
        break;
    case 0x08: // Timer 1-A Underflow (8-Bits only)
        PMR_IRQ_ACT1 |= 0x04;
        break;
    case 0x09: // Timer 3 Underflow
        PMR_IRQ_ACT1 |= 0x02;
        break;
    case 0x0A: // Timer 3 Pivot
        PMR_IRQ_ACT1 |= 0x01;
        break;
    case 0x0B: // 32 Hz
        PMR_IRQ_ACT2 |= 0x20;
        break;
    case 0x0C: //  8 Hz
        PMR_IRQ_ACT2 |= 0x10;
        break;
    case 0x0D: //  2 Hz
        PMR_IRQ_ACT2 |= 0x08;
        break;
    case 0x0E: //  1 Hz
        PMR_IRQ_ACT2 |= 0x04;
        break;
    case 0x0F: // IR Receiver
        PMR_IRQ_ACT4 |= 0x80;
        break;
    case 0x10: // Shock Sensor
        PMR_IRQ_ACT4 |= 0x40;
        break;
    case 0x13: // Cartridge Ejected
        PMR_IRQ_ACT2 |= 0x02;
        break;
    case 0x14: // Cartridge IRQ
        PMR_IRQ_ACT2 |= 0x01;
        break;
    case 0x15: // Power Key
        PMR_IRQ_ACT3 |= 0x80;
        break;
    case 0x16: // Right Key
        PMR_IRQ_ACT3 |= 0x40;
        break;
    case 0x17: // Left Key
        PMR_IRQ_ACT3 |= 0x20;
        break;
    case 0x18: // Down Key
        PMR_IRQ_ACT3 |= 0x10;
        break;
    case 0x19: // Up Key
        PMR_IRQ_ACT3 |= 0x08;
        break;
    case 0x1A: // C Key
        PMR_IRQ_ACT3 |= 0x04;
        break;
    case 0x1B: // B Key
        PMR_IRQ_ACT3 |= 0x02;
        break;
    case 0x1C: // A Key
        PMR_IRQ_ACT3 |= 0x01;
        break;
    }
}

GS_VOID PmIRQ::Process()
{
    RETURN_ON_FAIL(0 != m_state.bEnable);

    if (PMR_IRQ_PRI1 & 0xC0)
    {
        if ((PMR_IRQ_ENA1 & 0x80) && (PMR_IRQ_ACT1 & 0x80)) { SysCallIrq(0x03); return; }
        if ((PMR_IRQ_ENA1 & 0x40) && (PMR_IRQ_ACT1 & 0x40)) { SysCallIrq(0x04); return; }
    }
    if (PMR_IRQ_PRI1 & 0x30)
    {
        if ((PMR_IRQ_ENA1 & 0x20) && (PMR_IRQ_ACT1 & 0x20)) { SysCallIrq(0x05); return; }
        if ((PMR_IRQ_ENA1 & 0x10) && (PMR_IRQ_ACT1 & 0x10)) { SysCallIrq(0x06); return; }
    }
    if (PMR_IRQ_PRI1 & 0x0C)
    {
        if ((PMR_IRQ_ENA1 & 0x08) && (PMR_IRQ_ACT1 & 0x08)) { SysCallIrq(0x07); return; }
        if ((PMR_IRQ_ENA1 & 0x04) && (PMR_IRQ_ACT1 & 0x04)) { SysCallIrq(0x08); return; }
    }
    if (PMR_IRQ_PRI1 & 0x03)
    {
        if ((PMR_IRQ_ENA1 & 0x02) && (PMR_IRQ_ACT1 & 0x02)) { SysCallIrq(0x09); return; }
        if ((PMR_IRQ_ENA1 & 0x01) && (PMR_IRQ_ACT1 & 0x01)) { SysCallIrq(0x0A); return; }
    }
    if (PMR_IRQ_PRI2 & 0xC0)
    {
        if ((PMR_IRQ_ENA2 & 0x20) && (PMR_IRQ_ACT2 & 0x20)) { SysCallIrq(0x0B); return; }
        if ((PMR_IRQ_ENA2 & 0x10) && (PMR_IRQ_ACT2 & 0x10)) { SysCallIrq(0x0C); return; }
        if ((PMR_IRQ_ENA2 & 0x08) && (PMR_IRQ_ACT2 & 0x08)) { SysCallIrq(0x0D); return; }
        if ((PMR_IRQ_ENA2 & 0x04) && (PMR_IRQ_ACT2 & 0x04)) { SysCallIrq(0x0E); return; }
    }
    if (PMR_IRQ_PRI3 & 0x03)
    {
        if ((PMR_IRQ_ENA4 & 0x80) && (PMR_IRQ_ACT4 & 0x80)) { SysCallIrq(0x0F); return; }
        if ((PMR_IRQ_ENA4 & 0x40) && (PMR_IRQ_ACT4 & 0x40)) { SysCallIrq(0x10); return; }
    }
    if (PMR_IRQ_PRI2 & 0x30)
    {
        if ((PMR_IRQ_ENA2 & 0x02) && (PMR_IRQ_ACT2 & 0x02)) { SysCallIrq(0x13); return; }
        if ((PMR_IRQ_ENA2 & 0x01) && (PMR_IRQ_ACT2 & 0x01)) { SysCallIrq(0x14); return; }
    }
    if (PMR_IRQ_PRI2 & 0x0C)
    {
        if ((PMR_IRQ_ENA3 & 0x80) && (PMR_IRQ_ACT3 & 0x80)) { SysCallIrq(0x15); return; }
        if ((PMR_IRQ_ENA3 & 0x40) && (PMR_IRQ_ACT3 & 0x40)) { SysCallIrq(0x16); return; }
        if ((PMR_IRQ_ENA3 & 0x20) && (PMR_IRQ_ACT3 & 0x20)) { SysCallIrq(0x17); return; }
        if ((PMR_IRQ_ENA3 & 0x10) && (PMR_IRQ_ACT3 & 0x10)) { SysCallIrq(0x18); return; }
        if ((PMR_IRQ_ENA3 & 0x08) && (PMR_IRQ_ACT3 & 0x08)) { SysCallIrq(0x19); return; }
        if ((PMR_IRQ_ENA3 & 0x04) && (PMR_IRQ_ACT3 & 0x04)) { SysCallIrq(0x1A); return; }
        if ((PMR_IRQ_ENA3 & 0x02) && (PMR_IRQ_ACT3 & 0x02)) { SysCallIrq(0x1B); return; }
        if ((PMR_IRQ_ENA3 & 0x01) && (PMR_IRQ_ACT3 & 0x01)) { SysCallIrq(0x1C); return; }
    }
    if (PMR_IRQ_PRI2 & 0x03)
    {
        if ((PMR_IRQ_ENA4 & 0x04) && (PMR_IRQ_ACT4 & 0x04)) { SysCallIrq(0x1D); return; }
        if ((PMR_IRQ_ENA4 & 0x02) && (PMR_IRQ_ACT4 & 0x02)) { SysCallIrq(0x1E); return; }
        if ((PMR_IRQ_ENA4 & 0x01) && (PMR_IRQ_ACT4 & 0x01)) { SysCallIrq(0x1F); return; }
    }
}
