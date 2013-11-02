#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMLCD_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMLCD_H__

#include "System/PmUnit.h"

// Liquid Crystal Display
class PmLCD : public PmUnit
{
public:
    PmLCD(PmSystem *pSystem);
    virtual ~PmLCD();

    virtual GS_VOID Reset(GS_BOOL hardReset);

    virtual GS_BYTE ReadReg(GS_BYTE reg);
    virtual GS_VOID WriteReg(GS_BYTE reg, GS_BYTE val);

public:
    GS_VOID CopyFrameBuffer(const GS_BYTE *pFrameBuffer);
    GS_VOID RefreshScreen();
    GS_VOID RenderScreen(GS_BYTE *pScreen, GS_UINT rowBytes) const;

private:
    GS_BYTE ReadCtrl();
    GS_BYTE Read();
    GS_VOID WriteCtrl(GS_BYTE val);
    GS_VOID Write(GS_BYTE val);

    GS_VOID SetContrast(GS_BYTE level);

private:
    enum { DATA_WIDTH    = 132 };
    enum { DATA_HEIGHT   =  65 };

    struct State
    {
        GS_BYTE ucMinIntensity;
        GS_BYTE ucMaxIntensity;
        GS_BYTE bSetContrast;
        GS_BYTE ucContrastLevel;
        GS_BYTE bDisplayOn;
        GS_BYTE bSetAllPixels;
        GS_BYTE bInvertAllPixels;
        GS_BYTE ucPage;
        GS_BYTE ucStartRow;
        GS_BYTE bRowReverse;
        GS_BYTE ucColumn;
        GS_BYTE bColumnReverse; // Segment Driver Direction Select
        GS_BYTE bRmwMode; // Read Modify Write mode
        GS_BYTE ucRmwColumn; // Column when Read Modify Write started
        GS_BYTE bRequireDummyRead;

        State() { Reset(); }
        GS_VOID Reset() { gs_zero(this, sizeof(*this)); }
    };

    State m_state;

private:
    GS_BOOL   m_changed;
    GS_BYTE * m_pData;    // Graphic Display Data RAM (GDDRAM), 132 x 65 x 1bpp
    GS_BYTE * m_pScreen0; // Curent screen, 96 x 64, 0 or 1
    GS_BYTE * m_pScreen1; // Auxiliary screen, 96 x 64, used for render
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMLCD_H__
