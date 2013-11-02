#include "PmTimer.h"
#include "Base/PmHardRegMap.h"

PmTimer::PmTimer(PmSystem *pSystem, GS_BYTE *pReg)
    : PmUnit(pSystem)
    , m_pReg(pReg)
{
    SetReady();
}

PmTimer::~PmTimer()
{
}

GS_VOID PmTimer::Reset(GS_BOOL hardReset)
{
    if (hardReset) { m_state.Reset(); }

    m_state.timer1.uiLoPreset = 0xFFFFFFFF;
    m_state.timer1.uiHiPreset = 0xFFFFFFFF;
    m_state.timer2.uiLoPreset = 0xFFFFFFFF;
    m_state.timer2.uiHiPreset = 0xFFFFFFFF;
    m_state.timer3.uiLoPreset = 0xFFFFFFFF;
    m_state.timer3.uiHiPreset = 0xFFFFFFFF;
}

GS_BYTE PmTimer::ReadReg(GS_BYTE reg)
{
    // 0x08 to 0x0F, 0x18 to 0x1F, 0x30 to 0x41, 0x48 to 0x4F
    switch(reg)
    {
    case 0x08: // Second Counter Control
        return PMR_SEC_CTRL & 0x01;
    case 0x09: // Second Counter Low
        return (GS_BYTE)m_state.uiSecondCount;
    case 0x0A: // Second Counter Med
        return (GS_BYTE)(m_state.uiSecondCount >> 8);
    case 0x0B: // Second Counter High
        return (GS_BYTE)(m_state.uiSecondCount >> 16);
    case 0x18: // Timer 1 Prescaler
        return PMR_TMR1_SCALE;
    case 0x19: // Timer 1 Osc. Select + Osc. Enable
        return PMR_TMR1_OSC & 0x33;
    case 0x1A: // Timer 2 Prescaler
        return PMR_TMR2_SCALE;
    case 0x1B: // Timer 2 Osc. Select
        return PMR_TMR2_OSC & 0x03;
    case 0x1C: // Timer 3 Prescaler
        return PMR_TMR3_SCALE;
    case 0x1D: // Timer 3 Osc. Select
        return PMR_TMR3_OSC & 0x03;
    case 0x30: // Timer 1 Control L
        return (PMR_TMR1_CTRL_L & 0x80)
             ? (PMR_TMR1_CTRL_L & 0x85)  // 16-Bits Mode
             : (PMR_TMR1_CTRL_L & 0x0D); //  8-Bits Mode
    case 0x31: // Timer 1 Control H
        return (PMR_TMR1_CTRL_L & 0x80)
             ? (PMR_TMR1_CTRL_H & 0x08)  // 16-Bits Mode
             : (PMR_TMR1_CTRL_H & 0x0D); //  8-Bits Mode
    case 0x32: // Timer 1 Preset L
        return (GS_BYTE)(m_state.timer1.uiLoPreset >> 24);
    case 0x33: // Timer 1 Preset H
        return (GS_BYTE)(m_state.timer1.uiHiPreset >> 24);
    case 0x34: // Timer 1 Pivot L
        return PMR_TMR1_PVT_L;
    case 0x35: // Timer 1 Pivot H
        return PMR_TMR1_PVT_H;
    case 0x36: // Timer 1 Count L
        return (GS_BYTE)(m_state.timer1.uiLoCount >> 24);
    case 0x37: // Timer 1 Count H
        return (GS_BYTE)(m_state.timer1.uiHiCount >> 24);
    case 0x38: // Timer 2 Control L
        return (PMR_TMR2_CTRL_L & 0x80)
             ? (PMR_TMR2_CTRL_L & 0x85)  // 16-Bits Mode
             : (PMR_TMR2_CTRL_L & 0x0D); //  8-Bits Mode
    case 0x39: // Timer 2 Control H
        return (PMR_TMR2_CTRL_L & 0x80)
             ? (PMR_TMR2_CTRL_H & 0x08)  // 16-Bits Mode
             : (PMR_TMR2_CTRL_H & 0x0D); // 8-Bits Mode
    case 0x3A: // Timer 2 Preset L
        return (GS_BYTE)(m_state.timer2.uiLoPreset >> 24);
    case 0x3B: // Timer 2 Preset H
        return (GS_BYTE)(m_state.timer2.uiHiPreset >> 24);
    case 0x3C: // Timer 2 Pivot L
        return PMR_TMR2_PVT_L;
    case 0x3D: // Timer 2 Pivot H
        return PMR_TMR2_PVT_H;
    case 0x3E: // Timer 2 Count L
        return (GS_BYTE)(m_state.timer2.uiLoCount >> 24);
    case 0x3F: // Timer 2 Count H
        return (GS_BYTE)(m_state.timer2.uiHiCount >> 24);
    case 0x40: // 256 Hz Timer Control
        return PMR_TMR256_CTRL;
    case 0x41: // 256 Hz Timer Counter
        return m_state.ui256HzCounter >> 24;
    case 0x48: // Timer 3 Control L
        return (PMR_TMR3_CTRL_L & 0x80)
             ? (PMR_TMR3_CTRL_L & 0x85)  // 16-Bits Mode
             : (PMR_TMR3_CTRL_L & 0x0D); //  8-Bits Mode
    case 0x49: // Timer 3 Control H
        return (PMR_TMR3_CTRL_L & 0x80)
             ? (PMR_TMR3_CTRL_H & 0x08)  // 16-Bits Mode
             : (PMR_TMR3_CTRL_H & 0x0D); //  8-Bits Mode
    case 0x4A: // Timer 3 Preset L
        return (GS_BYTE)(m_state.timer3.uiLoPreset >> 24);
    case 0x4B: // Timer 3 Preset H
        return (GS_BYTE)(m_state.timer3.uiHiPreset >> 24);
    case 0x4C: // Timer 3 Pivot L
        return (GS_BYTE)m_state.timer3.usPivot;
    case 0x4D: // Timer 3 Pivot H
        return (GS_BYTE)(m_state.timer3.usPivot >> 8);
    case 0x4E: // Timer 3 Count L
        return (GS_BYTE)(m_state.timer3.uiLoCount >> 24);
    case 0x4F: // Timer 3 Count H
        return (GS_BYTE)(m_state.timer3.uiHiCount >> 24);
    default:   // Unused
        GS_ASSERT(GS_FALSE);
        return 0;
    }
}

void PmTimer::WriteReg(GS_BYTE reg, GS_BYTE val)
{
    // 0x08 to 0x0F, 0x18 to 0x1F, 0x30 to 0x41, 0x48 to 0x4F
    switch(reg)
    {
    case 0x08: // Second Counter Control
        if (val & 0x02) { m_state.uiSecondCount = 0; }
        PMR_SEC_CTRL = val & 0x01;
        return;
    case 0x09: // Second Counter Low
    case 0x0A: // Second Counter Med
    case 0x0B: // Second Counter High
        return;
    case 0x18: // Timer 1 Prescaler
        PMR_TMR1_SCALE = val;
        UpdateScalarTimer1();
        return;
    case 0x19: // Timer 1 Osc. Select + Osc. Enable
        PMR_TMR1_OSC = val & 0x33;
        m_state.bOscillator2Enabled = val & 0x10;
        m_state.bOscillator1Enabled = val & 0x20;
        UpdateScalarTimer1();
        UpdateScalarTimer2();
        UpdateScalarTimer3();
        return;
    case 0x1A: // Timer 2 Prescaler
        PMR_TMR2_SCALE = val;
        UpdateScalarTimer2();
        return;
    case 0x1B: // Timer 2 Osc. Select
        PMR_TMR2_OSC = val & 0x03;
        UpdateScalarTimer2();
        return;
    case 0x1C: // Timer 3 Prescaler
        PMR_TMR3_SCALE = val;
        UpdateScalarTimer3();
        return;
    case 0x1D: // Timer 3 Osc. Select
        PMR_TMR3_OSC = val & 0x03;
        UpdateScalarTimer3();
        return;
    case 0x30: // Timer 1 Control L
        PMR_TMR1_CTRL_L = val & 0x8D;
        if (val & 0x02)
        {
            if (PMR_TMR1_CTRL_L & 0x80) // 16-Bits
            {
                m_state.timer1.PresetLo();
                m_state.timer1.PresetHi();
            }
            else // 8-Bits
            {
                m_state.timer1.PresetLo();
            }
        }
        return;
    case 0x31: // Timer 1 Control H
        PMR_TMR1_CTRL_H = val & 0x0D;
        if (val & 0x02)
        {
            if (PMR_TMR1_CTRL_L & 0x80) // 16-Bits
            {
            }
            else // 8-Bits
            {
                m_state.timer1.PresetHi();
            }
        }
        return;
    case 0x32: // Timer 1 Preset L
        m_state.timer1.uiLoPreset = val << 24;
        return;
    case 0x33: // Timer 1 Preset H
        m_state.timer1.uiHiPreset = val << 24;
        return;
    case 0x34: // Timer 1 Pivot L
        PMR_TMR1_PVT_L = val;
        return;
    case 0x35: // Timer 1 Pivot H
        PMR_TMR1_PVT_H = val;
        return;
    case 0x36: // Timer 1 Count L
        return;
    case 0x37: // Timer 1 Count H
        return;
    case 0x38: // Timer 2 Control L
        PMR_TMR2_CTRL_L = val & 0x8D;
        if (val & 0x02)
        {
            if (PMR_TMR2_CTRL_L & 0x80)
            {
                m_state.timer2.PresetLo();
                m_state.timer2.PresetHi();
            }
            else // 8-Bits
            {
                m_state.timer2.PresetLo();
            }
        }
        return;
    case 0x39: // Timer 2 Control H
        PMR_TMR2_CTRL_H = val & 0x0D;
        if (val & 0x02)
        {
            if (PMR_TMR2_CTRL_L & 0x80)
            {
            }
            else // 8-Bits
            {
                m_state.timer2.PresetHi();
            }
        }
        return;
    case 0x3A: // Timer 2 Preset L
        m_state.timer2.uiLoPreset = val << 24;
        return;
    case 0x3B: // Timer 2 Preset H
        m_state.timer2.uiHiPreset = val << 24;
        return;
    case 0x3C: // Timer 2 Pivot L
        PMR_TMR2_PVT_L = val;
        return;
    case 0x3D: // Timer 2 Pivot H
        PMR_TMR2_PVT_H = val;
        return;
    case 0x3E: // Timer 2 Count L
        return;
    case 0x3F: // Timer 2 Count H
        return;
    case 0x40: // 256 Hz Timer Control
        if (val & 0x02) { m_state.ui256HzCounter = 0; }
        PMR_TMR256_CTRL = val & 0x01;
        return;
    case 0x41: // 256 Hz Timer Counter
        return;
    case 0x48: // Timer 3 Control L
        PMR_TMR3_CTRL_L = val & 0x8D;
        if (val & 0x02)
        {
            if (PMR_TMR3_CTRL_L & 0x80)
            {
                m_state.timer3.PresetLo();
                m_state.timer3.PresetHi();
            }
            else // 8-Bits
            {
                m_state.timer3.PresetLo();
            }
        }
        return;
    case 0x49: // Timer 3 Control H
        PMR_TMR3_CTRL_H = val & 0x0D;
        if (val & 0x02)
        {
            if (PMR_TMR3_CTRL_L & 0x80)
            {
            }
            else // 8-Bits
            {
                m_state.timer3.PresetHi();
            }
        }
        return;
    case 0x4A: // Timer 3 Preset L
        m_state.timer3.uiLoPreset = val << 24;
        return;
    case 0x4B: // Timer 3 Preset H
        m_state.timer3.uiHiPreset = val << 24;
        return;
    case 0x4C: // Timer 3 Pivot L
        m_state.timer3.SetPivotLo(val);
        return;
    case 0x4D: // Timer 3 Pivot H
        m_state.timer3.SetPivotHi(val);
        return;
    case 0x4E: // Timer 3 Count L
        return;
    case 0x4F: // Timer 3 Count H
        return;
    }
}

GS_VOID PmTimer::Sync(GS_UINT cycles)
{
    // Process 256Hz Timer (Increment)
    if (PMR_TMR256_CTRL)
    {
        GS_UINT32 preCount = m_state.ui256HzCounter;
        m_state.ui256HzCounter += cycles * 0x01000000 / (PmBase::PM_CPU_RATE >> 8); // cycles * 0x0100000000 / PmBase::PM_CPU_RATE
        if ((preCount & 0x08000000) ^ (m_state.ui256HzCounter & 0x08000000)) // 32Hz
        {
            SysSetIrqAct(PM_INTR_0B);
        }
        if ((preCount & 0x20000000) ^ (m_state.ui256HzCounter & 0x20000000)) // 8Hz
        {
            SysSetIrqAct(PM_INTR_0C);
        }
        if ((preCount & 0x80000000) ^ (m_state.ui256HzCounter & 0x80000000)) // 2Hz
        {
            SysSetIrqAct(PM_INTR_0D);
        }
        if (preCount > m_state.ui256HzCounter) // 1Hz
        {
            SysSetIrqAct(PM_INTR_0E);
        }
    }

    // Process Second Timer (Increment)
    if (PMR_SEC_CTRL)
    {
        m_state.uiCycleCount += cycles;
        if (m_state.uiCycleCount >= PmBase::PM_CPU_RATE)
        {
            m_state.uiCycleCount -= PmBase::PM_CPU_RATE;
            ++m_state.uiSecondCount;
        }
    }

    // Process Timer 1 (Decrement)
    if (PMR_TMR1_CTRL_L & 0x80) // 1x 16-Bits Timer
    {
        if (PMR_TMR1_CTRL_L & 0x04)
        {
            if (m_state.timer1.Dec16BitsLo(cycles))
            {
                if (m_state.timer1.Dec16BitsHi()) // IRQ Timer 1-Hi underflow
                {
                    SysSetIrqAct(PM_INTR_07);
                }
            }
        }
    }
    else // 2x 8-Bits Timers
    {
        if (PMR_TMR1_CTRL_L & 0x04) // Timer Lo
        {
            if (m_state.timer1.Dec8BitsLo(cycles)) // IRQ Timer 1-Lo underflow
            {
                SysSetIrqAct(PM_INTR_08);
            }
        }
        if (PMR_TMR1_CTRL_H & 0x04) // Timer Hi
        {
            if (m_state.timer1.Dec8BitsHi(cycles)) // IRQ Timer 1-Hi underflow
            {
                SysSetIrqAct(PM_INTR_07);
            }
        }
    }

    // Process Timer 2 (Decrement)
    if (PMR_TMR2_CTRL_L & 0x80) // 1x 16-Bits Timer
    {
        if (PMR_TMR2_CTRL_L & 0x04)
        {
            if (m_state.timer2.Dec16BitsLo(cycles))
            {
                if (m_state.timer2.Dec16BitsHi()) // IRQ Timer 2-Hi underflow
                {
                    SysSetIrqAct(PM_INTR_05);
                }
            }
        }
    }
    else // 2x 8-Bits Timers
    {
        if (PMR_TMR2_CTRL_L & 0x04) // Timer Lo
        {
            if (m_state.timer2.Dec8BitsLo(cycles)) // IRQ Timer 2-Lo underflow
            {
                SysSetIrqAct(PM_INTR_06);
            }
        }
        if (PMR_TMR2_CTRL_H & 0x04) // Timer Hi
        {
            if (m_state.timer2.Dec8BitsHi(cycles)) // IRQ Timer 2-Hi underflow
            {
                SysSetIrqAct(PM_INTR_05);
            }
        }
    }

    // Process Timer 3
    if (PMR_TMR3_CTRL_L & 0x80) // 1x 16-Bits Timer
    {
        if (PMR_TMR3_CTRL_L & 0x04)
        {
            if (m_state.timer3.Dec16BitsLo(cycles))
            {
                if (m_state.timer3.Dec16BitsHi()) // IRQ Timer 3 underflow
                {
                    SysSetIrqAct(PM_INTR_09);
                }
            }

            GS_BOOL overPivot = !Timer3CheckPivot();
            m_state.timer3.UpdateCountLo();
            m_state.timer3.UpdateCountHi();
            if (overPivot && Timer3CheckPivot()) // IRQ Timer 3 Pivot
            {
                SysSetIrqAct(PM_INTR_0A);
            }
        }
    }
    else // 2x 8-Bits Timers
    {
        if (PMR_TMR3_CTRL_L & 0x04) // Timer Lo
        {
            m_state.timer3.Dec8BitsLo(cycles); // No check return
            m_state.timer3.UpdateCountLo();
        }
        if (PMR_TMR3_CTRL_H & 0x04) // Timer Hi
        {
            if (m_state.timer3.Dec8BitsHi(cycles)) // IRQ Timer 3 underflow
            {
                SysSetIrqAct(PM_INTR_09);
            }

            GS_BOOL overPivot = !Timer3CheckPivot();
            m_state.timer3.UpdateCountHi();
            if (overPivot && Timer3CheckPivot()) // IRQ Timer 3 Pivot
            {
                SysSetIrqAct(PM_INTR_0A);
            }
        }
    }
}

GS_BOOL PmTimer::Timer3CheckPivot() const
{
    return (m_state.timer3.usCount <= m_state.timer3.usPivot);
}

GS_VOID PmTimer::UpdateScalarTimer1()
{
    UpdateScalar(m_state.timer1, PMR_TMR1_OSC, PMR_TMR1_SCALE);
}

GS_VOID PmTimer::UpdateScalarTimer2()
{
    UpdateScalar(m_state.timer2, PMR_TMR2_OSC, PMR_TMR2_SCALE);
}

GS_VOID PmTimer::UpdateScalarTimer3()
{
    UpdateScalar(m_state.timer3, PMR_TMR3_OSC, PMR_TMR3_SCALE);
}

GS_BOOL PmTimer::Timer::Dec16BitsLo(GS_UINT cycles)
{
    GS_UINT32 preCount = uiLoCount;
    uiLoCount -= uiLoDecrement * cycles;
    if (preCount < uiLoCount)
    {
        return GS_TRUE;
    }

    return GS_FALSE;
}

GS_BOOL PmTimer::Timer::Dec16BitsHi()
{
    GS_UINT32 preCount = uiHiCount;
    uiHiCount -= 0x01000000;
    if (preCount < uiHiCount)
    {
        PresetLo();
        PresetHi();
        return GS_TRUE;
    }

    return GS_FALSE;
}

GS_BOOL PmTimer::Timer::Dec8BitsLo(GS_UINT cycles)
{
    GS_UINT32 preCount = uiLoCount;
    uiLoCount -= uiLoDecrement * cycles;
    if (preCount < uiLoCount)
    {
        PresetLo();
        return GS_TRUE;
    }

    return GS_FALSE;
}

GS_BOOL PmTimer::Timer::Dec8BitsHi(GS_UINT cycles)
{
    GS_UINT32 preCount = uiHiCount;
    uiHiCount -= uiHiDecrement * cycles;
    if (preCount < uiHiCount)
    {
        PresetHi();
        return GS_TRUE;
    }

    return GS_FALSE;
}

const GS_UINT32 PmTimer::CALCULATE_DECREASE_ON_OSCILLATOR_1[16] =
{
    // Osci1 disabled
    0, 0, 0, 0, 0, 0, 0, 0,
    // Osci1 Enabled
    (16777216 / 2),    // 2000000 Hz
    (16777216 / 8),    //  500000 Hz
    (16777216 / 32),   //  125000 Hz
    (16777216 / 64),   //   62500 Hz
    (16777216 / 128),  //   31250 Hz
    (16777216 / 256),  //   15625 Hz
    (16777216 / 1024), //    3906.25 Hz
    (16777216 / 4096)  //     976.5625 Hz
};

const GS_UINT32 PmTimer::CALCULATE_DECREASE_ON_OSCILLATOR_2[16] =
{
    // Osci2 disabled
    0, 0, 0, 0, 0, 0, 0, 0,
    // Osci2 Enabled (Aproximate values)
    (16777216 / 122),  // 32768 Hz
    (16777216 / 244),  // 16384 Hz
    (16777216 / 488),  //  8192 Hz
    (16777216 / 977),  //  4096 Hz
    (16777216 / 1953), //  2048 Hz
    (16777216 / 3906), //  1024 Hz
    (16777216 / 7812), //   512 Hz
    (16777216 / 15625) //   256 Hz
};

GS_VOID PmTimer::UpdateScalar(Timer &timer, GS_BYTE osc, GS_BYTE scale) const
{
    timer.uiLoDecrement = 0;
    if (osc & 0x01)
    {
        if (m_state.bOscillator2Enabled)
        {
            timer.uiLoDecrement = CALCULATE_DECREASE_ON_OSCILLATOR_2[scale & 0xF];
        }
    }
    else
    {
        if (m_state.bOscillator1Enabled)
        {
            timer.uiLoDecrement = CALCULATE_DECREASE_ON_OSCILLATOR_1[scale & 0xF];
        }
    }

    timer.uiHiDecrement = 0;
    if (osc & 0x02)
    {
        if (m_state.bOscillator2Enabled)
        {
            timer.uiHiDecrement = CALCULATE_DECREASE_ON_OSCILLATOR_2[(scale >> 4) & 0xF];
        }
    }
    else
    {
        if (m_state.bOscillator1Enabled)
        {
            timer.uiHiDecrement = CALCULATE_DECREASE_ON_OSCILLATOR_1[(scale >> 4) & 0xF];
        }
    }
}
