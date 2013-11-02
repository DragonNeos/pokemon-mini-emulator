#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMBASE_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMBASE_H__

#include "KernelDef.h"

union PmReg32
{
    struct
    {
#ifdef BIG_ENDIAN
        GS_BYTE B3;
        GS_BYTE B2;
        GS_BYTE B1;
        GS_BYTE B0;
#else
        GS_BYTE B0;
        GS_BYTE B1;
        GS_BYTE B2;
        GS_BYTE B3;
#endif
    } B;

    struct
    {
#ifdef BIG_ENDIAN
        GS_UINT16 W1;
        GS_UINT16 W0;
#else
        GS_UINT16 W0;
        GS_UINT16 W1;
#endif
    } W;

    GS_UINT32 I;

    PmReg32() : I(0) {}
};

class PmBase
{
    DISALLOW_CONSTRUCT(PmBase);

public:
    enum
    {
        PM_BIOS_SIZE   = 0x1000, 
        PM_RAM_SIZE    = 0x1000, 
        PM_REG_SIZE    = 0x0100, 
        PM_EEPROM_SIZE = 0x2000, 
    };

    enum
    {
        PM_SCREEN_WIDTH  = 96, 
        PM_SCREEN_HEIGHT = 64, 
    };

    enum
    {
        PM_SOUND_BUFFER_SIZE = 4 * 1024
    };

    enum
    {
        PM_CPU_RATE = 4000000, 
        PM_AUDIO_SAMPLING_RATE = 44100, 
    };

    static const GS_BYTE PM_FREE_BIOS[PM_BIOS_SIZE];
    static const GS_BYTE PM_REG_INIT[PM_REG_SIZE];
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_BASE_PMBASE_H__
