#ifndef __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMPRC_H__
#define __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMPRC_H__

#include "System/PmUnit.h"

// Program Rendering Chip
class PmPRC : public PmUnit
{
public:
    PmPRC(PmSystem *pSystem, GS_BYTE *pReg, GS_BYTE *pRam);
    virtual ~PmPRC();

    virtual GS_VOID Reset(GS_BOOL hardReset);

    virtual GS_BYTE ReadReg(GS_BYTE reg);
    virtual GS_VOID WriteReg(GS_BYTE reg, GS_BYTE val);

public:
    GS_VOID Sync(GS_UINT32 cycles);
    GS_VOID Render();

private:
    GS_VOID InitInvBitTable();
    GS_VOID DrawSpriteTile(GS_BYTE config, GS_INT x, GS_INT y, GS_UINT drawTileIndex, GS_UINT maskTileIndex);

private:
    enum
    {
        PM_INTR_03 = 0x03, // PRC Copy Complete
        PM_INTR_04 = 0x04  // PRC Frame Divider Overflow
    };

    enum { PM_PRC_RATE = 4678 }; // Aproximate

    struct State
    {
        GS_UINT32 uiCounter;
        GS_UINT32 uiMapTileBase;
        GS_UINT32 uiSpriteBase;
        GS_BYTE   emMode;    // 0 = Off, 1 = Copy, 3 = BG+Spr+Copy
        GS_BYTE   ucRateMatch;
        GS_BYTE   ucMapPixelX;
        GS_BYTE   ucMapPixelY;
        GS_BYTE   ucMapTileWidth;
        GS_BYTE   ucMapTileHeight;
        GS_BYTE   emState;

        State() { Reset(); }
        GS_VOID Reset() { gs_zero(this, sizeof(*this)); }
    };

    State m_state;

private:
    GS_BYTE *       m_pReg;
    GS_BYTE *       m_pFrameBuffer;
    const GS_BYTE * m_pSpriteAttributes;
    const GS_BYTE * m_pTileMap;
    GS_BYTE         m_invBitTable[0x100];
};

#endif // __KERNEL_EMULATIONKIT_POKEMINILIB_SOURCE_UNITS_PMPRC_H__
