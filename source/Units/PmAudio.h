#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMAUDIO_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMAUDIO_H__

#include "System/PmUnit.h"
#include <stdint.h>

class PmAudio : public PmUnit
{
public:
    PmAudio(PmSystem *pSystem, GS_BYTE *pReg);
    virtual ~PmAudio();

    virtual GS_VOID Reset(GS_BOOL hardReset);

    virtual GS_BYTE ReadReg(GS_BYTE reg);
    virtual GS_VOID WriteReg(GS_BYTE reg, GS_BYTE val);

public:
    GS_VOID Sync(GS_UINT32 cycles);
    GS_UINT SoundDataCount() const { return m_soundCache.count; }
    GS_VOID OutputSound(GS_BYTE *pBuffer, GS_UINT size);

private:
    enum { PM_AUDIO_COUNTER_SCALE = PmBase::PM_CPU_RATE * 4 };

    enum
    {
        PM_AUDIO_SILENCE = 0x00, 
        PM_AUDIO_MED_VOL = 0x20, 
        PM_AUDIO_MAX_VOL = 0x40, 
    };

    GS_BYTE GetVolume() const;

    struct SoundCache
    {
        GS_BYTE *pBuffer;
        GS_UINT size;
        GS_UINT count;
        GS_UINT readPos;
        GS_UINT writePos;

        SoundCache() { gs_zero(this, sizeof(*this)); }
        GS_VOID Reset();
        GS_BYTE Read();
        GS_VOID Write(GS_BYTE volume);
    };

private:
    struct State
    {
        GS_UINT32 uiSampleCounter;
        GS_BYTE sVolume;

        State() { Reset(); }
        GS_VOID Reset() { gs_zero(this, sizeof(*this)); }
    };

    State m_state;

private:
    GS_BYTE * m_pReg;
    SoundCache m_soundCache;
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMAUDIO_H__
