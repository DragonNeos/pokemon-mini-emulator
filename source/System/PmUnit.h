#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_SYSTEM_PMUNIT_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_SYSTEM_PMUNIT_H__

#include "Base/PmBase.h"

class PmSystem;

class PmUnit
{
public:
    PmUnit(PmSystem *pSystem);
    virtual ~PmUnit() {}

    GS_BOOL CheckReady() const { return m_ready; }
    virtual GS_VOID Reset(GS_BOOL hardReset) = 0;

    virtual GS_BYTE ReadReg(GS_BYTE reg) = 0;
    virtual GS_VOID WriteReg(GS_BYTE reg, GS_BYTE val) = 0;

protected:
    GS_VOID SetReady() { m_ready = GS_TRUE; }

    GS_BYTE SysRead(GS_UINT32 addr);
    GS_VOID SysWrite(GS_UINT32 addr, GS_BYTE val);
    GS_VOID SysSetIrq(GS_BOOL enable);
    GS_VOID SysSetIrqAct(GS_BYTE intr);
    GS_VOID SysCallIrq(GS_BYTE intr);
    GS_VOID SysCopyFrameBufferToLcd(const GS_BYTE *pFrameBuffer);
    GS_VOID SysRefreshLcd();
    GS_VOID SysSetCpuStall(GS_BOOL stall);
    GS_BOOL SysTimer3CheckPivot() const;

private:
    PmSystem * m_pSystem;
    GS_BOOL m_ready;
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_SYSTEM_PMUNIT_H__
