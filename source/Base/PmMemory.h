#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMMEMORY_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMMEMORY_H__

#include "PmBase.h"

struct PmMemory
{
    GS_BYTE * bios; // 0x000000 - 0x001000 Internal BIOS
    GS_BYTE * ram;  // 0x001000 - 0x002000 RAM
    GS_BYTE * reg;  // 0x002000 - 0x002100 Hardware Registers
    GS_BYTE * cart; // 0x002100 - 0x200000 Cartridge Memory

    GS_BYTE * rom;  // 0x000000 - 0x200000 Whole Memory
    GS_UINT32 romSize;

    PmMemory() { gs_zero(this, sizeof(*this)); }

    GS_VOID Init(GS_BYTE *_rom, GS_UINT _romSize)
    {
        rom = _rom;
        romSize = _romSize;
        bios = rom;
        ram  = bios + PmBase::PM_BIOS_SIZE;
        reg  = ram  + PmBase::PM_RAM_SIZE;
        cart = reg  + PmBase::PM_REG_SIZE;
    }

    GS_BYTE & Value(GS_UINT32 addr) const
    {
        GS_ASSERT(addr < romSize);
        if (addr >= romSize) { addr = romSize - 1; }
        return rom[addr];
    }
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMMEMORY_H__
