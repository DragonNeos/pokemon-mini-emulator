#include "PmMapper.h"

PmMapper * PmMapper::Create(const PmMemory *pMem, GS_UINT type)
{
    switch (type)
    {
    case 2:
    case 1:
    case 0:
    default:
        return new PmMapper0(pMem);
    }
}

PmMapper0::PmMapper0(const PmMemory *pMem)
    : PmMapper(pMem)
{
}

GS_BYTE PmMapper0::Read(GS_UINT32 addr)
{
    return m_pMem->Value(addr);
}

GS_VOID PmMapper0::Write(GS_UINT32 addr, GS_BYTE val)
{
    if (0x1000 <= addr && addr < 0x2000)
    {
        m_pMem->Value(addr) = val;
    }
    else
    {
        //GS_ASSERT(GS_FALSE);
    }
}
