#include "PmAudio.h"
#include "Base/PmHardRegMap.h"

PmAudio::PmAudio(PmSystem *pSystem, GS_BYTE *pReg)
    : PmUnit(pSystem)
    , m_pReg(pReg)
{
    m_soundCache.size = PmBase::PM_SOUND_BUFFER_SIZE;
    m_soundCache.pBuffer = GS_MALLOCZ_OBJ_N(GS_BYTE, m_soundCache.size);
    RETURN_ON_FAIL(GS_NULL != m_soundCache.pBuffer);

    SetReady();
}

PmAudio::~PmAudio()
{
    SafeFreePointer(m_soundCache.pBuffer);
}

GS_VOID PmAudio::Reset(GS_BOOL /*hardReset*/)
{
    m_state.Reset();
    m_soundCache.Reset();
}

GS_BYTE PmAudio::ReadReg(GS_BYTE reg)
{
    // 0x70 to 0x71
    switch(reg)
    {
    case 0x70: // Unknown Audio Control
        return PMR_AUD_CTRL & 0x07;
    case 0x71: // Audio Volume Control
        return PMR_AUD_VOL & 0x07;
    default:
        GS_ASSERT(GS_FALSE);
        return 0;
    }
}

GS_VOID PmAudio::WriteReg(GS_BYTE reg, GS_BYTE val)
{
    // 0x70 to 0x71
    switch(reg)
    {
    case 0x70: // Unknown Audio Control
        PMR_AUD_CTRL = val & 0x07;
        break;
    case 0x71: // Audio Volume Control
        PMR_AUD_VOL = val & 0x07;
        break;
    default:
        GS_ASSERT(GS_FALSE);
    }

    // Calculate volume
    if (PMR_AUD_CTRL & 0x03) // Mute audio
    {
        m_state.sVolume = PM_AUDIO_SILENCE;
    }
    else
    {
        switch (PMR_AUD_VOL & 3)
        {
        case 0: // 0% Sound
            m_state.sVolume = PM_AUDIO_SILENCE;
            break;
        case 1:
        case 2: // 50% Sound
            m_state.sVolume = PM_AUDIO_MED_VOL;
            break;
        case 3: // 100% Sound
            m_state.sVolume = PM_AUDIO_MAX_VOL;
            break;
        }
    }
}

GS_VOID PmAudio::Sync(GS_UINT32 cycles)
{
    m_state.uiSampleCounter += (cycles * (PM_AUDIO_COUNTER_SCALE / PmBase::PM_CPU_RATE)) * PmBase::PM_AUDIO_SAMPLING_RATE;
    while (m_state.uiSampleCounter >= PM_AUDIO_COUNTER_SCALE)
    {
        m_state.uiSampleCounter -= PM_AUDIO_COUNTER_SCALE;
        GS_BYTE volume = GetVolume();
        m_soundCache.Write(volume);
    }
}

GS_VOID PmAudio::OutputSound(GS_BYTE *pBuffer, GS_UINT size)
{
    GS_UINT count = gs_min(SoundDataCount(), size);
    for (GS_UINT i = 0; i < count; ++i)
    {
        *(pBuffer++) = m_soundCache.Read();
    }
}

GS_BYTE PmAudio::GetVolume() const
{
    return SysTimer3CheckPivot()? m_state.sVolume: PM_AUDIO_SILENCE;
}

GS_VOID PmAudio::SoundCache::Reset()
{
    count = readPos = writePos = 0;
}

GS_BYTE PmAudio::SoundCache::Read()
{
    GS_BYTE volume = PM_AUDIO_SILENCE;
    GS_ASSERT(count  > 0);
    if (count > 0)
    {
        volume = pBuffer[readPos];
        if (++readPos >= size) { readPos = 0; }
        --count;
    }

    return volume;
}

GS_VOID PmAudio::SoundCache::Write(GS_BYTE volume)
{
    GS_ASSERT(count < size);
    if (count < size)
    {
        pBuffer[writePos] = volume;
        if (++writePos >= size) { writePos = 0; }
        ++count;
    }
}
