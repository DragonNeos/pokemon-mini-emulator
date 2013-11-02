#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_EXPORT_IGSEMULATION_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_EXPORT_IGSEMULATION_H__

#include "GsKernelType.h"

class IGsSimpleStream;

class IGsPmSystem
{
public:
    virtual ~IGsPmSystem() {}

public: // const values
    virtual GS_UINT SaveSize() const = 0;
    virtual GS_UINT ScreenWidth() const = 0;
    virtual GS_UINT ScreenHeight() const = 0;
    virtual GS_UINT CpuRate() const = 0;
    virtual GS_UINT SoundBufferSize() const = 0;
    virtual GS_UINT SoundSamplingRate() const = 0;

public:
    virtual GS_BOOL Initialize(GS_BYTE *pRom, GS_UINT romSize, GS_BYTE *pSave) = 0;
    virtual GS_VOID Reset(GS_BOOL hardReset) = 0;
    virtual GS_VOID Destroy() = 0;

    virtual GS_UINT EmulateFrame() = 0;

    virtual GS_VOID Render(GS_BYTE *pScreen, GS_UINT rowBytes) const = 0;
    virtual GS_UINT SoundDataCount() const = 0;
    virtual GS_VOID OutputSound(GS_BYTE *pBuffer, GS_UINT size) = 0;

    virtual GS_BOOL Rumbling() const = 0;

    enum PM_KEY
    {
        PM_KEY_A, 
        PM_KEY_B, 
        PM_KEY_C, 
        PM_KEY_UP, 
        PM_KEY_DOWN, 
        PM_KEY_LEFT, 
        PM_KEY_RIGHT, 
        PM_KEY_POWER, 

        PM_KEY_NONE
    };

    virtual GS_VOID OnKeyUp(PM_KEY key) = 0;
    virtual GS_VOID OnKeyDown(PM_KEY key) = 0;
    virtual GS_VOID OnShock() = 0;
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_EXPORT_IGSEMULATION_H__
