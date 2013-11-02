#include "PmUnit.h"
#include "PmSystem.h"

PmUnit::PmUnit(PmSystem *pSystem)
    : m_pSystem(pSystem)
    , m_ready(GS_FALSE)
{
    GS_ASSERT(GS_NULL != m_pSystem);
}

GS_BYTE PmUnit::SysRead(GS_UINT32 addr)
{
    return m_pSystem->Read(addr);
}

GS_VOID PmUnit::SysWrite(GS_UINT32 addr, GS_BYTE val)
{
    m_pSystem->Write(addr, val);
}

GS_VOID PmUnit::SysSetIrq(GS_BOOL enable)
{
    m_pSystem->SetIrq(enable);
}

GS_VOID PmUnit::SysSetIrqAct(GS_BYTE intr)
{
    m_pSystem->SetIrqAct(intr);
}

GS_VOID PmUnit::SysCallIrq(GS_BYTE intr)
{
    m_pSystem->CallIrq(intr);
}

GS_VOID PmUnit::SysCopyFrameBufferToLcd(const GS_BYTE *pFrameBuffer)
{
    m_pSystem->CopyFrameBufferToLcd(pFrameBuffer);
}

GS_VOID PmUnit::SysRefreshLcd()
{
    m_pSystem->RefreshLcd();
}

GS_VOID PmUnit::SysSetCpuStall(GS_BOOL stall)
{
    m_pSystem->SetCpuStall(stall);
}

GS_BOOL PmUnit::SysTimer3CheckPivot() const
{
    return m_pSystem->Timer3CheckPivot();
}
