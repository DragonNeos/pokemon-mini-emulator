#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMTIMER_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMTIMER_H__

#include "System/PmUnit.h"

class PmTimer : public PmUnit
{
public:
    PmTimer(PmSystem *pSystem, GS_BYTE *pReg);
    virtual ~PmTimer();

    virtual GS_VOID Reset(GS_BOOL hardReset);

    virtual GS_BYTE ReadReg(GS_BYTE reg);
    virtual GS_VOID WriteReg(GS_BYTE reg, GS_BYTE val);

public:
    GS_VOID Sync(GS_UINT cycles);
    GS_BOOL Timer3CheckPivot() const;

private:
    enum
    {
        PM_INTR_05 = 0x05, // Timer 2-B Underflow
        PM_INTR_06 = 0x06, // Timer 2-A Underflow (8-Bits only)
        PM_INTR_07 = 0x07, // Timer 1-B Underflow
        PM_INTR_08 = 0x08, // Timer 1-A Underflow (8-Bits only)
        PM_INTR_09 = 0x09, // Timer 3 Underflow
        PM_INTR_0A = 0x0A, // Timer 3 Pivot
        PM_INTR_0B = 0x0B, // 32 Hz
        PM_INTR_0C = 0x0C, //  8 Hz
        PM_INTR_0D = 0x0D, //  2 Hz
        PM_INTR_0E = 0x0E  //  1 Hz
    };

    GS_VOID UpdateScalarTimer1();
    GS_VOID UpdateScalarTimer2();
    GS_VOID UpdateScalarTimer3();

private:
    struct Timer
    {
        GS_UINT32 uiLoDecrement;
        GS_UINT32 uiHiDecrement;
        GS_UINT32 uiLoCount;
        GS_UINT32 uiHiCount;
        GS_UINT32 uiLoPreset;
        GS_UINT32 uiHiPreset;

        GS_VOID PresetLo() { uiLoCount = uiLoPreset; }
        GS_VOID PresetHi() { uiHiCount = uiHiPreset; }

        // Return underflow or not
        GS_BOOL Dec16BitsLo(GS_UINT cycles);
        GS_BOOL Dec16BitsHi();
        GS_BOOL Dec8BitsLo(GS_UINT cycles);
        GS_BOOL Dec8BitsHi(GS_UINT cycles);
    };

    struct TimerEx : public Timer
    {
        GS_UINT16 usCount;
        GS_UINT16 usPivot;

        GS_VOID UpdateCountLo() { usCount &= 0xFF00; usCount |= (uiLoCount >> 24); }
        GS_VOID UpdateCountHi() { usCount &= 0x00FF; usCount |= ((uiHiCount >> 24) << 8); }
        GS_VOID SetPivotLo(GS_BYTE val) { usPivot &= 0xFF00; usPivot |= val; }
        GS_VOID SetPivotHi(GS_BYTE val) { usPivot &= 0x00FF; usPivot |= (val << 8); }
    };

    struct State
    {
        GS_UINT32 uiSecondCount;
        GS_UINT32 uiCycleCount;
        GS_UINT32 ui256HzCounter;
        Timer     timer1;
        Timer     timer2;
        TimerEx   timer3;
        GS_BYTE   bOscillator1Enabled;
        GS_BYTE   bOscillator2Enabled;

        State() { Reset(); }
        GS_VOID Reset() { gs_zero(this, sizeof(*this)); }
    };

    static const GS_UINT32 CALCULATE_DECREASE_ON_OSCILLATOR_1[16];
    static const GS_UINT32 CALCULATE_DECREASE_ON_OSCILLATOR_2[16];
    GS_VOID UpdateScalar(Timer &timer, GS_BYTE osc, GS_BYTE scale) const;

    State m_state;

private:
    GS_BYTE * m_pReg;
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMTIMER_H__
