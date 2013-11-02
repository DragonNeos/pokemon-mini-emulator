#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMMAPPER_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMMAPPER_H__

#include "PmMemory.h"

class PmMapper
{
public:
    static PmMapper * Create(const PmMemory *pMem, GS_UINT type);

public:
    PmMapper(const PmMemory *pMem) : m_pMem(pMem) {}
    virtual ~PmMapper() {}

public:
    virtual GS_BYTE Read(GS_UINT32 addr) = 0;
    virtual GS_VOID Write(GS_UINT32 addr, GS_BYTE val) = 0;

protected:
    const PmMemory * m_pMem;
};

class PmMapper0 : public PmMapper
{
public:
    PmMapper0(const PmMemory *pMem);
    virtual ~PmMapper0() {}

public:
    virtual GS_BYTE Read(GS_UINT32 addr);
    virtual GS_VOID Write(GS_UINT32 addr, GS_BYTE val);
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMMAPPER_H__
