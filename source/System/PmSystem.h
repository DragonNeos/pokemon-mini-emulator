#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_SYSTEM_PMSYSTEM_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_SYSTEM_PMSYSTEM_H__

#include "IGsPmSystem.h"
#include "Base/PmMapper.h"
#include "Units/PmCPU.h"
#include "Units/PmIO.h"
#include "Units/PmIRQ.h"
#include "Units/PmPRC.h"
#include "Units/PmLCD.h"
#include "Units/PmTimer.h"
#include "Units/PmAudio.h"

class PmSystem : public IGsPmSystem
{
public:
    PmSystem();
    virtual ~PmSystem();

public:
    virtual GS_UINT SaveSize() const { return PmBase::PM_EEPROM_SIZE; }
    virtual GS_UINT ScreenWidth() const { return PmBase::PM_SCREEN_WIDTH; }
    virtual GS_UINT ScreenHeight() const { return PmBase::PM_SCREEN_HEIGHT; }
    virtual GS_UINT CpuRate() const { return PmBase::PM_CPU_RATE; }
    virtual GS_UINT SoundBufferSize() const { return PmBase::PM_SOUND_BUFFER_SIZE; }
    virtual GS_UINT SoundSamplingRate() const { return PmBase::PM_AUDIO_SAMPLING_RATE; }

public:
    virtual GS_BOOL Initialize(GS_BYTE *pRom, GS_UINT romSize, GS_BYTE *pSave);
    virtual GS_VOID Reset(GS_BOOL hardReset);
    virtual GS_VOID Destroy();

    virtual GS_UINT EmulateFrame();
    virtual GS_VOID Render(GS_BYTE *pScreen, GS_UINT rowBytes) const;
    virtual GS_UINT SoundDataCount() const;
    virtual GS_VOID OutputSound(GS_BYTE *pBuffer, GS_UINT size);
    virtual GS_BOOL Rumbling() const;

    virtual GS_VOID OnKeyUp(PM_KEY key);
    virtual GS_VOID OnKeyDown(PM_KEY key);
    virtual GS_VOID OnShock();

private:
    PmIO::IO_KEY GetInnerKey(PM_KEY key) const;

public:
    GS_BYTE Read(GS_UINT32 addr);
    GS_VOID Write(GS_UINT32 addr, GS_BYTE val);
    GS_VOID SetCpuStall(GS_BOOL stall);
    GS_VOID CallIrq(GS_BYTE intr);
    GS_VOID SetIrq(GS_BOOL enable);
    GS_VOID SetIrqAct(GS_BYTE intr);
    GS_VOID CopyFrameBufferToLcd(const GS_BYTE *pFrameBuffer);
    GS_VOID RefreshLcd();
    GS_BOOL Timer3CheckPivot() const;

private:
    PmMemory * m_pMem;
    GS_BYTE *  m_pReg;
    PmMapper * m_pMapper;
    PmCPU *    m_pCPU;
    PmIO *     m_pIO;
    PmIRQ *    m_pIRQ;
    PmPRC *    m_pPRC;
    PmLCD *    m_pLCD;
    PmTimer *  m_pTimer;
    PmAudio *  m_pAudio;
    GS_BOOL    m_lcdRefreshed;
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_SYSTEM_PMSYSTEM_H__
