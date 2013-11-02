#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMIRQ_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMIRQ_H__

#include "System/PmUnit.h"

// Interrupt ReQuest
class PmIRQ : public PmUnit
{
public:
    PmIRQ(PmSystem *pSystem, GS_BYTE *pReg);
    virtual ~PmIRQ();

    virtual GS_VOID Reset(GS_BOOL hardReset);

    virtual GS_BYTE ReadReg(GS_BYTE reg);
    virtual GS_VOID WriteReg(GS_BYTE reg, GS_BYTE val);

public:
    GS_VOID SetEnable(GS_BOOL enable);
    GS_VOID Set(GS_BYTE intr);
    GS_VOID Process();

private:
    struct State
    {
        GS_BYTE bEnable;

        State() { Reset(); }
        GS_VOID Reset() { gs_zero(this, sizeof(*this)); }
    };

    State m_state;

private:
    GS_BYTE * m_pReg;
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMIRQ_H__
